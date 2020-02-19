/***************************************************************************************************
*	SkinMeshCode Version 2.00
*	LastUpdate	: 2019/10/09
**/
#ifndef C_DX9SKIN_MESH_H
#define C_DX9SKIN_MESH_H
//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���B4005�F�Ē�`.
#pragma warning( disable : 4005 )

#include "CDX9SkinMeshParser.h"

// �f�o�C�X�֌W���p�\����.
struct CD3DXSKINMESH_INIT
{
	HWND hWnd;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
};

//�ő�e�N�X�`����.
const int TEX_NO = 4;

//==================================================================================================
//
//	�X�L�����b�V���N���X.
//
//==================================================================================================
class CDX9SkinMesh
{
public:


	struct  CHANGE_TEXTURES
	{
		CHAR szTextureName[512];				//��������.
		CHAR szTextureName_2[512];				//�Y��.

		ID3D11ShaderResourceView* pTexture;		//�؂�ւ���e�N�X�`���A�h���X.
		ID3D11ShaderResourceView* pTextureTest;	//�؂�ւ���e�N�X�`���A�h���X2��.


		ID3D11ShaderResourceView* pDefTexture;	//�؂�ւ��O�̃e�N�X�`���A�h���X.
		CHAR szTargetTextureName[512];			//�؂�ւ���Ώۂ̃e�N�X�`����.
		CHANGE_TEXTURES()
			: szTextureName()
			, szTextureName_2()
			, pTexture(nullptr)
			, pTextureTest(nullptr)
			, pDefTexture(nullptr)
			, szTargetTextureName()
		{}

		~CHANGE_TEXTURES()
		{
			Release();
		}
		void Release()
		{
			SAFE_RELEASE(pTextureTest);
			SAFE_RELEASE(pTexture);

			pDefTexture = nullptr;	//�A�h���X�݂̂Ȃ̂�nullptr�ŏ�����.

		}
	};


	//���܂���p
	int m_IsEnabelChangeTexttureSystem;

	void SetEnabelChangeTexttureSystem(int SkinChange) {
		m_IsEnabelChangeTexttureSystem = SkinChange;
	}

	//�{�X���܂���p.
	int m_B_IChangeTexttureSystem[TEX_NO];
	//��.
	void SetChangeTexttureSystem_Atama(int BSkinChange) {
		m_B_IChangeTexttureSystem[0] = BSkinChange;
	}
	//��.
	void SetChangeTexttureSystem_Karada(int BSkinChange) {
		m_B_IChangeTexttureSystem[1] = BSkinChange;
	}
	//�r.
	void SetChangeTexttureSystem_UdeL(int BSkinChange) {
		m_B_IChangeTexttureSystem[2] = BSkinChange;
	}
	//�r.
	void SetChangeTexttureSystem_UdeR(int BSkinChange) {
		m_B_IChangeTexttureSystem[3] = BSkinChange;
	}

	//���܂���p.
	std::shared_ptr < CHANGE_TEXTURES> m_pChangeTextures;
	//�{�X���܂���p.
	CHANGE_TEXTURES* m_pBChangeTextures[TEX_NO];

	void SetChangeTextures(std::shared_ptr < CHANGE_TEXTURES> pCT) {
		m_pChangeTextures = pCT;
	}

	void SetBChangeTextures(CHANGE_TEXTURES* pCT) {
		m_pBChangeTextures[0] = pCT;
	}
	void SetBChangeTextures1(CHANGE_TEXTURES* pCT) {
		m_pBChangeTextures[1] = pCT;
	}
	void SetBChangeTextures2(CHANGE_TEXTURES* pCT) {
		m_pBChangeTextures[2] = pCT;
	}
	void SetBChangeTextures3(CHANGE_TEXTURES* pCT) {
		m_pBChangeTextures[3] = pCT;
	}



