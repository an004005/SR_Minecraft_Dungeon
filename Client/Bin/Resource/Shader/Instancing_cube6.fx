//--------------------------------------------------------------------------------------
// File: Instancing.fx
//
// The effect file for the Instancing sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


texture3D g_txHead : TEXTURE0;
texture3D g_txBody : TEXTURE1;
texture3D g_txArmL : TEXTURE2;
texture3D g_txArmR : TEXTURE3;
texture3D g_txLegL : TEXTURE4;
texture3D g_txLegR : TEXTURE5;

float4x4 g_mView : VIEW;			// World matrix for object
float4x4 g_mProj : PROJECTION;		// World matrix for object

samplerCUBE g_samHead =
sampler_state
{
    Texture = <g_txHead>;
};
samplerCUBE g_samBody =
sampler_state
{
    Texture = <g_txBody>;
};
samplerCUBE g_samArmL =
sampler_state
{
    Texture = <g_txArmL>;
};
samplerCUBE g_samArmR =
sampler_state
{
    Texture = <g_txArmR>;
};
samplerCUBE g_samLegL =
sampler_state
{
    Texture = <g_txLegL>;
};
samplerCUBE g_samLegR =
sampler_state
{
    Texture = <g_txLegR>;
};

struct PS_INPUT
{
	float3 mUV : TEXCOORD0;
	float mOrder : TEXCOORD1;
};

void VS_HWInstancing( float3 vPos : POSITION0,
					float3 vTex0 : TEXCOORD0,
					float4 vRight : BLENDWEIGHT0,
					float4 vUp : BLENDWEIGHT1,
					float4 vLook : BLENDWEIGHT2,
					float4 vPosValue : BLENDWEIGHT3,
					float fOrder : TEXCOORD1,
					out float4 oPos : POSITION,
					out float3 oTex0 : TEXCOORD0,
					out float oOrder : TEXCOORD1)
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
	oOrder = fOrder;
}


//-----------------------------------------------------------------------------
// Name: PS
// Type: Pixel shader
// Desc: This shader outputs the pixel's color by modulating the texture's
//		 color with diffuse material color
//-----------------------------------------------------------------------------
float4 PS(PS_INPUT Input) : COLOR0
{
	if (Input.mOrder <= 0.01f)
		return texCUBE( g_samHead, Input.mUV );
	else if (Input.mOrder <= 0.11f)
		return texCUBE( g_samBody, Input.mUV );
	else if (Input.mOrder <= 0.21f)
		return texCUBE( g_samArmL, Input.mUV );
	else if (Input.mOrder < 0.31f)
		return texCUBE( g_samArmR, Input.mUV );
	else if (Input.mOrder < 0.41f)
		return texCUBE( g_samLegL, Input.mUV );
	else if (Input.mOrder < 0.51f)
		return texCUBE( g_samLegR, Input.mUV );
	return texCUBE( g_samLegL, Input.mUV );
}


technique THW_Instancing
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_HWInstancing();
        PixelShader  = compile ps_2_0 PS();
    }
}
