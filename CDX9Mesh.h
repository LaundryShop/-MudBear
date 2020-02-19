#ifndef CDX9MESH_H
#define CDX9MESH_H
//警告についてのｺｰﾄﾞ分析を無効にする. 4005:再定義.
#pragma warning( disable:4005)
#include <Windows.h>
#include <d3dx9.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>
#include "MyMacro.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//「D3DX～」の定義使用時に必要になる.

#define ALIGN16	_declspec(align(16))

//ｽﾌｨｱ構造体.
struct SPHERE
{
	D3DXVECTOR3	vCenter;	//中心.
	float		fRadius;	//半径. 
}; 
//ﾊﾞｳﾝﾃﾞｨﾝｸﾞﾎﾞｯｸｽ構造体.
struct BBOX
{
	D3DXVECTOR3	vPosMax;	//最大頂点.
	D3DXVECTOR3	vPosMin;	//最小頂点.
	D3DXVECTOR3	vAxisX;		//X軸.
	D3DXVECTOR3	vAxisY;		//Y軸.
	D3DXVECTOR3	vAxisZ;		//Z軸.
	FLOAT		LengthX;	//X長さ.
	FLOAT		LengthY;	//Y長さ.
	FLOAT		LengthZ;	//Z長さ.

	//初期化.
	BBOX()
		: vPosMax(), vPosMin()
		, vAxisX(1.0f,0.0f,0.0f)
		, vAxisY(0.0f,1.0f,0.0f)
		, vAxisZ(0.0f,0.0f,1.0f)
		, LengthX(0.0f), LengthY(0.0f), LengthZ(0.0f)
	{}
};

//ﾒｯｼｭﾃﾞｰﾀをﾌｧｲﾙから取り出す為だけにDirectX9を使用する.
//※ﾚﾝﾀﾞﾘﾝｸﾞ(描画)は、DirectX11で行う.
class CDX9Mesh
{
public:
	CDX9Mesh();		//ｺﾝｽﾄﾗｸﾀ.
	~CDX9Mesh();	//ﾃﾞｽﾄﾗｸﾀ.

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, const char* fileName,bool Falg);
	//Dx9初期化.
	HRESULT InitDx9(HWND hWnd);
	//ﾒｯｼｭ読み込み.
	HRESULT LoadXMesh(const char* fileName);
	//解放関数.
	void Release();

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();

	//ﾚﾝﾀﾞﾘﾝｸﾞ用.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vCamPos);

	//位置情報を設定.
	void SetPosition(float x, float y, float z) {
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}
	void SetPosition(const D3DXVECTOR3& vPos) {
		m_vPos = vPos;
	}
	//位置情報を取得.
	D3DXVECTOR3 GetPosition(){
		return m_vPos;
	}
	//回転情報を設定.
	void SetRotation(const D3DXVECTOR3& vRot) {
		m_vRot = vRot;
	}
	//回転情報を取得.
	D3DXVECTOR3 GetRotation() {
		return m_vRot;
	}
	//拡縮情報を設定.
	void	SetScale(const float Scale)	{
		m_vScale = Scale;
	}
	//拡縮情報を取得.
	float	GetScale()	{
		return m_vScale;
	}

	//ﾒｯｼｭを取得.
	LPD3DXMESH GetMesh() {
		return m_pMesh;
	}

	HRESULT InitBlend(bool Falg);
	void SetBlend(bool EnableAlpha);

	void SetAlpha(float Alpha) {
		m_fAlpha = Alpha;
	}

	SPHERE	m_Sphere;	//ｽﾌｨｱ構造体.
	BBOX	m_BBox;		//Bﾎﾞｯｸｽ構造体.

private:
	//======================================
	//	構造体.
	//======================================
	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義.
	//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;			//ﾜｰﾙﾄﾞ行列.
		D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成変換行列.	
		D3DXVECTOR4	vLightDir;	//ﾗｲﾄ方向.
		D3DXVECTOR4	vCamPos;	//ｶﾒﾗ位置(視点位置).
		ALIGN16 float       fAlpha;		//アルファ値.
	};
	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;	//ｱﾝﾋﾞｴﾝﾄ色(環境色).
		D3DXVECTOR4	vDiffuse;	//ﾃﾞｨﾌｭｰｽﾞ色(拡散反射色).
		D3DXVECTOR4	vSpecular;	//ｽﾍﾟｷｭﾗ色(鏡面反射色).
	};
	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
		D3DXVECTOR3	Normal;	//法線(陰影計算に必須).
		D3DXVECTOR2	Tex;	//ﾃｸｽﾁｬ座標.
	};
	//ﾏﾃﾘｱﾙ構造体.
	struct MY_MATERIAL
	{
		D3DXVECTOR4	Ambient;	//ｱﾝﾋﾞｴﾝﾄ.
		D3DXVECTOR4	Diffuse;	//ﾃﾞｨﾌｭｰｽﾞ.
		D3DXVECTOR4	Specular;	//ｽﾍﾟｷｭﾗ.
		char szTextureName[64];	//ﾃｸｽﾁｬﾌｧｲﾙ名.
		ID3D11ShaderResourceView* pTexture;//ﾃｸｽﾁｬ.
		DWORD dwNumFace;		//そのﾏﾃﾘｱﾙのﾎﾟﾘｺﾞﾝ数.
		//ｺﾝｽﾄﾗｸﾀ.
		MY_MATERIAL()
			: Ambient(),Diffuse(),Specular(),szTextureName()
			, pTexture(nullptr),dwNumFace(0)
		{
		}
		//ﾃﾞｽﾄﾗｸﾀ.
		~MY_MATERIAL() {
			SAFE_RELEASE(pTexture);
		}
	};
	
	HWND				m_hWnd;		//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.

	//Dx9.
	LPDIRECT3D9			m_pD3d9;	//Dx9ｵﾌﾞｼﾞｪｸﾄ.
	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.

	//Dx11.
	ID3D11Device*			m_pDevice11;	//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pContext11;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.

	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pCBufferPerMesh;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾒｯｼｭ毎).
	ID3D11Buffer*			m_pCBufferPerMaterial;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ(ﾏﾃﾘｱﾙ毎).

	ID3D11Buffer*				m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.
	ID3D11Buffer**				m_ppIndexBuffer;	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ.
	ID3D11SamplerState*			m_pSampleLinear;	//ｻﾝﾌﾟﾗ:ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.

	LPD3DXMESH			m_pMesh;		//ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	DWORD				m_NumMaterials;	//ﾏﾃﾘｱﾙ数.

	MY_MATERIAL*	m_pMaterials;	//ﾏﾃﾘｱﾙ構造体.
	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_EnableTexture;//ﾃｸｽﾁｬあり.

	ID3D11BlendState*		m_pAlphaBlend;		//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ有効.
	ID3D11BlendState*		m_pNoAlphaBlend;	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ無効.


	float		m_vScale;	//拡大縮小値(x,y,z等倍).
	D3DXVECTOR3		m_vRot;		//回転値(x,y,z).
								//※x=Pitch, y=Yaw, z=Roll.
	D3DXVECTOR3		m_vPos;		//位置(x,y,z).

	float			m_fAlpha;	//alpha.
};

#endif//#ifndef CDX9MESH_H