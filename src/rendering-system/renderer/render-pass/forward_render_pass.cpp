#include <string>
#include <glm/gtc/type_ptr.hpp>

#include <rendering-system/renderer/rendering_context.h>
#include <rendering-system/renderer/render-pass/forward_render_pass.h>
#include <rendering-system/low-level-gpu-systems/low_level_gpu_system.h>
#include <rendering-system/low-level-gpu-systems/gpu_shader_manager.h>

#include <rendering-system/gpu-resource-system/gpu_resource_system.h>
#include <rendering-system/gpu-resource-system/gpu_mesh_system.h>
#include <rendering-system/gpu-resource-system/gpu_material_manager.h>

#include <rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h>
#include <rendering-system/low-level-gpu-systems/gpu_pipeline_manager.h>

namespace TheEngine::RenderingSystem
{



    void ForwardRenderPass::setState()
    {
       
        //get pipeline handle and bind
        m_lowLevelGPUSystem.getGPUPipelineManager().getGPUPipelineState(m_pipelineHandle).bind();

        GPUBufferInfo gpuBufferInfo = m_gpuResourceSystem.getGPUMeshSystem().getGPUBufferInfoForVetexFormat(VertexFormat::STANDARD, BufferUsage::STATIC);
        glBindVertexBuffer(0, gpuBufferInfo.bufferHandle, 0, 32);

      //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_animationSystem.getSkeletalAnimationManager().getJointMatrixSSBO().bufferHandle);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_gpuResourceSystem.getGPUMaterialManager().getGPUBufferInfoForMaterial(ShadingModel::PBR_METALLIC_ROUGHNESS).bufferHandle);
       //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_lightSystem.getLightSSBO().bufferHandle);


       


       
        //glViewport(0, 0, m_renderingContext.swapChainImageWidth, m_renderingContext.swapChainImageHeight);
        glViewport(0, 0, 400, 400);
        glUniformMatrix4fv(0, 1, GL_FALSE, &m_renderingContext.projection[0][0]);
        glUniformMatrix4fv(1, 1, GL_FALSE, &m_renderingContext.view[0][0]);

        glClearDepth(1.0);
        glClearColor(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    }


	ForwardRenderPass::ForwardRenderPass(RenderingContext& renderingContext) :
		IRenderPass(renderingContext)
	{

        VertexLayout pbrLayout;//our vao

        pbrLayout.addAttribute(0, 3, PrimitiveDataType::FLOAT, false, 0);  // Position
        pbrLayout.addAttribute(1, 3, PrimitiveDataType::FLOAT, false, 12); // Normal
        pbrLayout.addAttribute(2, 2, PrimitiveDataType::FLOAT, false, 24); // UV

        const std::string vertexShader_Normal = R"(
#version 460 core
//#extension GL_ARB_bindless_texture : require


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;




/****PER FRAME DATA****/
layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;





/**** INDIVIDUAL UNIFORMS (Replacement for SSBO) ****/
layout(location = 10) uniform mat4 modelMatrix;
layout(location = 11) uniform uint materialId;







/*OUT_TO_FRAFMENT_SHADER*/

out vec2 vs_texCoord_0;







out vec3 vs_position;
out vec3 vs_normal;
flat out mat4 viewMatrix;

flat out uint vs_materialId;

void main()
{





    vec4 position = vec4(inPosition, 1.0);

    viewMatrix = view;

    gl_Position = projection * view * modelMatrix * position;





    /** Outputs To Fragment Shader **/

    vs_materialId = materialId;

//yeah dont multiple view matrix
    vs_normal = normalize(mat3(view * modelMatrix) * inNormal);

    vs_position = vec3((view * modelMatrix * vec4(inPosition, 1.0)).xyz);


    vs_texCoord_0 = inTexCoord;

}

)";



        static const std::string pbrNormalBaseFragmentCode = R"(
#version 460 core
//#extension GL_ARB_gpu_shader_int64 : require //not supported by my graphics card
//#extension GL_ARB_bindless_texture : require






struct PBRMetallicRoughnessMaterial 
{

		//N = 4
		//Alignement : 4N , 4 * 4 =16
		vec4 baseColorFactor;//4N ,offset =  0
		vec3 emissiveFactor;// offset = 16

		//Alignement : N , 4
		float metallicFactor ; //offset = 28
		float roughnessFactor; //offset = 32

		//float padding = 0;//offset = 36

		// Alignement : 2N , 2*4 = 8 
		uvec2 albedoTextureHandle; //offset = 40
		uvec2 metallicRoughnessTextureHandle ; //offset = 48

		uvec2 normalTextureHandle ; //offset = 56
		uvec2 occlusionTextureHandle;// offset = 64
		uvec2 emissiveTextureHandle;//offset =  72




};


layout(std430, binding = 2) readonly buffer MaterialData 
{
    PBRMetallicRoughnessMaterial materials[];
};


/*
struct Light
{



};

layout(binding = 3, std430) readonly buffer LightData 
{

 Light light[];

};

*/







// --- PBR Utility Functions  ---



const float PI = 3.14159265359;


// Distribution (D) term: Trowbridge-Reitz GGX
float distributionGGX(vec3 N, vec3 H, float roughness) 
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom; 

    return a2 / denom;
}


// Geometry (G) term: Schlick-GGX
float geometrySchlickGGX(float NdotV, float roughness) 
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}


