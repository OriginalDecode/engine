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
"struct AtmosphereParameters\n{\n"\
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
"} atmosphere_parameters;\n";

const char* shader_functions = \
"Number ClampCosine(Number mu) {\r\n"\
"  return clamp(mu, Number(-1.0), Number(1.0));\r\n"\
"}\r\n"\
"Length ClampDistance(Length d) {\r\n"\
"  return max(d, 0.0 * m);\r\n"\
"}\r\n"\
"Length ClampRadius(IN(AtmosphereParameters) atmosphere, Length r) {\r\n"\
"  return clamp(r, atmosphere.bottom_radius, atmosphere.top_radius);\r\n"\
"}\r\n"\
"Length SafeSqrt(Area a) {\r\n"\
"  return sqrt(max(a, 0.0 * m2));\r\n"\
"}\r\n"\
"Length DistanceToTopAtmosphereBoundary(IN(AtmosphereParameters) atmosphere,\r\n"\
"    Length r, Number mu) {\r\n"\
"  assert(r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  Area discriminant = r * r * (mu * mu - 1.0) +\r\n"\
"      atmosphere.top_radius * atmosphere.top_radius;\r\n"\
"  return ClampDistance(-r * mu + SafeSqrt(discriminant));\r\n"\
"}\r\n"\
"Length DistanceToBottomAtmosphereBoundary(IN(AtmosphereParameters) atmosphere,\r\n"\
"    Length r, Number mu) {\r\n"\
"  assert(r >= atmosphere.bottom_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  Area discriminant = r * r * (mu * mu - 1.0) +\r\n"\
"      atmosphere.bottom_radius * atmosphere.bottom_radius;\r\n"\
"  return ClampDistance(-r * mu - SafeSqrt(discriminant));\r\n"\
"}\r\n"\
"bool RayIntersectsGround(IN(AtmosphereParameters) atmosphere,\r\n"\
"    Length r, Number mu) {\r\n"\
"  assert(r >= atmosphere.bottom_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  return mu < 0.0 && r * r * (mu * mu - 1.0) +\r\n"\
"      atmosphere.bottom_radius * atmosphere.bottom_radius >= 0.0 * m2;\r\n"\
"}\r\n"\
"Length ComputeOpticalLengthToTopAtmosphereBoundary(\r\n"\
"    IN(AtmosphereParameters) atmosphere, Length scale_height,\r\n"\
"    Length r, Number mu) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  static const int SAMPLE_COUNT = 500;\r\n"\
"  Length dx =\r\n"\
"      DistanceToTopAtmosphereBoundary(atmosphere, r, mu) / Number(SAMPLE_COUNT);\r\n"\
"  Length result = 0.0 * m;\r\n"\
"  for (int i = 0; i <= SAMPLE_COUNT; ++i) {\r\n"\
"    Length d_i = Number(i) * dx;\r\n"\
"    Length r_i = sqrt(d_i * d_i + 2.0 * r * mu * d_i + r * r);\r\n"\
"    Number y_i = exp(-(r_i - atmosphere.bottom_radius) / scale_height);\r\n"\
"    Number weight_i = i == 0 || i == SAMPLE_COUNT ? 0.5 : 1.0;\r\n"\
"    result += y_i * weight_i * dx;\r\n"\
"  }\r\n"\
"  return result;\r\n"\
"}\r\n"\
"DimensionlessSpectrum ComputeTransmittanceToTopAtmosphereBoundary(\r\n"\
"    IN(AtmosphereParameters) atmosphere, Length r, Number mu) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  return exp(-(\r\n"\
"      atmosphere.rayleigh_scattering *\r\n"\
"          ComputeOpticalLengthToTopAtmosphereBoundary(\r\n"\
"              atmosphere, atmosphere.rayleigh_scale_height, r, mu) +\r\n"\
"      atmosphere.mie_extinction *\r\n"\
"          ComputeOpticalLengthToTopAtmosphereBoundary(\r\n"\
"              atmosphere, atmosphere.mie_scale_height, r, mu)));\r\n"\
"}\r\n"\
"Number GetTextureCoordFromUnitRange(Number x, int texture_size) {\r\n"\
"  return 0.5 / Number(texture_size) + x * (1.0 - 1.0 / Number(texture_size));\r\n"\
"}\r\n"\
"Number GetUnitRangeFromTextureCoord(Number u, int texture_size) {\r\n"\
"  return (u - 0.5 / Number(texture_size)) / (1.0 - 1.0 / Number(texture_size));\r\n"\
"}\r\n"\
"float2 GetTransmittanceTextureUvFromRMu(IN(AtmosphereParameters) atmosphere,\r\n"\
"    Length r, Number mu) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  Length H = sqrt(atmosphere.top_radius * atmosphere.top_radius -\r\n"\
"      atmosphere.bottom_radius * atmosphere.bottom_radius);\r\n"\
"  Length rho =\r\n"\
"      SafeSqrt(r * r - atmosphere.bottom_radius * atmosphere.bottom_radius);\r\n"\
"  Length d = DistanceToTopAtmosphereBoundary(atmosphere, r, mu);\r\n"\
"  Length d_min = atmosphere.top_radius - r;\r\n"\
"  Length d_max = rho + H;\r\n"\
"  Number x_mu = (d - d_min) / (d_max - d_min);\r\n"\
"  Number x_r = rho / H;\r\n"\
"  return float2(GetTextureCoordFromUnitRange(x_mu, TRANSMITTANCE_TEXTURE_WIDTH),\r\n"\
"              GetTextureCoordFromUnitRange(x_r, TRANSMITTANCE_TEXTURE_HEIGHT));\r\n"\
"}\r\n"\
"void GetRMuFromTransmittanceTextureUv(IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(float2) uv, OUT(Length) r, OUT(Number) mu) {\r\n"\
"  assert(uv.x >= 0.0 && uv.x <= 1.0);\r\n"\
"  assert(uv.y >= 0.0 && uv.y <= 1.0);\r\n"\
"  Number x_mu = GetUnitRangeFromTextureCoord(uv.x, TRANSMITTANCE_TEXTURE_WIDTH);\r\n"\
"  Number x_r = GetUnitRangeFromTextureCoord(uv.y, TRANSMITTANCE_TEXTURE_HEIGHT);\r\n"\
"  Length H = sqrt(atmosphere.top_radius * atmosphere.top_radius -\r\n"\
"      atmosphere.bottom_radius * atmosphere.bottom_radius);\r\n"\
"  Length rho = H * x_r;\r\n"\
"  r = sqrt(rho * rho + atmosphere.bottom_radius * atmosphere.bottom_radius);\r\n"\
"  Length d_min = atmosphere.top_radius - r;\r\n"\
"  Length d_max = rho + H;\r\n"\
"  Length d = d_min + x_mu * (d_max - d_min);\r\n"\
"  mu = d == 0.0 * m ? Number(1.0) : (H * H - rho * rho - d * d) / (2.0 * r * d);\r\n"\
"  mu = ClampCosine(mu);\r\n"\
"}\r\n"\
"DimensionlessSpectrum ComputeTransmittanceToTopAtmosphereBoundaryTexture(\r\n"\
"    IN(AtmosphereParameters) atmosphere, IN(float2) gl_frag_coord) {\r\n"\
"  static const float2 TRANSMITTANCE_TEXTURE_SIZE =\r\n"\
"      float2(TRANSMITTANCE_TEXTURE_WIDTH, TRANSMITTANCE_TEXTURE_HEIGHT);\r\n"\
"  Length r;\r\n"\
"  Number mu;\r\n"\
"  GetRMuFromTransmittanceTextureUv(\r\n"\
"      atmosphere, gl_frag_coord / TRANSMITTANCE_TEXTURE_SIZE, r, mu);\r\n"\
"  return ComputeTransmittanceToTopAtmosphereBoundary(atmosphere, r, mu);\r\n"\
"}\r\n"\
"DimensionlessSpectrum GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    Length r, Number mu) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  float2 uv = GetTransmittanceTextureUvFromRMu(atmosphere, r, mu);\r\n"\
"  return DimensionlessSpectrum(texture(transmittance_texture, uv));\r\n"\
"}\r\n"\
"DimensionlessSpectrum GetTransmittance(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    Length r, Number mu, Length d, bool ray_r_mu_intersects_ground) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  assert(d >= 0.0 * m);\r\n"\
"  Length r_d = ClampRadius(atmosphere, sqrt(d * d + 2.0 * r * mu * d + r * r));\r\n"\
"  Number mu_d = ClampCosine((r * mu + d) / r_d);\r\n"\
"  if (ray_r_mu_intersects_ground) {\r\n"\
"    return min(\r\n"\
"        GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"            atmosphere, transmittance_texture, r_d, -mu_d) /\r\n"\
"        GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"            atmosphere, transmittance_texture, r, -mu),\r\n"\
"        DimensionlessSpectrum(1.0));\r\n"\
"  } else {\r\n"\
"    return min(\r\n"\
"        GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"            atmosphere, transmittance_texture, r, mu) /\r\n"\
"        GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"            atmosphere, transmittance_texture, r_d, mu_d),\r\n"\
"        DimensionlessSpectrum(1.0));\r\n"\
"  }\r\n"\
"}\r\n"\
"void ComputeSingleScatteringIntegrand(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    Length r, Number mu, Number mu_s, Number nu, Length d,\r\n"\
"    bool ray_r_mu_intersects_ground,\r\n"\
"    OUT(DimensionlessSpectrum) rayleigh, OUT(DimensionlessSpectrum) mie) {\r\n"\
"  Length r_d = ClampRadius(atmosphere, sqrt(d * d + 2.0 * r * mu * d + r * r));\r\n"\
"  Number mu_s_d = ClampCosine((r * mu_s + d * nu) / r_d);\r\n"\
"  if (RayIntersectsGround(atmosphere, r_d, mu_s_d)) {\r\n"\
"    rayleigh = DimensionlessSpectrum(0.0);\r\n"\
"    mie = DimensionlessSpectrum(0.0);\r\n"\
"  } else {\r\n"\
"    DimensionlessSpectrum transmittance =\r\n"\
"        GetTransmittance(\r\n"\
"            atmosphere, transmittance_texture, r, mu, d,\r\n"\
"            ray_r_mu_intersects_ground) *\r\n"\
"        GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"            atmosphere, transmittance_texture, r_d, mu_s_d);\r\n"\
"    rayleigh = transmittance * exp(\r\n"\
"        -(r_d - atmosphere.bottom_radius) / atmosphere.rayleigh_scale_height);\r\n"\
"    mie = transmittance * exp(\r\n"\
"        -(r_d - atmosphere.bottom_radius) / atmosphere.mie_scale_height);\r\n"\
"  }\r\n"\
"}\r\n"\
"Length DistanceToNearestAtmosphereBoundary(IN(AtmosphereParameters) atmosphere,\r\n"\
"    Length r, Number mu, bool ray_r_mu_intersects_ground) {\r\n"\
"  if (ray_r_mu_intersects_ground) {\r\n"\
"    return DistanceToBottomAtmosphereBoundary(atmosphere, r, mu);\r\n"\
"  } else {\r\n"\
"    return DistanceToTopAtmosphereBoundary(atmosphere, r, mu);\r\n"\
"  }\r\n"\
"}\r\n"\
"void ComputeSingleScattering(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    Length r, Number mu, Number mu_s, Number nu,\r\n"\
"    bool ray_r_mu_intersects_ground,\r\n"\
"    OUT(IrradianceSpectrum) rayleigh, OUT(IrradianceSpectrum) mie) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  assert(mu_s >= -1.0 && mu_s <= 1.0);\r\n"\
"  assert(nu >= -1.0 && nu <= 1.0);\r\n"\
"  static const int SAMPLE_COUNT = 50;\r\n"\
"  Length dx =\r\n"\
"      DistanceToNearestAtmosphereBoundary(atmosphere, r, mu,\r\n"\
"          ray_r_mu_intersects_ground) / Number(SAMPLE_COUNT);\r\n"\
"  DimensionlessSpectrum rayleigh_sum = DimensionlessSpectrum(0.0);\r\n"\
"  DimensionlessSpectrum mie_sum = DimensionlessSpectrum(0.0);\r\n"\
"  for (int i = 0; i <= SAMPLE_COUNT; ++i) {\r\n"\
"    Length d_i = Number(i) * dx;\r\n"\
"    DimensionlessSpectrum rayleigh_i;\r\n"\
"    DimensionlessSpectrum mie_i;\r\n"\
"    ComputeSingleScatteringIntegrand(atmosphere, transmittance_texture,\r\n"\
"        r, mu, mu_s, nu, d_i, ray_r_mu_intersects_ground, rayleigh_i, mie_i);\r\n"\
"    Number weight_i = (i == 0 || i == SAMPLE_COUNT) ? 0.5 : 1.0;\r\n"\
"    rayleigh_sum += rayleigh_i * weight_i;\r\n"\
"    mie_sum += mie_i * weight_i;\r\n"\
"  }\r\n"\
"  rayleigh = rayleigh_sum * dx * atmosphere.solar_irradiance *\r\n"\
"      atmosphere.rayleigh_scattering;\r\n"\
"  mie = mie_sum * dx * atmosphere.solar_irradiance * atmosphere.mie_scattering;\r\n"\
"}\r\n"\
"InverseSolidAngle RayleighPhaseFunction(Number nu) {\r\n"\
"  InverseSolidAngle k = 3.0 / (16.0 * PI * sr);\r\n"\
"  return k * (1.0 + nu * nu);\r\n"\
"}\r\n"\
"InverseSolidAngle MiePhaseFunction(Number g, Number nu) {\r\n"\
"  InverseSolidAngle k = 3.0 / (8.0 * PI * sr) * (1.0 - g * g) / (2.0 + g * g);\r\n"\
"  return k * (1.0 + nu * nu) / pow(1.0 + g * g - 2.0 * g * nu, 1.5);\r\n"\
"}\r\n"\
"float4 GetScatteringTextureUvwzFromRMuMuSNu(IN(AtmosphereParameters) atmosphere,\r\n"\
"    Length r, Number mu, Number mu_s, Number nu,\r\n"\
"    bool ray_r_mu_intersects_ground) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  assert(mu_s >= -1.0 && mu_s <= 1.0);\r\n"\
"  assert(nu >= -1.0 && nu <= 1.0);\r\n"\
"  Length H = sqrt(atmosphere.top_radius * atmosphere.top_radius -\r\n"\
"      atmosphere.bottom_radius * atmosphere.bottom_radius);\r\n"\
"  Length rho =\r\n"\
"      SafeSqrt(r * r - atmosphere.bottom_radius * atmosphere.bottom_radius);\r\n"\
"  Number u_r = GetTextureCoordFromUnitRange(rho / H, SCATTERING_TEXTURE_R_SIZE);\r\n"\
"  Length r_mu = r * mu;\r\n"\
"  Area discriminant =\r\n"\
"      r_mu * r_mu - r * r + atmosphere.bottom_radius * atmosphere.bottom_radius;\r\n"\
"  Number u_mu;\r\n"\
"  if (ray_r_mu_intersects_ground) {\r\n"\
"    Length d = -r_mu - SafeSqrt(discriminant);\r\n"\
"    Length d_min = r - atmosphere.bottom_radius;\r\n"\
"    Length d_max = rho;\r\n"\
"    u_mu = 0.5 - 0.5 * GetTextureCoordFromUnitRange(d_max == d_min ? 0.0 :\r\n"\
"        (d - d_min) / (d_max - d_min), SCATTERING_TEXTURE_MU_SIZE / 2);\r\n"\
"  } else {\r\n"\
"    Length d = -r_mu + SafeSqrt(discriminant + H * H);\r\n"\
"    Length d_min = atmosphere.top_radius - r;\r\n"\
"    Length d_max = rho + H;\r\n"\
"    u_mu = 0.5 + 0.5 * GetTextureCoordFromUnitRange(\r\n"\
"        (d - d_min) / (d_max - d_min), SCATTERING_TEXTURE_MU_SIZE / 2);\r\n"\
"  }\r\n"\
"  Length d = DistanceToTopAtmosphereBoundary(\r\n"\
"      atmosphere, atmosphere.bottom_radius, mu_s);\r\n"\
"  Length d_min = atmosphere.top_radius - atmosphere.bottom_radius;\r\n"\
"  Length d_max = H;\r\n"\
"  Number a = (d - d_min) / (d_max - d_min);\r\n"\
"  Number A =\r\n"\
"      -2.0 * atmosphere.mu_s_min * atmosphere.bottom_radius / (d_max - d_min);\r\n"\
"  Number u_mu_s = GetTextureCoordFromUnitRange(\r\n"\
"      max(1.0 - a / A, 0.0) / (1.0 + a), SCATTERING_TEXTURE_MU_S_SIZE);\r\n"\
"  Number u_nu = (nu + 1.0) / 2.0;\r\n"\
"  return float4(u_nu, u_mu_s, u_mu, u_r);\r\n"\
"}\r\n"\
"void GetRMuMuSNuFromScatteringTextureUvwz(IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(float4) uvwz, OUT(Length) r, OUT(Number) mu, OUT(Number) mu_s,\r\n"\
"    OUT(Number) nu, OUT(bool) ray_r_mu_intersects_ground) {\r\n"\
"  assert(uvwz.x >= 0.0 && uvwz.x <= 1.0);\r\n"\
"  assert(uvwz.y >= 0.0 && uvwz.y <= 1.0);\r\n"\
"  assert(uvwz.z >= 0.0 && uvwz.z <= 1.0);\r\n"\
"  assert(uvwz.w >= 0.0 && uvwz.w <= 1.0);\r\n"\
"  Length H = sqrt(atmosphere.top_radius * atmosphere.top_radius -\r\n"\
"      atmosphere.bottom_radius * atmosphere.bottom_radius);\r\n"\
"  Length rho =\r\n"\
"      H * GetUnitRangeFromTextureCoord(uvwz.w, SCATTERING_TEXTURE_R_SIZE);\r\n"\
"  r = sqrt(rho * rho + atmosphere.bottom_radius * atmosphere.bottom_radius);\r\n"\
"  if (uvwz.z < 0.5) {\r\n"\
"    Length d_min = r - atmosphere.bottom_radius;\r\n"\
"    Length d_max = rho;\r\n"\
"    Length d = d_min + (d_max - d_min) * GetUnitRangeFromTextureCoord(\r\n"\
"        1.0 - 2.0 * uvwz.z, SCATTERING_TEXTURE_MU_SIZE / 2);\r\n"\
"    mu = d == 0.0 * m ? Number(-1.0) :\r\n"\
"        ClampCosine(-(rho * rho + d * d) / (2.0 * r * d));\r\n"\
"    ray_r_mu_intersects_ground = true;\r\n"\
"  } else {\r\n"\
"    Length d_min = atmosphere.top_radius - r;\r\n"\
"    Length d_max = rho + H;\r\n"\
"    Length d = d_min + (d_max - d_min) * GetUnitRangeFromTextureCoord(\r\n"\
"        2.0 * uvwz.z - 1.0, SCATTERING_TEXTURE_MU_SIZE / 2);\r\n"\
"    mu = d == 0.0 * m ? Number(1.0) :\r\n"\
"        ClampCosine((H * H - rho * rho - d * d) / (2.0 * r * d));\r\n"\
"    ray_r_mu_intersects_ground = false;\r\n"\
"  }\r\n"\
"  Number x_mu_s =\r\n"\
"      GetUnitRangeFromTextureCoord(uvwz.y, SCATTERING_TEXTURE_MU_S_SIZE);\r\n"\
"  Length d_min = atmosphere.top_radius - atmosphere.bottom_radius;\r\n"\
"  Length d_max = H;\r\n"\
"  Number A =\r\n"\
"      -2.0 * atmosphere.mu_s_min * atmosphere.bottom_radius / (d_max - d_min);\r\n"\
"  Number a = (A - x_mu_s * A) / (1.0 + x_mu_s * A);\r\n"\
"  Length d = d_min + min(a, A) * (d_max - d_min);\r\n"\
"  mu_s = d == 0.0 * m ? Number(1.0) :\r\n"\
"     ClampCosine((H * H - d * d) / (2.0 * atmosphere.bottom_radius * d));\r\n"\
"  nu = ClampCosine(uvwz.x * 2.0 - 1.0);\r\n"\
"}\r\n"\
"void GetRMuMuSNuFromScatteringTextureFragCoord(\r\n"\
"    IN(AtmosphereParameters) atmosphere, IN(float3) gl_frag_coord,\r\n"\
"    OUT(Length) r, OUT(Number) mu, OUT(Number) mu_s, OUT(Number) nu,\r\n"\
"    OUT(bool) ray_r_mu_intersects_ground) {\r\n"\
"  static const float4 SCATTERING_TEXTURE_SIZE = float4(\r\n"\
"      SCATTERING_TEXTURE_NU_SIZE - 1,\r\n"\
"      SCATTERING_TEXTURE_MU_S_SIZE,\r\n"\
"      SCATTERING_TEXTURE_MU_SIZE,\r\n"\
"      SCATTERING_TEXTURE_R_SIZE);\r\n"\
"  Number frag_coord_nu =\r\n"\
"      floor(gl_frag_coord.x / Number(SCATTERING_TEXTURE_MU_S_SIZE));\r\n"\
"  Number frag_coord_mu_s =\r\n"\
"      mod(gl_frag_coord.x, Number(SCATTERING_TEXTURE_MU_S_SIZE));\r\n"\
"  float4 uvwz =\r\n"\
"      float4(frag_coord_nu, frag_coord_mu_s, gl_frag_coord.y, gl_frag_coord.z) /\r\n"\
"          SCATTERING_TEXTURE_SIZE;\r\n"\
"  GetRMuMuSNuFromScatteringTextureUvwz(\r\n"\
"      atmosphere, uvwz, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\r\n"\
"  nu = clamp(nu, mu * mu_s - sqrt((1.0 - mu * mu) * (1.0 - mu_s * mu_s)),\r\n"\
"      mu * mu_s + sqrt((1.0 - mu * mu) * (1.0 - mu_s * mu_s)));\r\n"\
"}\r\n"\
"void ComputeSingleScatteringTexture(IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture, IN(float3) gl_frag_coord,\r\n"\
"    OUT(IrradianceSpectrum) rayleigh, OUT(IrradianceSpectrum) mie) {\r\n"\
"  Length r;\r\n"\
"  Number mu;\r\n"\
"  Number mu_s;\r\n"\
"  Number nu;\r\n"\
"  bool ray_r_mu_intersects_ground;\r\n"\
"  GetRMuMuSNuFromScatteringTextureFragCoord(atmosphere, gl_frag_coord,\r\n"\
"      r, mu, mu_s, nu, ray_r_mu_intersects_ground);\r\n"\
"  ComputeSingleScattering(atmosphere, transmittance_texture,\r\n"\
"      r, mu, mu_s, nu, ray_r_mu_intersects_ground, rayleigh, mie);\r\n"\
"}\r\n"\
"TEMPLATE(AbstractSpectrum)\r\n"\
"AbstractSpectrum GetScattering(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(AbstractScatteringTexture TEMPLATE_ARGUMENT(AbstractSpectrum))\r\n"\
"        scattering_texture,\r\n"\
"    Length r, Number mu, Number mu_s, Number nu,\r\n"\
"    bool ray_r_mu_intersects_ground) {\r\n"\
"  float4 uvwz = GetScatteringTextureUvwzFromRMuMuSNu(\r\n"\
"      atmosphere, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\r\n"\
"  Number tex_coord_x = uvwz.x * Number(SCATTERING_TEXTURE_NU_SIZE - 1);\r\n"\
"  Number tex_x = floor(tex_coord_x);\r\n"\
"  Number lerp = tex_coord_x - tex_x;\r\n"\
"  float3 uvw0 = float3((tex_x + uvwz.y) / Number(SCATTERING_TEXTURE_NU_SIZE),\r\n"\
"      uvwz.z, uvwz.w);\r\n"\
"  float3 uvw1 = float3((tex_x + 1.0 + uvwz.y) / Number(SCATTERING_TEXTURE_NU_SIZE),\r\n"\
"      uvwz.z, uvwz.w);\r\n"\
"  return AbstractSpectrum(texture(scattering_texture, uvw0) * (1.0 - lerp) +\r\n"\
"      texture(scattering_texture, uvw1) * lerp);\r\n"\
"}\r\n"\
"RadianceSpectrum GetScattering(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(ReducedScatteringTexture) single_rayleigh_scattering_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\r\n"\
"    IN(ScatteringTexture) multiple_scattering_texture,\r\n"\
"    Length r, Number mu, Number mu_s, Number nu,\r\n"\
"    bool ray_r_mu_intersects_ground,\r\n"\
"    int scattering_order) {\r\n"\
"  if (scattering_order == 1) {\r\n"\
"    IrradianceSpectrum rayleigh = GetScattering(\r\n"\
"        atmosphere, single_rayleigh_scattering_texture, r, mu, mu_s, nu,\r\n"\
"        ray_r_mu_intersects_ground);\r\n"\
"    IrradianceSpectrum mie = GetScattering(\r\n"\
"        atmosphere, single_mie_scattering_texture, r, mu, mu_s, nu,\r\n"\
"        ray_r_mu_intersects_ground);\r\n"\
"    return rayleigh * RayleighPhaseFunction(nu) +\r\n"\
"        mie * MiePhaseFunction(atmosphere.mie_phase_function_g, nu);\r\n"\
"  } else {\r\n"\
"    return GetScattering(\r\n"\
"        atmosphere, multiple_scattering_texture, r, mu, mu_s, nu,\r\n"\
"        ray_r_mu_intersects_ground);\r\n"\
"  }\r\n"\
"}\r\n"\
"IrradianceSpectrum GetIrradiance(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(IrradianceTexture) irradiance_texture,\r\n"\
"    Length r, Number mu_s);\r\n"\
"RadianceDensitySpectrum ComputeScatteringDensity(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_rayleigh_scattering_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\r\n"\
"    IN(ScatteringTexture) multiple_scattering_texture,\r\n"\
"    IN(IrradianceTexture) irradiance_texture,\r\n"\
"    Length r, Number mu, Number mu_s, Number nu, int scattering_order) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  assert(mu_s >= -1.0 && mu_s <= 1.0);\r\n"\
"  assert(nu >= -1.0 && nu <= 1.0);\r\n"\
"  assert(scattering_order >= 2);\r\n"\
"  float3 zenith_direction = float3(0.0, 0.0, 1.0);\r\n"\
"  float3 omega = float3(sqrt(1.0 - mu * mu), 0.0, mu);\r\n"\
"  Number sun_dir_x = omega.x == 0.0 ? 0.0 : (nu - mu * mu_s) / omega.x;\r\n"\
"  Number sun_dir_y = sqrt(max(1.0 - sun_dir_x * sun_dir_x - mu_s * mu_s, 0.0));\r\n"\
"  float3 omega_s = float3(sun_dir_x, sun_dir_y, mu_s);\r\n"\
"  static static const int SAMPLE_COUNT = 16;\r\n"\
"  static const Angle dphi = pi / Number(SAMPLE_COUNT);\r\n"\
"  static const Angle dtheta = pi / Number(SAMPLE_COUNT);\r\n"\
"  RadianceDensitySpectrum rayleigh_mie =\r\n"\
"      RadianceDensitySpectrum(0.0 * watt_per_cubic_meter_per_sr_per_nm);\r\n"\
"  for (int l = 0; l < SAMPLE_COUNT; ++l) {\r\n"\
"    Angle theta = (Number(l) + 0.5) * dtheta;\r\n"\
"    Number cos_theta = cos(theta);\r\n"\
"    Number sin_theta = sin(theta);\r\n"\
"    bool ray_r_theta_intersects_ground =\r\n"\
"        RayIntersectsGround(atmosphere, r, cos_theta);\r\n"\
"    Length distance_to_ground = 0.0 * m;\r\n"\
"    DimensionlessSpectrum transmittance_to_ground = DimensionlessSpectrum(0.0);\r\n"\
"    DimensionlessSpectrum ground_albedo = DimensionlessSpectrum(0.0);\r\n"\
"    if (ray_r_theta_intersects_ground) {\r\n"\
"      distance_to_ground =\r\n"\
"          DistanceToBottomAtmosphereBoundary(atmosphere, r, cos_theta);\r\n"\
"      transmittance_to_ground =\r\n"\
"          GetTransmittance(atmosphere, transmittance_texture, r, cos_theta,\r\n"\
"              distance_to_ground, true /* ray_intersects_ground */);\r\n"\
"      ground_albedo = atmosphere.ground_albedo;\r\n"\
"    }\r\n"\
"    for (int m = 0; m < 2 * SAMPLE_COUNT; ++m) {\r\n"\
"      Angle phi = (Number(m) + 0.5) * dphi;\r\n"\
"      float3 omega_i =\r\n"\
"          float3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);\r\n"\
"      SolidAngle domega_i = (dtheta / rad) * (dphi / rad) * sin(theta) * sr;\r\n"\
"      Number nu1 = dot(omega_s, omega_i);\r\n"\
"      RadianceSpectrum incident_radiance = GetScattering(atmosphere,\r\n"\
"          single_rayleigh_scattering_texture, single_mie_scattering_texture,\r\n"\
"          multiple_scattering_texture, r, omega_i.z, mu_s, nu1,\r\n"\
"          ray_r_theta_intersects_ground, scattering_order - 1);\r\n"\
"      float3 ground_normal =\r\n"\
"          normalize(zenith_direction * r + omega_i * distance_to_ground);\r\n"\
"      IrradianceSpectrum ground_irradiance = GetIrradiance(\r\n"\
"          atmosphere, irradiance_texture, atmosphere.bottom_radius,\r\n"\
"          dot(ground_normal, omega_s));\r\n"\
"      incident_radiance += transmittance_to_ground *\r\n"\
"          ground_albedo * (1.0 / (PI * sr)) * ground_irradiance;\r\n"\
"      Number nu2 = dot(omega, omega_i);\r\n"\
"      Number rayleigh_density = exp(\r\n"\
"          -(r - atmosphere.bottom_radius) / atmosphere.rayleigh_scale_height);\r\n"\
"      Number mie_density = exp(\r\n"\
"          -(r - atmosphere.bottom_radius) / atmosphere.mie_scale_height);\r\n"\
"      rayleigh_mie += incident_radiance * (\r\n"\
"          atmosphere.rayleigh_scattering * rayleigh_density *\r\n"\
"              RayleighPhaseFunction(nu2) +\r\n"\
"          atmosphere.mie_scattering * mie_density *\r\n"\
"              MiePhaseFunction(atmosphere.mie_phase_function_g, nu2)) *\r\n"\
"          domega_i;\r\n"\
"    }\r\n"\
"  }\r\n"\
"  return rayleigh_mie;\r\n"\
"}\r\n"\
"RadianceSpectrum ComputeMultipleScattering(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    IN(ScatteringDensityTexture) scattering_density_texture,\r\n"\
"    Length r, Number mu, Number mu_s, Number nu,\r\n"\
"    bool ray_r_mu_intersects_ground) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu >= -1.0 && mu <= 1.0);\r\n"\
"  assert(mu_s >= -1.0 && mu_s <= 1.0);\r\n"\
"  assert(nu >= -1.0 && nu <= 1.0);\r\n"\
"  static const int SAMPLE_COUNT = 50;\r\n"\
"  Length dx =\r\n"\
"      DistanceToNearestAtmosphereBoundary(\r\n"\
"          atmosphere, r, mu, ray_r_mu_intersects_ground) /\r\n"\
"              Number(SAMPLE_COUNT);\r\n"\
"  RadianceSpectrum rayleigh_mie_sum =\r\n"\
"      RadianceSpectrum(0.0 * watt_per_square_meter_per_sr_per_nm);\r\n"\
"  for (int i = 0; i <= SAMPLE_COUNT; ++i) {\r\n"\
"    Length d_i = Number(i) * dx;\r\n"\
"    Length r_i =\r\n"\
"        ClampRadius(atmosphere, sqrt(d_i * d_i + 2.0 * r * mu * d_i + r * r));\r\n"\
"    Number mu_i = ClampCosine((r * mu + d_i) / r_i);\r\n"\
"    Number mu_s_i = ClampCosine((r * mu_s + d_i * nu) / r_i);\r\n"\
"    RadianceSpectrum rayleigh_mie_i =\r\n"\
"        GetScattering(\r\n"\
"            atmosphere, scattering_density_texture, r_i, mu_i, mu_s_i, nu,\r\n"\
"            ray_r_mu_intersects_ground) *\r\n"\
"        GetTransmittance(\r\n"\
"            atmosphere, transmittance_texture, r, mu, d_i,\r\n"\
"            ray_r_mu_intersects_ground) *\r\n"\
"        dx;\r\n"\
"    Number weight_i = (i == 0 || i == SAMPLE_COUNT) ? 0.5 : 1.0;\r\n"\
"    rayleigh_mie_sum += rayleigh_mie_i * weight_i;\r\n"\
"  }\r\n"\
"  return rayleigh_mie_sum;\r\n"\
"}\r\n"\
"RadianceDensitySpectrum ComputeScatteringDensityTexture(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_rayleigh_scattering_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\r\n"\
"    IN(ScatteringTexture) multiple_scattering_texture,\r\n"\
"    IN(IrradianceTexture) irradiance_texture,\r\n"\
"    IN(float3) gl_frag_coord, int scattering_order) {\r\n"\
"  Length r;\r\n"\
"  Number mu;\r\n"\
"  Number mu_s;\r\n"\
"  Number nu;\r\n"\
"  bool ray_r_mu_intersects_ground;\r\n"\
"  GetRMuMuSNuFromScatteringTextureFragCoord(atmosphere, gl_frag_coord,\r\n"\
"      r, mu, mu_s, nu, ray_r_mu_intersects_ground);\r\n"\
"  return ComputeScatteringDensity(atmosphere, transmittance_texture,\r\n"\
"      single_rayleigh_scattering_texture, single_mie_scattering_texture,\r\n"\
"      multiple_scattering_texture, irradiance_texture, r, mu, mu_s, nu,\r\n"\
"      scattering_order);\r\n"\
"}\r\n"\
"RadianceSpectrum ComputeMultipleScatteringTexture(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    IN(ScatteringDensityTexture) scattering_density_texture,\r\n"\
"    IN(float3) gl_frag_coord, OUT(Number) nu) {\r\n"\
"  Length r;\r\n"\
"  Number mu;\r\n"\
"  Number mu_s;\r\n"\
"  bool ray_r_mu_intersects_ground;\r\n"\
"  GetRMuMuSNuFromScatteringTextureFragCoord(atmosphere, gl_frag_coord,\r\n"\
"      r, mu, mu_s, nu, ray_r_mu_intersects_ground);\r\n"\
"  return ComputeMultipleScattering(atmosphere, transmittance_texture,\r\n"\
"      scattering_density_texture, r, mu, mu_s, nu,\r\n"\
"      ray_r_mu_intersects_ground);\r\n"\
"}\r\n"\
"IrradianceSpectrum ComputeDirectIrradiance(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    Length r, Number mu_s) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu_s >= -1.0 && mu_s <= 1.0);\r\n"\
"  return atmosphere.solar_irradiance *\r\n"\
"      GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"          atmosphere, transmittance_texture, r, mu_s) * max(mu_s, 0.0);\r\n"\
"}\r\n"\
"IrradianceSpectrum ComputeIndirectIrradiance(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(ReducedScatteringTexture) single_rayleigh_scattering_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\r\n"\
"    IN(ScatteringTexture) multiple_scattering_texture,\r\n"\
"    Length r, Number mu_s, int scattering_order) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu_s >= -1.0 && mu_s <= 1.0);\r\n"\
"  assert(scattering_order >= 1);\r\n"\
"  static const int SAMPLE_COUNT = 32;\r\n"\
"  const Angle dphi = pi / Number(SAMPLE_COUNT);\r\n"\
"  const Angle dtheta = pi / Number(SAMPLE_COUNT);\r\n"\
"  IrradianceSpectrum result =\r\n"\
"      IrradianceSpectrum(0.0 * watt_per_square_meter_per_nm);\r\n"\
"  float3 omega_s = float3(sqrt(1.0 - mu_s * mu_s), 0.0, mu_s);\r\n"\
"  for (int j = 0; j < SAMPLE_COUNT / 2; ++j) {\r\n"\
"    Angle theta = (Number(j) + 0.5) * dtheta;\r\n"\
"    bool ray_r_theta_intersects_ground =\r\n"\
"        RayIntersectsGround(atmosphere, r, cos(theta));\r\n"\
"    for (int i = 0; i < 2 * SAMPLE_COUNT; ++i) {\r\n"\
"      Angle phi = (Number(i) + 0.5) * dphi;\r\n"\
"      float3 omega =\r\n"\
"          float3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta));\r\n"\
"      SolidAngle domega = (dtheta / rad) * (dphi / rad) * sin(theta) * sr;\r\n"\
"      Number nu = dot(omega, omega_s);\r\n"\
"      result += GetScattering(atmosphere, single_rayleigh_scattering_texture,\r\n"\
"          single_mie_scattering_texture, multiple_scattering_texture,\r\n"\
"          r, omega.z, mu_s, nu, ray_r_theta_intersects_ground,\r\n"\
"          scattering_order) *\r\n"\
"              omega.z * domega;\r\n"\
"    }\r\n"\
"  }\r\n"\
"  return result;\r\n"\
"}\r\n"\
"float2 GetIrradianceTextureUvFromRMuS(IN(AtmosphereParameters) atmosphere,\r\n"\
"    Length r, Number mu_s) {\r\n"\
"  assert(r >= atmosphere.bottom_radius && r <= atmosphere.top_radius);\r\n"\
"  assert(mu_s >= -1.0 && mu_s <= 1.0);\r\n"\
"  Number x_r = (r - atmosphere.bottom_radius) /\r\n"\
"      (atmosphere.top_radius - atmosphere.bottom_radius);\r\n"\
"  Number x_mu_s = mu_s * 0.5 + 0.5;\r\n"\
"  return float2(GetTextureCoordFromUnitRange(x_mu_s, IRRADIANCE_TEXTURE_WIDTH),\r\n"\
"              GetTextureCoordFromUnitRange(x_r, IRRADIANCE_TEXTURE_HEIGHT));\r\n"\
"}\r\n"\
"void GetRMuSFromIrradianceTextureUv(IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(float2) uv, OUT(Length) r, OUT(Number) mu_s) {\r\n"\
"  assert(uv.x >= 0.0 && uv.x <= 1.0);\r\n"\
"  assert(uv.y >= 0.0 && uv.y <= 1.0);\r\n"\
"  Number x_mu_s = GetUnitRangeFromTextureCoord(uv.x, IRRADIANCE_TEXTURE_WIDTH);\r\n"\
"  Number x_r = GetUnitRangeFromTextureCoord(uv.y, IRRADIANCE_TEXTURE_HEIGHT);\r\n"\
"  r = atmosphere.bottom_radius +\r\n"\
"      x_r * (atmosphere.top_radius - atmosphere.bottom_radius);\r\n"\
"  mu_s = ClampCosine(2.0 * x_mu_s - 1.0);\r\n"\
"}\r\n"\
"static const float2 IRRADIANCE_TEXTURE_SIZE =\r\n"\
"    float2(IRRADIANCE_TEXTURE_WIDTH, IRRADIANCE_TEXTURE_HEIGHT);\r\n"\
"IrradianceSpectrum ComputeDirectIrradianceTexture(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    IN(float2) gl_frag_coord) {\r\n"\
"  Length r;\r\n"\
"  Number mu_s;\r\n"\
"  GetRMuSFromIrradianceTextureUv(\r\n"\
"      atmosphere, gl_frag_coord / IRRADIANCE_TEXTURE_SIZE, r, mu_s);\r\n"\
"  return ComputeDirectIrradiance(atmosphere, transmittance_texture, r, mu_s);\r\n"\
"}\r\n"\
"IrradianceSpectrum ComputeIndirectIrradianceTexture(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(ReducedScatteringTexture) single_rayleigh_scattering_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\r\n"\
"    IN(ScatteringTexture) multiple_scattering_texture,\r\n"\
"    IN(float2) gl_frag_coord, int scattering_order) {\r\n"\
"  Length r;\r\n"\
"  Number mu_s;\r\n"\
"  GetRMuSFromIrradianceTextureUv(\r\n"\
"      atmosphere, gl_frag_coord / IRRADIANCE_TEXTURE_SIZE, r, mu_s);\r\n"\
"  return ComputeIndirectIrradiance(atmosphere,\r\n"\
"      single_rayleigh_scattering_texture, single_mie_scattering_texture,\r\n"\
"      multiple_scattering_texture, r, mu_s, scattering_order);\r\n"\
"}\r\n"\
"IrradianceSpectrum GetIrradiance(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(IrradianceTexture) irradiance_texture,\r\n"\
"    Length r, Number mu_s) {\r\n"\
"  float2 uv = GetIrradianceTextureUvFromRMuS(atmosphere, r, mu_s);\r\n"\
"  return IrradianceSpectrum(texture(irradiance_texture, uv));\r\n"\
"}\r\n"\
"#ifdef COMBINED_SCATTERING_TEXTURES\r\n"\
"float3 GetExtrapolatedSingleMieScattering(\r\n"\
"    IN(AtmosphereParameters) atmosphere, IN(float4) scattering) {\r\n"\
"  if (scattering.r == 0.0) {\r\n"\
"    return float3(0.0);\r\n"\
"  }\r\n"\
"  return scattering.rgb * scattering.a / scattering.r *\r\n"\
"	    (atmosphere.rayleigh_scattering.r / atmosphere.mie_scattering.r) *\r\n"\
"	    (atmosphere.mie_scattering / atmosphere.rayleigh_scattering);\r\n"\
"}\r\n"\
"#endif\r\n"\
"IrradianceSpectrum GetCombinedScattering(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(ReducedScatteringTexture) scattering_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\r\n"\
"    Length r, Number mu, Number mu_s, Number nu,\r\n"\
"    bool ray_r_mu_intersects_ground,\r\n"\
"    OUT(IrradianceSpectrum) single_mie_scattering) {\r\n"\
"  float4 uvwz = GetScatteringTextureUvwzFromRMuMuSNu(\r\n"\
"      atmosphere, r, mu, mu_s, nu, ray_r_mu_intersects_ground);\r\n"\
"  Number tex_coord_x = uvwz.x * Number(SCATTERING_TEXTURE_NU_SIZE - 1);\r\n"\
"  Number tex_x = floor(tex_coord_x);\r\n"\
"  Number lerp = tex_coord_x - tex_x;\r\n"\
"  float3 uvw0 = float3((tex_x + uvwz.y) / Number(SCATTERING_TEXTURE_NU_SIZE),\r\n"\
"      uvwz.z, uvwz.w);\r\n"\
"  float3 uvw1 = float3((tex_x + 1.0 + uvwz.y) / Number(SCATTERING_TEXTURE_NU_SIZE),\r\n"\
"      uvwz.z, uvwz.w);\r\n"\
"#ifdef COMBINED_SCATTERING_TEXTURES\r\n"\
"  float4 combined_scattering =\r\n"\
"      texture(scattering_texture, uvw0) * (1.0 - lerp) +\r\n"\
"      texture(scattering_texture, uvw1) * lerp;\r\n"\
"  IrradianceSpectrum scattering = IrradianceSpectrum(combined_scattering);\r\n"\
"  single_mie_scattering =\r\n"\
"      GetExtrapolatedSingleMieScattering(atmosphere, combined_scattering);\r\n"\
"#else\r\n"\
"  IrradianceSpectrum scattering = IrradianceSpectrum(\r\n"\
"      texture(scattering_texture, uvw0) * (1.0 - lerp) +\r\n"\
"      texture(scattering_texture, uvw1) * lerp);\r\n"\
"  single_mie_scattering = IrradianceSpectrum(\r\n"\
"      texture(single_mie_scattering_texture, uvw0) * (1.0 - lerp) +\r\n"\
"      texture(single_mie_scattering_texture, uvw1) * lerp);\r\n"\
"#endif\r\n"\
"  return scattering;\r\n"\
"}\r\n"\
"RadianceSpectrum GetSkyRadiance(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    IN(ReducedScatteringTexture) scattering_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\r\n"\
"    Position camera, IN(Direction) view_ray, Length shadow_length,\r\n"\
"    IN(Direction) sun_direction, OUT(DimensionlessSpectrum) transmittance) {\r\n"\
"  Length r = length(camera);\r\n"\
"  Length rmu = dot(camera, view_ray);\r\n"\
"  Length distance_to_top_atmosphere_boundary = -rmu -\r\n"\
"      sqrt(rmu * rmu - r * r + atmosphere.top_radius * atmosphere.top_radius);\r\n"\
"  if (distance_to_top_atmosphere_boundary > 0.0 * m) {\r\n"\
"    camera = camera + view_ray * distance_to_top_atmosphere_boundary;\r\n"\
"    r = atmosphere.top_radius;\r\n"\
"    rmu += distance_to_top_atmosphere_boundary;\r\n"\
"  }\r\n"\
"  if (r > atmosphere.top_radius) {\r\n"\
"    transmittance = DimensionlessSpectrum(1.0);\r\n"\
"    return RadianceSpectrum(0.0 * watt_per_square_meter_per_sr_per_nm);\r\n"\
"  }\r\n"\
"  Number mu = rmu / r;\r\n"\
"  Number mu_s = dot(camera, sun_direction) / r;\r\n"\
"  Number nu = dot(view_ray, sun_direction);\r\n"\
"  bool ray_r_mu_intersects_ground = RayIntersectsGround(atmosphere, r, mu);\r\n"\
"  transmittance = ray_r_mu_intersects_ground ? DimensionlessSpectrum(0.0) :\r\n"\
"      GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"          atmosphere, transmittance_texture, r, mu);\r\n"\
"  IrradianceSpectrum single_mie_scattering;\r\n"\
"  IrradianceSpectrum scattering;\r\n"\
"  if (shadow_length == 0.0 * m) {\r\n"\
"    scattering = GetCombinedScattering(\r\n"\
"        atmosphere, scattering_texture, single_mie_scattering_texture,\r\n"\
"        r, mu, mu_s, nu, ray_r_mu_intersects_ground,\r\n"\
"        single_mie_scattering);\r\n"\
"  } else {\r\n"\
"    Length d = shadow_length;\r\n"\
"    Length r_p =\r\n"\
"        ClampRadius(atmosphere, sqrt(d * d + 2.0 * r * mu * d + r * r));\r\n"\
"    Number mu_p = (r * mu + d) / r_p;\r\n"\
"    Number mu_s_p = (r * mu_s + d * nu) / r_p;\r\n"\
"    scattering = GetCombinedScattering(\r\n"\
"        atmosphere, scattering_texture, single_mie_scattering_texture,\r\n"\
"        r_p, mu_p, mu_s_p, nu, ray_r_mu_intersects_ground,\r\n"\
"        single_mie_scattering);\r\n"\
"    DimensionlessSpectrum shadow_transmittance =\r\n"\
"        GetTransmittance(atmosphere, transmittance_texture,\r\n"\
"            r, mu, shadow_length, ray_r_mu_intersects_ground);\r\n"\
"    scattering = scattering * shadow_transmittance;\r\n"\
"    single_mie_scattering = single_mie_scattering * shadow_transmittance;\r\n"\
"  }\r\n"\
"  return scattering * RayleighPhaseFunction(nu) + single_mie_scattering *\r\n"\
"      MiePhaseFunction(atmosphere.mie_phase_function_g, nu);\r\n"\
"}\r\n"\
"RadianceSpectrum GetSkyRadianceToPoint(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    IN(ReducedScatteringTexture) scattering_texture,\r\n"\
"    IN(ReducedScatteringTexture) single_mie_scattering_texture,\r\n"\
"    Position camera, IN(Position) point, Length shadow_length,\r\n"\
"    IN(Direction) sun_direction, OUT(DimensionlessSpectrum) transmittance) {\r\n"\
"  Direction view_ray = normalize(point - camera);\r\n"\
"  Length r = length(camera);\r\n"\
"  Length rmu = dot(camera, view_ray);\r\n"\
"  Length distance_to_top_atmosphere_boundary = -rmu -\r\n"\
"      sqrt(rmu * rmu - r * r + atmosphere.top_radius * atmosphere.top_radius);\r\n"\
"  if (distance_to_top_atmosphere_boundary > 0.0 * m) {\r\n"\
"    camera = camera + view_ray * distance_to_top_atmosphere_boundary;\r\n"\
"    r = atmosphere.top_radius;\r\n"\
"    rmu += distance_to_top_atmosphere_boundary;\r\n"\
"  }\r\n"\
"  Number mu = rmu / r;\r\n"\
"  Number mu_s = dot(camera, sun_direction) / r;\r\n"\
"  Number nu = dot(view_ray, sun_direction);\r\n"\
"  Length d = length(point - camera);\r\n"\
"  bool ray_r_mu_intersects_ground = RayIntersectsGround(atmosphere, r, mu);\r\n"\
"  transmittance = GetTransmittance(atmosphere, transmittance_texture,\r\n"\
"      r, mu, d, ray_r_mu_intersects_ground);\r\n"\
"  IrradianceSpectrum single_mie_scattering;\r\n"\
"  IrradianceSpectrum scattering = GetCombinedScattering(\r\n"\
"      atmosphere, scattering_texture, single_mie_scattering_texture,\r\n"\
"      r, mu, mu_s, nu, ray_r_mu_intersects_ground,\r\n"\
"      single_mie_scattering);\r\n"\
"  d = max(d - shadow_length, 0.0 * m);\r\n"\
"  Length r_p = ClampRadius(atmosphere, sqrt(d * d + 2.0 * r * mu * d + r * r));\r\n"\
"  Number mu_p = (r * mu + d) / r_p;\r\n"\
"  Number mu_s_p = (r * mu_s + d * nu) / r_p;\r\n"\
"  IrradianceSpectrum single_mie_scattering_p;\r\n"\
"  IrradianceSpectrum scattering_p = GetCombinedScattering(\r\n"\
"      atmosphere, scattering_texture, single_mie_scattering_texture,\r\n"\
"      r_p, mu_p, mu_s_p, nu, ray_r_mu_intersects_ground,\r\n"\
"      single_mie_scattering_p);\r\n"\
"  DimensionlessSpectrum shadow_transmittance = transmittance;\r\n"\
"  if (shadow_length > 0.0 * m) {\r\n"\
"    shadow_transmittance = GetTransmittance(atmosphere, transmittance_texture,\r\n"\
"        r, mu, d, ray_r_mu_intersects_ground);\r\n"\
"  }\r\n"\
"  scattering = scattering - shadow_transmittance * scattering_p;\r\n"\
"  single_mie_scattering =\r\n"\
"      single_mie_scattering - shadow_transmittance * single_mie_scattering_p;\r\n"\
"#ifdef COMBINED_SCATTERING_TEXTURES\r\n"\
"  single_mie_scattering = GetExtrapolatedSingleMieScattering(\r\n"\
"      atmosphere, float4(scattering, single_mie_scattering.r));\r\n"\
"#endif\r\n"\
"  single_mie_scattering = single_mie_scattering *\r\n"\
"      smoothstep(Number(0.0), Number(0.01), mu_s);\r\n"\
"  return scattering * RayleighPhaseFunction(nu) + single_mie_scattering *\r\n"\
"      MiePhaseFunction(atmosphere.mie_phase_function_g, nu);\r\n"\
"}\r\n"\
"IrradianceSpectrum GetSunAndSkyIrradiance(\r\n"\
"    IN(AtmosphereParameters) atmosphere,\r\n"\
"    IN(TransmittanceTexture) transmittance_texture,\r\n"\
"    IN(IrradianceTexture) irradiance_texture,\r\n"\
"    IN(Position) point, IN(Direction) normal, IN(Direction) sun_direction,\r\n"\
"    OUT(IrradianceSpectrum) sky_irradiance) {\r\n"\
"  Length r = length(point);\r\n"\
"  Number mu_s = dot(point, sun_direction) / r;\r\n"\
"  sky_irradiance = GetIrradiance(atmosphere, irradiance_texture, r, mu_s) *\r\n"\
"      (1.0 + dot(normal, point) / r) * 0.5;\r\n"\
"  return atmosphere.solar_irradiance *\r\n"\
"      GetTransmittanceToTopAtmosphereBoundary(\r\n"\
"          atmosphere, transmittance_texture, r, mu_s) *\r\n"\
"      smoothstep(-atmosphere.sun_angular_radius / rad,\r\n"\
"                 atmosphere.sun_angular_radius / rad,\r\n"\
"                 mu_s) *\r\n"\
"      max(dot(normal, sun_direction), 0.0);\r\n"\
"}\r\n"\
"";
