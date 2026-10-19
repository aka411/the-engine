#pragma once
#include <glad/glad.h>
#include <string>




namespace TheEngine::RenderingSystem::OpenGLBackend
{


	GLuint compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode);




}