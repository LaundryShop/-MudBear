/***************************************************************************************************
*	SkinMeshCode Version 1.70
*	LastUpdate	: 2018/10/17
**/
#ifndef C_D3DXSKINMESH_H
#define C_D3DXSKINMESH_H
// 警告についてのコード分析を無効にする。4005：再定義.
#pragma warning( disable : 4005 )

#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <crtdbg.h>
#include "MyMacro.h"
#include<memory>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")


// 最大ボーン数.
#define MAX_BONES		255
// 最大ｱﾆﾒｰｼｮﾝｾｯﾄ数.
#define MAX_ANIM_SET	100
//最大テクスチャ数.
const int TEX_NO = 4;


// シェーダーに渡す値.
struct SHADER_SKIN_GLOBAL0
{	
	D3DXVECTOR4 vLightDir;	// ライト方向.
	D3DXVECTOR4 vEye;		// カメラ位置.
};

struct SHADER_SKIN_GLOBAL1
{
	D3DXMATRIX	mW;			// ワールド行列.
	D3DXMATRIX	mWVP;		// ワールドから射影までの変換行列.
	D3DXVECTOR4 vAmbient;	// アンビエント光.
	D3DXVECTOR4 vDiffuse;	// ディフューズ色.
	D3DXVECTOR4 vSpecular;	// 鏡面反射.
};

struct SHADER_GLOBAL_BONES
{
	D3DXMATRIX mBone[MAX_BONES];
	SHADER_GLOBAL_BONES()
	{
		for( int i=0; i<MAX_BONES; i++ )
		{
			D3DXMatrixIdentity( &mBone[i] );
		}
	}
};

// オリジナル　マテリアル構造体.
struct MY_SKINMATERIAL
{
	CHAR szName[110];
	D3DXVECTOR4	Ka;	// アンビエント.
	D3DXVECTOR4	Kd;	// ディフューズ.
	D3DXVECTOR4	Ks;	// スペキュラー.
	CHAR szTextureName[512];	// テクスチャーファイル名.
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;	// そのマテリアルであるポリゴン数.
	MY_SKINMATERIAL()
	{
		ZeroMemory( this, sizeof( MY_SKINMATERIAL ) );
	}
	~MY_SKINMATERIAL()
	{
		SAFE_RELEASE( pTexture );
	}
};

// 頂点構造体.
struct MY_SKINVERTEX
{
	D3DXVECTOR3	vPos;		// 頂点位置.
	D3DXVECTOR3	vNorm;		// 頂点法線.
	D3DXVECTOR2	vTex;		// UV座標.
	UINT bBoneIndex[4];		// ボーン 番号.
	float bBoneWeight[4];	// ボーン 重み.
	MY_SKINVERTEX()
	{
		ZeroMemory( this, sizeof( MY_SKINVERTEX ) );
	}
};
// ボーン構造体.
struct BONE
{
	D3DXMATRIX mBindPose;	// 初期ポーズ（ずっと変わらない）.
	D3DXMATRIX mNewPose;	// 現在のポーズ（その都度変わる）.
	DWORD dwNumChild;		// 子の数.
	int iChildIndex[50];	// 自分の子の“インデックス”50個まで.
	CHAR Name[256];

	BONE()
	{
		ZeroMemory( this, sizeof( BONE ) );
		D3DXMatrixIdentity( &mBindPose );
		D3DXMatrixIdentity( &mNewPose );
	}
};

// パーツメッシュ構造体.
struct SKIN_PARTS_MESH 
{
	DWORD	dwNumVert;
	DWORD	dwNumFace;
	DWORD	dwNumUV;
	DWORD			dwNumMaterial;
	MY_SKINMATERIAL*	pMaterial;
	char				TextureFileName[8][256];	// テクスチャーファイル名(8枚まで).
	bool				bTex;

	ID3D11Buffer*	pVertexBuffer;
	ID3D11Buffer**	ppIndexBuffer;

	// ボーン.
	int		iNumBone;
	BONE*	pBoneArray;

	bool	bEnableBones;	// ボーンの有無フラグ.

