//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------
//---------------------------------
//	Samplers & Textures
//---------------------------------


cbuffer Positions : register(b0)
{
	float4 camera_position;
	float4 light_position;
};

SamplerState linear_Wrap : register ( s0 );
Texture2D AlbedoTexture  : register ( t0 );
Texture2D DepthTexture   : register ( t1 );

//---------------------------------
//	Deferred Base Pixel Structs
//---------------------------------

struct VS_OUTPUT
{
	float4 pos 		: SV_POSITION;
	float3 normal 	: NORMAL;
	float2 uv 		: TEXCOORD;
	float3 binorm 	: BINORMAL;
	float3 tang 	: TANGENT;
	float4 worldpos : POSITION;
	float4 tex 		: TEX;
};


//---------------------------------
//	Deferred Base Pixel Shader
//---------------------------------

static const float4 ESun = float4(500,250,250,0);
static const float g = -0.75;
static const float4 br = float4(5.5e-6, 13.0e-6, 22.4e-6, 1);
static const float bm = 21e-6;
static const float pi = 3.14;

float4 rayleigh(float theta) //rayleigh
{
	return 3 / ( 16 * pi ) * br * ( 1 + cos(theta) * cos(theta));
}

float4 mie(float theta) //mie
{
	return 1 / ( 4 * pi ) * bm * (( 1 - g ) * ( 1 - g )) / ( pow( 1 + g * g - 2 * g * cos(theta), 3 / 2) ); 
}

float henyeygreenstein(float theta)
{
	return ( ( 1 - g ) * ( 1 - g )) / 4 * pi * pow(( ( 1 + g ) * ( 1 + g ) ) - ( 2 * g * cos(theta)), 3/2);
}

float2 RayVsSphere(float3 ray0, float3 ray_direction, float sphere_radius) 
{
    // ray-sphere intersection that assumes
    // the sphere is centered at the origin.
    // No intersection when result.x > result.y
    float a = dot(ray_direction, ray_direction);
    float b = 2.0 * dot(ray_direction, ray0);
    float c = dot(ray0, ray0) - (sphere_radius * sphere_radius);
    float d = (b*b) - 4.0*a*c;
    if (d < 0.0) return float2(1e5,-1e5);
    return float2(
        (-b - sqrt(d))/(2.0*a),
        (-b + sqrt(d))/(2.0*a)
    );
}


float4 atmospheric(float theta, float s)
{
	return (rayleigh(theta) * mie(theta)) / (br + bm) * light_position * (1 - exp( -(br+bm) * s ));
}

static const int iSteps = 32;
static const int jSteps = 8;

float3 atmosphere(float3 r, float3 r0, float3 pSun, float iSun, float rPlanet, float rAtmos, float3 kRlh, float kMie, float shRlh, float shMie, float gv) 
{
    // Normalize the sun and view directions.
    pSun = normalize(pSun);
    r = normalize(r);

    // Calculate the step size of the primary ray.
    float2 p = RayVsSphere(r0, r, rAtmos);
    if (p.x > p.y) return float3(0,0,0);
    p.y = min(p.y, RayVsSphere(r0, r, rPlanet).x);
    float iStepSize = (p.y - p.x) / float(iSteps);

    // Initialize the primary ray time.
    float iTime = 0.0;

    // Initialize accumulators for Rayleigh and Mie scattering.
    float3 totalRlh = float3(0,0,0);
    float3 totalMie = float3(0,0,0);

    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = gv * gv;
    float pRlh = 3.0 / (16.0 * pi) * (1.0 + mumu); //rayleigh
    float pMie = 3.0 / (8.0 * pi) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * gv, 1.5) * (2.0 + gg)); //mie

    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {

        // Calculate the primary ray sample position.
        float3 iPos = r0 + r * (iTime + iStepSize * 0.5);

        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;

        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Calculate the step size of the secondary ray.
        float jStepSize = RayVsSphere(iPos, pSun, rAtmos).y / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0;

        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {

            // Calculate the secondary ray sample position.
            float3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;

            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        float3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}


float4 PS(VS_OUTPUT input) : SV_Target
{
	float height = input.worldpos.y;
	float4 tex = input.tex;
	input.tex /= input.tex.w;
	float2 texCoord = input.tex.xy;	
	float depth = DepthTexture.Sample(linear_Wrap, texCoord).x;

	if(depth < 1.f)
		discard;
	
    float3 camPos = float3(0,0,0);
	float2 uv = texCoord * 2 - 1;
	float3 ray_dir = float3(uv, 1);
	float3 sun_pos = float3(0,0, 1);
	float3 output_2 = atmosphere(
		ray_dir,
	 	camera_position,
	  	light_position,                   // position of the sun
        14.0,                           // intensity of the sun
        6371e3,                         // radius of the planet in meters
        6471e3,                         // radius of the atmosphere in meters
        float3(5.5e-6, 13.0e-6, 22.4e-6), // Rayleigh scattering coefficient
        21e-6,                          // Mie scattering coefficient
        8e3,                            // Rayleigh scale height
        1.2e3,                          // Mie scale height
        0.988 );
	return float4(output_2, 1);

	float4 light_dir = normalize( input.worldpos - light_position );
	float4 view_dir = normalize(input.worldpos - camera_position);
	float length_of = length(input.worldpos + camera_position);

	float n_theta = acos(dot(normalize(light_dir - input.worldpos), view_dir));

 	//float theta = acos(dot(normalize(light_dir - input.worldpos), normalize(input.worldpos - camera_position) * float4(-1,0,0,0)));

	float4 output = atmospheric(n_theta, length_of) * 10000 * 5 ; 
	output = pow( clamp ( output / ( output + 1.0), 0, 1),  1 /2.2);
	return output;
}