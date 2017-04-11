#include "stdafx.h"
#include "Atmosphere.h"
#include <Engine/Engine.h>
#include <Engine/DirectX11.h>
#include <Engine/Model.h>
#include <Engine/AtmosphereConstants.h>
#include <Engine/Effect.h>

#include <string.h>


void Atmosphere::Initiate(float inner_radius, float outer_radius, const CU::Vector3f& position)
{
	m_Engine = Engine::GetInstance();
	m_API = m_Engine->GetAPI();
	m_Camera = m_Engine->GetCamera();

	m_OuterRadius = outer_radius;
	m_OuterOrientation.SetPosition(position);

	m_InnerRadius = inner_radius;
	m_InnerOrientation.SetPosition(position);

	m_VertexBuffer = m_API->CreateConstantBuffer(sizeof(cbVertex));
	m_PixelBuffer = m_API->CreateConstantBuffer(sizeof(cbPixel));

	m_OuterOrientation = CU::Matrix44f::CreateScaleMatrix(CU::Vector4f(m_OuterRadius, m_OuterRadius, m_OuterRadius, 1)) * m_OuterOrientation;
	m_InnerOrientation = CU::Matrix44f::CreateScaleMatrix(CU::Vector4f(m_InnerRadius, m_InnerRadius, m_InnerRadius, 1)) * m_InnerOrientation;

	const VirtualFileSystem& vfs = m_Engine->GetVFS();
	std::string atmosphere = m_Engine->LoadModel(vfs.GetFile("Models/atmosphere.fbx"), "Shaders/T_Skysphere.json", false);

	m_InnerSphere = m_Engine->GetModel(atmosphere);
	m_OuterSphere = m_Engine->GetModel(atmosphere);
	m_InnerSphere->SetIsSkysphere(true);
	m_OuterSphere->SetIsSkysphere(true);


	m_VertexStruct.m_InnerRadius = m_InnerRadius;
	m_VertexStruct.m_OuterRadius = m_OuterRadius;

	m_PixelStruct.m_InnerRadius = m_InnerRadius;
	m_PixelStruct.m_OuterRadius = m_OuterRadius;

	m_InnerSphere->SetOrientation(m_InnerOrientation);
	m_OuterSphere->SetOrientation(m_OuterOrientation);


	constexpr double kSolarIrradiance[] = {
		1.11776, 1.14259, 1.01249, 1.14716, 1.72765, 1.73054, 1.6887, 1.61253,
		1.91198, 2.03474, 2.02042, 2.02212, 1.93377, 1.95809, 1.91686, 1.8298,
		1.8685, 1.8931, 1.85149, 1.8504, 1.8341, 1.8345, 1.8147, 1.78158, 1.7533,
		1.6965, 1.68194, 1.64654, 1.6048, 1.52143, 1.55622, 1.5113, 1.474, 1.4482,
		1.41018, 1.36775, 1.34188, 1.31429, 1.28303, 1.26758, 1.2367, 1.2082,
		1.18737, 1.14683, 1.12362, 1.1058, 1.07124, 1.04992
	};
	constexpr s32 array_size = sizeof(kSolarIrradiance) / sizeof(double);

	CU::GrowingArray<double> wavelengths;
	CU::GrowingArray<double> solar_irradiance;
	CU::GrowingArray<double> rayleigh_scattering;
	CU::GrowingArray<double> mie_scattering;
	CU::GrowingArray<double> mie_extinction;
	CU::GrowingArray<double> ground_albedo; //This should not be needed.

	for ( s32 l = kLambdaMin; l <= kLambdaMax; l += 10 )
	{

		double lambda = ( double ) l * 1e-3; // µ meters
		double mie = kMieAngstromBeta / kMieScaleHeight * pow(lambda, -kMieAngstromAlpha);
		wavelengths.Add(l);

		if ( m_UseConstantSolarSpectrum )
		{
			solar_irradiance.Add(kConstantSolarIrradiance);
		}
		else
		{
			solar_irradiance.Add(kSolarIrradiance[( l - kLambdaMin ) / 10]);
		}

		rayleigh_scattering.Add(kRayleigh * pow(lambda, -4));
		mie_scattering.Add(mie * kMieSingleScatteringAlbedo);
		mie_extinction.Add(mie);
		ground_albedo.Add(kGroundAlbedo);
	}

	auto to_string = [=, &wavelengths](const CU::GrowingArray<double>& v, double scale)
	{
		double r = Interpolate(wavelengths, v, kLambdaR) * scale;
		double g = Interpolate(wavelengths, v, kLambdaG) * scale;
		double b = Interpolate(wavelengths, v, kLambdaB) * scale;
		return "float3(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ")";
	};

	double sky_red, sky_green, sky_blue;
	ComputeScattering(wavelengths, solar_irradiance, -3, &sky_red, &sky_green, &sky_blue);

	double sun_red, sun_green, sun_blue;
	ComputeScattering(wavelengths, solar_irradiance, 0, &sun_red, &sun_green, &sun_blue);

	using namespace atmosphere;

	m_ShaderHeader =
		"#define IN(x) const x\n"
		"#define OUT(x) x\n"
		"#define TEMPLATE(x)\n"
		"#define TEMPLATE_ARGUMENT(x)\n"
		"#define assert(x)\n"
		"static const int TRANSMITTANCE_TEXTURE_WIDTH = " +
		std::to_string(TRANSMITTANCE_TEXTURE_WIDTH) + ";\n" +
		"static const int TRANSMITTANCE_TEXTURE_HEIGHT = " +
		std::to_string(TRANSMITTANCE_TEXTURE_HEIGHT) + ";\n" +
		"static const int SCATTERING_TEXTURE_R_SIZE = " +
		std::to_string(SCATTERING_TEXTURE_R_SIZE) + ";\n" +
		"static const int SCATTERING_TEXTURE_MU_SIZE = " +
		std::to_string(SCATTERING_TEXTURE_MU_SIZE) + ";\n" +
		"static const int SCATTERING_TEXTURE_MU_S_SIZE = " +
		std::to_string(SCATTERING_TEXTURE_MU_S_SIZE) + ";\n" +
		"static const int SCATTERING_TEXTURE_NU_SIZE = " +
		std::to_string(SCATTERING_TEXTURE_NU_SIZE) + ";\n" +
		"static const int IRRADIANCE_TEXTURE_WIDTH = " +
		std::to_string(IRRADIANCE_TEXTURE_WIDTH) + ";\n" +
		"static const int IRRADIANCE_TEXTURE_HEIGHT = " +
		std::to_string(IRRADIANCE_TEXTURE_HEIGHT) + ";\n" +
		"#define COMBINED_SCATTERING_TEXTURES\n" +
		shader_definitions +
		//"static AtmosphereParameters atmosphere_parameters;\n" +
		"static const float3 SOLAR_IRRADIANCE = " + to_string(solar_irradiance, 1.0) + ";\n" +
		"static const float SUN_ANGULAR_RADIUS = " + std::to_string(kSunAngularRadius) + ";\n" +
		"static const float BOTTOM_RADIUS = " + std::to_string(kBottomRadius / kUnitLengthInMeters) + ";\n" +
		"static const float TOP_RADIUS = " + std::to_string(kTopRadius / kUnitLengthInMeters) + ";\n" +
		"static const float RAYLEIGH_SCALE_HEIGHT = " + std::to_string(kRayleighScaleHeight / kUnitLengthInMeters) + ";\n" +
		"static const float3 RAYLEIGH_SCATTERING = " + to_string(rayleigh_scattering, kUnitLengthInMeters) + ";\n" +
		"static const float MIE_SCALE_HEIGHT = " + std::to_string(kMieScaleHeight / kUnitLengthInMeters) + ";\n" +
		"static const float3 MIE_SCATTERING = " + to_string(mie_scattering, kUnitLengthInMeters) + ";\n" +
		"static const float3 MIE_EXTINCTION = " + to_string(mie_extinction, kUnitLengthInMeters) + ";\n" +
		"static const float MIE_PHASE_FUNCTION_G = " + std::to_string(kMiePhaseFunctionG) + ";\n" +
		"static const float3 GROUND_ALBEDO = " + to_string(ground_albedo, 1.0) + ";\n" +
		"static const float MU_S_MIN = " + std::to_string(kMaxSunZenithAngle) + ";\n" +
		"static const float3 SKY_SPECTRAL_RADIANCE_TO_LUMINANCE = float3(" +
		std::to_string(sky_red) + "," +
		std::to_string(sky_green) + "," +
		std::to_string(sky_blue) + ");\n" +
		"static const float3 SUN_SPECTRAL_RADIANCE_TO_LUMINANCE = float3(" +
		std::to_string(sun_red) + "," +
		std::to_string(sun_green) + "," +
		std::to_string(sun_blue) + ");\n" +
		shader_functions + 
		shader_main;

	m_TransmittanceTexture = new Texture;
	m_TransmittanceTexture->Initiate(TRANSMITTANCE_TEXTURE_WIDTH, TRANSMITTANCE_TEXTURE_HEIGHT,
		DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "Atmosphere : TransmittanceTexture");

	m_ScatteringTexture = new Texture;
	m_ScatteringTexture->Initiate3DTexture(SCATTERING_TEXTURE_WIDTH, SCATTERING_TEXTURE_HEIGHT, SCATTERING_TEXTURE_DEPTH,
		DXGI_FORMAT_R16G16B16A16_FLOAT, "Atmosphere : ScatteringTexture");
	/*
		m_OptionalSingleMieScatteringTexture = new Texture;
		m_ScatteringTexture->Initiate3DTexture(SCATTERING_TEXTURE_WIDTH, SCATTERING_TEXTURE_HEIGHT, SCATTERING_TEXTURE_DEPTH,
			DXGI_FORMAT_R16G16B16A16_FLOAT, "Atmosphere : ScatteringTexture");
	*/

	m_IrradianceTexture = new Texture;
	m_IrradianceTexture->Initiate(IRRADIANCE_TEXTURE_WIDTH, IRRADIANCE_TEXTURE_HEIGHT,
		DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "Atmosphere : IrradianceTexture");


	std::string shader = m_ShaderHeader + kAtmosphereShader;

	std::stringstream shader_name;
	shader_name << "kAtmosphereShader" << ".out";
	std::ofstream out(shader_name.str());

	out << shader;
	out.flush();
	out.close();

	const s8* source = shader.c_str();
	const s32 shader_length = shader.length();
	const s32 size =  shader_length * 8;
	IBlob* compiled = nullptr;
	IBlob* message = nullptr;
	//HRESULT hr = D3DCompile(source,size, "plebcity",nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,	"main", "ps_5_0",	0,	0, &compiled, &message);
	HRESULT hr = m_Engine->CompileShaderFromFile(shader_name.str(), "main", "ps_5_0", 0, compiled, message);
	//HRESULT hr = m_Engine->CompileShaderFromMemory(source, size, "atmosphere_initiate", "main", "ps_5_0", 0, compiled, message);

	if ( message )
	{
		DL_WARNING("%s", ( char* ) message->GetBufferPointer());
		DL_WARNINGBOX(( char* ) message->GetBufferPointer());
	}

	IPixelShader* pixelshader = nullptr;
	hr = m_API->GetDevice()->CreatePixelShader(compiled->GetBufferPointer(), compiled->GetBufferSize(), nullptr, &pixelshader);
	m_AtmosphereShader.compiledShader = pixelshader;



	//_________
	// .Init(4)

	Texture* deltaIrradiance = nullptr;
	Texture* deltaRayleighScattering = nullptr;
	Texture* deltaMieScattering = nullptr;
	Texture* deltaScatteringDensity = nullptr;
	Texture* deltaMultipleScattering = nullptr;


	deltaIrradiance = new Texture;
	deltaIrradiance->Initiate(IRRADIANCE_TEXTURE_WIDTH, IRRADIANCE_TEXTURE_HEIGHT,
		DEFAULT_USAGE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_FLOAT, "deltaIrradiance");

	deltaRayleighScattering = new Texture;
	deltaRayleighScattering->Initiate3DTexture(SCATTERING_TEXTURE_WIDTH, SCATTERING_TEXTURE_HEIGHT, SCATTERING_TEXTURE_DEPTH,
		DXGI_FORMAT_R16G16B16A16_FLOAT, "rayleightScatteringTexture");

	deltaMieScattering = new Texture;
	deltaMieScattering->Initiate3DTexture(SCATTERING_TEXTURE_WIDTH, SCATTERING_TEXTURE_HEIGHT, SCATTERING_TEXTURE_DEPTH,
		DXGI_FORMAT_R16G16B16A16_FLOAT, "mieScatteringTexture");

	deltaScatteringDensity = new Texture;
	deltaScatteringDensity->Initiate3DTexture(SCATTERING_TEXTURE_WIDTH, SCATTERING_TEXTURE_HEIGHT, SCATTERING_TEXTURE_DEPTH,
		DXGI_FORMAT_R16G16B16A16_FLOAT, "densityScatteringTexture");

	deltaMultipleScattering = deltaRayleighScattering; //same memory address?


	SAFE_DELETE(deltaIrradiance);
	SAFE_DELETE(deltaRayleighScattering);
	SAFE_DELETE(deltaMieScattering);
	SAFE_DELETE(deltaScatteringDensity);
	SAFE_DELETE(deltaMultipleScattering);
}

