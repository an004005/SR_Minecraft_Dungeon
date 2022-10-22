
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector g_baseColor;
float g_Rate;
textureCUBE		g_DefaultTexture;

sampler		DefaultSampler = sampler_state 
{
	texture = g_DefaultTexture;
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

	vector		vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	vPosition = mul(vPosition, g_ViewMatrix);
	vPosition = mul(vPosition, g_ProjMatrix);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;
	// Out.vTexUV = In.vTexUV.y + g_Time;
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
	Out.vColor = g_baseColor;
	Out.vColor.a = (1.f - g_Rate) *Out.vColor.a;

	return Out;
}

technique DefaultTechnique
{
	pass Normal_0
	{
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		BlendOp = Add;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}


