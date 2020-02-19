#ifndef CDX9MESH_H
#define CDX9MESH_H
//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���. 4005:�Ē�`.
#pragma warning( disable:4005)
#include <Windows.h>
#include <d3dx9.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>
#include "MyMacro.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//�uD3DX�`�v�̒�`�g�p���ɕK�v�ɂȂ�.

#define ALIGN16	_declspec(align(16))

//�̨��\����.
struct SPHERE
{
	D3DXVECTOR3	vCenter;	//���S.
	float		fRadius;	//���a. 
}; 
//�޳��ިݸ��ޯ���\����.
struct BBOX
{
	D3DXVECTOR3	vPosMax;	//�ő咸�_.
	D3DXVECTOR3	vPosMin;	//�ŏ����_.
	D3DXVECTOR3	vAxisX;		//X��.
	D3DXVECTOR3	vAxisY;		//Y��.
	D3DXVECTOR3	vAxisZ;		//Z��.
	FLOAT		LengthX;	//X����.
	FLOAT		LengthY;	//Y����.
	FLOAT		LengthZ;	//Z����.

	//������.
	BBOX()
		: vPosMax(), vPosMin()
		, vAxisX(1.0f,0.0f,0.0f)
		, vAxisY(0.0f,1.0f,0.0f)
		, vAxisZ(0.0f,0.0f,1.0f)
		, LengthX(0.0f), LengthY(0.0f), LengthZ(0.0f)
	{}
};

//ү���ް���̧�ق�����o���ׂ�����DirectX9���g�p����.
//�������ݸ�(�`��)�́ADirectX11�ōs��.
class CDX9Mesh
{
public:
	CDX9Mesh();		//�ݽ�׸�.
	~CDX9Mesh();	//�޽�׸�.

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, const char* fileName,bool Falg);
	//Dx9������.
	HRESULT InitDx9(HWND hWnd);
	//ү���ǂݍ���.
	HRESULT LoadXMesh(const char* fileName);
	//����֐�.
	void Release();

	//����ލ쐬.
	HRESULT InitShader();

	//�����ݸޗp.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos);

	//�ʒu����ݒ�.
	void SetPosition(float x, float y, float z) {
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void SetPosition(const D3DXVECTOR3& vPos) {
		m_vPos = vPos;
	}
	//�ʒu�����擾.
	D3DXVECTOR3 GetPosition(){
		return m_vPos;
	}
	//��]����ݒ�.
	void SetRotation(const D3DXVECTOR3& vRot) {
		m_vRot = vRot;
	}
	//��]�����擾.
	D3DXVECTOR3 GetRotation() {
		return m_vRot;
	}
	//�g�k����ݒ�.
	void	SetScale(const float Scale)	{
		m_vScale = Scale;
	}
	//�g�k�����擾.
	float	GetScale()	{
		return m_vScale;
	}

	//ү�����擾.
	LPD3DXMESH GetMesh() {
		return m_pMesh;
	}

	HRESULT InitBlend(bool Falg);
	void SetBlend(bool EnableAlpha);

	void SetAlpha(float Alpha) {
		m_fAlpha = Alpha;
	}

	SPHERE	m_Sphere;	//�̨��\����.
	BBOX	m_BBox;		//B�ޯ���\����.

private:
	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;			//ܰ��ލs��.
		D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s��.	
		D3DXVECTOR4	vLightDir;	//ײĕ���.
		D3DXVECTOR4	vCamPos;	//��׈ʒu(���_�ʒu).
		ALIGN16 float       fAlpha;		//�A���t�@�l.
	};
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;	//���޴�ĐF(���F).
		D3DXVECTOR4	vDiffuse;	//�ި̭��ސF(�g�U���ːF).
		D3DXVECTOR4	vSpecular;	//��߷�אF(���ʔ��ːF).
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR3	Normal;	//�@��(�A�e�v�Z�ɕK�{).
		D3DXVECTOR2	Tex;	//ø������W.
	};
	//��رٍ\����.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	//���޴��.
		D3DXVECTOR4	Diffuse;	//�ި̭���.
		D3DXVECTOR4	Specular;	//��߷��.
		char szTextureName[64];	//ø���̧�ٖ�.
		ID3D11ShaderResourceView* pTexture;//ø���.
		DWORD dwNumFace;		//������رق���غ�ݐ�.
		//�ݽ�׸�.
		MY_MATERIAL()
			: Ambient(),Diffuse(),Specular(),szTextureName()
			, pTexture(nullptr),dwNumFace(0)
		{
		}
		//�޽�׸�.
		~MY_MATERIAL() {
			SAFE_RELEASE(pTexture);
		}
	};
	
	HWND				m_hWnd;		//����޳�����.

	//Dx9.
	LPDIRECT3D9			m_pD3d9;	//Dx9��޼ު��.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9���޲���޼ު��.

	//Dx11.
	ID3D11Device*			m_pDevice11;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;	//���޲���÷��.

	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pCBufferPerMesh;	//�ݽ����ޯ̧(ү����).
	ID3D11Buffer*			m_pCBufferPerMaterial;//�ݽ����ޯ̧(��رٖ�).

	ID3D11Buffer*				m_pVertexBuffer;	//���_�ޯ̧.
	ID3D11Buffer**				m_ppIndexBuffer;	//���ޯ���ޯ̧.
	ID3D11SamplerState*			m_pSampleLinear;	//�����:ø����Ɋe��̨����������.

	LPD3DXMESH			m_pMesh;		//ү����޼ު��.
	DWORD				m_NumMaterials;	//��رِ�.

	MY_MATERIAL*	m_pMaterials;	//��رٍ\����.
	DWORD			m_NumAttr;		//������.
	DWORD			m_AttrID[300];	//����ID ��300�����܂�.

	bool			m_EnableTexture;//ø�������.

	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.


	float		m_vScale;	//�g��k���l(x,y,z���{).
	D3DXVECTOR3		m_vRot;		//��]�l(x,y,z).
								//��x=Pitch, y=Yaw, z=Roll.
	D3DXVECTOR3		m_vPos;		//�ʒu(x,y,z).

	float			m_fAlpha;	//alpha.
};

#endif//#ifndef CDX9MESH_H