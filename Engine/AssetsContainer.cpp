#include "stdafx.h"
#include "AssetsContainer.h"
#include "Model.h"
#include "Texture.h"
#include "Effect.h"
#include "ShaderFactory.h"
#include "Texture.h"
#include <Engine/Sprite.h>

AssetsContainer::~AssetsContainer()
{
	SAFE_DELETE(m_ShaderFactory);
	SAFE_DELETE(m_ModelLoader);
	
	for (auto it = m_Models.begin(); it != m_Models.end(); it++)
		SAFE_DELETE(it->second);

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
	m_ModelLoader = new CModelImporter;
	m_Textures.empty();
	m_Effects.empty();
	m_Models.empty();
	m_Sprites.empty();

// 	u64 hash = Hash("default_cube");
// 	auto it = m_Models.emplace(hash , new Model);
// 	it.first->second->CreateCube();

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

Model* AssetsContainer::GetModel(u64 key)
{
	auto it = m_Models.find(key);

	if (it != m_Models.end())
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

u64 AssetsContainer::LoadTexture(const std::string& filepath)
{
	static Ticket_Mutex texture_mutex;
	BeginTicketMutex(&texture_mutex);

	u64 hash = Hash(filepath.c_str());

	if (m_Textures.find(hash) == m_Textures.end())
	{
		graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
		IShaderResourceView* srv = device.CreateTextureFromFile(filepath, false, &Engine::GetAPI()->GetContext());
		if (srv != nullptr)
		{
			Texture* texture = new Texture(srv);
			m_Textures.emplace(hash, texture);
			EndTicketMutex(&texture_mutex);
			return Hash(filepath.c_str());
		}
	}
	EndTicketMutex(&texture_mutex);
	return Hash(filepath.c_str());
}

u64 AssetsContainer::LoadEffect(const std::string& filepath)
{
	static Ticket_Mutex effect_mutex;
	BeginTicketMutex(&effect_mutex);

	u64 hash = Hash(filepath.c_str());
	if (m_Effects.find(hash) == m_Effects.end())
	{
		Effect* effect = new Effect(filepath);
		m_Effects.emplace(hash, effect);
		m_ShaderFactory->LoadShader(effect);
		EndTicketMutex(&effect_mutex);
		return Hash(filepath.c_str());
	}

	EndTicketMutex(&effect_mutex);
	return Hash(filepath.c_str());
}

u64 AssetsContainer::LoadSprite(const std::string& path)
{
	static Ticket_Mutex sprite_mutex;
	BeginTicketMutex(&sprite_mutex);

	u64 hash = Hash(path.c_str());
	if (m_Sprites.find(hash) == m_Sprites.end())
	{
		Sprite* sprite = new Sprite;
		m_Sprites.emplace(hash, sprite);
		sprite->Initiate(path);
		EndTicketMutex(&sprite_mutex);
		return Hash(path.c_str());
	}

	EndTicketMutex(&sprite_mutex);
	return Hash(path.c_str());
}

