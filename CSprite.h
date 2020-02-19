#ifndef CSPRITE_H
#define CSPRITE_H
//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )
#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>					//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include "MyMacro.h"

//���C�u�����ǂݍ���.
#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "d3dx11.lib")
#pragma comment( lib, "d3d11.lib")
#pragma comment( lib, "d3dx10.lib")	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#define ALIGN16	_declspec(align(16))

const int EN_MAX = 100;

/**************************************************
*	���ײĸ׽.
**/
class CSprite
{
public:
	CSprite();	//�ݽ�׸�.
	~CSprite();	//�޽�׸�.

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


	//������.
	HRESULT Init(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, const char* fileName, SPRITE_STATE* pSs);

	//���.
	void Release();

	//����ލ쐬.
	HRESULT InitShader();
	//���ٍ쐬.
	HRESULT InitModel(const char* fileName);
	//�����ݸޗp.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vCamPos);


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
	void SetUVPosition(D3DXVECTOR2 UVpos) {
		m_UV = UVpos;
	}




private:
	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	//���_�̍\����.

	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mWVP;		//���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.
		D3DXVECTOR4 vColor;		//�J���[(RGBA�̌^�ɍ��킹��).
		D3DXVECTOR4 vUV;		//UV���W(x,y�̂ݎg�p).
		ALIGN16 float       fAlpha;		//�A���t�@�l.

	
	};

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


	int						m_AnimCount;			//�J�E���^.
	D3DXVECTOR2				m_UV;					//�e�N�X�`��UV���W.


	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.


	D3DXVECTOR3				m_vPos;					//�ʒu.

	SPRITE_STATE			m_SState;

	float					m_fAlpha;

};

#endif//#ifndef CSPRITE_H