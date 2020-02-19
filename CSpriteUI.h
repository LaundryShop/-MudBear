#ifndef CSPRITE_UI_H
#define CSPRITE_UI_H
//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)
#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include "MyMacro.h"
#include "Global.h"

//ײ���ؓǂݍ���.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//�uD3DX�`�v�̒�`�g�p���ɕK�v.

#define ALIGN16	_declspec(align(16))


/**************************************************
*	���ײ�UI�׽.
**/
class CSpriteUI
{
public:
	//�������\����(float�^)
	struct WHSIZE_FLOAT
	{
		float w;
		float h;
	};
	//���ײč\����.
	struct SPRITE_STATE
	{
		WHSIZE_FLOAT	Disp;	//�\��������.
		WHSIZE_FLOAT    Base;	//���摜������.
		WHSIZE_FLOAT	Stride;	//1�R�}������̕�����.
	};

	CSpriteUI();	//�ݽ�׸�.
	~CSpriteUI();	//�޽�׸�.

	//������.
	HRESULT Init(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName, SPRITE_STATE* pSs);

	//���.
	void Release();

	//����ލ쐬.
	HRESULT InitShader();
	//���ٍ쐬.
	HRESULT InitModel();
	//ø����쐬.
	HRESULT CreateTexture(const char* fileName,
		ID3D11ShaderResourceView** pTexture);

	//�����ݸޗp.
	void Render();

	//�Q�[�W�������ݸޗp.
	void GaugeRender(float fScale_x);

	HRESULT InitBlend();
	void SetBlend(bool EnableAlpha);

	//�ʒu����ݒ�.
	void SetPosition(float x, float y, float z) {
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void SetPosition(const D3DXVECTOR3& vPos) {
		m_vPos = vPos;
	}

	void SetPattern(LONG x, LONG y) {
		m_PatternNo.x = x;
		m_PatternNo.y = y;
	}

	void SetAlpha(const float Alpha) {
		m_fAlpha = Alpha;
	}

private:
	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct SHADER_CONSTANT_BUFFER
	{

		ALIGN16 D3DMATRIX	mW;				//ܰ��ލs��.
		ALIGN16	float		fViewPortWidth;	//�ޭ��߰ĕ�.
		ALIGN16	float		fViewPortHeight;//�ޭ��߰č���.
		ALIGN16 float       fAlpha;			//�A���t�@�l(���߂Ŏg�p����).
		ALIGN16 D3DXVECTOR2	vUV;			//UV���W.
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR2	Tex;	//ø������W.
	};

	//�����؂ɂЂƂ�.
	ID3D11Device*			m_pDevice11;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;	//���޲���÷��.

	//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer;	//�ݽ����ޯ̧.

	//�����ق��Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer;	//���_�ޯ̧.

	ID3D11SamplerState*		m_pSampleLinear;	//�����:ø����Ɋe��̨����������.
	ID3D11ShaderResourceView*	m_pTexture;		//ø���.

	int		m_AnimCount;	//����.
	D3DXVECTOR2	m_UV;		//ø���UV���W.

	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.


	D3DXVECTOR3		m_vPos;	//�ʒu.

	float			m_fAlpha;	//�A���t�@�l.

	SPRITE_STATE	m_SState;	//�X�v���C�g���.
	POINT			m_PatternNo;//�p�^�[���ԍ�.
};

#endif//#ifndef CSPRITE_UI_H