	SKIN_PARTS_MESH()
	{
		ZeroMemory( this, sizeof( SKIN_PARTS_MESH ) );
		pVertexBuffer = NULL;
		ppIndexBuffer = NULL;
		pBoneArray = NULL;
	}
};



// デバイス関係受取用構造体.
struct CD3DXSKINMESH_INIT
{
	HWND hWnd;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
};

// 派生フレーム構造体.
//	それぞれのメッシュ用の最終ワールド行列を追加する.
struct MYFRAME: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
	SKIN_PARTS_MESH* pPartsMesh;
	MYFRAME(){
		ZeroMemory( this, sizeof( MYFRAME ));
	}
};
// 派生メッシュコンテナー構造体.
//	コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD dwWeight;				// 重みの個数（重みとは頂点への影響。）.
	DWORD dwBoneNum;			// ボーンの数.
	LPD3DXBUFFER pBoneBuffer;	// ボーン・テーブル.
	D3DXMATRIX** ppBoneMatrix;	// 全てのボーンのワールド行列の先頭ポインタ.
	D3DXMATRIX* pBoneOffsetMatrices;// フレームとしてのボーンのワールド行列のポインタ.
};
// Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる.
//	ID3DXAllocateHierarchyは派生すること想定して設計されている.
class MY_HIERARCHY: public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY(){}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
		CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );	
};

//==================================================================================================
//
//	パーサークラス.
//
//==================================================================================================
class D3DXPARSER
{
public:
	MY_HIERARCHY cHierarchy;
	MY_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;

	LPD3DXANIMATIONCONTROLLER m_pAnimController;//ﾃﾞﾌｫﾙﾄで一つ.
	LPD3DXANIMATIONSET m_pAnimSet[MAX_ANIM_SET];

	HRESULT LoadMeshFromX( LPDIRECT3DDEVICE9, const char* );
	HRESULT AllocateBoneMatrix( LPD3DXMESHCONTAINER );
	HRESULT AllocateAllBoneMatrices( LPD3DXFRAME );
	VOID UpdateFrameMatrices( LPD3DXFRAME, LPD3DXMATRIX );


