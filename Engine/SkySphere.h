#pragma once
#include "snowblind_shared.h"
class CModel;
class Effect;
class Camera;
class DirectX11;

struct SkysphereLayer
{
	CModel* m_Model;
	Texture* m_Texture;
	bool m_ShouldRotate = false;
};

class SkySphere
{
public:
	SkySphere() = default;
	bool Initiate(const std::string& model_filepath, const std::string& skysphere_shader, Camera* camera);
	bool AddLayer(const std::string& layer_filepath, const std::string& layer_shader);
	bool CleanUp();
	void Render(CU::Matrix44f& anOrientation, Texture* aDepthTexture);
	void SetPosition(const CU::Vector3f& aPosition);
	void SetLightDir(const CU::Vector3f& dir);
	void SetLightPos(const CU::Vector4f& light_position);
	//void Update(float dt);
private:
	Camera* myCamera;
#ifdef SNOWBLIND_DX11
	DirectX11* myAPI;
#else
	Vulkan* myAPI;
#endif
	CU::Matrix44f myOrientation;
	CU::GrowingArray<SkysphereLayer> m_Layers;

	struct cbPixelShader
	{
		CU::Vector4f m_CameraPos;
		CU::Vector4f m_CameraDir;
		CU::Vector4f m_LightPos;
		CU::Vector4f m_LightDir;

	} m_PixelShaderStruct;

	struct cbVertexShader
	{
		CU::Vector4f m_CameraPos;
		CU::Vector4f m_CameraDir;
		CU::Vector4f m_LightDir;
		CU::Vector4f m_LightPos;
		float m_CameraMagnitude2;
		float m_CameraMagnitude;
		float padding[2];
	} m_VertexShaderStruct;



	IBuffer* m_cbPixelShader = nullptr;
	IBuffer* m_cbVertexShader = nullptr;


};
