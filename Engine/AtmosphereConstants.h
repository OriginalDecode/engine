/**
* Copyright (c) 2017 Eric Bruneton
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. Neither the name of the copyright holders nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*/

/*<h2>atmosphere/constants.h</h2>

<p>This file defines the size of the precomputed texures used in our atmosphere
model. It also provides tabulated values of the <a href=
"https://en.wikipedia.org/wiki/CIE_1931_color_space#Color_matching_functions"
>CIE color matching functions</a> and the conversion matrix from the <a href=
"https://en.wikipedia.org/wiki/CIE_1931_color_space">XYZ</a> to the
<a href="https://en.wikipedia.org/wiki/SRGB">sRGB</a> color spaces (which are
needed to convert the spectral radiance samples computed by our algorithm to
sRGB luminance values).
*/

#ifndef ATMOSPHERE_CONSTANTS_H_
#define ATMOSPHERE_CONSTANTS_H_
constexpr s32 kLambdaMin = 360;
constexpr s32 kLambdaMax = 830;
constexpr float kSunAngularRadius = 0.00935f / 2.f;
constexpr float kSunSolidAngle = 3.1415926f * kSunAngularRadius * kSunAngularRadius;
constexpr float kUnitLengthInMeters = 1000.f;
constexpr double kPi = 3.141593;
constexpr double kConstantSolarIrradiance = 1.5;
constexpr double kBottomRadius = 6360000.0;
constexpr double kTopRadius = 6420000.0;
constexpr double kRayleigh = 1.24062e-6;
constexpr double kRayleighScaleHeight = 8000.0;
constexpr double kMieScaleHeight = 1200.0;
constexpr double kMieAngstromAlpha = 0.0;
constexpr double kMieAngstromBeta = 5.328e-3;
constexpr double kMieSingleScatteringAlbedo = 0.9;
constexpr double kMiePhaseFunctionG = 0.8;
constexpr double kGroundAlbedo = 0.1;
constexpr double kMaxSunZenithAngle = 102.0 / 180.0 * kPi;
static constexpr double kLambdaR = 680.0;
static constexpr double kLambdaG = 550.0;
static constexpr double kLambdaB = 440.0;
namespace atmosphere {

	constexpr int TRANSMITTANCE_TEXTURE_WIDTH = 256;
	constexpr int TRANSMITTANCE_TEXTURE_HEIGHT = 64;

	constexpr int SCATTERING_TEXTURE_R_SIZE = 32;
	constexpr int SCATTERING_TEXTURE_MU_SIZE = 128;
	constexpr int SCATTERING_TEXTURE_MU_S_SIZE = 32;
	constexpr int SCATTERING_TEXTURE_NU_SIZE = 8;

	constexpr int SCATTERING_TEXTURE_WIDTH =
		SCATTERING_TEXTURE_NU_SIZE * SCATTERING_TEXTURE_MU_S_SIZE;
	constexpr int SCATTERING_TEXTURE_HEIGHT = SCATTERING_TEXTURE_MU_SIZE;
	constexpr int SCATTERING_TEXTURE_DEPTH = SCATTERING_TEXTURE_R_SIZE;

	constexpr int IRRADIANCE_TEXTURE_WIDTH = 64;
	constexpr int IRRADIANCE_TEXTURE_HEIGHT = 16;

	// The conversion factor between watts and lumens.
	constexpr double MAX_LUMINOUS_EFFICACY = 683.0;

	// Values from "CIE (1931) 2-deg color matching functions", see
	// "http://web.archive.org/web/20081228084047/
	//    http://www.cvrl.org/database/data/cmfs/ciexyz31.txt".
	constexpr double CIE_2_DEG_COLOR_MATCHING_FUNCTIONS[380] = {
		360, 0.000129900000, 0.000003917000, 0.000606100000,
		365, 0.000232100000, 0.000006965000, 0.001086000000,
		370, 0.000414900000, 0.000012390000, 0.001946000000,
		375, 0.000741600000, 0.000022020000, 0.003486000000,
		380, 0.001368000000, 0.000039000000, 0.006450001000,
		385, 0.002236000000, 0.000064000000, 0.010549990000,
		390, 0.004243000000, 0.000120000000, 0.020050010000,
		395, 0.007650000000, 0.000217000000, 0.036210000000,
		400, 0.014310000000, 0.000396000000, 0.067850010000,
		405, 0.023190000000, 0.000640000000, 0.110200000000,
		410, 0.043510000000, 0.001210000000, 0.207400000000,
		415, 0.077630000000, 0.002180000000, 0.371300000000,
		420, 0.134380000000, 0.004000000000, 0.645600000000,
		425, 0.214770000000, 0.007300000000, 1.039050100000,
		430, 0.283900000000, 0.011600000000, 1.385600000000,
		435, 0.328500000000, 0.016840000000, 1.622960000000,
		440, 0.348280000000, 0.023000000000, 1.747060000000,
		445, 0.348060000000, 0.029800000000, 1.782600000000,
		450, 0.336200000000, 0.038000000000, 1.772110000000,
		455, 0.318700000000, 0.048000000000, 1.744100000000,
		460, 0.290800000000, 0.060000000000, 1.669200000000,
		465, 0.251100000000, 0.073900000000, 1.528100000000,
		470, 0.195360000000, 0.090980000000, 1.287640000000,
		475, 0.142100000000, 0.112600000000, 1.041900000000,
		480, 0.095640000000, 0.139020000000, 0.812950100000,
		485, 0.057950010000, 0.169300000000, 0.616200000000,
		490, 0.032010000000, 0.208020000000, 0.465180000000,
		495, 0.014700000000, 0.258600000000, 0.353300000000,
		500, 0.004900000000, 0.323000000000, 0.272000000000,
		505, 0.002400000000, 0.407300000000, 0.212300000000,
		510, 0.009300000000, 0.503000000000, 0.158200000000,
		515, 0.029100000000, 0.608200000000, 0.111700000000,
		520, 0.063270000000, 0.710000000000, 0.078249990000,
		525, 0.109600000000, 0.793200000000, 0.057250010000,
		530, 0.165500000000, 0.862000000000, 0.042160000000,
		535, 0.225749900000, 0.914850100000, 0.029840000000,
		540, 0.290400000000, 0.954000000000, 0.020300000000,
		545, 0.359700000000, 0.980300000000, 0.013400000000,
		550, 0.433449900000, 0.994950100000, 0.008749999000,
		555, 0.512050100000, 1.000000000000, 0.005749999000,
		560, 0.594500000000, 0.995000000000, 0.003900000000,
		565, 0.678400000000, 0.978600000000, 0.002749999000,
		570, 0.762100000000, 0.952000000000, 0.002100000000,
		575, 0.842500000000, 0.915400000000, 0.001800000000,
		580, 0.916300000000, 0.870000000000, 0.001650001000,
		585, 0.978600000000, 0.816300000000, 0.001400000000,
		590, 1.026300000000, 0.757000000000, 0.001100000000,
		595, 1.056700000000, 0.694900000000, 0.001000000000,
		600, 1.062200000000, 0.631000000000, 0.000800000000,
		605, 1.045600000000, 0.566800000000, 0.000600000000,
		610, 1.002600000000, 0.503000000000, 0.000340000000,
		615, 0.938400000000, 0.441200000000, 0.000240000000,
		620, 0.854449900000, 0.381000000000, 0.000190000000,
		625, 0.751400000000, 0.321000000000, 0.000100000000,
		630, 0.642400000000, 0.265000000000, 0.000049999990,
		635, 0.541900000000, 0.217000000000, 0.000030000000,
		640, 0.447900000000, 0.175000000000, 0.000020000000,
		645, 0.360800000000, 0.138200000000, 0.000010000000,
		650, 0.283500000000, 0.107000000000, 0.000000000000,
		655, 0.218700000000, 0.081600000000, 0.000000000000,
		660, 0.164900000000, 0.061000000000, 0.000000000000,
		665, 0.121200000000, 0.044580000000, 0.000000000000,
		670, 0.087400000000, 0.032000000000, 0.000000000000,
		675, 0.063600000000, 0.023200000000, 0.000000000000,
		680, 0.046770000000, 0.017000000000, 0.000000000000,
		685, 0.032900000000, 0.011920000000, 0.000000000000,
		690, 0.022700000000, 0.008210000000, 0.000000000000,
		695, 0.015840000000, 0.005723000000, 0.000000000000,
		700, 0.011359160000, 0.004102000000, 0.000000000000,
		705, 0.008110916000, 0.002929000000, 0.000000000000,
		710, 0.005790346000, 0.002091000000, 0.000000000000,
		715, 0.004109457000, 0.001484000000, 0.000000000000,
		720, 0.002899327000, 0.001047000000, 0.000000000000,
		725, 0.002049190000, 0.000740000000, 0.000000000000,
		730, 0.001439971000, 0.000520000000, 0.000000000000,
		735, 0.000999949300, 0.000361100000, 0.000000000000,
		740, 0.000690078600, 0.000249200000, 0.000000000000,
		745, 0.000476021300, 0.000171900000, 0.000000000000,
		750, 0.000332301100, 0.000120000000, 0.000000000000,
		755, 0.000234826100, 0.000084800000, 0.000000000000,
		760, 0.000166150500, 0.000060000000, 0.000000000000,
		765, 0.000117413000, 0.000042400000, 0.000000000000,
		770, 0.000083075270, 0.000030000000, 0.000000000000,
		775, 0.000058706520, 0.000021200000, 0.000000000000,
		780, 0.000041509940, 0.000014990000, 0.000000000000,
		785, 0.000029353260, 0.000010600000, 0.000000000000,
		790, 0.000020673830, 0.000007465700, 0.000000000000,
		795, 0.000014559770, 0.000005257800, 0.000000000000,
		800, 0.000010253980, 0.000003702900, 0.000000000000,
		805, 0.000007221456, 0.000002607800, 0.000000000000,
		810, 0.000005085868, 0.000001836600, 0.000000000000,
		815, 0.000003581652, 0.000001293400, 0.000000000000,
		820, 0.000002522525, 0.000000910930, 0.000000000000,
		825, 0.000001776509, 0.000000641530, 0.000000000000,
		830, 0.000001251141, 0.000000451810, 0.000000000000,
	};

	// The conversion matrix from XYZ to linear sRGB color spaces.
	// Values from https://en.wikipedia.org/wiki/SRGB.
	constexpr double XYZ_TO_SRGB[9] = {
		+3.2406, -1.5372, -0.4986,
		-0.9689, +1.8758, +0.0415,
		+0.0557, -0.2040, +1.0570
	};

}  // namespace atmosphere

#endif  // ATMOSPHERE_CONSTANTS_H_


