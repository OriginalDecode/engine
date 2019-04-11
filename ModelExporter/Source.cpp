#define _MODEL_EXPORTER

#include "DL_Debug/DL_Debug.h"

#include "CommonLib/Threadpool.h"
#include "CommonLib/Timer/Timer.h"
#include "CommonLib/Utilities.h"

#include "Engine/engine_shared.h"
#include "Engine/ModelImporter.h"
#include "Engine/ModelExporter.h"
#include "Engine/Model.h"

#include <cstdio>
#include <vector>

std::vector<std::string> g_StatusText;

void PrintStatus();
int ThreadVectorPushBack(std::string toPrint);
void Export(std::string path);

int main(int argc, const char* argv[])
{
	const bool isThreaded = true;

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
				if (file.find(".fbx") != file.npos)
				{
					std::string filepath(folder);
					filepath += "\\" + file;
					exportList.push_back(filepath);
				}
			}
		}
	}
	g_StatusText.reserve(exportList.size());
	Threadpool threadpool;
	threadpool.Initiate("exporter");
	if (isFile || !isThreaded)
	{
		Export(targeted_compile);
	}
	else
	{
		for (auto& filepath : exportList)
		{
			threadpool.AddWork(Work([=]() {
				std::string str("Starting to load ");
				str += filepath;
				int index = ThreadVectorPushBack(str);
				Export(filepath);
				std::string finished("Finished loading ");
				finished += filepath;
				g_StatusText[index] = finished;
				PrintStatus();
				
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
	printf("%s\n", t);

	threadpool.CleanUp();
	DL_Debug::Debug::Destroy();

	std::getchar();
	return 0;
}

void Export(std::string path)
{
	ModelExporter exporter;
	exporter.SetOldFormat(false);
	CModelImporter importer;
	importer.Init(&exporter);
	Model model;
	importer.LoadModel<Model>(path, &model, nullptr, ModelImportUtil::IGNORE_FILL);
}

void PrintStatus()
{
	system("cls");
	for (const std::string& str : g_StatusText)
	{
		printf("%s\n", str.c_str());
	}
}

int ThreadVectorPushBack(std::string toPrint)
{
	static Ticket_Mutex mutex;
	BeginTicketMutex(&mutex);

	int index = -1;
	g_StatusText.push_back(toPrint);
	index = g_StatusText.size() - 1;
	PrintStatus();

	EndTicketMutex(&mutex);
	return index;
}
