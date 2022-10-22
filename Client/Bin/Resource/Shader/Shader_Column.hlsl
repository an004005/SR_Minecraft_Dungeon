
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

float		g_Time = 0.f;
textureCUBE		g_DefaultTexture;

sampler		DefaultSampler = sampler_state 
{
	texture = g_DefaultTexture;
	// minfilter = linear;
	// magfilter = linear;
};


struct VS_IN
{

	float3		vPosition : POSITION;
	
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	vector	vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;	
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	vector		vColor = (vector)0.f;

	Out.vColor = texCUBE(DefaultSampler, In.vTexUV);

	vColor.a = 1.f;
	vColor.r = max((1 - g_Time), 0.f);

	Out.vColor.r = min((Out.vColor.r + vColor.r), 1);

	return Out;
}

technique DefaultTechnique
{
	pass Default
	{
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		BlendOp = Add;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}