const char* shader_definitions = \
"SamplerState _sampler : register ( s0 );\n"\
"#define Length float\n"\
"#define Wavelength float\n"\
"#define Angle float\n"\
"#define SolidAngle float\n"\
"#define Power float\n"\
"#define LuminousPower float\n"\
"#define Number float\n"\
"#define Area float\n"\
"#define Volume float\n"\
"#define NumberDensity float\n"\
"#define Irradiance float\n"\
"#define Radiance float\n"\
"#define SpectralPower float\n"\
"#define SpectralIrradiance float\n"\
"#define SpectralRadiance float\n"\
"#define SpectralRadianceDensity float\n"\
"#define ScatteringCoefficient float\n"\
"#define InverseSolidAngle float\n"\
"#define LuminousIntensity float\n"\
"#define Luminance float\n"\
"#define Illuminance float\n"\
"#define AbstractSpectrum float3\n"\
"#define DimensionlessSpectrum float3\n"\
"#define PowerSpectrum float3\n"\
"#define IrradianceSpectrum float3\n"\
"#define RadianceSpectrum float3\n"\
"#define RadianceDensitySpectrum float3\n"\
"#define ScatteringSpectrum float3\n"\
"#define Position float3\n"\
"#define Direction float3\n"\
"#define Luminance3 float3\n"\
"#define Illuminance3 float3\n"\
"#define TransmittanceTexture texture2D\n"\
"#define AbstractScatteringTexture texture3D\n"\
"#define ReducedScatteringTexture texture3D\n"\
"#define ScatteringTexture texture3D\n"\
"#define ScatteringDensityTexture texture3D\n"\
"#define IrradianceTexture texture2D\n"\
"static const Length m = 1.0;\n"\
"static const Wavelength nm = 1.0;\n"\
"static const Angle rad = 1.0;\n"\
"static const SolidAngle sr = 1.0;\n"\
"static const Power watt = 1.0;\n"\
"static const LuminousPower lm = 1.0;\n"\
"static const float PI = 3.14159265358979323846;\n"\
"static const Length km = 1000.0 * m;\n"\
"static const Area m2 = m * m;\n"\
"static const Volume m3 = m * m * m;\n"\
"static const Angle pi = PI * rad;\n"\
"static const Angle deg = pi / 180.0;\n"\
"static const Irradiance watt_per_square_meter = watt / m2;\n"\
"static const Radiance watt_per_square_meter_per_sr = watt / (m2 * sr);\n"\
"static const SpectralIrradiance watt_per_square_meter_per_nm = watt / (m2 * nm);\n"\
"static const SpectralRadiance watt_per_square_meter_per_sr_per_nm = watt / (m2 * sr * nm);\n"\
"static const SpectralRadianceDensity watt_per_cubic_meter_per_sr_per_nm = watt / (m3 * sr * nm);\n"\
"static const LuminousIntensity cd = lm / sr;\n"\
"static const LuminousIntensity kcd = 1000.0 * cd;\n"\
"static const Luminance cd_per_square_meter = cd / m2;\n"\
"static const Luminance kcd_per_square_meter = kcd / m2;\n"\
"struct AtmosphereParameters\n"\
"{\n"\
"	IrradianceSpectrum solar_irradiance;\n"\
"	Angle sun_angular_radius;\n"\
"	Length bottom_radius;\n"\
"	Length top_radius;\n"\
"	Length rayleigh_scale_height;\n"\
"	ScatteringSpectrum rayleigh_scattering;\n"\
"	Length mie_scale_height;\n"\
"	ScatteringSpectrum mie_scattering;\n"\
"	ScatteringSpectrum mie_extinction;\n"\
"	Number mie_phase_function_g;\n"\
"	DimensionlessSpectrum ground_albedo;\n"\
"	Number mu_s_min;\n"\
"};\n";

