#pragma once
#include <memory>
#include <string>




namespace TheEngine::Platform
{
	//Yeah there is still lots design to hash out

	struct FileData
	{
		//for taking ownership of data from external libriaries heap
		//need more thought here , seems very brittle though
		std::unique_ptr < std::byte[], void(*)(void*) > data = { nullptr,nullptr };
		size_t size = 0;// size in bytes

	};


	class FileSystem
	{

	private:


	public:

		FileSystem();

		FileData readFile(const std::string& pathToFile);
		//methods for stream reading etc


	};





}