void Atmosphere::CleanUp()
{
	SAFE_RELEASE(m_PixelBuffer);
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_DELETE(m_TransmittanceTexture);
	SAFE_DELETE(m_ScatteringTexture);
	SAFE_DELETE(m_OptionalSingleMieScatteringTexture);
	SAFE_DELETE(m_IrradianceTexture);
}

void Atmosphere::Render(const CU::Matrix44f& orientation, Texture* depth)
{

	m_API->SetBlendState(eBlendStates::ALPHA_BLEND);
	m_API->SetDepthStencilState(eDepthStencilState::Z_ENABLED, 1);
	m_API->SetRasterizer(eRasterizer::CULL_NONE);

	UpdateCameraData();

	m_API->UpdateConstantBuffer(m_VertexBuffer, &m_VertexStruct);
	m_API->UpdateConstantBuffer(m_PixelBuffer, &m_PixelStruct);

	IDevContext* ctx = m_API->GetContext();
	ctx->OMSetRenderTargets(1, m_API->GetBackbufferRef(), depth->GetDepthView());

	ctx->VSSetConstantBuffers(1, 1, &m_VertexBuffer);
	ctx->PSSetConstantBuffers(0, 1, &m_PixelBuffer);

	m_OuterSphere->Render(orientation, m_Camera->GetPerspective());
	m_InnerSphere->Render(orientation, m_Camera->GetPerspective());

}