const char* shader_functions = \
"Number ClampCosine(Number mu)\n"\
"{\n"\
"	return clamp(mu, Number(-1.0), Number(1.0));\n"\
"}\n\n"\
"Length ClampDistance(Length d)"\
"{\n"\
"	return max(d, 0.0 * m);\n"\
"}\n\n"\
"Length ClampRadius(IN(AtmosphereParameters) atmosphere, Length r)\n"\
"{\n"\
"	return clamp(r, atmosphere.bottom_radius, atmosphere.top_radius);\n"\
"}\n\n"\
"Length SafeSqrt(Area a)\n"\
"{\n"\
"	return sqrt(max(a, 0.0 * m2));\n"\
"}\n\n"\
"Length DistanceToTopAtmosphereBoundary(IN(AtmosphereParameters) atmosphere, Length r, Number mu)\n"\
"{\n"\
"	assert(r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	Area discriminant = r * r * (mu * mu - 1.0) + atmosphere.top_radius * atmosphere.top_radius;\n"\
"	return ClampDistance(-r * mu + SafeSqrt(discriminant));\n"\
"}\n\n"\
"Length DistanceToBottomAtmosphereBoundary(IN(AtmosphereParameters) atmosphere, Length r, Number mu)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	Area discriminant = r * r * (mu * mu - 1.0) + atmosphere.bottom_radius * atmosphere.bottom_radius;\n"\
"	return ClampDistance(-r * mu - SafeSqrt(discriminant));\n"\
"}\n\n"\
"bool RayIntersectsGround(IN(AtmosphereParameters) atmosphere, Length r, Number mu)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	return mu < 0.0 && r * r * (mu * mu - 1.0) + atmosphere.bottom_radius * atmosphere.bottom_radius >= 0.0 * m2;\n"\
"}\n\n"\
"Length ComputeOpticalLengthToTopAtmosphereBoundary(IN(AtmosphereParameters) atmosphere, Length scale_height, Length r, Number mu)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	static const int SAMPLE_COUNT = 500;\n"\
"	Length dx =\n"\
"	    DistanceToTopAtmosphereBoundary(atmosphere, r, mu) / Number(SAMPLE_COUNT);\n"\
"	Length result = 0.0 * m;\n"\
"	for (int i = 0; i <= SAMPLE_COUNT; ++i)\n"\
"	{\n"\
"	  Length d_i = Number(i) * dx;\n"\
"	  Length r_i = sqrt(d_i * d_i + 2.0 * r * mu * d_i + r * r);\n"\
"	  Number y_i = exp(-(r_i - atmosphere.bottom_radius) / scale_height);\n"\
"	  Number weight_i = i == 0 || i == SAMPLE_COUNT ? 0.5 : 1.0;\n"\
"	  result += y_i * weight_i * dx;\n"\
"	}\n\n"\
"	return result;\n"\
"}\n\n"\
"DimensionlessSpectrum ComputeTransmittanceToTopAtmosphereBoundary(IN(AtmosphereParameters) atmosphere, Length r, Number mu)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	return exp(-(atmosphere.rayleigh_scattering * ComputeOpticalLengthToTopAtmosphereBoundary(atmosphere, atmosphere.rayleigh_scale_height, r, mu) + atmosphere.mie_extinction * "\
"				ComputeOpticalLengthToTopAtmosphereBoundary(atmosphere, atmosphere.mie_scale_height, r, mu)));\n"\
"}\n\n"\
"Number GetTextureCoordFromUnitRange(Number x, int texture_size)\n"\
"{\n"\
"	return 0.5 / Number(texture_size) + x * (1.0 - 1.0 / Number(texture_size));"\
"}\n\n"\
"Number GetUnitRangeFromTextureCoord(Number u, int texture_size) {\n"\
"  return (u - 0.5 / Number(texture_size)) / (1.0 - 1.0 / Number(texture_size));\n"\
"}\n\n"\
"float2 GetTransmittanceTextureUvFromRMu(IN(AtmosphereParameters) atmosphere, Length r, Number mu)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	Length H = sqrt(atmosphere.top_radius * atmosphere.top_radius - atmosphere.bottom_radius * atmosphere.bottom_radius);\n"\
"	Length rho = SafeSqrt(r * r - atmosphere.bottom_radius * atmosphere.bottom_radius);\n"\
"	Length d = DistanceToTopAtmosphereBoundary(atmosphere, r, mu);\n"\
"	Length d_min = atmosphere.top_radius - r;\n"\
"	Length d_max = rho + H;\n"\
"	Number x_mu = (d - d_min) / (d_max - d_min);\n"\
"	Number x_r = rho / H;\n"\
"	return float2(GetTextureCoordFromUnitRange(x_mu, TRANSMITTANCE_TEXTURE_WIDTH), GetTextureCoordFromUnitRange(x_r, TRANSMITTANCE_TEXTURE_HEIGHT));\n"\
"}\n\n"\
"void GetRMuFromTransmittanceTextureUv(IN(AtmosphereParameters) atmosphere, IN(float2) uv, OUT(Length) r, OUT(Number) mu)\n"\
"{\n"\
"	assert(uv.x >= 0.0 && uv.x <= 1.0);\n"\
"	assert(uv.y >= 0.0 && uv.y <= 1.0);\n"\
"	Number x_mu = GetUnitRangeFromTextureCoord(uv.x, TRANSMITTANCE_TEXTURE_WIDTH);\n"\
"	Number x_r = GetUnitRangeFromTextureCoord(uv.y, TRANSMITTANCE_TEXTURE_HEIGHT);\n"\
"	Length H = sqrt(atmosphere.top_radius * atmosphere.top_radius -\n"\
"	    atmosphere.bottom_radius * atmosphere.bottom_radius);\n"\
"	Length rho = H * x_r;\n"\
"	r = sqrt(rho * rho + atmosphere.bottom_radius * atmosphere.bottom_radius);\n"\
"	Length d_min = atmosphere.top_radius - r;\n"\
"	Length d_max = rho + H;\n"\
"	Length d = d_min + x_mu * (d_max - d_min);\n"\
"	mu = d == 0.0 * m ? Number(1.0) : (H * H - rho * rho - d * d) / (2.0 * r * d);\n"\
"	mu = ClampCosine(mu);\n"\
"}\n\n"\
"DimensionlessSpectrum ComputeTransmittanceToTopAtmosphereBoundaryTexture(IN(AtmosphereParameters) atmosphere, IN(float2) gl_frag_coord)\n"
"{\n"\
"	static const float2 TRANSMITTANCE_TEXTURE_SIZE = float2(TRANSMITTANCE_TEXTURE_WIDTH, TRANSMITTANCE_TEXTURE_HEIGHT);\n"\
"	Length r;\n"\
"	Number mu;\n"\
"	GetRMuFromTransmittanceTextureUv(atmosphere, gl_frag_coord / TRANSMITTANCE_TEXTURE_SIZE, r, mu);\n"\
"	return ComputeTransmittanceToTopAtmosphereBoundary(atmosphere, r, mu);\n"\
"}\n\n"\
"DimensionlessSpectrum GetTransmittanceToTopAtmosphereBoundary(IN(AtmosphereParameters) atmosphere,IN(TransmittanceTexture) transmittance_texture, Length r, Number mu)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	float2 uv = GetTransmittanceTextureUvFromRMu(atmosphere, r, mu);\n"\
"	DimensionlessSpectrum dSpec = transmittance_texture.Sample(_sampler, uv);"
"	return dSpec;\n"\
"}\n\n"\
"DimensionlessSpectrum GetTransmittance(IN(AtmosphereParameters) atmosphere, IN(TransmittanceTexture) transmittance_texture, Length r, Number mu, Length d, bool ray_r_mu_intersects_ground)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	assert(d >= 0.0 * m);\n"\
"	Length r_d = ClampRadius(atmosphere, sqrt(d * d + 2.0 * r * mu * d + r * r));\n"\
"	Number mu_d = ClampCosine((r * mu + d) / r_d);\n"\
"	if (ray_r_mu_intersects_ground)\n"\
"	{\n"\
"		return min(GetTransmittanceToTopAtmosphereBoundary( atmosphere, transmittance_texture, r_d, -mu_d) / GetTransmittanceToTopAtmosphereBoundary(atmosphere, transmittance_texture, r, -mu), DimensionlessSpectrum(1,1,1));\n"\
"	}\n"\
"	else \n"\
"	{\n"\
"		return min(GetTransmittanceToTopAtmosphereBoundary(atmosphere, transmittance_texture, r, mu) / GetTransmittanceToTopAtmosphereBoundary(atmosphere, transmittance_texture, r_d, mu_d), DimensionlessSpectrum(1,1,1));\n"\
"	}\n"\
"}\n\n"\
"void ComputeSingleScatteringIntegrand(IN(AtmosphereParameters) atmosphere, IN(TransmittanceTexture) transmittance_texture, Length r, Number mu, Number mu_s, Number nu, Length d, bool ray_r_mu_intersects_ground, OUT(DimensionlessSpectrum) rayleigh, OUT(DimensionlessSpectrum) mie)\n"\
"{\n"\
"	Length r_d = ClampRadius(atmosphere, sqrt(d * d + 2.0 * r * mu * d + r * r));\n"\
"	Number mu_s_d = ClampCosine((r * mu_s + d * nu) / r_d);\n"\
"	if (RayIntersectsGround(atmosphere, r_d, mu_s_d))\n"\
"	{\n"\
"		rayleigh = DimensionlessSpectrum(0,0,0);\n"\
"		mie = DimensionlessSpectrum(0,0,0);\n"\
"	} "\
"	else\n"\
"	{\n"\
"		DimensionlessSpectrum transmittance = GetTransmittance(atmosphere, transmittance_texture, r, mu, d, ray_r_mu_intersects_ground) * GetTransmittanceToTopAtmosphereBoundary(atmosphere, transmittance_texture, r_d, mu_s_d);\n"\
"		rayleigh = transmittance * exp(-(r_d - atmosphere.bottom_radius) / atmosphere.rayleigh_scale_height);\n"\
"		mie = transmittance * exp(-(r_d - atmosphere.bottom_radius) / atmosphere.mie_scale_height);\n"\
"	}\n"\
"}\n\n"\
"Length DistanceToNearestAtmosphereBoundary(IN(AtmosphereParameters) atmosphere, Length r, Number mu, bool ray_r_mu_intersects_ground)\n"
"{\n"\
"	if (ray_r_mu_intersects_ground)\n"\
"	{\n"\
"		return DistanceToBottomAtmosphereBoundary(atmosphere, r, mu);\n"\
"	} "\
"	else\n"\
"	{\n"\
"		return DistanceToTopAtmosphereBoundary(atmosphere, r, mu);\n"\
"  }\n"\
"}\n\n"\
"void ComputeSingleScattering(IN(AtmosphereParameters) atmosphere, IN(TransmittanceTexture) transmittance_texture, Length r, Number mu, Number mu_s, Number nu, bool ray_r_mu_intersects_ground, OUT(IrradianceSpectrum) rayleigh, OUT(IrradianceSpectrum) mie)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	assert(mu_s >= -1.0 && mu_s <= 1.0);\n"\
"	assert(nu >= -1.0 && nu <= 1.0);\n"\
"	static const int SAMPLE_COUNT = 50;\n"\
"	Length dx = DistanceToNearestAtmosphereBoundary(atmosphere, r, mu, ray_r_mu_intersects_ground) / Number(SAMPLE_COUNT);\n"\
"	DimensionlessSpectrum rayleigh_sum = DimensionlessSpectrum(0,0,0);\n"\
"	DimensionlessSpectrum mie_sum = DimensionlessSpectrum(0,0,0);\n"\
"	for (int i = 0; i <= SAMPLE_COUNT; ++i)\n"\
"	{\n"\
"		Length d_i = Number(i) * dx;\n"\
"		DimensionlessSpectrum rayleigh_i;\n"\
"		DimensionlessSpectrum mie_i;\n"\
"		ComputeSingleScatteringIntegrand(atmosphere, transmittance_texture, r, mu, mu_s, nu, d_i, ray_r_mu_intersects_ground, rayleigh_i, mie_i);\n"\
"		Number weight_i = (i == 0 || i == SAMPLE_COUNT) ? 0.5 : 1.0;\n"\
"		rayleigh_sum += rayleigh_i * weight_i;\n"\
"		mie_sum += mie_i * weight_i;\n"\
"	}\n\n"\
"	rayleigh = rayleigh_sum * dx * atmosphere.solar_irradiance * atmosphere.rayleigh_scattering;\n"\
"	mie = mie_sum * dx * atmosphere.solar_irradiance * atmosphere.mie_scattering;\n"\
"}\n\n"\
"InverseSolidAngle RayleighPhaseFunction(Number nu)\n"\
"{\n"\
"	InverseSolidAngle k = 3.0 / (16.0 * PI * sr);\n"\
"	return k * (1.0 + nu * nu);\n"\
"}\n\n"\
"InverseSolidAngle MiePhaseFunction(Number g, Number nu)\n"\
"{\n"\
"	InverseSolidAngle k = 3.0 / (8.0 * PI * sr) * (1.0 - g * g) / (2.0 + g * g);\n"\
"	return k * (1.0 + nu * nu) / pow(1.0 + g * g - 2.0 * g * nu, 1.5);\n"\
"}\n\n"\
"float4 GetScatteringTextureUvwzFromRMuMuSNu(IN(AtmosphereParameters) atmosphere, Length r, Number mu, Number mu_s, Number nu, bool ray_r_mu_intersects_ground)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	assert(mu_s >= -1.0 && mu_s <= 1.0);\n"\
"	assert(nu >= -1.0 && nu <= 1.0);\n"\
"	Length H = sqrt(atmosphere.top_radius * atmosphere.top_radius - atmosphere.bottom_radius * atmosphere.bottom_radius);\n"\
"	Length rho = SafeSqrt(r * r - atmosphere.bottom_radius * atmosphere.bottom_radius);\n"\
"	Number u_r = GetTextureCoordFromUnitRange(rho / H, SCATTERING_TEXTURE_R_SIZE);\n"\
"	Length r_mu = r * mu;\n"\
"	Area discriminant = r_mu * r_mu - r * r + atmosphere.bottom_radius * atmosphere.bottom_radius;\n"\
"	Number u_mu;\n"\
"	if (ray_r_mu_intersects_ground)\n"\
"	{\n"\
"		Length d = -r_mu - SafeSqrt(discriminant);\n"\
"		Length d_min = r - atmosphere.bottom_radius;\n"\
"		Length d_max = rho;\n"\
"		u_mu = 0.5 - 0.5 * GetTextureCoordFromUnitRange(d_max == d_min ? 0.0 : (d - d_min) / (d_max - d_min), SCATTERING_TEXTURE_MU_SIZE / 2);\n"\
"	}\n"\
"	else\n"\
"	{\n"\
"		Length d = -r_mu + SafeSqrt(discriminant + H * H);\n"\
"		Length d_min = atmosphere.top_radius - r;\n"\
"		Length d_max = rho + H;\n"\
"		u_mu = 0.5 + 0.5 * GetTextureCoordFromUnitRange((d - d_min) / (d_max - d_min), SCATTERING_TEXTURE_MU_SIZE / 2);\n"\
"	}\n"\
"	Length d = DistanceToTopAtmosphereBoundary(atmosphere, atmosphere.bottom_radius, mu_s);\n"\
"	Length d_min = atmosphere.top_radius - atmosphere.bottom_radius;\n"\
"	Length d_max = H;\n"\
"	Number a = (d - d_min) / (d_max - d_min);\n"\
"	Number A = -2.0 * atmosphere.mu_s_min * atmosphere.bottom_radius / (d_max - d_min);\n"\
"	Number u_mu_s = GetTextureCoordFromUnitRange(max(1.0 - a / A, 0.0) / (1.0 + a), SCATTERING_TEXTURE_MU_S_SIZE);\n"\
"	Number u_nu = (nu + 1.0) / 2.0;\n"\
"	return float4(u_nu, u_mu_s, u_mu, u_r);\n"\
"}\n\n"\
"void GetRMuMuSNuFromScatteringTextureUvwz(IN(AtmosphereParameters) atmosphere, IN(float4) uvwz, OUT(Length) r, OUT(Number) mu, OUT(Number) mu_s, OUT(Number) nu, OUT(bool) ray_r_mu_intersects_ground)\n"\
"{\n"\
"	assert(uvwz.x >= 0.0 && uvwz.x <= 1.0);\n"\
"	assert(uvwz.y >= 0.0 && uvwz.y <= 1.0);\n"\
"	assert(uvwz.z >= 0.0 && uvwz.z <= 1.0);\n"\
"	assert(uvwz.w >= 0.0 && uvwz.w <= 1.0);\n"\
"	Length H = sqrt(atmosphere.top_radius * atmosphere.top_radius - atmosphere.bottom_radius * atmosphere.bottom_radius);\n"\
"	Length rho = H * GetUnitRangeFromTextureCoord(uvwz.w, SCATTERING_TEXTURE_R_SIZE);\n"\
"	r = sqrt(rho * rho + atmosphere.bottom_radius * atmosphere.bottom_radius);\n"\
"	if (uvwz.z < 0.5)\n"\
"	{\n"\
"		Length d_min = r - atmosphere.bottom_radius;\n"\
"		Length d_max = rho;\n"\
"		Length d = d_min + (d_max - d_min) * GetUnitRangeFromTextureCoord(1.0 - 2.0 * uvwz.z, SCATTERING_TEXTURE_MU_SIZE / 2);\n"\
"		mu = d == 0.0 * m ? Number(-1.0) : ClampCosine(-(rho * rho + d * d) / (2.0 * r * d));\n"\
"		ray_r_mu_intersects_ground = true;\n"\
"	}\n"\
"	else\n"\
"	{\n"\
"		Length d_min = atmosphere.top_radius - r;\n"\
"		Length d_max = rho + H;\n"\
"		Length d = d_min + (d_max - d_min) * GetUnitRangeFromTextureCoord(2.0 * uvwz.z - 1.0, SCATTERING_TEXTURE_MU_SIZE / 2);\n"\
"		mu = d == 0.0 * m ? Number(1.0) : ClampCosine((H * H - rho * rho - d * d) / (2.0 * r * d));\n"\
"		ray_r_mu_intersects_ground = false;\n"\
"	}\n"\
"	Number x_mu_s = GetUnitRangeFromTextureCoord(uvwz.y, SCATTERING_TEXTURE_MU_S_SIZE);\n"\
"	Length d_min = atmosphere.top_radius - atmosphere.bottom_radius;\n"\
"	Length d_max = H;\n"\
"	Number A = -2.0 * atmosphere.mu_s_min * atmosphere.bottom_radius / (d_max - d_min);\n"\
"	Number a = (A - x_mu_s * A) / (1.0 + x_mu_s * A);\n"\
"	Length d = d_min + min(a, A) * (d_max - d_min);\n"\
"	mu_s = d == 0.0 * m ? Number(1.0) : ClampCosine((H * H - d * d) / (2.0 * atmosphere.bottom_radius * d));\n"\
"	nu = ClampCosine(uvwz.x * 2.0 - 1.0);\n"\
"}\n\n"\
"void GetRMuMuSNuFromScatteringTextureFragCoord(IN(AtmosphereParameters) atmosphere, IN(float3) gl_frag_coord, OUT(Length) r, OUT(Number) mu, OUT(Number) mu_s, OUT(Number) nu, OUT(bool) ray_r_mu_intersects_ground)\n"\
"{\n"\
"	static const float4 SCATTERING_TEXTURE_SIZE = float4(SCATTERING_TEXTURE_NU_SIZE - 1, SCATTERING_TEXTURE_MU_S_SIZE, SCATTERING_TEXTURE_MU_SIZE, SCATTERING_TEXTURE_R_SIZE);\n"\
"	Number frag_coord_nu = floor(gl_frag_coord.x / Number(SCATTERING_TEXTURE_MU_S_SIZE));\n"\
"	Number frag_coord_mu_s = fmod(gl_frag_coord.x, Number(SCATTERING_TEXTURE_MU_S_SIZE));\n"\
"	float4 uvwz = float4(frag_coord_nu, frag_coord_mu_s, gl_frag_coord.y, gl_frag_coord.z) / SCATTERING_TEXTURE_SIZE;\n"\
"	GetRMuMuSNuFromScatteringTextureUvwz(atmosphere, uvwz, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"	nu = clamp(nu, mu * mu_s - sqrt((1.0 - mu * mu) * (1.0 - mu_s * mu_s)), mu * mu_s + sqrt((1.0 - mu * mu) * (1.0 - mu_s * mu_s)));\n"\
"}\n\n"\
"void ComputeSingleScatteringTexture(IN(AtmosphereParameters) atmosphere, IN(TransmittanceTexture) transmittance_texture, IN(float3) gl_frag_coord, OUT(IrradianceSpectrum) rayleigh, OUT(IrradianceSpectrum) mie)\n"\
"{\n"\
"	Length r;\n"\
"	Number mu;\n"\
"	Number mu_s;\n"\
"	Number nu;\n"\
"	bool ray_r_mu_intersects_ground;\n"\
"	GetRMuMuSNuFromScatteringTextureFragCoord(atmosphere, gl_frag_coord, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"	ComputeSingleScattering(atmosphere, transmittance_texture, r, mu, mu_s, nu, ray_r_mu_intersects_ground, rayleigh, mie);\n"\
"}\n\n"\
"TEMPLATE(AbstractSpectrum) AbstractSpectrum GetScattering( IN(AtmosphereParameters) atmosphere, IN(AbstractScatteringTexture TEMPLATE_ARGUMENT(AbstractSpectrum)) scattering_texture, Length r, Number mu, Number mu_s, Number nu, bool ray_r_mu_intersects_ground)\n"\
"{\n"\
"	float4 uvwz = GetScatteringTextureUvwzFromRMuMuSNu(atmosphere, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"	Number tex_coord_x = uvwz.x * Number(SCATTERING_TEXTURE_NU_SIZE - 1);\n"\
"	Number tex_x = floor(tex_coord_x);\n"\
"	Number lerp = tex_coord_x - tex_x;\n"\
"	float3 uvw0 = float3((tex_x + uvwz.y) / Number(SCATTERING_TEXTURE_NU_SIZE), uvwz.z, uvwz.w);\n"\
"	float3 uvw1 = float3((tex_x + 1.0 + uvwz.y) / Number(SCATTERING_TEXTURE_NU_SIZE), uvwz.z, uvwz.w);\n"\
"	AbstractSpectrum aSpec0 = scattering_texture.Sample(_sampler, uvw0);\n"\
"	AbstractSpectrum aSpec1 = scattering_texture.Sample(_sampler, uvw1);\n"\
"	return (aSpec0 * (1.0 - lerp) + aSpec1 * lerp);\n"\
"}\n\n"\
"RadianceSpectrum GetScattering(IN(AtmosphereParameters) atmosphere,\n"\
"IN(ReducedScatteringTexture) single_rayleigh_scattering_texture,\n"\
"IN(ReducedScatteringTexture) single_mie_scattering_texture,\n"\
"IN(ScatteringTexture) multiple_scattering_texture,\n"\
"Length r, Number mu, Number mu_s, Number nu, bool ray_r_mu_intersects_ground, int scattering_order)\n"\
"{\n"\
"	if (scattering_order == 1)\n"\
"	{\n"\
"		IrradianceSpectrum rayleigh = GetScattering(atmosphere, single_rayleigh_scattering_texture, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"		IrradianceSpectrum mie = GetScattering(atmosphere, single_mie_scattering_texture, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"		return rayleigh * RayleighPhaseFunction(nu) + mie * MiePhaseFunction(atmosphere.mie_phase_function_g, nu);\n"\
"	} \n"\
"	else\n"\
"	{\n"\
"		return GetScattering(atmosphere, multiple_scattering_texture, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"	}\n"\
"}\n\n"\
"IrradianceSpectrum GetIrradiance(IN(AtmosphereParameters) atmosphere, IN(IrradianceTexture) irradiance_texture, Length r, Number mu_s);\n"\
"RadianceDensitySpectrum ComputeScatteringDensity(\n"\
"    IN(AtmosphereParameters) atmosphere,\n"\
"    IN(TransmittanceTexture) transmittance_texture,\n"\
"    IN(ReducedScatteringTexture) single_rayleigh_scattering_texture,\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\n"\
"    IN(ScatteringTexture) multiple_scattering_texture,\n"\
"    IN(IrradianceTexture) irradiance_texture,\n"\
"    Length r, Number mu, Number mu_s, Number nu, int scattering_order)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	assert(mu_s >= -1.0 && mu_s <= 1.0);\n"\
"	assert(nu >= -1.0 && nu <= 1.0);\n"\
"	assert(scattering_order >= 2);\n"\
"	float3 zenith_direction = float3(0.0, 0.0, 1.0);\n"\
"	float3 omega = float3(sqrt(1.0 - mu * mu), 0.0, mu);\n"\
"	Number sun_dir_x = omega.x == 0.0 ? 0.0 : (nu - mu * mu_s) / omega.x;\n"\
"	Number sun_dir_y = sqrt(max(1.0 - sun_dir_x * sun_dir_x - mu_s * mu_s, 0.0));\n"\
"	float3 omega_s = float3(sun_dir_x, sun_dir_y, mu_s);\n"\
"	static const int SAMPLE_COUNT = 16;\n"\
"	static const Angle dphi = pi / Number(SAMPLE_COUNT);\n"\
"	static const Angle dtheta = pi / Number(SAMPLE_COUNT);\n"\
"	RadianceDensitySpectrum rayleigh_mie = RadianceDensitySpectrum(0,0,0) * watt_per_cubic_meter_per_sr_per_nm;\n"\
"	for (int l = 0; l < SAMPLE_COUNT; ++l)\n"\
"	{\n"\
"		Angle theta = (Number(l) + 0.5) * dtheta;\n"\
"		Number cos_theta = cos(theta);\n"\
"		Number sin_theta = sin(theta);\n"\
"		bool ray_r_theta_intersects_ground = RayIntersectsGround(atmosphere, r, cos_theta);\n"\
"		Length distance_to_ground = 0.0 * m;\n"\
"		DimensionlessSpectrum transmittance_to_ground = DimensionlessSpectrum(0,0,0);\n"\
"		DimensionlessSpectrum ground_albedo = DimensionlessSpectrum(0,0,0);\n"\
"		if (ray_r_theta_intersects_ground)\n"\
"		{\n"\
"			distance_to_ground = DistanceToBottomAtmosphereBoundary(atmosphere, r, cos_theta);\n"\
"			transmittance_to_ground = GetTransmittance(atmosphere, transmittance_texture, r, cos_theta, distance_to_ground, true /* ray_intersects_ground */);\n"\
"			ground_albedo = atmosphere.ground_albedo;\n"\
"		}\n"\
"		for (int m = 0; m < 2 * SAMPLE_COUNT; ++m)\n"\
"		{\n"\
"			Angle phi = (Number(m) + 0.5) * dphi;\n"\
"			float3 omega_i = float3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);\n"\
"			SolidAngle domega_i = (dtheta / rad) * (dphi / rad) * sin(theta) * sr;\n"\
"			Number nu1 = dot(omega_s, omega_i);\n"\
"			RadianceSpectrum incident_radiance = GetScattering(atmosphere, single_rayleigh_scattering_texture, single_mie_scattering_texture, multiple_scattering_texture, r, omega_i.z, mu_s, nu1, ray_r_theta_intersects_ground, scattering_order - 1);\n"\
"			float3 ground_normal = normalize(zenith_direction * r + omega_i * distance_to_ground);\n"\
"			IrradianceSpectrum ground_irradiance = GetIrradiance(atmosphere, irradiance_texture, atmosphere.bottom_radius, dot(ground_normal, omega_s));\n"\
"			incident_radiance += transmittance_to_ground * ground_albedo * (1.0 / (PI * sr)) * ground_irradiance;\n"\
"			Number nu2 = dot(omega, omega_i);\n"\
"			Number rayleigh_density = exp(-(r - atmosphere.bottom_radius) / atmosphere.rayleigh_scale_height);\n"\
"			Number mie_density = exp(-(r - atmosphere.bottom_radius) / atmosphere.mie_scale_height);\n"\
"			rayleigh_mie += incident_radiance * (atmosphere.rayleigh_scattering * rayleigh_density * RayleighPhaseFunction(nu2) + atmosphere.mie_scattering * mie_density * MiePhaseFunction(atmosphere.mie_phase_function_g, nu2)) * domega_i;\n"\
"		}\n"\
"	}\n"\
"	return rayleigh_mie;\n"\
"}\n\n"\
"RadianceSpectrum ComputeMultipleScattering(IN(AtmosphereParameters) atmosphere, IN(TransmittanceTexture) transmittance_texture, IN(ScatteringDensityTexture) scattering_density_texture, Length r, Number mu, Number mu_s, Number nu, bool ray_r_mu_intersects_ground)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu >= -1.0 && mu <= 1.0);\n"\
"	assert(mu_s >= -1.0 && mu_s <= 1.0);\n"\
"	assert(nu >= -1.0 && nu <= 1.0);\n"\
"	static const int SAMPLE_COUNT = 50;\n"\
"	Length dx = DistanceToNearestAtmosphereBoundary( atmosphere, r, mu, ray_r_mu_intersects_ground) / Number(SAMPLE_COUNT);\n"\
"	RadianceSpectrum rayleigh_mie_sum = RadianceSpectrum(0,0,0) * watt_per_square_meter_per_sr_per_nm;\n"\
"	for (int i = 0; i <= SAMPLE_COUNT; ++i)\n"
"	{\n"\
"		Length d_i = Number(i) * dx;\n"\
"		Length r_i = ClampRadius(atmosphere, sqrt(d_i * d_i + 2.0 * r * mu * d_i + r * r));\n"\
"		Number mu_i = ClampCosine((r * mu + d_i) / r_i);\n"\
"		Number mu_s_i = ClampCosine((r * mu_s + d_i * nu) / r_i);\n"\
"		RadianceSpectrum rayleigh_mie_i = GetScattering(atmosphere, scattering_density_texture, r_i, mu_i, mu_s_i, nu, ray_r_mu_intersects_ground) * GetTransmittance(atmosphere, transmittance_texture, r, mu, d_i, ray_r_mu_intersects_ground) * dx;\n"\
"		Number weight_i = (i == 0 || i == SAMPLE_COUNT) ? 0.5 : 1.0;\n"\
"		rayleigh_mie_sum += rayleigh_mie_i * weight_i;\n"\
"	}\n"\
"	return rayleigh_mie_sum;\n"\
"}\n\n"\
"RadianceDensitySpectrum ComputeScatteringDensityTexture(\n"\
"    IN(AtmosphereParameters) atmosphere,\n"\
"    IN(TransmittanceTexture) transmittance_texture,\n"\
"    IN(ReducedScatteringTexture) single_rayleigh_scattering_texture,\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\n"\
"    IN(ScatteringTexture) multiple_scattering_texture,\n"\
"    IN(IrradianceTexture) irradiance_texture,\n"\
"    IN(float3) gl_frag_coord, int scattering_order)\n"\
"{\n"\
"	Length r;\n"\
"	Number mu;\n"\
"	Number mu_s;\n"\
"	Number nu;\n"\
"	bool ray_r_mu_intersects_ground;\n"\
"	GetRMuMuSNuFromScatteringTextureFragCoord(atmosphere, gl_frag_coord, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"	return ComputeScatteringDensity(atmosphere, transmittance_texture, single_rayleigh_scattering_texture, single_mie_scattering_texture, multiple_scattering_texture, irradiance_texture, r, mu, mu_s, nu, scattering_order);\n"\
"}\n\n"\
"RadianceSpectrum ComputeMultipleScatteringTexture(\n"\
"    IN(AtmosphereParameters) atmosphere,\n"\
"    IN(TransmittanceTexture) transmittance_texture,\n"\
"    IN(ScatteringDensityTexture) scattering_density_texture,\n"\
"    IN(float3) gl_frag_coord, OUT(Number) nu)\n"\
"{\n"\
"	Length r;\n"\
"	Number mu;\n"\
"	Number mu_s;\n"\
"	bool ray_r_mu_intersects_ground;\n"\
"	GetRMuMuSNuFromScatteringTextureFragCoord(atmosphere, gl_frag_coord, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"	return ComputeMultipleScattering(atmosphere, transmittance_texture, scattering_density_texture, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"}\n\n"\

"IrradianceSpectrum ComputeDirectIrradiance(\n"\
"    IN(AtmosphereParameters) atmosphere,\n"\
"    IN(TransmittanceTexture) transmittance_texture,\n"\
"    Length r, Number mu_s)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu_s >= -1.0 && mu_s <= 1.0);\n"\
"	return atmosphere.solar_irradiance * GetTransmittanceToTopAtmosphereBoundary(atmosphere, transmittance_texture, r, mu_s) * max(mu_s, 0.0);\n"\
"}\n\n"\
"IrradianceSpectrum ComputeIndirectIrradiance(IN(AtmosphereParameters) atmosphere, IN(ReducedScatteringTexture) single_rayleigh_scattering_texture, IN(ReducedScatteringTexture) single_mie_scattering_texture, IN(ScatteringTexture) multiple_scattering_texture, Length r, Number mu_s, int scattering_order)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu_s >= -1.0 && mu_s <= 1.0);\n"\
"	assert(scattering_order >= 1);\n"\
"	static const int SAMPLE_COUNT = 32;\n"\
"	const Angle dphi = pi / Number(SAMPLE_COUNT);\n"\
"	const Angle dtheta = pi / Number(SAMPLE_COUNT);\n"\
"	IrradianceSpectrum result = IrradianceSpectrum(0,0,0) * watt_per_square_meter_per_nm;\n"\
"	float3 omega_s = float3(sqrt(1.0 - mu_s * mu_s), 0.0, mu_s);\n"\
"	for (int j = 0; j < SAMPLE_COUNT / 2; ++j)\n"\
"	{\n"\
"		Angle theta = (Number(j) + 0.5) * dtheta;\n"\
"		bool ray_r_theta_intersects_ground = RayIntersectsGround(atmosphere, r, cos(theta));\n"\
"		for (int i = 0; i < 2 * SAMPLE_COUNT; ++i)\n"\
"		{\n"\
"			Angle phi = (Number(i) + 0.5) * dphi;\n"\
"			float3 omega = float3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));\n"\
"			SolidAngle domega = (dtheta / rad) * (dphi / rad) * sin(theta) * sr;\n"\
"			Number nu = dot(omega, omega_s);\n"\
"			result += GetScattering(atmosphere, single_rayleigh_scattering_texture, single_mie_scattering_texture, multiple_scattering_texture, r, omega.z, mu_s, nu, ray_r_theta_intersects_ground, scattering_order) * omega.z * domega;\n"\
"		}\n"\
"	}\n"\
"	return result;\n"\
"}\n\n"\

"float2 GetIrradianceTextureUvFromRMuS(IN(AtmosphereParameters) atmosphere, Length r, Number mu_s)\n"\
"{\n"\
"	assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\n"\
"	assert(mu_s >= -1.0 && mu_s <= 1.0);\n"\
"	Number x_r = (r - atmosphere.bottom_radius) / (atmosphere.top_radius - atmosphere.bottom_radius);\n"\
"	Number x_mu_s = mu_s * 0.5 + 0.5;\n"\
"	return float2(GetTextureCoordFromUnitRange(x_mu_s, IRRADIANCE_TEXTURE_WIDTH), GetTextureCoordFromUnitRange(x_r, IRRADIANCE_TEXTURE_HEIGHT));\n"\
"}\n\n"\

"void GetRMuSFromIrradianceTextureUv(IN(AtmosphereParameters) atmosphere, IN(float2) uv, OUT(Length) r, OUT(Number) mu_s)\n"\
"{\n"\
"	assert(uv.x >= 0.0 && uv.x <= 1.0);\n"\
"	assert(uv.y >= 0.0 && uv.y <= 1.0);\n"\
"	Number x_mu_s = GetUnitRangeFromTextureCoord(uv.x, IRRADIANCE_TEXTURE_WIDTH);\n"\
"	Number x_r = GetUnitRangeFromTextureCoord(uv.y, IRRADIANCE_TEXTURE_HEIGHT);\n"\
"	r = atmosphere.bottom_radius + x_r * (atmosphere.top_radius - atmosphere.bottom_radius);\n"\
"	mu_s = ClampCosine(2.0 * x_mu_s - 1.0);\n"\
"}\n\n"\

"static const float2 IRRADIANCE_TEXTURE_SIZE = float2(IRRADIANCE_TEXTURE_WIDTH, IRRADIANCE_TEXTURE_HEIGHT);\n"\
"IrradianceSpectrum ComputeDirectIrradianceTexture(IN(AtmosphereParameters) atmosphere, IN(TransmittanceTexture) transmittance_texture, IN(float2) gl_frag_coord)\n"\
"{\n"\
"	Length r;\n"\
"	Number mu_s;\n"\
"	GetRMuSFromIrradianceTextureUv(atmosphere, gl_frag_coord / IRRADIANCE_TEXTURE_SIZE, r, mu_s);\n"\
"	return ComputeDirectIrradiance(atmosphere, transmittance_texture, r, mu_s);\n"\
"}\n\n"\

"IrradianceSpectrum ComputeIndirectIrradianceTexture(IN(AtmosphereParameters) atmosphere, IN(ReducedScatteringTexture) single_rayleigh_scattering_texture, IN(ReducedScatteringTexture) single_mie_scattering_texture, IN(ScatteringTexture) multiple_scattering_texture, IN(float2) gl_frag_coord, int scattering_order)\n"\
"{\n"\
"	Length r;\n"\
"	Number mu_s;\n"\
"	GetRMuSFromIrradianceTextureUv(atmosphere, gl_frag_coord / IRRADIANCE_TEXTURE_SIZE, r, mu_s);\n"\
"	return ComputeIndirectIrradiance(atmosphere, single_rayleigh_scattering_texture, single_mie_scattering_texture, multiple_scattering_texture, r, mu_s, scattering_order);\n"\
"}\n\n"\

"IrradianceSpectrum GetIrradiance(IN(AtmosphereParameters) atmosphere, IN(IrradianceTexture) irradiance_texture, Length r, Number mu_s)\n"\
"{\n"\
"	float2 uv = GetIrradianceTextureUvFromRMuS(atmosphere, r, mu_s);\n"\
"	IrradianceSpectrum iSpec = irradiance_texture.Sample(_sampler, uv);\n"\
"	return iSpec;\n"\
"}\n\n"\

"#ifdef COMBINED_SCATTERING_TEXTURES\n"\
"float3 GetExtrapolatedSingleMieScattering(IN(AtmosphereParameters) atmosphere, float4 scattering)\n"\
"{\n"\
"	if (scattering.r == 0.0)\n"\
"	{\n"\
"		return float3(0,0,0);\n"\
"	}\n"\
"	return scattering.rgb * scattering.a / scattering.r * (atmosphere.rayleigh_scattering.r / atmosphere.mie_scattering.r) * (atmosphere.mie_scattering / atmosphere.rayleigh_scattering);\n"\
"}\n\n"\
"#endif\n"\

"IrradianceSpectrum GetCombinedScattering(\n"\
"    IN(AtmosphereParameters) atmosphere,\n"\
"    IN(ReducedScatteringTexture) scattering_texture,\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\n"\
"    Length r, Number mu, Number mu_s, Number nu,\n"\
"    bool ray_r_mu_intersects_ground,\n"\
"    OUT(IrradianceSpectrum) single_mie_scattering)\n"\
"{\n"\
"	float4 uvwz = GetScatteringTextureUvwzFromRMuMuSNu(atmosphere, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\n"\
"	Number tex_coord_x = uvwz.x * Number(SCATTERING_TEXTURE_NU_SIZE - 1);\n"\
"	Number tex_x = floor(tex_coord_x);\n"\
"	Number lerp = tex_coord_x - tex_x;\n"\
"	float3 uvw0 = float3((tex_x + uvwz.y) / Number(SCATTERING_TEXTURE_NU_SIZE), uvwz.z, uvwz.w);\n"\
"	float3 uvw1 = float3((tex_x + 1.0 + uvwz.y) / Number(SCATTERING_TEXTURE_NU_SIZE), uvwz.z, uvwz.w);\n"\
"#ifdef COMBINED_SCATTERING_TEXTURES\n"\
"	float4 scatterTex0 = scattering_texture.Sample(_sampler, uvw0);\n"\
"	float4 scatterTex1 = scattering_texture.Sample(_sampler, uvw1);\n"\
"	float4 combined_scattering = scatterTex0 * (1.0 - lerp) + scatterTex1 * lerp;\n"\
"	IrradianceSpectrum scattering = IrradianceSpectrum(combined_scattering.rgb);\n"\
"	single_mie_scattering = GetExtrapolatedSingleMieScattering(atmosphere, combined_scattering);\n"\
"#else\n"\
"	IrradianceSpectrum iSpec0 = scattering_texture.Sample(_sampler, uvw0);\n"\
"	IrradianceSpectrum iSpec1 = scattering_texture.Sample(_sampler, uvw1);\n"\
"	IrradianceSpectrum scattering = iSpec0 * (1.0 - lerp) + iSpec1 * lerp);\n"\
"	IrradianceSpectrum iSpec2 = single_mie_scattering_texture.Sample(_sampler, uvw0);\n"\
"	IrradianceSpectrum iSpec3 = single_mie_scattering_texture.Sample(_sampler, uvw1);\n"\
"	single_mie_scattering = iSpec2 * (1.0 - lerp) + iSpec3 * lerp);\n"\
"#endif\n"\
"	return scattering;\n"\
"}\n\n"\

"RadianceSpectrum GetSkyRadiance(\n"\
"    IN(AtmosphereParameters) atmosphere,\n"\
"    IN(TransmittanceTexture) transmittance_texture,\n"\
"    IN(ReducedScatteringTexture) scattering_texture,\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\n"\
"    Position camera, IN(Direction) view_ray, Length shadow_length,\n"\
"    IN(Direction) sun_direction, OUT(DimensionlessSpectrum) transmittance)\n"
"{\n"\
"	Length r = length(camera);\n"\
"	Length rmu = dot(camera, view_ray);\n"\
"	Length distance_to_top_atmosphere_boundary = -rmu - sqrt(rmu * rmu - r * r + atmosphere.top_radius * atmosphere.top_radius);\n"\
"	if (distance_to_top_atmosphere_boundary > 0.0 * m)\n"\
"	{\n"\
"		camera = camera + view_ray * distance_to_top_atmosphere_boundary;\n"\
"		r = atmosphere.top_radius;\n"\
"		rmu += distance_to_top_atmosphere_boundary;\n"\
"	}\n"\
"	if (r > atmosphere.top_radius)\n"\
"	{\n"\
"		transmittance = DimensionlessSpectrum(1,1,1);\n"\
"		return RadianceSpectrum(0,0,0) * watt_per_square_meter_per_sr_per_nm;\n"\
"	}\n"\
"	Number mu = rmu / r;\n"\
"	Number mu_s = dot(camera, sun_direction) / r;\n"\
"	Number nu = dot(view_ray, sun_direction);\n"\
"	bool ray_r_mu_intersects_ground = RayIntersectsGround(atmosphere, r, mu);\n"\
"	transmittance = ray_r_mu_intersects_ground ? DimensionlessSpectrum(0,0,0) :\n"\
"	    GetTransmittanceToTopAtmosphereBoundary(\n"\
"	        atmosphere, transmittance_texture, r, mu);\n"\
"	IrradianceSpectrum single_mie_scattering;\n"\
"	IrradianceSpectrum scattering;\n"\
"	if (shadow_length == 0.0 * m)\n"\
"	{\n"\
"    scattering = GetCombinedScattering(\n"\
"        atmosphere, scattering_texture, single_mie_scattering_texture,\n"\
"        r, mu, mu_s, nu, ray_r_mu_intersects_ground,\n"\
"        single_mie_scattering);\n"\
"	}\n"\
"	else\n"\
"	{\n"\
"		Length d = shadow_length;\n"\
"		Length r_p = ClampRadius(atmosphere, sqrt(d * d + 2.0 * r * mu * d + r * r));\n"\
"		Number mu_p = (r * mu + d) / r_p;\n"\
"		Number mu_s_p = (r * mu_s + d * nu) / r_p;\n"\
"		scattering = GetCombinedScattering(atmosphere, scattering_texture, single_mie_scattering_texture, r_p, mu_p, mu_s_p, nu, ray_r_mu_intersects_ground, single_mie_scattering);\n"\
"		DimensionlessSpectrum shadow_transmittance = GetTransmittance(atmosphere, transmittance_texture,r, mu, shadow_length, ray_r_mu_intersects_ground);\n"\
"		scattering = scattering * shadow_transmittance;\n"\
"		single_mie_scattering = single_mie_scattering * shadow_transmittance;\n"\
"	}\n\n"\
"	return scattering * RayleighPhaseFunction(nu) + single_mie_scattering * MiePhaseFunction(atmosphere.mie_phase_function_g, nu);\n"\
"}\n\n"\
"RadianceSpectrum GetSkyRadianceToPoint(\n"\
"    IN(AtmosphereParameters) atmosphere,\n"\
"    IN(TransmittanceTexture) transmittance_texture,\n"\
"    IN(ReducedScatteringTexture) scattering_texture,\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\n"\
"    Position camera, IN(Position) _point, Length shadow_length,\n"\
"    IN(Direction) sun_direction, OUT(DimensionlessSpectrum) transmittance)\n"
"{\n"\
"	Direction view_ray = normalize(_point - camera);\n"\
"	Length r = length(camera);\n"\
"	Length rmu = dot(camera, view_ray);\n"\
"	Length distance_to_top_atmosphere_boundary = -rmu - sqrt(rmu * rmu - r * r + atmosphere.top_radius * atmosphere.top_radius);\n"\
"	if (distance_to_top_atmosphere_boundary > 0.0 * m)\n"
"	{\n"\
"		camera = camera + view_ray * distance_to_top_atmosphere_boundary;\n"\
"		r = atmosphere.top_radius;\n"\
"		rmu += distance_to_top_atmosphere_boundary;\n"\
"	}\n"\
"	Number mu = rmu / r;\n"\
"	Number mu_s = dot(camera, sun_direction) / r;\n"\
"	Number nu = dot(view_ray, sun_direction);\n"\
"	Length d = length(_point - camera);\n"\
"	bool ray_r_mu_intersects_ground = RayIntersectsGround(atmosphere, r, mu);\n"\
"	transmittance = GetTransmittance(atmosphere, transmittance_texture, r, mu, d, ray_r_mu_intersects_ground);\n"\
"	IrradianceSpectrum single_mie_scattering;\n"\
"	IrradianceSpectrum scattering = GetCombinedScattering(atmosphere, scattering_texture, single_mie_scattering_texture, r, mu, mu_s, nu, ray_r_mu_intersects_ground, single_mie_scattering);\n"\
"	d = max(d - shadow_length, 0.0 * m);\n"\
"	Length r_p = ClampRadius(atmosphere, sqrt(d * d + 2.0 * r * mu * d + r * r));\n"\
"	Number mu_p = (r * mu + d) / r_p;\n"\
"	Number mu_s_p = (r * mu_s + d * nu) / r_p;\n"\
"	IrradianceSpectrum single_mie_scattering_p;\n"\
"	IrradianceSpectrum scattering_p = GetCombinedScattering(atmosphere, scattering_texture, single_mie_scattering_texture, r_p, mu_p, mu_s_p, nu, ray_r_mu_intersects_ground, single_mie_scattering_p);\n"\
"	DimensionlessSpectrum shadow_transmittance = transmittance;\n"\
"	if (shadow_length > 0.0 * m)\n"\
"	{\n"\
"		shadow_transmittance = GetTransmittance(atmosphere, transmittance_texture, r, mu, d, ray_r_mu_intersects_ground);\n"\
"	}\n"\
"	scattering = scattering - shadow_transmittance * scattering_p;\n"\
"	single_mie_scattering = single_mie_scattering - shadow_transmittance * single_mie_scattering_p;\n"\
"#ifdef COMBINED_SCATTERING_TEXTURES\n"\
"	single_mie_scattering = GetExtrapolatedSingleMieScattering(atmosphere, float4(scattering, single_mie_scattering.r));\n"\
"#endif\n"\
"	single_mie_scattering = single_mie_scattering * smoothstep(Number(0.0), Number(0.01), mu_s);\n"\
"	return scattering * RayleighPhaseFunction(nu) + single_mie_scattering * MiePhaseFunction(atmosphere.mie_phase_function_g, nu);\n"\
"}\n\n"\
"IrradianceSpectrum GetSunAndSkyIrradiance(IN(AtmosphereParameters) atmosphere, IN(TransmittanceTexture) transmittance_texture, IN(IrradianceTexture) irradiance_texture,IN(Position) _point, IN(Direction) normal, IN(Direction) sun_direction, OUT(IrradianceSpectrum) sky_irradiance)\n"\
"{\n"\
"	Length r = length(_point);\n"\
"	Number mu_s = dot(_point, sun_direction) / r;\n"\
"	sky_irradiance = GetIrradiance(atmosphere, irradiance_texture, r, mu_s) * (1.0 + dot(normal, _point) / r) * 0.5;\n"\
"	return atmosphere.solar_irradiance * GetTransmittanceToTopAtmosphereBoundary(atmosphere, transmittance_texture, r, mu_s) * smoothstep(-atmosphere.sun_angular_radius / rad, atmosphere.sun_angular_radius / rad, mu_s) * max(dot(normal, sun_direction), 0.0);\n"\
"}\n\n"\
"";


const char* kComputeTransmittanceShader = R"(
	layout(location = 0) out float3 transmittance;
	void main() {
	  transmittance = ComputeTransmittanceToTopAtmosphereBoundaryTexture(
		  atmosphere_parameters, gl_FragCoord.xy);
	})";

const char* kComputeDirectIrradianceShader = R"(
	layout(location = 0) out float3 delta_irradiance;
	layout(location = 1) out float3 irradiance;
	texture2D transmittance_texture;
	void main() {
	  delta_irradiance = ComputeDirectIrradianceTexture(
		  atmosphere_parameters, transmittance_texture, gl_FragCoord.xy);
	  irradiance = float3(0,0,0);
	})";

const char* kComputeSingleScatteringShader = R"(
	layout(location = 0) out float3 delta_rayleigh;
	layout(location = 1) out float3 delta_mie;
	layout(location = 2) out float4 scattering;
	texture2D  transmittance_texture;
	int layer;
	void main() {
	  ComputeSingleScatteringTexture(
		  atmosphere_parameters, transmittance_texture, float3(gl_FragCoord.xy, layer + 0.5),
		  delta_rayleigh, delta_mie);
	  scattering = float4(delta_rayleigh.rgb, delta_mie.r);
	})";

const char* kComputeScatteringDensityShader = R"(
	layout(location = 0) out float3 scattering_density;
	texture2D  transmittance_texture;
	texture3D single_rayleigh_scattering_texture;
	texture3D single_mie_scattering_texture;
	texture3D multiple_scattering_texture;
	texture2D  irradiance_texture;
	int scattering_order;
	int layer;
	void main()\n\
	{\n\
		AtmosphereParameters atmosphere_parameters = (AtmosphereParameters)0;\n\
		atmosphere_parameters.solar_irradiance = SOLAR_IRRADIANCE;\n\
		atmosphere_parameters.sun_angular_radius = SUN_ANGULAR_RADIUS;\n\
		atmosphere_parameters.bottom_radius = BOTTOM_RADIUS;\n\
		atmosphere_parameters.top_radius = TOP_RADIUS;\n\
		atmosphere_parameters.rayleigh_scale_height = RAYLEIGH_SCALE_HEIGHT;\n\
		atmosphere_parameters.rayleigh_scattering = RAYLEIGH_SCATTERING;\n\
		atmosphere_parameters.mie_scale_height = MIE_SCALE_HEIGHT;\n\
		atmosphere_parameters.mie_scattering = MIE_SCATTERING;\n\
		atmosphere_parameters.mie_extinction = MIE_EXTINCTION;\n\
		atmosphere_parameters.mie_phase_function_g = MIE_PHASE_FUNCTION_G;\n\
		atmosphere_parameters.ground_albedo = GROUND_ALBEDO;\n\
		atmosphere_parameters.mu_s_min = MU_S_MIN;\n\
		scattering_density = ComputeScatteringDensityTexture(atmosphere_parameters, transmittance_texture, single_rayleigh_scattering_texture,single_mie_scattering_texture, multiple_scattering_texture,irradiance_texture, float3(gl_FragCoord.xy, layer + 0.5),scattering_order);
	})";

const char* kComputeIndirectIrradianceShader = R"(
	layout(location = 0) out float3 delta_irradiance;
	layout(location = 1) out float3 irradiance;
	texture3D single_rayleigh_scattering_texture;
	texture3D single_mie_scattering_texture;
	texture3D multiple_scattering_texture;
	int scattering_order;
	void main()\n\
	{\n\
		AtmosphereParameters atmosphere_parameters = (AtmosphereParameters)0;\n\
		atmosphere_parameters.solar_irradiance = SOLAR_IRRADIANCE;\n\
		atmosphere_parameters.sun_angular_radius = SUN_ANGULAR_RADIUS;\n\
		atmosphere_parameters.bottom_radius = BOTTOM_RADIUS;\n\
		atmosphere_parameters.top_radius = TOP_RADIUS;\n\
		atmosphere_parameters.rayleigh_scale_height = RAYLEIGH_SCALE_HEIGHT;\n\
		atmosphere_parameters.rayleigh_scattering = RAYLEIGH_SCATTERING;\n\
		atmosphere_parameters.mie_scale_height = MIE_SCALE_HEIGHT;\n\
		atmosphere_parameters.mie_scattering = MIE_SCATTERING;\n\
		atmosphere_parameters.mie_extinction = MIE_EXTINCTION;\n\
		atmosphere_parameters.mie_phase_function_g = MIE_PHASE_FUNCTION_G;\n\
		atmosphere_parameters.ground_albedo = GROUND_ALBEDO;\n\
		atmosphere_parameters.mu_s_min = MU_S_MIN;\n\		
		delta_irradiance = ComputeIndirectIrradianceTexture(atmosphere_parameters, single_rayleigh_scattering_texture,single_mie_scattering_texture, multiple_scattering_texture,gl_FragCoord.xy, scattering_order - 1);
		irradiance = delta_irradiance;
	})";

