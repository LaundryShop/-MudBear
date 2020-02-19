//��۰��ٕϐ�.
//ø���-�́Aڼ޽�� t(n).
Texture2D		g_Texture	: register(t0);
//�����-�́Aڼ޽�� s(n).
SamplerState	g_SamLinear	: register(s0);

//�e�̍ő吔(�\�����E��).
#define SHADOW_MAX 10

//�ݽ����ޯ̧.
//ү���P��.
cbuffer per_mesh	: register(b0)
{
	matrix	g_mW;	//ܰ��ލs��.
	matrix	g_mWVP;	//ܰ���,�ޭ�,��ۼު���݂̍����s��.
};
//��رْP��.
cbuffer per_material: register(b1)
{
	float4	g_Ambient;	//����.
	float4	g_Diffuse;	//�g�U���ˌ�.
	float4	g_Specular;	//���ʔ��ˌ�.
};
//�ڰђP��.
cbuffer per_frame	: register(b2)
{
	float4	g_vCamPos		: packoffset(c0);	//��׈ʒu.
	float4	g_vLightDir		: packoffset(c1);	//ײĕ���.
	float	g_fIntensity : packoffset(c2);	//ײċ��x(���邳).
	float4	g_vFogColor		: packoffset(c3);	//̫�ނ̔�.
	float	g_fFogAmount : packoffset(c4);	//̫�ނ̗�.
	float	g_fFogEnd : packoffset(c5);	//̫�ނ̍ő勗��(���`̫�ނł̂ݎg�p).
	float	g_fFogDensity : packoffset(c6);	//̫�ނ̖��x(�w��̫�ނł̂ݎg�p).
	float	g_fFogHeight : packoffset(c7);	//̫�ނ̑��݂��鍂��(��׳���̫�ތn�ł̂ݎg�p).
	float4	g_vCasterToLight: packoffset(c8);	//�e�𗎂Ƃ����ق�ײĂ������޸��.
	float4	g_vCasterPos[SHADOW_MAX]	: packoffset(c9);	//�e�𗎂Ƃ�������ʒu.
};

//���_����ނ̏o�����Ұ�.
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
//	���_�����.
//---------------------------------------
VS_OUT VS_Main(float4 Pos	: POSITION,
	float4 Norm : NORMAL,
	float2 Tex : TEXCOORD)
{
	VS_OUT Out = (VS_OUT)0;

	//��ۼު���ݕϊ�(ܰ��ށ��ޭ�����ۼު����).
	Out.Pos = mul(Pos, g_mWVP);

	//�@�������ق̎p���ɍ��킹��.
	// (���ق���]����Ζ@������]������K�v�����邽��).
	Out.Normal = mul(Norm, (float3x3)g_mW);

	//ײĕ���:
	// �ިڸ����ײẮA�ǂ��ł���������.�ʒu�͖��֌W.
	Out.Light = normalize(g_vLightDir);

	Out.PosWorld = mul(Pos, g_mW);

	//�����޸��:
	// ܰ��ދ�ԏ�ł̒��_���璸�_�֌������޸��.
	Out.EyeVector = normalize(g_vCamPos - Out.PosWorld);

	//ø������W.
	Out.Tex = Tex;

	return Out;
}

//---------------------------------------
//	�߸�ټ����.
//---------------------------------------
float4 PS_Main(VS_OUT In) : SV_Target
{
	//ø����װ.
	float4 texColor = g_Texture.Sample(g_SamLinear, In.Tex);

	//�����@�@.
	float4 ambient = texColor * g_Ambient;

	//�g�U���ˌ� �A.
	float NL = saturate(dot(In.Normal, In.Light));
	float4 diffuse = (g_Diffuse / 2 + texColor / 2)*NL;

	//���ʔ��ˌ� �B.
	float3 reflect = normalize(2 * NL * In.Normal - In.Light);
	float4 specular =
		pow(saturate(dot(reflect, In.EyeVector)), 4)*g_Specular;

	//̫����ٍŏI�F�@�@�A�B�̍��v.
	float4 Color = ambient + diffuse + specular;

	//---- �e�̏��� ----.
	for (int i = 0; i < SHADOW_MAX; i++) {
		float4 vToCaster = normalize(g_vCasterPos[i] - In.PosWorld);
		float cos = saturate(dot(g_vCasterToLight, vToCaster));
		//��߯�ײĂ̋t.��ݓ��͈Â�����.
		if (cos > 0.98f) {
			Color *= 0.5f;	//�Ƃ肠����0.5f.
//			break;//��ł��e��ݒ肵����I��.
		}
	}

	//ײċ��x�𔽉f.
	Color *= g_fIntensity.x;

	return Color;
}


//============ ø��������̏ꍇ ===============================.
//---------------------------------------
//	���_�����.
//---------------------------------------
VS_OUT VS_NoTex(float4 Pos		: POSITION,
	float4 Norm : NORMAL)
{
	VS_OUT Out = (VS_OUT)0;

	//��ۼު���ݕϊ�(ܰ��ށ��ޭ�����ۼު����).
	Out.Pos = mul(Pos, g_mWVP);

	//�@�������ق̎p���ɍ��킹��.
	// (���ق���]����Ζ@������]������K�v�����邽��).
	Out.Normal = mul(Norm, (float3x3)g_mW);

	return Out;
}

//---------------------------------------
//	�߸�ټ����.
//---------------------------------------
float4 PS_NoTex(VS_OUT In) : SV_Target
{
	float4 Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return Color;
}