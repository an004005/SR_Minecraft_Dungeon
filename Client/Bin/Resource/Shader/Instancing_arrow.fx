//--------------------------------------------------------------------------------------
// File: Instancing.fx
//
// The effect file for the Instancing sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


texture2D g_txScene : TEXTURE;		// texture for scene rendering
float4x4 g_mView : VIEW : register(c4);			// World matrix for object
float4x4 g_mProj : PROJECTION: register(c8) ;		// World matrix for object

sampler2D g_samScene =
sampler_state
{
    Texture = <g_txScene>;
};

struct PS_INPUT
{
	float2 mUV : TEXCOORD0;
};

void VS_HWInstancing( float3 vPos : POSITION0,
					float2 vTex0 : TEXCOORD0,
					float4 vRight : BLENDWEIGHT0,
					float4 vUp : BLENDWEIGHT1,
					float4 vLook : BLENDWEIGHT2,
					float4 vPosValue : BLENDWEIGHT3,
					out float4 oPos : POSITION,
					out float2 oTex0 : TEXCOORD0 )
{
	float4x4 world;
	world[0][0] = vRight.x;
	world[0][1] = vRight.y;
	world[0][2] = vRight.z;
	world[0][3] = vRight.w;

	world[1][0] = vUp.x;
	world[1][1] = vUp.y;
	world[1][2] = vUp.z;
	world[1][3] = vUp.w;

	world[2][0] = vLook.x;
	world[2][1] = vLook.y;
	world[2][2] = vLook.z;
	world[2][3] = vLook.w;

	world[3][0] = vPosValue.x;
	world[3][1] = vPosValue.y;
	world[3][2] = vPosValue.z;
	world[3][3] = vPosValue.w;

	oPos = mul( float4(vPos, 1.f), world );
	oPos = mul( oPos, g_mView );
	oPos = mul( oPos, g_mProj );
	
	// Just copy the texture coordinate & color through
	oTex0 = vTex0;
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


technique THW_Instancing
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_HWInstancing();
        PixelShader  = compile ps_2_0 PS();
    }
}
