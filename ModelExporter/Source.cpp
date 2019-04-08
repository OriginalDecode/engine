#include "CommonLib/Utilities.h"
#include "CommonLib/Threadpool.h"
#include "Engine/engine_shared.h"
#include "CommonLib/DataStructures/GrowingArray.h"
#include "Engine/ModelImporter.h"
#include "Engine/Model.h"
#include "../DL_Debug/DL_Debug.h"
#include <cstdio>

void thread_print(std::string toPrint)
{
	static Ticket_Mutex mutex;
	BeginTicketMutex(&mutex);
	printf("%s\n", toPrint.c_str());
	EndTicketMutex(&mutex);
}

int main(int argc, const char* argv[])
{
	DL_Debug::Debug::Create();
	std::vector<std::string> folders;
	cl::GetFolders(folders, "Data\\Model", true);

	std::vector<std::string> exportList;

	for (auto& folder : folders)
	{
		std::string _path(folder);
		_path += "/*.*";
		std::vector<std::string> files = cl::FindFilesInDirectory(_path.c_str());
		for (auto& file : files)
		{
			if (file.find(".dds") != file.npos)
			{
				//copy texture to different location
			}
			else if (file.find(".fbx") != file.npos)
			{
				std::string filepath(folder);
				filepath += "\\" + file;
				exportList.push_back(filepath);
			}
		}
	}


	Threadpool threadpool;
	threadpool.Initiate("exporter");

	for (auto& filepath : exportList)
	{

		CModelImporter importer;
		Model model;
		importer.LoadModel<Model>(filepath, &model, nullptr, ModelImportUtil::IGNORE_FILL);
		/*
				threadpool.AddWork(Work([=]() {
					std::string str("Starting to load ");
					str += filepath;
					thread_print(str);
					CModelImporter importer;
					Model model;
					importer.LoadModel<Model>(filepath, &model, nullptr, ModelImportUtil::IGNORE_FILL);
				}));*/
	}

	

	while (threadpool.HasWork()) 
	{
		threadpool.Update();
	}

	printf("finished exporting\n");

	threadpool.CleanUp();
	DL_Debug::Debug::Destroy();

	std::getchar();
	return 0;
}