void Atmosphere::SetLightData(const CU::Vector4f& direction, const CU::Vector4f& position)
{
	m_VertexStruct.m_LightDir = direction;
	m_VertexStruct.m_LightDir = position;

	m_PixelStruct.m_LightDir = direction;
	m_PixelStruct.m_LightPos = position;
}

void Atmosphere::UseConstantSolarSpectrum(bool useconstantspec)
{
	m_UseConstantSolarSpectrum = useconstantspec;
}

void Atmosphere::UpdateCameraData()
{
	m_VertexStruct.m_CameraDir = m_Camera->GetAt();
	m_VertexStruct.m_CameraPos = m_Camera->GetPosition();
	m_VertexStruct.m_CameraMagnitude = CU::Math::Length(m_Camera->GetPosition());
	m_VertexStruct.m_CameraMagnitude2 = CU::Math::Length2(m_Camera->GetPosition());


	m_PixelStruct.m_CameraDir = m_VertexStruct.m_CameraDir;
	m_PixelStruct.m_CameraPos = m_VertexStruct.m_CameraPos;
	m_PixelStruct.m_CameraMagnitude = m_VertexStruct.m_CameraMagnitude;
	m_PixelStruct.m_CameraMagnitude2 = m_VertexStruct.m_CameraMagnitude2;
}

