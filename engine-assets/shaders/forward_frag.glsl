#version 460 core
//#extension GL_ARB_gpu_shader_int64 : require //not supported by my graphics card
#extension GL_ARB_bindless_texture : require






struct PBRMetallicRoughnessMaterial
{

		//N = 4
		//Alignement : 4N , 4 * 4 =16
		vec4 baseColorFactor;//4N ,offset =  0
		vec3 emissiveFactor;// offset = 16

		//Alignement : N , 4
		float metallicFactor ; //offset = 28
		float roughnessFactor; //offset = 32

		//float padding ;//offset = 36

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


//PBRMetallicRoughnessMaterial material = materials[vs_materialId];


//FragColor =  material.baseColorFactor; //vec4(1.0,0.4,0.3,1.0);
 //FragColor = toLinear( texture2D(sampler2D(material.albedoTextureHandle), vs_texCoord_0));
//return;


//#if 0
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





const vec3 LIGHT_DIRECTION_WORLD =  normalize(vec3(0.0, 0.0, -1));//vec3(light[0].direction.xyz);//
const vec3 LIGHT_COLOR = vec3(1.0, 1.0, 1.0); //vec3(light[0].color.xyz);//



const vec3 CAMERA_POS = vec3(0.0, 0.0, 0.0); //camera always at origin in view space




vec3 lightDirView =  LIGHT_DIRECTION_WORLD;


vec3 L = normalize(lightDirView);
vec3 V = normalize(CAMERA_POS - vs_position); // View vector (points from fragment to camera)


vec3 N = finalNormal; // Use the TBN-transformed normal

vec3 F0_final = mix(F0, baseColor.rgb, metallic);


vec3 H = normalize(V + L)/2.0f; // Halfway vector
float NdotL = max(dot(N, L), 0.0);


vec3 Lo = vec3(0.0); // Initialize to zero!
vec3 albedo = baseColor.rgb; // Define this outside so ambient can use it


// --- PBR BRDF Terms ---
if (NdotL > 0.0)
{

float D = distributionGGX(N, H, roughness);
float G = geometrySmith(N, V, L, roughness);
vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0_final);



vec3 kS = F;
vec3 kD = vec3(1.0) - kS;
kD *= 1.0 - metallic;

vec3 numerator   = D * G * F;
float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.001;
vec3 specular = numerator / denominator;


albedo = baseColor.rgb;
vec3 diffuse = (kD * albedo) / PI;




// --- Final Lighting Composition ---


// Light contribution
Lo = (diffuse + specular) * LIGHT_COLOR * NdotL;

}

vec3 ambient = vec3(0.01) * albedo * occlusion;

// Final color
vec3 finalColor = ambient + Lo + emissive;



// FragColor = vec4(finalColor, baseColor.a);

if(baseColor.a < 0.001)
{
discard;
}

FragColor = toSRGB(vec4(finalColor, baseColor.a));
//#endif
}