// Geometry term: Smith function (combining V and L components)
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) 
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


// Fresnel (F) term: Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0)
 {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
 }





/*** IN FROM VERTEX SHADER ****/

in vec2 vs_texCoord_0;








flat in uint vs_materialId;

 in vec3 vs_position;
 in vec3 vs_normal;

flat in mat4 viewMatrix;




/*** OUT TO FRAMEBUFFER ***/
out vec4 FragColor;


vec4 toLinear(vec4 srgbColor) 
{
    vec3 linearRGB = pow(srgbColor.rgb, vec3(2.2));
    return vec4(linearRGB, srgbColor.a);
}

vec4 toSRGB(vec4 linearColor) 
{
    vec3 srgb = pow(linearColor.rgb, vec3(1.0 / 2.2));
    return vec4(srgb, linearColor.a);
}



void main()
{


PBRMetallicRoughnessMaterial material = materials[vs_materialId];


FragColor =  material.baseColorFactor; //vec4(1.0,0.4,0.3,1.0);
return;


#if 0
PBRMetallicRoughnessMaterial material = materials[vs_materialId];










    vec4 baseColor = material.baseColorFactor;

    float metallic = material.metallicFactor;
    float roughness =  material.roughnessFactor;

    vec3 F0 = vec3(0.04);
    float occlusion = 1.0;

    vec3 emissive = material.emissiveFactor;

    

    // ****** Albedo Texture Fetch ******//



        vec2 uv = vs_texCoord_0;
        
        //baseColor *= texture2D(sampler2D(material.albedoTextureHandle), uv);
        baseColor *= toLinear( texture2D(sampler2D(material.albedoTextureHandle), uv));



    // ********Metallic Roughness Texture Fetch ****//

 
       
        vec4 mrSample = texture2D(sampler2D(material.metallicRoughnessTextureHandle), uv);

        // Gltf packs metallic into B and roughness into G channel
        metallic *= mrSample.b;
        roughness *= mrSample.g;
   
    

    
        // Occlusion is stored in the Red channel (R)
        occlusion = texture2D(sampler2D(material.occlusionTextureHandle), uv).r;
    



        // Emissive color is multiplied by the texture sample
        emissive *= texture2D(sampler2D(material.emissiveTextureHandle), uv).rgb;
    



    // ********** Normal Map Fetch (TBN basis)********//

vec3 finalNormal;


 #ifdef HAS_TANGENT
    if (normalTexturePresent != 0) 
    {

        
        //  Get the TBN frame
        vec3 T = normalize(vs_tangent.xyz);
        vec3 N_in = normalize(vs_normal);
       
        
        //  glTF  handedness:
        vec3 B = (vs_tangent.w > 0.0) ? cross(N_in, T) : -cross(N_in, T);

        mat3 tbn = mat3(T, B, N_in);
        
 
        vec2 uv = getTexCoord(normalTexCoordIndex);
        vec3 normalSample = texture2D(sampler2D(material.normalTextureHandle), uv).rgb;


        normalSample = normalize(normalSample * 2.0 - 1.0); 


       finalNormal  = normalize(tbn * normalSample);

    }
#else

finalNormal = vs_normal;

#endif



/*****LIGHTING  CALCULATIONS******/





const vec3 LIGHT_DIRECTION_WORLD =  normalize(vec3(0.5, -0.8, 0.2));//vec3(light[0].direction.xyz);//
const vec3 LIGHT_COLOR = vec3(1.0, 1.0, 1.0); //vec3(light[0].color.xyz);//



const vec3 CAMERA_POS = vec3(0.0, 0.0, 0.0); //camera always at origin in view space




vec3 lightDirView = mat3(viewMatrix) * LIGHT_DIRECTION_WORLD;


vec3 L = -normalize(lightDirView);
vec3 V = normalize(CAMERA_POS - vs_position); // View vector (points from fragment to camera)


vec3 N = finalNormal; // Use the TBN-transformed normal

vec3 F0_final = mix(F0, baseColor.rgb, metallic); 


vec3 H = normalize(V + L); // Halfway vector
float NdotL = max(dot(N, L), 0.0);

// --- PBR BRDF Terms ---

float D = distributionGGX(N, H, roughness);
float G = geometrySmith(N, V, L, roughness);
vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0_final);



