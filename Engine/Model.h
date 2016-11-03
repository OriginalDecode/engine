#pragma once
#include "BaseModel.h"
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
struct ID3D11Buffer;

enum eModelStates : int
{
	LIGHT_MESH,
	_COUNT
};
namespace Snowblind
{
static Ticket_Mutex g_ModelMutex;
	class CModel : public CBaseModel
	{
		friend class CModelImporter;
	public:
		CModel();
		~CModel();
		CModel* CreateModel();
		void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;

		void AddChild(CModel* aChild);
		void SetIsLightmesh();
		void SetPosition(const CU::Vector3f& aPosition);
		CU::Matrix44f& GetOrientation();
		void SetOrientation(CU::Matrix44f orientation) { myOrientation = orientation; }
		void Update(float dt);
	private:
		void InitConstantBuffer();
		CU::GrowingArray<SVertexTypePosCol> myVertices;

		CU::GrowingArray<CSurface*> mySurfaces;
		CU::GrowingArray<CModel*> myChildren;

		CU::Matrix44f myOrientation;

		std::bitset<eModelStates::_COUNT> myModelStates;

		void SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;
	};
}