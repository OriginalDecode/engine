#pragma once


#include <DataStructures/GrowingArray.h>

class Camera;
class DirectX11;
class Engine;
class Texture;

class Atmosphere
{
public:
	Atmosphere() = default;


	void Initiate(float inner_radius, float outer_radius, const CU::Vector3f& position );
	void CleanUp();

	void Render(const CU::Matrix44f& orientation, Texture* depth);
	void SetLightData(const CU::Vector4f& direction, const CU::Vector4f& position);
	void UseConstantSolarSpectrum(bool useconstantspec);
private:
	Engine* m_Engine							= nullptr;
	DirectX11* m_API							= nullptr;
	Camera* m_Camera							= nullptr;
	CModel* m_InnerSphere						= nullptr;
	CModel* m_OuterSphere						= nullptr;

	float m_InnerRadius = 0.f;
	float m_OuterRadius = 0.f;
	CU::Matrix44f m_InnerOrientation;
	CU::Matrix44f m_OuterOrientation;

	struct cbPixel
	{
		CU::Vector4f m_CameraDir;
		CU::Vector4f m_CameraPos;
		CU::Vector4f m_LightDir;
		CU::Vector4f m_LightPos;
		float m_InnerRadius;
		float m_OuterRadius;
		float m_CameraMagnitude;
		float m_CameraMagnitude2;
	} m_PixelStruct;
	IBuffer* m_PixelBuffer						= nullptr;

	struct cbVertex
	{
		CU::Vector4f m_CameraDir;
		CU::Vector4f m_CameraPos;
		CU::Vector4f m_LightDir;
		CU::Vector4f m_LightPos;
		float m_InnerRadius;
		float m_OuterRadius;
		float m_CameraMagnitude;
		float m_CameraMagnitude2;
	} m_VertexStruct;
	IBuffer* m_VertexBuffer						= nullptr;

	bool m_UseConstantSolarSpectrum				= false;

	Texture* m_TransmittanceTexture					= nullptr;
	Texture* m_ScatteringTexture					= nullptr;
	Texture* m_OptionalSingleMieScatteringTexture	= nullptr;
	Texture* m_IrradianceTexture					= nullptr;

	std::string m_ShaderHeader;

	CompiledShader m_AtmosphereShader;


	void UpdateCameraData();

	void ComputeScattering(const CU::GrowingArray<double>& wavelengths, const CU::GrowingArray<double>& wavelengths_function, double lambda_power, double* red, double* green, double* blue);
	double Interpolate(const CU::GrowingArray<double>& wavelengths, const CU::GrowingArray<double>& wavelengths_function, double wavelength);
	double CieColorMatchingFunctionTableValue(double wavelength, int column);
};

