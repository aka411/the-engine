#version 460 core
#extension GL_ARB_bindless_texture : require


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