	//���b�V���P��.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;			//���[���h�s��.
		D3DXMATRIX	mWVP;		//���[���h����ˉe�܂ł̕ϊ��s��.
	};

	//�}�e���A���P��.
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;	//�A���r�G���g��.
		D3DXVECTOR4 vDiffuse;	//�f�B�t���[�Y�F.
		D3DXVECTOR4 vSpecular;	//���ʔ���.
	};

	//�V�F�[�_�[�ɓn���l.
	struct CBUFFER_PER_FRAME
	{
		D3DXVECTOR4 vLightDir;	//���C�g����.
		D3DXVECTOR4 vEye;		//�J�����ʒu.
	};

	//�{�[���P��.
	struct CBUFFER_PER_BONES
	{
		D3DXMATRIX mBone[D3DXPARSER::MAX_BONES];
		CBUFFER_PER_BONES()
		{
			for (int i = 0; i < D3DXPARSER::MAX_BONES; i++)
			{
				D3DXMatrixIdentity(&mBone[i]);
			}
		}
	};


	//���_�\����.
	struct MY_SKINVERTEX
	{
		D3DXVECTOR3	vPos;		//���_�ʒu.
		D3DXVECTOR3	vNorm;		//���_�@��.
		D3DXVECTOR2	vTex;		//UV���W.
		UINT bBoneIndex[4];		//�{�[�� �ԍ�.
		float bBoneWeight[4];	//�{�[�� �d��.
		MY_SKINVERTEX()
			: vPos()
			, vNorm()
			, vTex()
			, bBoneIndex()
			, bBoneWeight()
		{}
	};

	//�p�[�T�[�N���X����A�j���[�V�����R���g���[���[���擾����.
	LPD3DXANIMATIONCONTROLLER GetAnimController()
	{
		return m_pD3dxMesh->m_pAnimController;
	}




	//���\�b�h.
	CDX9SkinMesh();
	~CDX9SkinMesh();

	HRESULT Init(CD3DXSKINMESH_INIT* si, const char* fileName);
	//X�t�@�C������X�L�����b�V�����쐬����.
	HRESULT LoadXMesh(const char* fileName);
	//�`��֐�.
	void Render(const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj,
		const D3DXVECTOR3& vLight,
		const D3DXVECTOR3& vEye,
		const LPD3DXANIMATIONCONTROLLER pAC = NULL);
	//����֐�.
	HRESULT Release();

	double GetAnimSpeed() { return m_dAnimSpeed; }
	void SetAnimSpeed(double dSpeed) { m_dAnimSpeed = dSpeed; }

	double GetAnimTime() { return m_dAnimTime; }
	void SetAnimTime(double dTime) { m_dAnimTime = dTime; }

	//�A�j���[�V�����R���g���[�����擾.
	LPD3DXANIMATIONCONTROLLER GetAnimationController() { return m_pD3dxMesh->m_pAnimController; }

	//�A�j���[�V�����Z�b�g�̐؂�ւ�.
	void ChangeAnimSet(int index, LPD3DXANIMATIONCONTROLLER pAC = nullptr);
	//�A�j���[�V�����Z�b�g�̐؂�ւ�(�J�n�t���[���w��\��).
	void ChangeAnimSet_StartPos(int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC = nullptr);

	//�A�j���[�V������~���Ԃ��擾.
	double GetAnimPeriod(int index);
	//�A�j���[�V���������擾.
	int GetAnimMax(LPD3DXANIMATIONCONTROLLER pAC = NULL);

	//�w�肵���{�[�����(���W�E�s��)���擾����֐�.
	bool GetMatrixFromBone(const char* sBoneName, D3DXMATRIX* pOutMat);
	bool GetPosFromBone(const char* sBoneName, D3DXVECTOR3* pOutPos);
	bool GetDeviaPosFromBone(const char* sBoneName, D3DXVECTOR3* pOutPos, D3DXVECTOR3 vSpecifiedPos = { 0.0f, 0.0f, 0.0f });


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
	D3DXVECTOR3 GetPosition() {
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
	//�g�k����ݒ�E�擾.
	void SetScale(float Scale) {
		m_vScale.x = Scale;
		m_vScale.y = Scale;
		m_vScale.z = Scale;

	}
	D3DXVECTOR3 GetScale() {
		return m_vScale;
	}

	HRESULT InitBlend();
	void SetBlend(bool EnableAlpha);

private:
	HWND m_hWnd;

	//Dx9.
	LPDIRECT3DDEVICE9		m_pDevice9;

	//Dx11.
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;
	ID3D11SamplerState*		m_pSampleLinear;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ID3D11Buffer*			m_pCBufferPerMesh;		//�R���X�^���g�o�b�t�@(���b�V����).
	ID3D11Buffer*			m_pCBufferPerMaterial;	//�R���X�^���g�o�b�t�@(�}�e���A����).
	ID3D11Buffer*			m_pCBufferPerFrame;		//�R���X�^���g�o�b�t�@(�t���[����).
	ID3D11Buffer*			m_pCBufferPerBone;		//�R���X�^���g�o�b�t�@(�{�[����).

	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vRot;
	D3DXVECTOR3		m_vScale;

	D3DXMATRIX		m_mWorld;
	D3DXMATRIX		m_mRotation;

	D3DXMATRIX		m_mView;
	D3DXMATRIX		m_mProj;

	D3DXVECTOR3		m_vLight;
	D3DXVECTOR3		m_vEye;

	//�A�j���[�V�������x.
	double m_dAnimSpeed;
	double m_dAnimTime;

	//��������p��.
	SKIN_PARTS_MESH* m_pReleaseMaterial;

	//���b�V��.
	D3DXPARSER* m_pD3dxMesh;


	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.


	//XFile�̃p�X.
	CHAR	m_FilePath[256];

	//�A�j���[�V�����t���[��.
	int		m_iFrame;

	HRESULT CreateDeviceDx9(HWND hWnd);
	HRESULT InitShader();
	HRESULT CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer);
	void RecursiveSetNewPoseMatrices(BONE* pBone, D3DXMATRIX* pmParent);

	//�S�Ẵ��b�V�����쐬����.
	void BuildAllMesh(D3DXFRAME* pFrame);

	//���b�V�����쐬����.
	HRESULT CreateAppMeshFromD3DXMesh(LPD3DXFRAME pFrame);

	//X�t�@�C������X�L���֘A�̏���ǂݏo���֐�.
	HRESULT ReadSkinInfo(MYMESHCONTAINER* pContainer, MY_SKINVERTEX* pvVB, SKIN_PARTS_MESH* pParts);

	//�{�[�������̃|�[�Y�ʒu�ɃZ�b�g����֐�.
	void SetNewPoseMatrices(SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer);
	//����(���݂�)�|�[�Y�s���Ԃ��֐�.
	D3DXMATRIX GetCurrentPoseMatrix(SKIN_PARTS_MESH* pParts, int index);

	//�t���[���`��.
	void DrawFrame(LPD3DXFRAME pFrame);
	//�p�[�c�`��.
	void DrawPartsMesh(SKIN_PARTS_MESH* p, D3DXMATRIX World, MYMESHCONTAINER* pContainer);
	void DrawPartsMeshStatic(SKIN_PARTS_MESH* pMesh, D3DXMATRIX World, MYMESHCONTAINER* pContainer);

	//�S�Ẵ��b�V�����폜.
	void DestroyAllMesh(D3DXFRAME* pFrame);
	HRESULT DestroyAppMeshFromD3DXMesh(LPD3DXFRAME p);

	//�R���X�^���g�o�b�t�@�쐬�֐�.
	HRESULT CreateCBuffer(ID3D11Buffer** pConstantBuffer, UINT size);
	//�T���v���[�쐬�֐�.
	HRESULT CreateLinearSampler(ID3D11SamplerState** pSampler);

};

#endif//#ifndef C_DX9SKIN_MESH_H