//��۰��ٕϐ�.

//ø����� ڼ޽� t(n).
Texture2D		g_Texture	: register(t0);
//����ׂ� ڼ޽� s(n).
SamplerState	g_samLinear	: register(s0);

//�ݽ����ޯ̧.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;			//ܰ��ލs��.
	matrix	g_mWVP;			//ܰ���,�ޭ�,��ۼު���݂̍����s��.
	float4	g_vLightDir;	//ײĂ̕����޸��.
	float4	g_vCamPos;		//��׈ʒu(���_�ʒu).
	float   g_fAlpha;		//�A���t�@�l.
	
};
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		//���޴�ĐF(���F).
	float4	g_vDiffuse;		//�ި̭��ސF(�g�U���ːF).
	float4	g_vSpecular;	//��߷�אF(���ʔ��ːF).
};

//���_����ނ̏o�����Ұ�.
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
//	���_(�ްï��)�����.
//-------------------------------------------------
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float4 Norm: NORMAL,
				  float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//�@����ܰ��ދ�Ԃ�.
	Norm.w = 0;	//w=0�ňړ������𔽉f�����Ȃ�.
	output.Normal = mul(Norm, g_mW);
	output.Pos = mul(Pos, g_mWVP);
	//ײĕ���.
	output.Light = g_vLightDir;
	//�����޸��.
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vCamPos - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL*Normal - LightDir);
	float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_vDiffuse * NL + Specular * g_vSpecular;

	

	//ø������W��n��.
	output.Tex = Tex;

	return output;
}

//*************************************************
//	׽�ײ��(���ؑ��Őݒ肷�鍀�ځB�����ł͐G��Ȃ�).
//*************************************************

//-------------------------------------------------
//	�߸�ټ����.
//-------------------------------------------------
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 Color = g_Texture.Sample(g_samLinear, input.Tex);
		
	float4 RetColor = input.Color / 2 + Color / 2;

	RetColor.a = Color.a;
	RetColor.a *= g_fAlpha;

	return RetColor;
}

//========= ø��������p ========================================
//-------------------------------------------------
//	���_(�ްï��)�����.
//-------------------------------------------------
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm: NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//�@����ܰ��ދ�Ԃ�.
	Norm.w = 0;	//w=0�ňړ������𔽉f�����Ȃ�.
	output.Normal = mul(Norm, g_mW);
	output.Pos = mul(Pos, g_mWVP);
	//ײĕ���.
	output.Light = g_vLightDir;
	//�����޸��.
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
//	�߸�ټ����.
//-------------------------------------------------
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	float4 Color = input.Color;
	Color.a *= g_fAlpha;
	return Color;
}

//=====================================================================
//	�ȍ~�A����غ�ݗp����.
//=====================================================================
//�ݽ����ޯ̧.
cbuffer Ita_Global : register(b2)	//ڼ޽��𐔕��ǉ�.
{
	matrix g_WVP;	//ܰ���,�ޭ�,��ۼު���݂܂ł̕ϊ��s��.
	float4 g_Color;	//�F.
	float4 g_UV;	//UV���W.
	
	float   g_fHideAlpha;	//�B���e�N�X�`���p.

};

//�\����.
struct VS_ITAOUT
{
	float4	Pos : SV_Position;
	float2	Tex	: TEXCOORD;
	
};

//���_�����.
VS_ITAOUT VS_Ita(float4 Pos : POSITION,
				 float2 Tex : TEXCOORD)
{
	VS_ITAOUT output = (VS_ITAOUT)0;
	output.Pos = mul(Pos, g_WVP);
	output.Tex = Tex;

	//ø�����UV���W�𑀍삷��.
	output.Tex.x += g_UV.x;
	output.Tex.y += g_UV.y;


	return output;
}

//�߸�ټ����.
float4 PS_Ita( VS_ITAOUT input ) : SV_Target
{
	float4 Color = g_Texture.Sample(g_samLinear, input.Tex);//�F��Ԃ�.
	Color.a *= g_fHideAlpha;
	return Color;
}