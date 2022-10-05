//--------------------------------------------------------------------------------------
// File: Instancing.fx
//
// The effect file for the Instancing sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
texture2D g_txScene : TEXTURE;		// texture for scene rendering
float4x4 g_mWorld : WORLD : register(c0); // World matrix for object
float4x4 g_mView : VIEW : register(c4);			// World matrix for object
float4x4 g_mProj : PROJECTION: register(c8) ;		// World matrix for object

// only used for vs_2_0 Shader Instancing
// #define g_nNumBatchInstance 100
static const int g_nNumBatchInstance = 100;
float4x4 g_vBoxInstance_Position[g_nNumBatchInstance] : BOXINSTANCEARRAY_POSITION : register(c13);
//-----------------------------------------------------------------------------
// Texture samplers
//-----------------------------------------------------------------------------
sampler2D g_samScene =
sampler_state
{
    Texture = <g_txScene>;
};


//-----------------------------------------------------------------------------
// Name: VS_ShaderInstancing
// Type: Vertex shader (Shader Instancing)
// Desc: This shader computes standard transform and lighting for unlit, texture-mapped triangles.
//-----------------------------------------------------------------------------

struct VS_INPUT
{
	float3 mPosition : POSITION;
	float2 mUV : TEXCOORD0;
	float mIdx : TEXCOORD1;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mUV : TEXCOORD0;
};

struct PS_INPUT
{
	float2 mUV : TEXCOORD0;
};

VS_OUTPUT VS_ShaderInstancing(VS_INPUT Input)
{
	VS_OUTPUT output;
	float4x4 tmp = g_vBoxInstance_Position[20];
	output.mPosition = mul( float4(Input.mPosition, 1.f),  g_vBoxInstance_Position[Input.mIdx]);
	output.mPosition = mul( output.mPosition, g_mView );
	output.mPosition = mul( output.mPosition, g_mProj );
	
	output.mUV = Input.mUV;
	return  output;
}


//-----------------------------------------------------------------------------
// Name: PS
// Type: Pixel shader
// Desc: This shader outputs the pixel's color by modulating the texture's
//		 color with diffuse material color
//-----------------------------------------------------------------------------
float4 PS(PS_INPUT Input) : COLOR0
{
    // Lookup texture and modulate it with diffuse
    return tex2D( g_samScene, Input.mUV );
}

//-----------------------------------------------------------------------------
// Name: TShader_Instancing
// Type: Technique
// Desc: Renders scene through Shader Instancing (batching)
//-----------------------------------------------------------------------------
technique TShader_Instancing
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_ShaderInstancing();
        PixelShader  = compile ps_2_0 PS();
		ZEnable = true;
        AlphaBlendEnable = false;
    }
}
