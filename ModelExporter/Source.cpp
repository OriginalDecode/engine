#define _MODEL_EXPORTER

#include "CommonLib/Utilities.h"
#include "CommonLib/Threadpool.h"
#include "Engine/engine_shared.h"
#include "CommonLib/DataStructures/GrowingArray.h"
#include "Engine/ModelImporter.h"
#include "Engine/Model.h"
#include "../DL_Debug/DL_Debug.h"
#include <cstdio>
#include "CommonLib/Timer/TimeManager.h"
#include "Engine/Engine.h"

void thread_print(std::string toPrint)
{
	static Ticket_Mutex mutex;
	BeginTicketMutex(&mutex);
	printf("%s\n", toPrint.c_str());
	EndTicketMutex(&mutex);
}


void Export(std::string path);

int main(int argc, const char* argv[])
{
	CU::Timer timer;
	timer.Start();
	std::string targeted_compile;
	if(argc > 1 )
		targeted_compile = argv[1];
	bool isFile = false;
	if(!targeted_compile.empty())
		isFile = targeted_compile.rfind(".") != targeted_compile.npos;

	DL_Debug::Debug::Create();
	std::vector<std::string> folders;

	if(targeted_compile.empty())
		cl::GetFolders(folders, "Data\\Model", true);
	else if(!isFile)
		cl::GetFolders(folders, targeted_compile.c_str(), true);


	std::vector<std::string> exportList;
	if (!isFile)
	{
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
	}

	Threadpool threadpool;
	threadpool.Initiate("exporter");
	if (isFile)
	{
		Export(targeted_compile);
	}
	else
	{
		for (auto& filepath : exportList)
		{
			threadpool.AddWork(Work([=]() {
				Export(filepath);
			}));
		}
	}


	while (threadpool.HasWork() || !threadpool.CurrentWorkFinished())
	{
		threadpool.Update();
	}



	timer.Update();
	const float time = timer.GetTotalTime().GetSeconds();

	char t[100];
	sprintf_s(t, "finished exporting after : %.3fs", time);

	thread_print(t);

	threadpool.CleanUp();
	DL_Debug::Debug::Destroy();

	std::getchar();
	return 0;
}

void Export(std::string path)
{
	std::string str("Starting to load ");
	str += path;
	thread_print(str);
	ModelExporter exporter;
	exporter.SetOldFormat(false);
	CModelImporter importer;
	importer.Init(&exporter);
	Model model;
	importer.LoadModel<Model>(path, &model, nullptr, ModelImportUtil::IGNORE_FILL);
}