const char* kComputeMultipleScatteringShader = R"(
	layout(location = 0) out float3 delta_multiple_scattering;
	layout(location = 1) out float4 scattering;
	texture2D transmittance_texture;
	texture3D scattering_density_texture;
	int layer;
	void main()\n\
	{\n\
		AtmosphereParameters atmosphere_parameters = (AtmosphereParameters)0;\n\
		atmosphere_parameters.solar_irradiance = SOLAR_IRRADIANCE;\n\
		atmosphere_parameters.sun_angular_radius = SUN_ANGULAR_RADIUS;\n\
		atmosphere_parameters.bottom_radius = BOTTOM_RADIUS;\n\
		atmosphere_parameters.top_radius = TOP_RADIUS;\n\
		atmosphere_parameters.rayleigh_scale_height = RAYLEIGH_SCALE_HEIGHT;\n\
		atmosphere_parameters.rayleigh_scattering = RAYLEIGH_SCATTERING;\n\
		atmosphere_parameters.mie_scale_height = MIE_SCALE_HEIGHT;\n\
		atmosphere_parameters.mie_scattering = MIE_SCATTERING;\n\
		atmosphere_parameters.mie_extinction = MIE_EXTINCTION;\n\
		atmosphere_parameters.mie_phase_function_g = MIE_PHASE_FUNCTION_G;\n\
		atmosphere_parameters.ground_albedo = GROUND_ALBEDO;\n\
		atmosphere_parameters.mu_s_min = MU_S_MIN;\n\	
		float nu;
		delta_multiple_scattering = ComputeMultipleScatteringTexture(atmosphere_parameters, transmittance_texture, scattering_density_texture, float3(gl_FragCoord.xy, layer + 0.5), nu);
		scattering = float4(delta_multiple_scattering.rgb / RayleighPhaseFunction(nu), 0.0);
	})";

