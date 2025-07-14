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


class ICPUResource
{
protected:
	ICPUResource() = default;

public:
	virtual CPUResourceType getType() const = 0;

	virtual ~ICPUResource() = default;
};


}


