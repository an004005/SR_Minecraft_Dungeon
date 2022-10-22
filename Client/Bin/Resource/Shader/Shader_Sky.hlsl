matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture		g_DefaultTexture;

sampler		DefaultSampler = sampler_state 
{
	texture = g_DefaultTexture;
	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3		vPosition : POSITION;		
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;	
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT output = (VS_OUT)0;

    // Translation은 하지 않고 Rotation만 적용한다
    float4 viewPos = mul(float4(In.vPosition, 0), g_ViewMatrix);
    float4 clipSpacePos = mul(viewPos, g_ProjMatrix);
    
    // w/w=1이기 때문에 항상 깊이가 1로 유지된다
    output.vPosition = clipSpacePos.xyww;
    output.vTexUV = In.vTexUV;

    return output;	
}

struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = tex2D(DefaultSampler, In.vTexUV);
	return Out;
}

technique DefaultTechnique
{
	pass Normal_0
	{
		// AlphaBlendEnable = true;
		// SrcBlend = SrcAlpha;
		// DestBlend = InvSrcAlpha;
		// BlendOp = Add;
		CULLMODE = NONE;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}