	int GetNumVertices( MYMESHCONTAINER* pContainer );
	int GetNumFaces( MYMESHCONTAINER* pContainer );
	int GetNumMaterials( MYMESHCONTAINER* pContainer );
	int GetNumUVs( MYMESHCONTAINER* pContainer );
	int GetNumBones( MYMESHCONTAINER* pContainer );
	int GetNumBoneVertices( MYMESHCONTAINER* pContainer, int iBoneIndex);
	DWORD GetBoneVerticesIndices( MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup );
	double GetBoneVerticesWeights( MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup );
	D3DXVECTOR3 GetVertexCoord( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR3 GetNormal( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR2 GetUV( MYMESHCONTAINER* pContainer, DWORD iIndex );
	int GetIndex( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR4 GetAmbient( MYMESHCONTAINER* pContainer, int iIndex );
	D3DXVECTOR4 GetDiffuse( MYMESHCONTAINER* pContainer, int iIndex );
	D3DXVECTOR4 GetSpecular( MYMESHCONTAINER* pContainer, int iIndex );
	CHAR* GetTexturePath( MYMESHCONTAINER* pContainer, int index );
	float GetSpecularPower( MYMESHCONTAINER* pContainer, int iIndex );
	int GeFaceMaterialIndex( MYMESHCONTAINER* pContainer, int iFaceIndex );
	int GetFaceVertexIndex( MYMESHCONTAINER* pContainer, int iFaceIndex, int iIndexInFace );
	D3DXMATRIX GetBindPose( MYMESHCONTAINER* pContainer, int iBoneIndex );
	D3DXMATRIX GetNewPose( MYMESHCONTAINER* pContainer, int iBoneIndex );
	CHAR* GetBoneName( MYMESHCONTAINER* pContainer, int iBoneIndex );

	// メッシュコンテナを取得する.
	LPD3DXMESHCONTAINER GetMeshContainer( LPD3DXFRAME pFrame );

	// アニメーションセットの切り替え.
	void ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC=NULL );
	// アニメーションセットの切り替え(開始フレーム指定可能版).
	void ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// アニメーション停止時間を取得.
	double GetAnimPeriod( int index );
	// アニメーション数を取得.
	int GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// 指定したボーン情報(座標・行列)を取得する関数.
	bool GetMatrixFromBone(const char* sBoneName, D3DXMATRIX* pOutMat );
	bool GetPosFromBone(const char* sBoneName, D3DXVECTOR3* pOutPos );

	// メッシュ解放.
	HRESULT ReleaseMesh( LPD3DXFRAME pFrame );



	// 一括解放処理.
	HRESULT Release();
};

//==================================================================================================
//
//	スキンメッシュクラス.
//
//==================================================================================================
class clsD3DXSKINMESH
{
public:
	//---------------------------------------------------
	//	吉田追加　テクスチャ切り替え.
	//---------------------------------------------------

	struct  CHANGE_TEXTURES
	{
		CHAR szTextureName[512];				//少し汚い.
		CHAR szTextureName_2[512];				//綺麗.

		ID3D11ShaderResourceView* pTexture;		//切り替えるテクスチャアドレス.
		ID3D11ShaderResourceView* pTextureTest;	//切り替えるテクスチャアドレス2つ目.


		ID3D11ShaderResourceView* pDefTexture;	//切り替え前のテクスチャアドレス.
		CHAR szTargetTextureName[512];			//切り替える対象のテクスチャ名.
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
			SAFE_RELEASE(pTextureTest);
			SAFE_RELEASE(pTexture);

			pDefTexture = nullptr;	//アドレスのみなのでnullptrで初期化.
		}
	};


	//くまさん用
	int m_IsEnabelChangeTexttureSystem;

	void SetEnabelChangeTexttureSystem(int SkinChange) {
		m_IsEnabelChangeTexttureSystem = SkinChange;
	}
	
	//ボスくまさん用.
	int m_B_IChangeTexttureSystem[TEX_NO];
	//頭.
	void SetChangeTexttureSystem_Atama(int BSkinChange) {
		m_B_IChangeTexttureSystem[0] = BSkinChange;
	}
	//体.
	void SetChangeTexttureSystem_Karada(int BSkinChange) {
		m_B_IChangeTexttureSystem[1] = BSkinChange;
	}
	//腕.
	void SetChangeTexttureSystem_UdeL(int BSkinChange) {
		m_B_IChangeTexttureSystem[2] = BSkinChange;
	}
	//腕.
	void SetChangeTexttureSystem_UdeR(int BSkinChange) {
		m_B_IChangeTexttureSystem[3] = BSkinChange;
	}

	//くまさん用.
	std::shared_ptr < CHANGE_TEXTURES> m_pChangeTextures;
	//ボスくまさん用.
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

//-----------------------------------------------------------------------------------

	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mRotation;

	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;
	D3DXVECTOR3 m_vLight;
	D3DXVECTOR3 m_vEye;

	//アニメーション速度.
	double m_dAnimSpeed;

	// メソッド.
	clsD3DXSKINMESH();
	~clsD3DXSKINMESH();

	HRESULT Init( CD3DXSKINMESH_INIT* si );
	// Xファイルからスキンメッシュを作成する.
	HRESULT CreateFromX( const char* szFileName );
	// 描画関数.
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye, LPD3DXANIMATIONCONTROLLER pAC=NULL );
	// 解放関数.
	HRESULT Release();

	double GetAnimSpeed()				{ return m_dAnimSpeed;		}
	void SetAnimSpeed( double dSpeed )	{ m_dAnimSpeed = dSpeed;	}

