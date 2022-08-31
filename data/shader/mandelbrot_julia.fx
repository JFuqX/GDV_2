/**
 * @file mandelbrot_julia.fx
 * @author John Reinecke
 * Contact: j.reinecke@skyridge.eu
 */

//Translation from GLSL to HLSL for the function (reference): https://docs.microsoft.com/en-us/windows/uwp/gaming/glsl-to-hlsl-reference

// #############################################################################
// Definition of the constant Buffers
// #############################################################################
cbuffer VSBuffer : register(b0)         // Register the constant buffer on slot 0
{
	float4x4 g_ViewProjectionMatrix;
	float4x4 g_WorldMatrix;
};

cbuffer PSBuffer : register(b0)         // Register the pixel constant buffer on slot 0 (no collision to VSBuffer because of different shader stage)
{
	float2 m_Resolution;
	float m_Time;
};

// #############################################################################
// Defition of input and output data of the vertex shader.
// #############################################################################
struct VSInput
{
	float3 m_Position : POSITION;
};

struct PSInput
{
	float4 m_Position : SV_POSITION;
};

// #############################################################################
// Additional Functions
// #############################################################################

//Basic mandelbrot function
//Source: https://www.youtube.com/shorts/h5PuIm6fRr8
//Source: http://nuclear.mutantstargoat.com/articles/sdr_fract/#julia
//Source: https://www.shadertoy.com/view/wtBBWd
//Source: http://www.fractalforums.com/programming/mandelbrot-in-xna-plus-hlsl/

static const float MAX_ITER = 128.0f;
float mandelbrot(float2 uv) {
	float2 c = 4.0f * uv - float2(1.0f, 0.0f);
	c = c / pow(m_Time, 4.0f) - float2(0.65f, 0.45f);
	float2 z = float2(0.0f, 0.0f);
	float iter = 0.0f;
	for (float i; i < MAX_ITER; i++) {
		z = float2(z.x * z.x - z.y * z.y, 
			2.0f * z.x * z.y) + c;
		if (dot(z, z) > 4.0f) return iter / MAX_ITER;
		iter++;
	}
	return 0.0f;
}

// Simple hashfunction
float3 hashfunction(float c) {
	float x = frac(sin(c) * 5625.246f);
	float y = frac(sin(c + x) * 2216.486f);
	float z = frac(sin(c * y) * 8276.352f);
	return float3(x, y, z);
}

//Basic julia fractal function (UNFINISHED)
//Source: https://blog.nostatic.org/2009/09/fractal-rendering-on-gpu-mandelbrot-and.html
//Source: http://nuclear.mutantstargoat.com/articles/sdr_fract/#julia

float julia(float2 uv) {

	float2 z = float2(0.0f, 0.0f);
	z.x = 3.0f * (uv.x - 0.5f);
	z.y = 2.0f * (uv.y - 0.5f);

	float iter;
	for (iter = 0.0f; iter < MAX_ITER; iter++) {
		float x = (z.x * z.x - z.y * z.y) + uv.x;
		float y = (z.y * z.x + z.x * z.y) + uv.y;

		if (dot(z, z) > 4.0f) return iter / MAX_ITER;
		//z.x = x;
		//z.y = y;

		
	}
	return 0.0f;
}

// #############################################################################
// Vertex Shader
// #############################################################################

PSInput VSShader(VSInput _Input)
{
	float4 WSPosition;

	PSInput Output = (PSInput)0;
	WSPosition = mul(float4(_Input.m_Position, 1.0f), g_WorldMatrix);
	Output.m_Position = mul(WSPosition, g_ViewProjectionMatrix);

	return Output;
}

// #############################################################################
// Pixel Shader
// #############################################################################
float4 PSShader(PSInput Input) : SV_Target
{
	float2 uv = (Input.m_Position.xy - 0.5f * m_Resolution.xy) / m_Resolution.y;
	float3 color = float3(0.0f, 0.0f, 0.0f); //Basecolor black defined

	float m = mandelbrot(uv); //Running the mandelbrot function

	//#### currently not working###
	//float m = julia(uv); //Running the julia fractal function
	//#### currently not working###

	color += hashfunction(m); //Hashfunction to change the color

	float4 fragColor = float4(color, 1);

	return fragColor;
}
