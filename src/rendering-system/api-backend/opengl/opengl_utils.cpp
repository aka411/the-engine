#include "rendering-system/api-backend/opengl/opengl_utils.h"
#include <stdio.h>
#include <iostream>


void GLAPIENTRY DebugMessageCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{

	if (type == GL_DEBUG_TYPE_OTHER || type == GL_DEBUG_TYPE_PERFORMANCE)
		return;


	fprintf(stderr, "GL Debug Message:\n");
	fprintf(stderr, "  Source: 0x%x\n", source);
	fprintf(stderr, "  Type: 0x%x\n", type);
	fprintf(stderr, "  ID: %d\n", id);
	fprintf(stderr, "  Severity: 0x%x\n", severity);
	fprintf(stderr, "  Message: %s\n", message);
	fprintf(stderr, "------------------------------------------------\n");

	// Crash the program if it's a high-severity error
	if (severity == GL_DEBUG_SEVERITY_HIGH)
	{

		// __debugbreak(); // Windows-specific
	}
}





//another one
void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		}
		}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		}
		}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		}
		}();
	std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}