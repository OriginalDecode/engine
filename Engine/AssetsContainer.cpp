#include "stdafx.h"
#include "AssetsContainer.h"
#include "Model.h"
#include "Texture.h"
#include "Effect.h"
#include "ShaderFactory.h"
#include "Texture.h"
#include <Engine/Sprite.h>
#include <Engine/AssetFactory.h>

AssetsContainer::~AssetsContainer()
{
	SAFE_DELETE(m_ShaderFactory);
	
	for (auto it = m_Textures.begin(); it != m_Textures.end(); it++)
		SAFE_DELETE(it->second);

	for (auto it = m_Sprites.begin(); it != m_Sprites.end(); it++)
		SAFE_DELETE(it->second);

	for (auto it = m_Effects.begin(); it != m_Effects.end(); it++)
		SAFE_DELETE(it->second);

#ifndef FINAL
	SAFE_DELETE(m_Watcher);
#endif

}

void AssetsContainer::Initiate()
{
	m_ShaderFactory = new ShaderFactory;
	m_Textures.empty();
	m_Effects.empty();
	m_Models.empty();
	m_Sprites.empty();

	HashType mod_key = LoadModel<Model>("data/engineassets/cube_100x100.fbx", "Shaders/deferred_base.json", false);
	mod_key = LoadModel<Model>("data/exported/cube_100x100.LPMF", "Shaders/debug_pbl_instanced.json", false);
	u64 mat_key = LoadMaterial("Data/Material/mat_default.json");


	RefPointer<Model> model = GetModel<Model>(mod_key.m_Hash);
	Material* pMaterial = GetMaterial(mat_key);


	m_Models.emplace(cl::Hash("default"), model);
	m_Materials.emplace(cl::Hash("default"), pMaterial);
#ifndef FINAL
	m_Watcher = new FileWatcher;
#endif
}

Texture* AssetsContainer::GetTexture(u64 key)
{
	auto it = m_Textures.find(key);

	if (it != m_Textures.end())
		return it->second;

	return nullptr;
}

Effect* AssetsContainer::GetEffect(u64 key)
{
	auto it = m_Effects.find(key);

	if (it != m_Effects.end())
		return it->second;

	return nullptr;
}



Material* AssetsContainer::GetMaterial(u64 key)
{
	auto it = m_Materials.find(key);
	if (it != m_Materials.end())
		return it->second;

	return nullptr;
}

Model* AssetsContainer::GetModelDirect(u64 key)
{
	return GetModel<Model>(key).GetData();
}

Sprite* AssetsContainer::GetSprite(u64 key)
{
	auto it = m_Sprites.find(key);

	if (it != m_Sprites.end())
		return it->second;

	return nullptr;
}

void AssetsContainer::Update()
{
	m_ShaderFactory->Update();
#ifndef FINAL
	m_Watcher->FlushChanges();
#endif
}

void AssetsContainer::ReloadTexture(Texture* texture)
{
}

void AssetsContainer::AddTexture(Texture* pTexture, u64 key)
{
	if (m_Textures.find(key) == m_Textures.end())
	{
		m_Textures.emplace(key, pTexture);
		//m_Textures.insert(std::pair<u64, Texture*>(key, pTexture));
	}
}

u64 AssetsContainer::LoadTexture(std::string filepath, bool make_mips)
{
	static Ticket_Mutex texture_mutex;
	BeginTicketMutex(&texture_mutex);

	u64 hash = cl::Hash(filepath.c_str());

	if (m_Textures.find(hash) == m_Textures.end())
	{
		graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
		ITexture2D* tex = nullptr;
		IShaderResourceView* srv = device.CreateTextureFromFile(filepath, tex, make_mips, &Engine::GetAPI()->GetContext());
		if (srv != nullptr)
		{
			Texture* texture = nullptr;
			
#ifdef _DEBUG
			if (tex)
				texture = new Texture(srv, tex, filepath);
			else
				texture = new Texture(srv, filepath);
#else
			if (tex)
				texture = new Texture(srv, tex);
			else
				texture = new Texture(srv);
#endif
			m_Textures.emplace(hash, texture);
			EndTicketMutex(&texture_mutex);
			return hash;
		}
	}
	EndTicketMutex(&texture_mutex);
	return hash;
}

u64 AssetsContainer::LoadEffect(std::string filepath)
{
	static Ticket_Mutex effect_mutex;
	BeginTicketMutex(&effect_mutex);

	u64 hash = cl::Hash(filepath.c_str());
	if (m_Effects.find(hash) == m_Effects.end())
	{
		Effect* effect = new Effect(filepath);
		m_Effects.emplace(hash, effect);
		m_ShaderFactory->LoadShader(effect);
		EndTicketMutex(&effect_mutex);
		return hash;
	}

	EndTicketMutex(&effect_mutex);
	return hash;
}

u64 AssetsContainer::LoadSprite(std::string path)
{
	static Ticket_Mutex sprite_mutex;
	BeginTicketMutex(&sprite_mutex);

	u64 hash = cl::Hash(path.c_str());
	if (m_Sprites.find(hash) == m_Sprites.end())
	{
		Sprite* sprite = new Sprite;
		m_Sprites.emplace(hash, sprite);
		sprite->Initiate(path);
		EndTicketMutex(&sprite_mutex);
		return hash;
	}

	EndTicketMutex(&sprite_mutex);
	return hash;
}

u64 AssetsContainer::LoadMaterial(std::string path)
{
	static Ticket_Mutex material_mutex;
	BeginTicketMutex(&material_mutex);

	u64 hash = cl::Hash(path.c_str());
	if (m_Materials.find(hash) == m_Materials.end())
	{
		Material* material = new Material(hash);
		m_Materials.emplace(hash, material);
		AssetFactory::GetInstance().CreateMaterial(path, material);
#ifdef _DEBUG
		//debug::DebugHandle::GetInstance()->RegisterMaterial(material, path);
#endif
		EndTicketMutex(&material_mutex);
		return hash;
	}
	EndTicketMutex(&material_mutex);
	return hash;
}

CompiledShader* AssetsContainer::GetShader(const char* key) const
{
	return m_ShaderFactory->GetShader(cl::Hash(key));
}

ShaderFactory* AssetsContainer::GetShaderFactory()
{
	return m_ShaderFactory;
}
