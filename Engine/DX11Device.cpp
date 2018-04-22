#include "stdafx.h"

#include "DX11Device.h"
#include <Engine/DX11Context.h>

#include <DDSTextureLoader.h>
#include <d3dcompiler.h>
#include <DL_Debug.h>
#include <Engine/IGraphicsAPI.h>
#include <DXTK/WICTextureLoader.h>
#include <DXTex/DirectXTex.h>
namespace graphics
{

	DX11Device::DX11Device(ID3D11Device* device)
		: m_Device(device)
	{
	}

	DX11Device::~DX11Device()
	{
		m_Device->Release();
		m_Device = nullptr;
	}

	IVertexShader* DX11Device::CreateVertexShader(IShaderBlob* pShader, const std::string& debug_name)
	{
		ID3D11VertexShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create vertex shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	IPixelShader* DX11Device::CreatePixelShader(IShaderBlob* pShader, const std::string& debug_name)
	{
		ID3D11PixelShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create pixel shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	IGeometryShader* DX11Device::CreateGeometryShader(IShaderBlob* pShader, const std::string& debug_name)
	{
		ID3D11GeometryShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create geometry shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	IHullShader* DX11Device::CreateHullShader(IShaderBlob* pShader, const std::string& debug_name)
	{
		ID3D11HullShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create hull shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	IDomainShader* DX11Device::CreateDomainShader(IShaderBlob* pShader, const std::string& debug_name)
	{
		ID3D11DomainShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create domain shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}

	IComputeShader* DX11Device::CreateComputeShader(IShaderBlob* pShader, const std::string& debug_name)
	{
		ID3D11ComputeShader* shader = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader);
		HRESULT hr = m_Device->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
#ifndef FINAL
		DL_ASSERT_EXP(hr == S_OK, "Failed to create compute shader!");
		DirectX11::SetDebugName(shader, debug_name);
#endif
		return shader;
	}



	IShaderResourceView* DX11Device::CreateTextureFromFile(const std::string& filepath, bool generate_mips, IGraphicsContext* ctx)
	{
		DX11Context* pCtx = static_cast<DX11Context*>(ctx);

		std::wstring path = cl::ToWideStr(filepath);

		ID3D11ShaderResourceView* srv = nullptr;
		ID3D11Resource* resource = nullptr;

		if (filepath.find(".dds") == filepath.npos)
		{

			DirectX::ScratchImage image;
			if (filepath.find(".tga") != filepath.npos || filepath.find(".TGA") != filepath.npos)
			{
				HRESULT hr = DirectX::LoadFromTGAFile(path.c_str(), nullptr, image);
				assert(hr != S_OK);
				hr = DirectX::CreateTexture(m_Device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &resource);
				assert(hr != S_OK);

				hr = m_Device->CreateShaderResourceView(resource, nullptr, &srv);
				assert(hr != S_OK);

				resource->Release();
				return srv;

			}


			HRESULT hr = DirectX::CreateWICTextureFromFile(m_Device, path.c_str(), &resource, &srv);
#ifndef FINAL
			DirectX11::HandleErrors(hr, "Failed to load texture");
#endif
			resource->Release();
			return srv;
		}

		if (generate_mips)
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFileEx(m_Device
				, static_cast<ID3D11DeviceContext*>(pCtx->GetContext())
				, path.c_str()
				, 0
				, D3D11_USAGE_DEFAULT
				, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET //has to be bound as a render target to actually generate the mips
				, D3D11_CPU_ACCESS_READ
				, D3D11_RESOURCE_MISC_GENERATE_MIPS
				, false
				, &resource //might want to output to a texture2d object?
				, &srv);

#ifndef FINAL
			DL_ASSERT_EXP(hr != S_OK, "Failed to load texture");
#endif
			resource->Release();
			return srv;
		}

		HRESULT hr = DirectX::CreateDDSTextureFromFile(m_Device
			, nullptr
			, path.c_str()
			, &resource //might want to output to a texture2d object?
			, &srv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to load texture");
#endif
		resource->Release();
		return srv;
	}

	IShaderResourceView* DX11Device::CreateTextureFromFile(const std::string& filepath, ITexture2D*& tex_out, bool generate_mips, IGraphicsContext* ctx)
	{
		DX11Context* pCtx = static_cast<DX11Context*>(ctx);

		std::wstring path = cl::ToWideStr(filepath);

		ID3D11ShaderResourceView* srv = nullptr;
		ID3D11Resource* resource = nullptr;

		if (filepath.find(".dds") == filepath.npos)
		{


			DirectX::ScratchImage image;
			if (filepath.find(".tga") != filepath.npos || filepath.find(".TGA") != filepath.npos)
			{
				DirectX::LoadFromTGAFile(path.c_str(), nullptr, image);
				DirectX::CreateTexture(m_Device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), &resource);

				m_Device->CreateShaderResourceView(resource, nullptr, &srv);
				tex_out = resource;
				return srv;

			}


			HRESULT hr = DirectX::CreateWICTextureFromFile(m_Device, path.c_str(), &resource, &srv);
#ifndef FINAL
			DirectX11::HandleErrors(hr, "Failed to load texture");
#endif
			tex_out = resource;
			return srv;
		}

		if (generate_mips)
		{
			HRESULT hr = DirectX::CreateDDSTextureFromFileEx(m_Device
															 , static_cast<ID3D11DeviceContext*>(pCtx->GetContext())
															 , path.c_str()
															 , 0
															 , D3D11_USAGE_DEFAULT
															 , D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET //has to be bound as a render target to actually generate the mips
															 , D3D11_CPU_ACCESS_READ
															 , D3D11_RESOURCE_MISC_GENERATE_MIPS
															 , false
															 , &resource //might want to output to a texture2d object?
															 , &srv);
			tex_out = resource;

#ifndef FINAL
			DL_ASSERT_EXP(hr != S_OK, "Failed to load texture");
#endif
			return srv;
		}

		HRESULT hr = DirectX::CreateDDSTextureFromFile(m_Device
													   , nullptr
													   , path.c_str()
													   , &resource //might want to output to a texture2d object?
													   , &srv);
		tex_out = resource;
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to load texture");
#endif

		return srv;
	}

	ITexture2D* DX11Device::CreateTexture2D(const Texture2DDesc& desc, const std::string& debug_name)
	{
		D3D11_TEXTURE2D_DESC text_desc;
		text_desc.Width = desc.m_Width;
		text_desc.Height = desc.m_Height;
		text_desc.MipLevels = desc.m_MipLevels;
		text_desc.Format = DirectX11::GetFormat(desc.m_Format);
		text_desc.SampleDesc.Count = desc.m_SampleCount;
		text_desc.SampleDesc.Quality = desc.m_SampleQuality;
		text_desc.Usage = DirectX11::GetUsage(desc.m_Usage); //GetUsage
		text_desc.BindFlags = DirectX11::GetBindFlag(desc.m_Binding); //GetBinding
		text_desc.CPUAccessFlags = DirectX11::GetCPUAccessFlag(desc.m_CPUAccessFlag);
		text_desc.MiscFlags = desc.m_MiscFlags;
		text_desc.ArraySize = desc.m_ArraySize;

		ID3D11Texture2D* texture = nullptr;
		HRESULT hr = m_Device->CreateTexture2D(&text_desc, NULL, &texture);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create Texture!");
		DirectX11::SetDebugName(texture, debug_name);
#endif
		return static_cast<ITexture2D*>(texture);
	}

	ITexture2D* DX11Device::CreateTexture2D(const Texture2DDesc& desc, s8* data, s32 pitch, const std::string& debug_name)
	{
		D3D11_TEXTURE2D_DESC text_desc;
		text_desc.Width = desc.m_Width;
		text_desc.Height = desc.m_Height;
		text_desc.MipLevels = desc.m_MipLevels;
		text_desc.Format = DirectX11::GetFormat(desc.m_Format);
		text_desc.SampleDesc.Count = desc.m_SampleCount;
		text_desc.SampleDesc.Quality = desc.m_SampleQuality;
		text_desc.Usage = DirectX11::GetUsage(desc.m_Usage); //GetUsage
		text_desc.BindFlags = DirectX11::GetBindFlag(desc.m_Binding); //GetBinding
		text_desc.CPUAccessFlags = DirectX11::GetCPUAccessFlag(desc.m_CPUAccessFlag);
		text_desc.MiscFlags = desc.m_MiscFlags;
		text_desc.ArraySize = desc.m_ArraySize;


		D3D11_SUBRESOURCE_DATA _data;
		_data.pSysMem = data;
		_data.SysMemPitch = pitch;

		ID3D11Texture2D* texture = nullptr;
		HRESULT hr = m_Device->CreateTexture2D(&text_desc, &_data, &texture);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create Texture!");
		DirectX11::SetDebugName(texture, debug_name);
#endif
		return static_cast<ITexture2D*>(texture);
	}

	IRenderTargetView* DX11Device::CreateRenderTarget(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name)
	{
		ID3D11RenderTargetView* rtv = nullptr;
		ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(pTexture);
		HRESULT hr = m_Device->CreateRenderTargetView(tex, nullptr, &rtv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create RenderTargetView");
		DirectX11::SetDebugName(rtv, debug_name);
#endif
		return rtv;
	}

	IShaderResourceView* DX11Device::CreateShaderResource(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name)
	{
		DL_ASSERT_EXP(pTexture, "pTexture was null!");
		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
		view_desc.Format = DirectX11::GetFormat(desc.m_Format);
		view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipLevels = 1;
		view_desc.Texture2D.MostDetailedMip = 0;

		ID3D11ShaderResourceView* srv = nullptr;
		ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(pTexture);
		HRESULT hr = m_Device->CreateShaderResourceView(tex, &view_desc, &srv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create ShaderResourceView");
		DirectX11::SetDebugName(srv, debug_name);
#endif
		return static_cast<IShaderResourceView*>(srv);
	}

	IShaderResourceView* DX11Device::CreateShaderResource(ITexture2D* pTexture, const std::string& debug_name)
	{
		ID3D11ShaderResourceView* srv = nullptr;
		ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(pTexture);
		HRESULT hr = m_Device->CreateShaderResourceView(tex, nullptr, &srv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create ShaderResourceView");
		DirectX11::SetDebugName(srv, debug_name);
#endif
		return static_cast<IShaderResourceView*>(srv);
	}

	IDepthStencilView* DX11Device::CreateDepthStencilView(const Texture2DDesc& desc, ITexture2D* pTexture, const std::string& debug_name)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depth_desc;
		ZeroMemory(&depth_desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		depth_desc.Format = DirectX11::GetFormat(desc.m_Format);
		depth_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_desc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* dsv = nullptr;
		ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(pTexture);
		HRESULT hr = m_Device->CreateDepthStencilView(tex, &depth_desc, &dsv);
#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create DepthStencilView");
		DirectX11::SetDebugName(dsv, debug_name);
#endif
		return static_cast<IDepthStencilView*>(dsv);
	}

	IInputLayout* DX11Device::CreateInputLayout(CompiledShader* pShader, const InputElementDesc* const pLayout, const s32 element_count)
	{
		D3D11_INPUT_ELEMENT_DESC *desc = new D3D11_INPUT_ELEMENT_DESC[element_count];
		for (s32 i = 0; i < element_count; i++)
		{
			const InputElementDesc& el = pLayout[i];
			D3D11_INPUT_ELEMENT_DESC& desc_el = desc[i];

			desc_el.SemanticName = el.m_Semantic.c_str();
			desc_el.SemanticIndex = el.m_SemanicIndex;
			desc_el.Format = DirectX11::GetFormat(el.m_Format);
			desc_el.InputSlot = el.m_InputSlot;
			desc_el.AlignedByteOffset = el.m_ByteOffset;
			desc_el.InputSlotClass = DirectX11::GetInputClass(el.m_ElementSpecification);
			desc_el.InstanceDataStepRate = el.m_InstanceDataStepRate;
		}


		ID3D11InputLayout* layout = nullptr;
		ID3D10Blob* blob = static_cast<ID3D10Blob*>(pShader->m_Blob);

		HRESULT hr = m_Device->CreateInputLayout(desc, element_count, blob->GetBufferPointer(), blob->GetBufferSize(), &layout);
#ifndef FINAL

		if (FAILED(hr))
		{
			DirectX11::HandleErrors(hr, "Failed to create InputLayout");
			return nullptr;
		}

		DirectX11::SetDebugName(layout, "InputLayout");
#endif
		delete desc;
		return layout;
	}

	IInputLayout* DX11Device::CreateInputLayout(CompiledShader* vertex_shader, const SInputLayout& layout)
	{
		return CreateInputLayout(vertex_shader, &layout.m_Elements[0], layout.m_Elements.Size());
	}

	IBuffer* DX11Device::CreateBuffer(const BufferDesc& buffer_desc, const std::string& debug_name)
	{
		ID3D11Buffer* buffer = nullptr;
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = DirectX11::GetBindFlag(buffer_desc.m_BindFlag);
		desc.CPUAccessFlags = DirectX11::GetCPUAccessFlag(buffer_desc.m_CPUAccessFlag);
		desc.Usage = DirectX11::GetUsage(buffer_desc.m_UsageFlag);
		desc.MiscFlags = buffer_desc.m_MiscFlags;
		desc.StructureByteStride = buffer_desc.m_StructuredByteStride;
		desc.ByteWidth = buffer_desc.m_ByteWidth;


		D3D11_SUBRESOURCE_DATA srd;
		HRESULT hr = S_OK;

		if (buffer_desc.m_Data)
		{
			srd.pSysMem = buffer_desc.m_Data;
			hr = m_Device->CreateBuffer(&desc, &srd, &buffer);
		}
		else
		{
			//DL_WARNING("subresource data is null, this is fine, but the developer might not have intended for it to be null.");
			hr = m_Device->CreateBuffer(&desc, nullptr, &buffer);
		}

#ifndef FINAL
		DirectX11::HandleErrors(hr, "Failed to create buffer!");
		DirectX11::SetDebugName(buffer, debug_name);
#endif

		return buffer;
	}

	IBuffer* DX11Device::CreateConstantBuffer(s32 size, const std::string& debug_name)
	{
		BufferDesc desc;
		desc.m_ByteWidth = size;
		desc.m_UsageFlag = DYNAMIC_USAGE;
		desc.m_BindFlag = BIND_CONSTANT_BUFFER;
		desc.m_CPUAccessFlag = WRITE;
		desc.m_MiscFlags = 0;
		desc.m_StructuredByteStride = 0;

		return CreateBuffer(desc, debug_name);
	}



	IShaderBlob* DX11Device::CompileShaderFromFile(const std::string& filepath, const char* entrypoint, const char* shader_type)
	{
		unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
		//shaderFlag |= D3D10_SHADER_WARNINGS_ARE_ERRORS;
		shaderFlag |= D3D10_SHADER_AVOID_FLOW_CONTROL;
#else
		shaderFlag |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

		ID3D10Blob* out_shader = nullptr;
		ID3D10Blob* out_message = nullptr;

		std::string feature_level(shader_type);
		feature_level += "_5_0";

		std::string path = filepath.c_str();

		std::wstring wPath(path.begin(), path.end());

		const D3D_SHADER_MACRO defines[] = {
#if !defined(_PROFILE) && !defined(_FINAL)
			"_DEBUG", "1",
#else
			"_FINAL", "1",
#endif
			0, 0
		};

		HRESULT hr = D3DCompileFromFile(
			wPath.c_str(),
			defines,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint,
			feature_level.c_str(),
			shaderFlag,
			0,
			&out_shader,
			&out_message);

		if (out_message != nullptr)
		{
			DL_WARNING("%s has generated warnings!", filepath.c_str());
			DL_WARNING("\n%s", (char*)out_message->GetBufferPointer());
		}

		DirectX11::HandleErrors(hr, "Failed to compile shader from file");
		return out_shader;

	}

	IShaderBlob* DX11Device::CompileShaderFromMemory(const s8* pData, s32 data_byte_size, const std::string& source_name, const char* entrypoint, const char* shader_type)
	{
		unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
		shaderFlag |= D3D10_SHADER_DEBUG;
		shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#else
		shaderFlag |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif

		std::string feature_level(shader_type);
		feature_level += "_5_0";

		ID3D10Blob* out_shader = nullptr;
		ID3D10Blob* out_message = nullptr;
		HRESULT hr = D3DCompile(
			pData,
			data_byte_size,
			source_name.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint,
			feature_level.c_str(),
			shaderFlag,
			0,
			&out_shader,
			&out_message);

		DirectX11::HandleErrors(hr, "Failed to compile shader from memory");
		if (out_message != nullptr)
		{
			DL_WARNING("%s has generated warnings!", source_name.c_str());
			DL_WARNING("\n%s", (char*)out_message->GetBufferPointer());
		}


		return out_shader;
	}

	bool DX11Device::FindInputSemantic(const char* to_find, IShaderBlob* blob)
	{
		ID3D11ShaderReflection* pReflector = nullptr;
		ID3D10Blob* pBlob = static_cast<ID3D10Blob*>(blob);
		D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pReflector);
		D3D11_SHADER_DESC shader_desc;

		pReflector->GetDesc(&shader_desc);
		s32 input_count = shader_desc.InputParameters;

		const u64 compare0 = Hash("INSTANCE");
		for (s32 i = 0; i < shader_desc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC param_desc;
			pReflector->GetInputParameterDesc(i, &param_desc);
			const u64 semantic_hash = Hash(param_desc.SemanticName);
			if (semantic_hash == compare0)
				return true;
		}

		return false;
	}





};