	// アニメーションセットの切り替え.
	void ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC=NULL );
	// アニメーションセットの切り替え(開始フレーム指定可能版).
	void ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// アニメーション停止時間を取得.
	double GetAnimPeriod( int index );
	// アニメーション数を取得.
	int GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// 指定したボーン情報(座標・行列)を取得する関数.
	bool GetMatrixFromBone( const char* sBoneName, D3DXMATRIX* pOutMat );
	bool GetPosFromBone( const char* sBoneName, D3DXVECTOR3* pOutPos );

	//アニメーションコントローラを取得.
	LPD3DXANIMATIONCONTROLLER GetAnimationController() {
		return m_pD3dxMesh->m_pAnimController;
	}

	//=================================================
	//小松が手を加えている.
	//=================================================
	//位置情報を設定.
	void SetPosition(const D3DXVECTOR3& Pos) {
		m_vPos = Pos;
	}
	//位置情報を取得.
	D3DXVECTOR3 GetPosition() {
		return m_vPos;
	}
	void SetRotation(float x, float y, float z) {
		m_vRot.x = x;
		m_vRot.y = y;
		m_vRot.z = z;
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
	void	SetScale(float x, float y, float z) {
		m_vScale.x = x;
		m_vScale.y = y;
		m_vScale.z = z;
	}
	void	SetScale(D3DXVECTOR3& Scale) { 
		m_vScale = Scale; 
	}
	//拡縮情報を取得.
	D3DXVECTOR3	GetScale() {
		return m_vScale; 
	}
	//=================================================
private:
	//=====================================================
	//小松が手を加えている.
	//=====================================================
	D3DXVECTOR3 m_vPos;		//ポジション(表示座標).
	D3DXVECTOR3	m_vRot;		//ローテーション(回転値).
	D3DXVECTOR3 m_vScale;	//スケール(拡大縮小値).
	//=====================================================

	HWND m_hWnd;
	// Dx9.
	LPDIRECT3D9 m_pD3d9;
	LPDIRECT3DDEVICE9 m_pDevice9;
	// Dx11.
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	ID3D11SamplerState*		m_pSampleLinear;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pConstantBufferBone;
	/////-------------------------------------
	// 吉田.
	ID3D11BlendState*		m_pAlphaBlend;		//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ有効.
	ID3D11BlendState*		m_pNoAlphaBlend;	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ無効.

	// メッシュ.
	D3DXPARSER*					m_pD3dxMesh;


	SKIN_PARTS_MESH*		m_pReleaseMaterial;

	// XFileのパス.
	CHAR	m_FilePath[256];

	// アニメーションフレーム.
	int				m_iFrame;
	float		m_Scale;

	HRESULT CreateDeviceDx9( HWND hWnd );
	HRESULT InitShader();
	HRESULT CreateIndexBuffer( DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer );
	void RecursiveSetNewPoseMatrices( BONE* pBone,D3DXMATRIX* pmParent );

	// 全てのメッシュを作成する.
	void BuildAllMesh( D3DXFRAME* pFrame );
	
	// メッシュを作成する.
	HRESULT CreateAppMeshFromD3DXMesh( LPD3DXFRAME pFrame );

	// Xファイルからスキン関連の情報を読み出す関数.
	HRESULT ReadSkinInfo( MYMESHCONTAINER* pContainer, MY_SKINVERTEX* pvVB, SKIN_PARTS_MESH* pParts );

	// ボーンを次のポーズ位置にセットする関数.
	void SetNewPoseMatrices( SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer );
	// 次の(現在の)ポーズ行列を返す関数.
	D3DXMATRIX GetCurrentPoseMatrix( SKIN_PARTS_MESH* pParts, int index );

	// フレーム描画.
	void DrawFrame( LPD3DXFRAME pFrame );
	// パーツ描画.
	void DrawPartsMesh( SKIN_PARTS_MESH* p, D3DXMATRIX World, MYMESHCONTAINER* pContainer );
	void DrawPartsMeshStatic( SKIN_PARTS_MESH* pMesh, D3DXMATRIX World, MYMESHCONTAINER* pContainer );

	//------------------------
	//吉田.
	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	HRESULT InitBlend();
	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ切り替え設定.
	void SetBlend(bool EnableAlpha);


	// 全てのメッシュを削除.
	void DestroyAllMesh( D3DXFRAME* pFrame );
	HRESULT DestroyAppMeshFromD3DXMesh( LPD3DXFRAME p );

};

#endif//#ifndef CD3DXSKINMESH_H