vec3 kS = F;
vec3 kD = vec3(1.0) - kS; 
kD *= 1.0 - metallic; 

vec3 numerator   = D * G * F;
float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.001; 
vec3 specular = numerator / denominator;


vec3 albedo = baseColor.rgb;
vec3 diffuse = (kD * albedo) / PI;




// --- Final Lighting Composition ---


// Light contribution
vec3 Lo = (diffuse + specular) * LIGHT_COLOR * NdotL;


vec3 ambient = vec3(0.01) * albedo * occlusion;

// Final color
vec3 finalColor = ambient + Lo + emissive;



// FragColor = vec4(finalColor, baseColor.a);

if(baseColor.a < 0.001)
{
discard;
}

//FragColor = toSRGB(vec4(finalColor, baseColor.a));
#endif
}




)";




       DepthStencilState depthStencilState;
       RasterizerState rasterizerState;
       BlendState blendState;

       


        ShaderCreateInfo shaderCreateInfo;
        shaderCreateInfo.vertexShaderString = vertexShader_Normal;
        shaderCreateInfo.fragmentShaderString = pbrNormalBaseFragmentCode;


        // 2. Define the State // shopping list
        PipelineDescriptor desc;
        desc.shader = m_lowLevelGPUSystem.getGPUShaderManager().createAndStoreShader("ForwardRenderPass_Shader", shaderCreateInfo);
        desc.layout = pbrLayout;
        desc.depthStencil = depthStencilState;
        desc.rasterizer = rasterizerState;
        desc.blend = blendState;



        //get pipeline manager from lowlevelsystems
        m_pipelineHandle = m_lowLevelGPUSystem.getGPUPipelineManager().createPipeline(desc);









 





	}






	void ForwardRenderPass::excecute(const DrawCallBucket& drawCallbucket)
	{

        setState();
        
        for (auto& drawCall : drawCallbucket.drawCalls)
        {


           const glm::mat4& modelMatrix = drawCall.transformation;
           const uint32_t& materialId = drawCall.materialId; // Example ID

            glUniformMatrix4fv(10, 1, GL_FALSE, &modelMatrix[0][0]);
            glUniform1ui(11, materialId);


            GLint firstVertex = static_cast<GLint>(drawCall.vertexOffsetInBuffer / 32);

            glDrawArrays(
                GL_TRIANGLES,
                firstVertex,
                static_cast<GLsizei>(drawCall.vertexCount)
            );
       
        }





	}



}