const char* kAtmosphereShader = "\
texture2D transmittance_texture;\n\
texture3D scattering_texture;\n \
texture3D single_mie_scattering_texture;\n\
texture2D irradiance_texture;\n\n\
RadianceSpectrum GetSkyRadiance(Position camera, Direction view_ray, Length shadow_length,Direction sun_direction, out DimensionlessSpectrum transmittance)\n\
{\n\
	AtmosphereParameters atmosphere_parameters;\n\
	atmosphere_parameters.solar_irradiance = SOLAR_IRRADIANCE;\n\
	atmosphere_parameters.sun_angular_radius = SUN_ANGULAR_RADIUS;\n\
	atmosphere_parameters.bottom_radius = BOTTOM_RADIUS;\n\
	atmosphere_parameters.top_radius = TOP_RADIUS;\n\
	atmosphere_parameters.rayleigh_scale_height = RAYLEIGH_SCALE_HEIGHT;\n\
	atmosphere_parameters.rayleigh_scattering = RAYLEIGH_SCATTERING;\n\
	atmosphere_parameters.mie_scale_height = MIE_SCALE_HEIGHT;\n\
	atmosphere_parameters.mie_scattering = MIE_SCATTERING;\n\
	atmosphere_parameters.mie_extinction = MIE_EXTINCTION;\n\
	atmosphere_parameters.mie_phase_function_g = MIE_PHASE_FUNCTION_G;\n\
	atmosphere_parameters.ground_albedo = GROUND_ALBEDO;\n\
	atmosphere_parameters.mu_s_min = MU_S_MIN;\n\
	return GetSkyRadiance(atmosphere_parameters, transmittance_texture, scattering_texture, single_mie_scattering_texture,camera, view_ray, shadow_length, sun_direction, transmittance);\n\
}\n\n\
RadianceSpectrum GetSkyRadianceToPoint(Position camera, Position _point, Length shadow_length, Direction sun_direction, out DimensionlessSpectrum transmittance)\n\
{\n\
	AtmosphereParameters atmosphere_parameters;\n\
	atmosphere_parameters.solar_irradiance = SOLAR_IRRADIANCE;\n\
	atmosphere_parameters.sun_angular_radius = SUN_ANGULAR_RADIUS;\n\
	atmosphere_parameters.bottom_radius = BOTTOM_RADIUS;\n\
	atmosphere_parameters.top_radius = TOP_RADIUS;\n\
	atmosphere_parameters.rayleigh_scale_height = RAYLEIGH_SCALE_HEIGHT;\n\
	atmosphere_parameters.rayleigh_scattering = RAYLEIGH_SCATTERING;\n\
	atmosphere_parameters.mie_scale_height = MIE_SCALE_HEIGHT;\n\
	atmosphere_parameters.mie_scattering = MIE_SCATTERING;\n\
	atmosphere_parameters.mie_extinction = MIE_EXTINCTION;\n\
	atmosphere_parameters.mie_phase_function_g = MIE_PHASE_FUNCTION_G;\n\
	atmosphere_parameters.ground_albedo = GROUND_ALBEDO;\n\
	atmosphere_parameters.mu_s_min = MU_S_MIN;\n\
	return GetSkyRadianceToPoint(atmosphere_parameters, transmittance_texture, scattering_texture, single_mie_scattering_texture, camera, _point, shadow_length, sun_direction, transmittance);\n\
}\n\n\
IrradianceSpectrum GetSunAndSkyIrradiance(Position p, Direction normal, Direction sun_direction, out IrradianceSpectrum sky_irradiance)\n\
{\n\
	AtmosphereParameters atmosphere_parameters;\n\
	atmosphere_parameters.solar_irradiance = SOLAR_IRRADIANCE;\n\
	atmosphere_parameters.sun_angular_radius = SUN_ANGULAR_RADIUS;\n\
	atmosphere_parameters.bottom_radius = BOTTOM_RADIUS;\n\
	atmosphere_parameters.top_radius = TOP_RADIUS;\n\
	atmosphere_parameters.rayleigh_scale_height = RAYLEIGH_SCALE_HEIGHT;\n\
	atmosphere_parameters.rayleigh_scattering = RAYLEIGH_SCATTERING;\n\
	atmosphere_parameters.mie_scale_height = MIE_SCALE_HEIGHT;\n\
	atmosphere_parameters.mie_scattering = MIE_SCATTERING;\n\
	atmosphere_parameters.mie_extinction = MIE_EXTINCTION;\n\
	atmosphere_parameters.mie_phase_function_g = MIE_PHASE_FUNCTION_G;\n\
	atmosphere_parameters.ground_albedo = GROUND_ALBEDO;\n\
	atmosphere_parameters.mu_s_min = MU_S_MIN;\n\
	return GetSunAndSkyIrradiance(atmosphere_parameters, transmittance_texture, irradiance_texture, p, normal, sun_direction, sky_irradiance);\n\
}\n\n\
Luminance3 GetSkyLuminance(Position camera, Direction view_ray, Length shadow_length,Direction sun_direction, out DimensionlessSpectrum transmittance)\n\
{\n\
	return GetSkyRadiance(camera, view_ray, shadow_length, sun_direction, transmittance) * SKY_SPECTRAL_RADIANCE_TO_LUMINANCE;\n\
}\n\n\
Luminance3 GetSkyLuminanceToPoint(Position camera, Position _point, Length shadow_length, Direction sun_direction, out DimensionlessSpectrum transmittance)\n\
{\n\
  return GetSkyRadianceToPoint(camera, _point, shadow_length, sun_direction, transmittance) * SKY_SPECTRAL_RADIANCE_TO_LUMINANCE;\n\
}\n\n\
Illuminance3 GetSunAndSkyIlluminance(Position p, Direction normal, Direction sun_direction, out IrradianceSpectrum sky_irradiance)\n\
{\n\
	IrradianceSpectrum sun_irradiance = GetSunAndSkyIrradiance(p, normal, sun_direction, sky_irradiance);\n\
	sky_irradiance *= SKY_SPECTRAL_RADIANCE_TO_LUMINANCE;\n\
	return sun_irradiance * SUN_SPECTRAL_RADIANCE_TO_LUMINANCE;\n\
}";


