#pragma once
#include <map>
#include <unordered_map>
#include "snowblind_shared.h"
#include <DataStructures/GrowingArray.h>
class CModelImporter;
class FileWatcher;
class ShaderFactory;
class Model;
class Effect;
class Texture;
class Sprite;
struct CompiledShader;
class Engine;
class AssetsContainer
{
public:

	enum eRequestType
	{
		MODEL,
		TEXTURE,
		SHADER,
		SPRITE,
	};


	AssetsContainer() = default;
	~AssetsContainer();

	void Initiate();

	void Update();

	void ReloadTexture(Texture* texture);
	Texture* GetTexture(std::string aFilePath);

	Effect* GetEffect(const std::string& aFilePath);
	Model* GetModel(const std::string& aFilePath);
	std::string LoadModel(std::string aFilePath, std::string effect, bool thread = true);

	void AddLoadRequest(std::string file, eRequestType request_type);

private:
	Engine* m_Engine = nullptr;
	FileWatcher* m_TextureWatcher = nullptr;
	Ticket_Mutex m_Mutex;
	std::unordered_map<std::string, Texture*> myTextures;
	std::unordered_map<std::string, Effect*> myEffects;
	std::unordered_map<std::string, Model*> myModels;
	std::unordered_map<std::string, Sprite*> mySprites;

	bool LoadTexture(std::string aFilePath);
	void LoadEffect(const std::string& aFilePath);

	ShaderFactory* m_ShaderFactory;
	CModelImporter* m_ModelLoader;

	struct LoadRequest
	{
		LoadRequest() = default;
		LoadRequest(eRequestType request_type, std::string file) 
			: m_Type(request_type)
			, m_File(file)
		{}
		eRequestType m_Type;
		std::string m_File;
	};

	CU::GrowingArray<LoadRequest> m_RequestList;

};
