#include "rendering-engine/opengl-backend/opengl_utils.h"
#include <stdio.h>


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