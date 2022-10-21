//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// UVAnimation
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string UVAnimation_Pass_0_Model : ModelData = "..\\..\\..\\..\\..\\..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Torus.3ds";

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

float4 gWorldLightPosition = float4( 500.00,500.00,-500.00,1.00);
float3 gLightColor = float3(1.0f, 1.0f, 0.7f);
float4 gWorldCameraPosition : ViewPosition;

float gTime : Time0_X;

float gWaveHeight
<
   string UIName = "gWaveHeight";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 3.00 );
float gSpeed
<
   string UIName = "gSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 2.00 );
float gWaveFrequency
<
   string UIName = "gWaveFrequency";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 10.00 );
float gUVSpeed
<
   string UIName = "gUVSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 0.25 );


struct VS_INPUT 
{
   float3 mPosition : POSITION;
   float3 mNormal: NORMAL;
   float3 mTangent: TANGENT;
   float3 mBinormal: BINORMAL;
   float2 mUV: TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 mPosition : POSITION;
   float2 mUV: TEXCOORD0;
	float3 m_LightDir :TEXCOORD1;
	float3 m_ViewDir : TEXCOORD2;
   float3 T : TEXCOORD3;
   float3 B : TEXCOORD4;
   float3 N : TEXCOORD5;
};

VS_OUTPUT UVAnimation_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
     float cosTime = gWaveHeight * cos(gTime * gSpeed + Input.mUV.x * gWaveFrequency);
     Input.mPosition.y += cosTime;

   Output.mPosition = mul( float4(Input.mPosition, 1.f), gWorldMatrix );
	Output.mPosition = mul( Output.mPosition, gViewMatrix );
   Output.mPosition = mul( Output.mPosition, gProjectionMatrix );

   Output.mUV = Input.mUV + float2(gTime * gUVSpeed, 0);
	
	float4 worldPosition = mul(Input.mPosition, gWorldMatrix);
	float3	lightDir = worldPosition.xyz - gWorldLightPosition.xyz;
	Output.m_LightDir = normalize(lightDir);

	float3 viewDir = normalize(worldPosition.xyz - gWorldCameraPosition.xyz);
	Output.m_ViewDir = viewDir;
	float3 worldNormal = mul(Input.mNormal, (float3x3)gWorldMatrix);
	Output.N = normalize(worldNormal);
	float3 worldTangent = mul(Input.mTangent, (float3x3)gWorldMatrix);
	Output.T = normalize(worldTangent);
	float3 worldBinormal = mul(Input.mBinormal, (float3x3)gWorldMatrix);
	Output.B = normalize(worldBinormal);
   
   return Output;
}




struct PS_INPUT
{
   float2 mUV : TEXCOORD0;
float3 mLightDir : TEXCOORD1;
   float3 mViewDir: TEXCOORD2;
   float3 T : TEXCOORD3;
   float3 B : TEXCOORD4;
   float3 N : TEXCOORD5;
};

texture DiffuseMap_Tex;
sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap_Tex);
};

texture EnvironmentMap_Tex

// samplerCUBE EnvironmentSampler = sampler_state
// {
// 	Texture = (EnvironmentMap_Tex);
// 	// MINFILTER = LINEAR;
// 	// MAGFILTER = LINEAR;
// }

// float3 gLightColor;



vector UVAnimation_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{


   float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
   TBN = transpose(TBN);
   float3 worldNormal = TBN;
   gLightColor = float3(1.0f, 1.0f, 0.7f);
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);

	float3 lightDir = normalize(Input.mLightDir);
   float3 diffuse = saturate(dot(worldNormal, -lightDir));
   diffuse = gLightColor * albedo.rgb * diffuse;


	float3 viewDir = normalize(Input.mViewDir); 
   float3 specular = 0;
   if ( diffuse.x > 0 )
	{
 	float3 reflection = reflect(lightDir, worldNormal);

      specular = saturate(dot(reflection, -viewDir ));
      specular = pow(specular, 20.0f);
      
      // specular *= gLightColor;
   }
float3 viewReflect = reflect(viewDir, worldNormal);
   float3 environment = texCUBE(EnvironmentSampler, viewReflect).rgb;

   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
   
   return float4(ambient + diffuse + specular + environment * 0.5f, 1);

}

//--------------------------------------------------------------//
// Technique Section for UVAnimation
//--------------------------------------------------------------//
technique UVAnimation
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 UVAnimation_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 UVAnimation_Pass_0_Pixel_Shader_ps_main();
   }

}

