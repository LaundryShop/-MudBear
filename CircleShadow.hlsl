//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬ-は、ﾚｼﾞｽﾀｰ t(n).
Texture2D		g_Texture	: register(t0);
//ｻﾝﾌﾟﾗ-は、ﾚｼﾞｽﾀｰ s(n).
SamplerState	g_SamLinear	: register(s0);

//影の最大数(表示限界数).
#define SHADOW_MAX 10

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
//ﾒｯｼｭ単位.
cbuffer per_mesh	: register(b0)
{
	matrix	g_mW;	//ﾜｰﾙﾄﾞ行列.
	matrix	g_mWVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成行列.
};
//ﾏﾃﾘｱﾙ単位.
cbuffer per_material: register(b1)
{
	float4	g_Ambient;	//環境光.
	float4	g_Diffuse;	//拡散反射光.
	float4	g_Specular;	//鏡面反射光.
};
//ﾌﾚｰﾑ単位.
cbuffer per_frame	: register(b2)
{
	float4	g_vCamPos		: packoffset(c0);	//ｶﾒﾗ位置.
	float4	g_vLightDir		: packoffset(c1);	//ﾗｲﾄ方向.
	float	g_fIntensity : packoffset(c2);	//ﾗｲﾄ強度(明るさ).
	float4	g_vFogColor		: packoffset(c3);	//ﾌｫｸﾞの白.
	float	g_fFogAmount : packoffset(c4);	//ﾌｫｸﾞの量.
	float	g_fFogEnd : packoffset(c5);	//ﾌｫｸﾞの最大距離(線形ﾌｫｸﾞでのみ使用).
	float	g_fFogDensity : packoffset(c6);	//ﾌｫｸﾞの密度(指数ﾌｫｸﾞでのみ使用).
	float	g_fFogHeight : packoffset(c7);	//ﾌｫｸﾞの存在する高さ(ｸﾞﾗｳﾝﾄﾞﾌｫｸﾞ系でのみ使用).
	float4	g_vCasterToLight: packoffset(c8);	//影を落とすﾓﾃﾞﾙとﾗｲﾄを結ぶﾍﾞｸﾄﾙ.
	float4	g_vCasterPos[SHADOW_MAX]	: packoffset(c9);	//影を落とすｷｬｽﾀｰ位置.
};

//頂点ｼｪｰﾀﾞの出力ﾊﾟﾗﾒｰﾀ.
struct VS_OUT
{
	float4	Pos		: SV_Position;
	float3	Normal	: TEXCOORD0;
	float2	Tex		: TEXCOORD1;
	float3	Light	: TEXCOORD2;
	float3	EyeVector:TEXCOORD3;
	float4	PosWorld: TEXCOORD4;
};

//---------------------------------------
//	頂点ｼｪｰﾀﾞ.
//---------------------------------------
VS_OUT VS_Main(float4 Pos	: POSITION,
	float4 Norm : NORMAL,
	float2 Tex : TEXCOORD)
{
	VS_OUT Out = (VS_OUT)0;

	//ﾌﾟﾛｼﾞｪｸｼｮﾝ変換(ﾜｰﾙﾄﾞ→ﾋﾞｭｰ→ﾌﾟﾛｼﾞｪｸｼｮﾝ).
	Out.Pos = mul(Pos, g_mWVP);

	//法線をﾓﾃﾞﾙの姿勢に合わせる.
	// (ﾓﾃﾞﾙが回転すれば法線も回転させる必要があるため).
	Out.Normal = mul(Norm, (float3x3)g_mW);

	//ﾗｲﾄ方向:
	// ﾃﾞｨﾚｸｼｮﾅﾙﾗｲﾄは、どこでも同じ方向.位置は無関係.
	Out.Light = normalize(g_vLightDir);

	Out.PosWorld = mul(Pos, g_mW);

	//視線ﾍﾞｸﾄﾙ:
	// ﾜｰﾙﾄﾞ空間上での頂点から頂点へ向かうﾍﾞｸﾄﾙ.
	Out.EyeVector = normalize(g_vCamPos - Out.PosWorld);

	//ﾃｸｽﾁｬ座標.
	Out.Tex = Tex;

	return Out;
}

//---------------------------------------
//	ﾋﾟｸｾﾙｼｪｰﾀﾞ.
//---------------------------------------
float4 PS_Main(VS_OUT In) : SV_Target
{
	//ﾃｸｽﾁｬｶﾗｰ.
	float4 texColor = g_Texture.Sample(g_SamLinear, In.Tex);

	//環境光　①.
	float4 ambient = texColor * g_Ambient;

	//拡散反射光 ②.
	float NL = saturate(dot(In.Normal, In.Light));
	float4 diffuse = (g_Diffuse / 2 + texColor / 2)*NL;

	//鏡面反射光 ③.
	float3 reflect = normalize(2 * NL * In.Normal - In.Light);
	float4 specular =
		pow(saturate(dot(reflect, In.EyeVector)), 4)*g_Specular;

	//ﾌｫﾝﾓﾃﾞﾙ最終色　①②③の合計.
	float4 Color = ambient + diffuse + specular;

	//---- 影の処理 ----.
	for (int i = 0; i < SHADOW_MAX; i++) {
		float4 vToCaster = normalize(g_vCasterPos[i] - In.PosWorld);
		float cos = saturate(dot(g_vCasterToLight, vToCaster));
		//ｽﾎﾟｯﾄﾗｲﾄの逆.ｺｰﾝ内は暗くする.
		if (cos > 0.98f) {
			Color *= 0.5f;	//とりあえず0.5f.
//			break;//一つでも影を設定したら終了.
		}
	}

	//ﾗｲﾄ強度を反映.
	Color *= g_fIntensity.x;

	return Color;
}


//============ ﾃｸｽﾁｬ無しの場合 ===============================.
//---------------------------------------
//	頂点ｼｪｰﾀﾞ.
//---------------------------------------
VS_OUT VS_NoTex(float4 Pos		: POSITION,
	float4 Norm : NORMAL)
{
	VS_OUT Out = (VS_OUT)0;

	//ﾌﾟﾛｼﾞｪｸｼｮﾝ変換(ﾜｰﾙﾄﾞ→ﾋﾞｭｰ→ﾌﾟﾛｼﾞｪｸｼｮﾝ).
	Out.Pos = mul(Pos, g_mWVP);

	//法線をﾓﾃﾞﾙの姿勢に合わせる.
	// (ﾓﾃﾞﾙが回転すれば法線も回転させる必要があるため).
	Out.Normal = mul(Norm, (float3x3)g_mW);

	return Out;
}

//---------------------------------------
//	ﾋﾟｸｾﾙｼｪｰﾀﾞ.
//---------------------------------------
float4 PS_NoTex(VS_OUT In) : SV_Target
{
	float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return Color;
}