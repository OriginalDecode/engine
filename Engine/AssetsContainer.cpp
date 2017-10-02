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

#ifndef FINAL
	m_Watcher = new FileWatcher;
#endif
}

Texture* AssetsContainer::GetTexture(const cl::HashString& path)
{
// 	if (path.contains(".dds") == false)
// 	{
// 		DL_MESSAGE("Failed to load %s, due to incorrect fileformat. Has to be .dds", path.c_str());
// 		DL_ASSERT("Failed to Load Texture, format not .dds. See log for more information.");
// 		return nullptr;
// 	}

	if (!LoadTexture(path))
		return nullptr;

	return m_Textures[path.GetHash()];
}



Effect* AssetsContainer::GetEffect(const cl::HashString& path)
{
	if (m_Effects.find(path.GetHash()) == m_Effects.end())
	{
		LoadEffect(path);
	}
	return m_Effects[path.GetHash()];
}

Model* AssetsContainer::GetModel(const cl::HashString& path)
{
	auto it = m_Models.find(path.GetHash());
	if (it == m_Models.end())
	{
		DL_MESSAGE("Requested Model : %s", path.c_str());
		DL_ASSERT("Failed to find requested model. Did you enter the correct path?");
	}
	return it->second;
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
	//texture->OnReload();
}

bool AssetsContainer::LoadTexture(const cl::HashString& filepath)
{
	BeginTicketMutex(&m_Mutex);
	if (m_Textures.find(filepath.GetHash()) == m_Textures.end())
	{
		graphics::IGraphicsDevice& device = Engine::GetAPI()->GetDevice();
		IShaderResourceView* srv = device.CreateTextureFromFile(filepath, false, &Engine::GetAPI()->GetContext());
		if (srv != nullptr)
		{
			Texture* texture = new Texture(srv);
			m_Textures[filepath.GetHash()] = texture;
			EndTicketMutex(&m_Mutex);
			return true;
		}
	}
	EndTicketMutex(&m_Mutex);
	return false;
}

Effect* AssetsContainer::LoadEffect(const cl::HashString& filepath)
{
	Effect* effect = new Effect(filepath.c_str());
	m_ShaderFactory->LoadShader(effect);
	m_Effects[filepath.GetHash()] = effect;
	return m_Effects[filepath.GetHash()];
}

Sprite* AssetsContainer::LoadSprite(const cl::HashString& path)
{
	Sprite* sprite = new Sprite;
	m_Sprites.emplace(path.GetHash(), sprite);
	sprite->Initiate(path);
	return sprite;
}

Sprite* AssetsContainer::GetSprite(const cl::HashString& path)
{
	for (auto it = m_Sprites.begin(); it != m_Sprites.end(); it++)
	{
		if (it->first != path.GetHash())
			continue;

		return it->second;
	}
	return LoadSprite(path);
}
