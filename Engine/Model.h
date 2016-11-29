#pragma once

#include <vector>

#include "BaseModel.h"
#include "snowblind_shared.h"

struct D3D11_INPUT_ELEMENT_DESC;

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
		bool CleanUp() override;
		CModel* CreateModel(const std::string& filename);
		void Render(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection, bool render_shadows = false) override;

		void AddChild(CModel* aChild);
		void SetIsLightmesh();
		void SetPosition(const CU::Vector3f& aPosition);
		CU::Matrix44f& GetOrientation();
		void SetOrientation(CU::Matrix44f orientation) { myOrientation = orientation; }
		void Update(float dt);

		std::vector<float> GetVertices();
		std::vector<s32> GetIndices();
		CU::Vector3f GetWHD() const { return m_WHD; }
		void SetWHD(CU::Vector3f whd);
	private:
		void InitConstantBuffer();
		CU::GrowingArray<SVertexTypePosCol> myVertices;
		CU::GrowingArray<s32> m_Indices;

		CU::GrowingArray<CSurface*> mySurfaces;
		CU::GrowingArray<CModel*> myChildren;

		CU::Matrix44f myOrientation;
		std::bitset<eModelStates::_COUNT> myModelStates;

		void SetMatrices(const CU::Matrix44f& aCameraOrientation, const CU::Matrix44f& aCameraProjection) override;
	};
}