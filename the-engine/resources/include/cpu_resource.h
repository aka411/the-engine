#pragma once

namespace TheEngine::Resource
{

enum class CPUResourceType
{
	UNDEFINED = 0,
	TEXTURE,
	SHADER,
	MATERIAL,
	MESH

};


class CPUResource
{
protected:
	CPUResource() = default;

public:
	virtual CPUResourceType getType() const = 0;

	virtual ~CPUResource() = default;
};


}