//__________________________________
//
void Atmosphere::ComputeScattering(const CU::GrowingArray<double>& wavelengths, const CU::GrowingArray<double>& wavelengths_function, double lambda_power, double* red, double* green, double* blue)
{
	*red = 0.0;
	*green = 0.0;
	*blue = 0.0;

	double solar_r = Interpolate(wavelengths, wavelengths_function, kLambdaR);
	double solar_g = Interpolate(wavelengths, wavelengths_function, kLambdaG);
	double solar_b = Interpolate(wavelengths, wavelengths_function, kLambdaB);

	s32 dlambda = 1;
	const double* xyz2srgb = atmosphere::XYZ_TO_SRGB;
	for ( s32 lambda = kLambdaMin; lambda < kLambdaMax; lambda += dlambda )
	{
		double x_bar = CieColorMatchingFunctionTableValue(lambda, 1);
		double y_bar = CieColorMatchingFunctionTableValue(lambda, 2);
		double z_bar = CieColorMatchingFunctionTableValue(lambda, 3);


		double r_bar = xyz2srgb[0] * x_bar + xyz2srgb[1] * y_bar + xyz2srgb[2] * z_bar;
		double g_bar = xyz2srgb[3] * x_bar + xyz2srgb[4] * y_bar + xyz2srgb[5] * z_bar;
		double b_bar = xyz2srgb[6] * x_bar + xyz2srgb[7] * y_bar + xyz2srgb[8] * z_bar;

		double irradiance = Interpolate(wavelengths, wavelengths_function, lambda);

		*red += r_bar * irradiance / solar_r * pow(lambda / kLambdaR, lambda_power);
		*green += g_bar * irradiance / solar_g * pow(lambda / kLambdaG, lambda_power);
		*blue += b_bar * irradiance / solar_b * pow(lambda / kLambdaB, lambda_power);
	}

	*red *= atmosphere::MAX_LUMINOUS_EFFICACY * dlambda;
	*green *= atmosphere::MAX_LUMINOUS_EFFICACY * dlambda;
	*blue *= atmosphere::MAX_LUMINOUS_EFFICACY * dlambda;
}