const char* shader_main = \
"cbuffer Variables : register ( b0 )\n"\
"{\n"\
"	float3 camera;\n"\
"	float exposure;\n"\
"	float3 white_point;\n"\
"	float3 earth_center;\n"\
"	float3 sun_direction;\n"\
"	float3 sun_radiance;\n"\
"	float2 sun_size;\n"\
"	float3 view_ray;\n"\
"};\n"\
"static const float3 kSphereCenter = float3(0.0, 0.0, 1.0);\n"\
"static const float kSphereRadius = 1.0;\n"\
"static const float3 kSphereAlbedo = float3(0.8, 0.8, 0.8);\n"\
"static const float3 kGroundAlbedo = float3(0.0, 0.0, 0.04);\n"\
"#ifdef USE_LUMINANCE\n"\
"#define GetSkyRadiance GetSkyLuminance\n"\
"#define GetSkyRadianceToPoint GetSkyLuminanceToPoint\n"\
"#define GetSunAndSkyIrradiance GetSunAndSkyIlluminance\n"\
"#endif\n"\
"float3 GetSkyRadiance(float3 camera, float3 view_ray, float shadow_length,\n"\
"    float3 sun_direction, out float3 transmittance);\n"\
"float3 GetSkyRadianceToPoint(float3 camera, float3 _point , float shadow_length,\n"\
"    float3 sun_direction, out float3 transmittance);\n"\
"float3 GetSunAndSkyIrradiance(\n"\
"    float3 p, float3 normal, float3 sun_direction, out float3 sky_irradiance);\n"\
"float GetSunVisibility(float3 _point , float3 sun_direction) {\n"\
"  float3 p = _point  - kSphereCenter;\n"\
"  float p_dot_v = dot(p, sun_direction);\n"\
"  float p_dot_p = dot(p, p);\n"\
"  float ray_sphere_center_squared_distance = p_dot_p - p_dot_v * p_dot_v;\n"\
"  float distance_to_intersection = -p_dot_v - sqrt(\n"\
"      kSphereRadius * kSphereRadius - ray_sphere_center_squared_distance);\n"\
"  if (distance_to_intersection > 0.0) {\n"\
"    float ray_sphere_distance =\n"\
"        kSphereRadius - sqrt(ray_sphere_center_squared_distance);\n"\
"    float ray_sphere_angular_distance = -ray_sphere_distance / p_dot_v;\n"\
"    return smoothstep(1.0, 0.0, ray_sphere_angular_distance / sun_size.x);\n"\
"  }\n"\
"  return 1.0;\n"\
"}\n"\
"float GetSkyVisibility(float3 _point ) {\n"\
"  float3 p = _point  - kSphereCenter;\n"\
"  float p_dot_p = dot(p, p);\n"\
"  return\n"\
"      1.0 + p.z / sqrt(p_dot_p) * kSphereRadius * kSphereRadius / p_dot_p;\n"\
"}\n"\
"void GetSphereShadowInOut(float3 view_direction, float3 sun_direction,\n"\
"    out float d_in, out float d_out) {\n"\
"  float3 pos = camera - kSphereCenter;\n"\
"  float pos_dot_sun = dot(pos, sun_direction);\n"\
"  float view_dot_sun = dot(view_direction, sun_direction);\n"\
"  float k = sun_size.x;\n"\
"  float l = 1.0 + k * k;\n"\
"  float a = 1.0 - l * view_dot_sun * view_dot_sun;\n"\
"  float b = dot(pos, view_direction) - l * pos_dot_sun * view_dot_sun -\n"\
"      k * kSphereRadius * view_dot_sun;\n"\
"  float c = dot(pos, pos) - l * pos_dot_sun * pos_dot_sun -\n"\
"      2.0 * k * kSphereRadius * pos_dot_sun - kSphereRadius * kSphereRadius;\n"\
"  float discriminant = b * b - a * c;\n"\
"  if (discriminant > 0.0) {\n"\
"    d_in = max(0.0, (-b - sqrt(discriminant)) / a);\n"\
"    d_out = (-b + sqrt(discriminant)) / a;\n"\
"    float d_base = -pos_dot_sun / view_dot_sun;\n"\
"    float d_apex = -(pos_dot_sun + kSphereRadius / k) / view_dot_sun;\n"\
"    if (view_dot_sun > 0.0) {\n"\
"      d_in = max(d_in, d_apex);\n"\
"      d_out = a > 0.0 ? min(d_out, d_base) : d_base;\n"\
"    } else {\n"\
"      d_in = a > 0.0 ? max(d_in, d_base) : d_base;\n"\
"      d_out = min(d_out, d_apex);\n"\
"    }\n"\
"  } else {\n"\
"    d_in = 0.0;\n"\
"    d_out = 0.0;\n"\
"  }\n"\
"}\n"\
"float4 main() : SV_Target\n"\
"{\n"\
"	float3 view_direction = normalize(view_ray);\n"\
"	float fragment_angular_size = length(ddx(view_ray) + ddy(view_ray)) / length(view_ray);\n"\
"	float shadow_in;\n"\
"	float shadow_out;\n"\
"	GetSphereShadowInOut(view_direction, sun_direction, shadow_in, shadow_out);\n"\
"	float lightshaft_fadein_hack = smoothstep(0.02, 0.04, dot(normalize(camera - earth_center), sun_direction));\n"\
"	float3 p = camera - kSphereCenter;\n"\
"	float p_dot_v = dot(p, view_direction);\n"\
"	float p_dot_p = dot(p, p);\n"\
"	float ray_sphere_center_squared_distance = p_dot_p - p_dot_v * p_dot_v;\n"\
"	float distance_to_intersection = -p_dot_v - sqrt(kSphereRadius * kSphereRadius - ray_sphere_center_squared_distance);\n"\
"	float sphere_alpha = 0.0;\n"\
"	float3 sphere_radiance = float3(0,0,0);\n"\
"	if (distance_to_intersection > 0.0)\n"\
"	{\n"\
"		float ray_sphere_distance = kSphereRadius - sqrt(ray_sphere_center_squared_distance);\n"\
"		float ray_sphere_angular_distance = -ray_sphere_distance / p_dot_v;\n"\
"		sphere_alpha = min(ray_sphere_angular_distance / fragment_angular_size, 1.0);\n"\
"		float3 _point = camera + view_direction * distance_to_intersection;\n"\
"		float3 normal = normalize(_point  - kSphereCenter);\n"\
"		float3 sky_irradiance;\n"\
"		float3 sun_irradiance = GetSunAndSkyIrradiance(_point  - earth_center, normal, sun_direction, sky_irradiance);\n"\
"		sphere_radiance = kSphereAlbedo * (1.0 / PI) * (sun_irradiance + sky_irradiance);\n"\
"		float shadow_length = max(0.0, min(shadow_out, distance_to_intersection) - shadow_in) * lightshaft_fadein_hack;\n"\
"		float3 transmittance;\n"\
"		float3 in_scatter = GetSkyRadianceToPoint(camera - earth_center, _point  - earth_center, shadow_length, sun_direction, transmittance);\n"\
"		sphere_radiance = sphere_radiance * transmittance + in_scatter;\n"\
"	}\n"\
"	p = camera - earth_center;\n"\
"	p_dot_v = dot(p, view_direction);\n"\
"	p_dot_p = dot(p, p);\n"\
"	float ray_earth_center_squared_distance = p_dot_p - p_dot_v * p_dot_v;\n"\
"	distance_to_intersection = -p_dot_v - sqrt(\n"\
"	    earth_center.z * earth_center.z - ray_earth_center_squared_distance);\n"\
"	float ground_alpha = 0.0;\n"\
"	float3 ground_radiance = float3(0,0,0);\n"\
"	if (distance_to_intersection > 0.0)\n"\
"	{\n"\
"		float3 _point  = camera + view_direction * distance_to_intersection;\n"\
"		float3 normal = normalize(_point  - earth_center);\n"\
"		float3 sky_irradiance;\n"\
"		float3 sun_irradiance = GetSunAndSkyIrradiance(_point  - earth_center, normal, sun_direction, sky_irradiance);\n"\
"		ground_radiance = kGroundAlbedo * (1.0 / PI) * (sun_irradiance * GetSunVisibility(_point , sun_direction) + sky_irradiance * GetSkyVisibility(_point ));\n"\
"		float shadow_length = max(0.0, min(shadow_out, distance_to_intersection) - shadow_in) * lightshaft_fadein_hack;\n"\
"		float3 transmittance;\n"\
"		float3 in_scatter = GetSkyRadianceToPoint(camera - earth_center, _point  - earth_center, shadow_length, sun_direction, transmittance);\n"\
"		ground_radiance = ground_radiance * transmittance + in_scatter;\n"\
"		ground_alpha = 1.0;\n"\
"	}\n"\
"	float shadow_length = max(0.0, shadow_out - shadow_in) * lightshaft_fadein_hack;\n"\
"	float3 transmittance;\n"\
"	float3 radiance = GetSkyRadiance(camera - earth_center, view_direction, shadow_length, sun_direction, transmittance);\n"\
"	if (dot(view_direction, sun_direction) > sun_size.y)\n"\
"	{\n"\
"		radiance = radiance + transmittance * sun_radiance;\n"\
"	}\n"\
"	radiance = lerp(radiance, ground_radiance, ground_alpha);\n"\
"	radiance = lerp(radiance, sphere_radiance, sphere_alpha);\n"\
"	float3 color = pow(float3(1,1,1) - exp(-radiance / white_point * exposure), 1.0 / 2.2);\n"\
"	return float4(color, 1);\n"\
"}";

//const char* kVertexShader = R"(
//struct VS_INPUT\n
//{\n
//	float2 vertex : POS;
//};\n
//struct VS_OUTPUT\n
//{\n
//	float2 vertex : POS;
//};\n
//
//layout(location = 0) in vec2 vertex;
//void main()\n
//{\n
//	gl_Position = vec4(vertex, 0.0, 1.0);
//})";