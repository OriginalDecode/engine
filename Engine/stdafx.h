#pragma once
#pragma warning(disable : 4005)
#pragma warning(disable : 4006)
#pragma warning(disable : 4099)
#ifdef SNOWBLIND_DX11
#include <d3dcompiler.h>
#include <DDSTextureLoader.h>
#endif

//----------------------------------------
// Common Utilities / Common Library
//----------------------------------------

#include <JSON/JSONReader.h>
#include <Math/Matrix/Matrix.h>
#include <Utilities.h>
#include <Randomizer.h>

//----------------------------------------
// DL_Debug
//----------------------------------------

#include <DL_Debug.h>

//----------------------------------------
// Engine
//----------------------------------------

#include "AssetsContainer.h"
#include "Camera.h"
#include "DeferredRenderer.h"
#include "DirectX11.h"
#include "Effect.h"
#include "Engine.h"
#include "EngineDefines.h"
#include "EmitterInstance.h"
#include "FileWatcher.h"
#include "Font.h"
#include "FontManager.h"
#include "IndexWrapper.h"
#include "Model.h"
#include "ShaderFactory.h"
#include "Text.h"
#include "Texture.h"
#include "VertexStructs.h"
#include "VertexTypes.h"
#include "VertexWrapper.h"
#include "snowblind_shared.h"



