#pragma once
#include <glm/ext/matrix_float4x4.hpp>

struct PushConst
{
	glm::mat4 modelMatrix;
	uint64_t ssboOffset = 0;
};