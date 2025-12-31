#pragma once
#include <string>


std::string normalUIShaderVertexCode = R"(
#version 460 core


//layout (location = 0) in vec4 inPosition;
//layout(location = 1) in vec4 color;




struct VertexData 
{
    vec4 positionIn;
    vec4 colorIn;

};

// readonly SSBO containing the data
layout(binding = 0, std430) readonly buffer ssbo1 
{
    VertexData data[];
};



uniform mat4 projection;

uniform mat4 model;

 


out vec4 vs_color;

void main()
{

 
 gl_Position = projection *  model * vec4(data[gl_VertexID].positionIn);

 vs_color = vec4(data[gl_VertexID].colorIn);
}





)";


std::string normalUIShaderFragmentCode = R"(
#version 460 core



in vec4 vs_color;

out vec4 FragColor;

void main()
{
	FragColor = vs_color;
}

)";





/*Text Shader Code*/

std::string textUIShaderVertexCode = R"(
#version 460 core
#extension GL_ARB_bindless_texture : require


layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inUV;


uniform mat4 projection;
uniform mat4 model;

 
out vec4 vs_color;

out vec2 uv;

void main()
{

 
 gl_Position = projection *  model * vec4(inPosition,1.0);

 vs_color = inColor;
 uv = inUV;
}

)";

std::string textUIShaderFragmentCode = R"(
#version 460 core


in vec4 vs_color;
in vec2 uv;

uniform uvec2 sdfTextureHandle;


out vec4 FragColor;

void main()
{





 float EdgeThreshold = 0.5; 



 

    float D = texture2D(sampler2D(sdfTextureHandle), uv).r;
    
   
    float W = fwidth(D) * 0.5; 

    float D_shifted = D; 

    

    float alpha_text = smoothstep(EdgeThreshold - W, EdgeThreshold + W, D_shifted);



    float alpha_total = smoothstep(EdgeThreshold - W, EdgeThreshold + W, D);
    

    vec3 final_color = mix(vs_color.rgb, vs_color.rgb, alpha_text);
    
 
    FragColor = vec4(final_color, alpha_total);


}


)";