double Atmosphere::Interpolate(const CU::GrowingArray<double>& wavelengths, const CU::GrowingArray<double>& wavelengths_function, double wavelength)
{
	DL_ASSERT_EXP(wavelengths_function.Size() == wavelengths.Size(), "Size of wavelengths does not match wavelengths_function");
	if ( wavelength < wavelengths[0] )
	{
		return wavelengths_function[0];
	}
	for ( s32 i = 0; i < wavelengths.Size() - 1; ++i )
	{
		if ( wavelength < wavelengths[i + 1] )
		{
			double u = ( wavelength - wavelengths[i] ) / ( wavelengths[i + 1] - wavelengths[i] );
			return wavelengths_function[i] * ( 1.0 - u ) + wavelengths_function[i + 1] * u;
		}
	}
	return wavelengths_function[wavelengths_function.Size() - 1];
}

double Atmosphere::CieColorMatchingFunctionTableValue(double wavelength, int column)
{
	if ( wavelength <= kLambdaMin || wavelength >= kLambdaMax ) {
		return 0.0;
	}
	double u = ( wavelength - kLambdaMin ) / 5.0;
	s32 row = ( s32 ) std::floor(u);
	DL_ASSERT_EXP(row >= 0 && row + 1 < 95, "Row was out of bounds!");
	DL_ASSERT_EXP(atmosphere::CIE_2_DEG_COLOR_MATCHING_FUNCTIONS[4 * row] <= wavelength && atmosphere::CIE_2_DEG_COLOR_MATCHING_FUNCTIONS[4 * ( row + 1 )] >= wavelength, "Out of Bounds!");
	u -= row;
	return atmosphere::CIE_2_DEG_COLOR_MATCHING_FUNCTIONS[4 * row + column] * ( 1.0 - u ) + atmosphere::CIE_2_DEG_COLOR_MATCHING_FUNCTIONS[4 * ( row + 1 ) + column] * u;
}


