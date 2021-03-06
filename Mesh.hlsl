//ｸﾞﾛｰﾊﾞﾙ変数.

//ﾃｸｽﾁｬは ﾚｼﾞｽﾀ t(n).
Texture2D		g_Texture	: register(t0);
//ｻﾝﾌﾟﾗは ﾚｼﾞｽﾀ s(n).
SamplerState	g_samLinear	: register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;			//ﾜｰﾙﾄﾞ行列.
	matrix	g_mWVP;			//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成行列.
	float4	g_vLightDir;	//ﾗｲﾄの方向ﾍﾞｸﾄﾙ.
	float4	g_vCamPos;		//ｶﾒﾗ位置(視点位置).
	float   g_fAlpha;		//アルファ値.
	
};
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		//ｱﾝﾋﾞｴﾝﾄ色(環境色).
	float4	g_vDiffuse;		//ﾃﾞｨﾌｭｰｽﾞ色(拡散反射色).
	float4	g_vSpecular;	//ｽﾍﾟｷｭﾗ色(鏡面反射色).
};

//頂点ｼｪｰﾀﾞの出力ﾊﾟﾗﾒｰﾀ.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	Color		: COLOR;
	float3	Light		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	EyeVector	: TEXCOORD2;
	float2	Tex			: TEXCOORD3;
};

//-------------------------------------------------
//	頂点(ﾊﾞｰﾃｯｸｽ)ｼｪｰﾀﾞ.
//-------------------------------------------------
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float4 Norm: NORMAL,
				  float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//法線をﾜｰﾙﾄﾞ空間に.
	Norm.w = 0;	//w=0で移動成分を反映させない.
	output.Normal = mul(Norm, g_mW);
	output.Pos = mul(Pos, g_mWVP);
	//ﾗｲﾄ方向.
	output.Light = g_vLightDir;
	//視線ﾍﾞｸﾄﾙ.
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vCamPos - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL*Normal - LightDir);
	float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_vDiffuse * NL + Specular * g_vSpecular;

	

	//ﾃｸｽﾁｬ座標を渡す.
	output.Tex = Tex;

	return output;
}

//*************************************************
//	ﾗｽﾀﾗｲｻﾞ(ｱﾌﾟﾘ側で設定する項目。ここでは触れない).
//*************************************************

//-------------------------------------------------
//	ﾋﾟｸｾﾙｼｪｰﾀﾞ.
//-------------------------------------------------
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 Color = g_Texture.Sample(g_samLinear, input.Tex);
		
	float4 RetColor = input.Color / 2 + Color / 2;

	RetColor.a = Color.a;
	RetColor.a *= g_fAlpha;

	return RetColor;
}

//========= ﾃｸｽﾁｬ無し用 ========================================
//-------------------------------------------------
//	頂点(ﾊﾞｰﾃｯｸｽ)ｼｪｰﾀﾞ.
//-------------------------------------------------
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm: NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//法線をﾜｰﾙﾄﾞ空間に.
	Norm.w = 0;	//w=0で移動成分を反映させない.
	output.Normal = mul(Norm, g_mW);
	output.Pos = mul(Pos, g_mWVP);
	//ﾗｲﾄ方向.
	output.Light = g_vLightDir;
	//視線ﾍﾞｸﾄﾙ.
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vCamPos - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL*Normal - LightDir);
	float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_vDiffuse * NL + Specular * g_vSpecular;

	return output;
}

//-------------------------------------------------
//	ﾋﾟｸｾﾙｼｪｰﾀﾞ.
//-------------------------------------------------
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	float4 Color = input.Color;
	Color.a *= g_fAlpha;
	return Color;
}

//=====================================================================
//	以降、板ﾎﾟﾘｺﾞﾝ用ｺｰﾄﾞ.
//=====================================================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer Ita_Global : register(b2)	//ﾚｼﾞｽﾀを数分追加.
{
	matrix g_WVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝまでの変換行列.
	float4 g_Color;	//色.
	float4 g_UV;	//UV座標.
	
	float   g_fHideAlpha;	//隠すテクスチャ用.

};

//構造体.
struct VS_ITAOUT
{
	float4	Pos : SV_Position;
	float2	Tex	: TEXCOORD;
	
};

//頂点ｼｪｰﾀﾞ.
VS_ITAOUT VS_Ita(float4 Pos : POSITION,
				 float2 Tex : TEXCOORD)
{
	VS_ITAOUT output = (VS_ITAOUT)0;
	output.Pos = mul(Pos, g_WVP);
	output.Tex = Tex;

	//ﾃｸｽﾁｬのUV座標を操作する.
	output.Tex.x += g_UV.x;
	output.Tex.y += g_UV.y;


	return output;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS_Ita( VS_ITAOUT input ) : SV_Target
{
	float4 Color = g_Texture.Sample(g_samLinear, input.Tex);//色を返す.
	Color.a *= g_fHideAlpha;
	return Color;
}