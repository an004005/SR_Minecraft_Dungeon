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
float4x4 gWorldMatrix : World;
float4x4 gProjectionMatrix : Projection;
float4x4 gViewMatrix : View;

float gTime : Time0_X;

float gUVSpeed
<
   string UIName = "gUVSpeed";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 2.00 );

struct VS_INPUT 
{
   float3 mPosition : POSITION;
   float2 mUV: TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 mPosition : POSITION;
   float2 mUV: TEXCOORD0;
};

VS_OUTPUT UVAnimation_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;
   
   Output.mPosition = mul(float4(Input.mPosition, 1.f), gWorldMatrix);
    
   Output.mPosition = mul(Output.mPosition, gViewMatrix);
   Output.mPosition = mul(Output.mPosition, gProjectionMatrix);
   
   Output.mUV = Input.mUV + float2(gTime, gUVSpeed);
   return Output;
}

struct PS_INPUT
{
   float2 mUV : TEXCOORD0;
};

texture2D DiffuseMap_Tex;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

float4 UVAnimation_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
   float4 albedo = tex2D(DiffuseSampler, Input.mUV);
   if (albedo.w > 0)
   {
	   return float4(0.f, 0.0f, 1.f, 1);
   }

   return float4(0, 0, 0, 0);
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

