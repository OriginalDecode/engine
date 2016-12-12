#pragma once
#include "VertexStructs.h"
#include <Math/Matrix/Matrix44.h>
#include <DataStructures/GrowingArray.h>


struct D3D11_INPUT_ELEMENT_DESC;
typedef struct ID3D11InputLayout IInputLayout;
typedef struct ID3D11Buffer IBuffer;
typedef struct ID3D11DeviceContext IDevContext;

namespace Snowblind
{

	class DirectX11;
	class Effect;
	class Engine;
	class CSurface;

	struct VertexIndexWrapper;
	struct VertexBufferWrapper;
	struct VertexDataWrapper;
	struct IndexBufferWrapper;

	class CBaseModel
	{
	public:
		CBaseModel();
		virtual ~CBaseModel() = 0;
		virtual bool CleanUp() = 0;
		virtual void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const CU::Vector4f& scale, bool render_shadows = false);
		void SetEffect(Effect* anEffect);
	protected:
		std::string m_Filename;
		void InitVertexBuffer();
		void InitIndexBuffer();
		virtual void InitConstantBuffer() = 0;
		virtual void UpdateConstantBuffer(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, const CU::Vector4f& scale) = 0;
		Engine* myEngine = nullptr;
		CU::Vector3f m_WHD;

		Effect* myEffect = nullptr;
		IDevContext* myContext = nullptr;
#ifdef SNOWBLIND_DX11
		DirectX11* myAPI = nullptr;
#else
		Vulkan* myAPI = nullptr;
#endif
		IInputLayout* myVertexLayout = nullptr;
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myVertexFormat;

		VertexIndexWrapper* myIndexData = nullptr;
		VertexDataWrapper* myVertexData = nullptr;

		VertexBufferWrapper* myVertexBuffer = nullptr;
		IndexBufferWrapper* myIndexBuffer = nullptr;

		IBuffer* myConstantBuffer = nullptr;
		SVertexBaseStruct* myConstantStruct = nullptr;


		bool myIsNULLObject = true;
		bool myIsLightMesh = false;
	};
};