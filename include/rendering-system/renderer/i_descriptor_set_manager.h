#pragma once

namespace TheEngine::RenderingSystem
{
	//WE DONT NEED THIS IN AGNOSTIC LAYER

	//Opengl does not even need this, but can make use to make shaders standardized
	//Design of this is very tricky

	//need a DescriptorSethandle that is hashd against the layout, which can be reused

	//DescriptorSetWrite needs DescriptorSetHandle,
	//i need to think about the workflow and design this 

	//This needs to be hidden and only for vulkan , opengl only needs a translater to 
	class IDescriptorSetManager
	{
	private:


	public:


	};





}