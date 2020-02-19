#ifndef CPARTICLE_H
#define CPARTICLE_H
//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)
#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include "MyMacro.h"

//ײ���ؓǂݍ���.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//�uD3DX�`�v�̒�`�g�p���ɕK�v.


/**************************************************
*	�߰è�ٸ׽.
**/
class CParticle
{
public:
	//�֐��̵��ް۰��.
	//�����֐����ŁA�������قȂ鐔��قȂ�^.
	CParticle(const int MaxParticle,
		const D3DXVECTOR3& vEmitPos);
	CParticle();	//�ݽ�׸�(�����Ȃ��ݽ�׸��܂�����̫�ĺݽ�׸��Ƃ�����).
	~CParticle();	//�޽�׸�.

	//������.
	HRESULT Init(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11);

	//���.
	void Release();

	//����ލ쐬.
	HRESULT InitShader();
	//���ٍ쐬.
	HRESULT InitModel();
	//ø����ǂݍ��݁��쐬.
	HRESULT InitTexture(const char* fileName);

	//�����ݸޗp.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vCamPos);

	HRESULT InitBlend();
	void SetBlend(bool EnableAlpha);

	//����ނ��O���֐�.
	void DetachShader();

	//�߰è�ق̈ړ�.
	void Run();

	//�S�Ă��߲�Ľ��ײĂ������ݸ�.
	void RenderAll(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vCamPos);

	//�ʒu����ݒ�.
	void SetPosition(float x, float y, float z) {
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void SetPosition(const D3DXVECTOR3& vPos) {
		m_vPos = vPos;
	}

private:
	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s��.	
//		D3DXVECTOR4	vColor;		//�װ(RGBA�̌^�ɍ��킹��).
//		D3DXVECTOR4	vUV;		//UV���W(x,y�̂ݎg�p).
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
//		D3DXVECTOR2	Tex;	//ø������W.
	};

	//�߰è�ٗp�\����.
	struct PART
	{
		D3DXVECTOR3	vPos;	//�ʒu.
		D3DXVECTOR3	vDir;	//����.
		float		Speed;	//���x.
		int			BirthFrame;//�J�n�ڰ�.
		//�\���̂̏�����(�ݽ�׸�).
		PART()
			: vPos(0.0f, 0.0f, 0.0f)
			, vDir(0.0f, 0.0f, 0.0f)
			, Speed(0.0f)
			, BirthFrame(0)
		{};
	};

	//�����؂ɂЂƂ�.
	ID3D11Device*			m_pDevice11;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;	//���޲���÷��.

	//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11GeometryShader*	m_pGeometryShader;	//�޵��ؼ����.
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
	float			m_Scale;//�g�k�l.

	const int		m_MaxParticle;	//�߰è�ٍő吔.
	D3DXVECTOR3		m_vEmitPos;		//���ˈʒu.
	PART*			m_pParticleArray;//�߰è�ٔz��.
	int				m_Frame;		//�ڰѐ�.
};

#endif//#ifndef CPARTICLE_H