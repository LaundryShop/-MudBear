#include "Main.h"

//=======================================================
//小松が手を加えている(ファイルの入出力に必要).
//=======================================================
#include <iostream>
#include <fstream>	//入力はifstream、出力はofstreamに必要.
#include <string>	//std::getline関数を使うために必要.
#include <sstream>	//これは必要.
#include <vector>	//これは必要.
using namespace std;
//=======================================================
//ｸﾞﾛｰﾊﾞﾙ変数.
CMain*	g_pCMain = nullptr;

//================================================
//	ﾒｲﾝ関数.
//================================================
INT WINAPI WinMain(
	HINSTANCE hInstance,	//ｲﾝｽﾀﾝｽ番号(ｳｨﾝﾄﾞｳの番号).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow)
{
	//出力にデリートしていない数を表示させる為のもの.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	g_pCMain = new CMain();	//初期化＆ｸﾗｽ宣言.

	if (g_pCMain != nullptr) {

		//ｳｨﾝﾄﾞｳ作成成功したら.
		if (SUCCEEDED(
			g_pCMain->InitWindow(
				hInstance,
				0, 0,
				WND_W, WND_H,
				WND_TITLE)))
		{
			//Dx11用の初期化.
			if (SUCCEEDED(g_pCMain->InitD3D()))
			{
				//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
				g_pCMain->Loop();
			}
		}
		//終了.
		g_pCMain->DestroyD3D();//Direct3Dの解放.

		SAFE_DELETE(g_pCMain);//ｸﾗｽの破棄.
	}

	return 0;
}

//================================================
//	ｳｨﾝﾄﾞｳﾌﾟﾛｼｰｼﾞｬｰ.
//================================================
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT uMsg,
	WPARAM wPara, LPARAM lParam)
{
	//ﾌﾟﾛｼｰｼﾞｬｰ.
	return g_pCMain->MsgProc(hWnd, uMsg, wPara, lParam);
}

/**********************************************************************
*	ﾒｲﾝｸﾗｽ.
**/
//=================================================
//	ｺﾝｽﾄﾗｸﾀ.
//=================================================
CMain::CMain()
	: m_hWnd(nullptr)//初期化ﾘｽﾄと呼ぶ.
	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)
	, m_pSwapChain(nullptr)
	, m_pBackBuffer_TexRTV(nullptr)
	, m_pBackBuffer_DSTex(nullptr)
	, m_pBackBuffer_DSTexDSV(nullptr)
	, m_pDepthStencilState(nullptr)
	, m_pDepthStencilStateOff(nullptr)

	, m_vLight(0.0f, 0.5f, -1.0f)	//ﾗｲﾄ方向.
	, m_pVertexShader(nullptr)
	, m_pVertexLayout(nullptr)
	, m_pPixelShader(nullptr)
	, m_pConstantBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pSampleLinear(nullptr)
	, m_pTexture(nullptr)

	, m_pCDxKumaMesh(nullptr)
	, m_pKumaSkin(nullptr)
	, m_pCDxB_KumaMesh(nullptr)
	, m_pB_KumaSkin(nullptr)
	, m_pSphere(nullptr)
	, m_pBHeadSphere(nullptr)
	, m_pBBodySphere(nullptr)
	, m_pBNeckSphere(nullptr)
	, m_pBArmSphere(nullptr)

	, m_pEnemy(nullptr)
	, m_pShot(nullptr)
	, m_pBom(nullptr)
	, m_pMud(nullptr)

	, m_pHide(nullptr)
	, m_pHideTex(nullptr)
	, m_pBHideTex(nullptr)
	, m_pBomHideTex(nullptr)

	, m_pFade(nullptr)
	, m_pUIFade(nullptr)
	, m_pArea(nullptr)
	, m_pTitle(nullptr)
    , m_pStartUI(nullptr)
    , m_pEndUI(nullptr)
	, m_pOverUI(nullptr)
	,m_pOverWord(nullptr)
	,m_pRetryButton(nullptr)
	,m_pBackButton(nullptr)
	, m_pClearUI(nullptr)
	, m_pReturnButton(nullptr)
	, m_enScene(enScene::enTitle)
	, m_enStageScene(enStageScene::enMission)

	

	, m_KumaParam()
	, m_BossKumaParam()
	, m_ShotParam()
	, m_BomParam()
	, m_E_HideParam()
	, m_M_HideParam()
	, m_B_HideParam()
	, m_MudParam()

	, m_pChangeTextures(nullptr)
	, m_pBChangeTextures(nullptr)
	, m_WMousePos()
	, m_HitPos()
	, vHeadBonePos()
	, vHeadBoneOldPos()
	, vBodyBonePos()
	, vBodyBoneOldPos()
	, vNeckBoneOldPos()
	, vArmRBonePos()
	, vArmLBonePos()
	, ShotWait(0)
	, m_alphaFlag(false)
	, m_pPlayer(nullptr)
	, m_pCamera(nullptr)
	, m_pMouse(nullptr)
	, m_pCursor(nullptr)
	, m_pGround(nullptr)
	, m_pGrassGround(nullptr)
	, m_pStone(nullptr)
	, m_pGrass(nullptr)
	, m_pBackground(nullptr)
	, m_pGunParts(nullptr)
	, m_pPump(nullptr)
	, m_GunDispFlag(false)

	, m_pTime(nullptr)
	, BearDataCnt(0)
	, m_ContinueFlag(false)

	, m_Damage(CPlayer::enDamageParam::enNoDamage)
	, m_pMudDamage_1(nullptr)
	, m_pMudDamage_2(nullptr)
	, m_pMudDamage_3(nullptr)

	, m_pDecremetGauge(nullptr)
	, m_pGaugeBar(nullptr)
	, m_pBossBearIcon(nullptr)
	, m_pSpaceBar(nullptr)
	, m_pSpaceLogo(nullptr)
	, m_SpaceDispflag(nullptr)
	, m_pWaterBalloon()
	, WaterBallonCnt(3)
	, WaterBallonDispCnt(3)
	, m_pBearMission(nullptr)
	, m_pBossMission(nullptr)

	, m_pInGameCleanUI(nullptr)
	, m_pMissionUI(nullptr)
	, m_pCleanUI(nullptr)
	, m_pBossCleanUI(nullptr)
	, m_pTotalBearUI(nullptr)
	, m_BearCleanCnt(0)
	, m_BearCleanDigit(0)
	, m_InsideBearClean()

	, m_BossCleanCnt(0)
	, m_TotalBearCnt(0)
	, m_pTimeUI(nullptr)
	, m_TimeCnt()

	, m_pBomArea(nullptr)
	, m_HaniParam()

	, m_SpeceKey(0)
	, m_P_Moveflag(false)
	, m_TitleBGM(nullptr)
	, m_pMainBGM(nullptr)
	, m_pBossBGM(nullptr)
	, m_pGameOverBGM(nullptr)
	, m_pGameClearBGM(nullptr)

	, m_pClickSE(nullptr)
	, m_pCircleSE(nullptr)
	, m_WaterShotSE(nullptr)
	, m_WaterBalloonSE(nullptr)
	, m_pPlayerDamageSE(nullptr)
	, m_pBearDamageSE(nullptr)
	, m_pPlayerMoveSE(nullptr)
	, m_pPumpSE(nullptr)
	, m_pSwishSE(nullptr)
	, m_Sound()
	, m_BGMflag(false)
	, m_DamageSEflag(false)
	, m_PumpSEflag(false)
{
}


//=================================================
//	ﾃﾞｽﾄﾗｸﾀ.
//=================================================
CMain::~CMain()
{
}

//ｳｨﾝﾄﾞｳ初期化関数.
HRESULT CMain::InitWindow(
	HINSTANCE hInstance,	//ｲﾝｽﾀﾝｽ.
	INT x, INT y,			//ｳｨﾝﾄﾞｳx,y座標.
	INT width, INT height,	//ｳｨﾝﾄﾞｳ幅,高さ.
	const char* WindowName)	//ｳｨﾝﾄﾞｳ名.
{
	//ｳｨﾝﾄﾞｳの定義.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));//初期化(0を設定).

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//↓がマウスの表示させないよう
	ShowCursor(FALSE);	//開発中は消しとくといい.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = APP_NAME;
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	//ｳｨﾝﾄﾞｳｸﾗｽをWindowsに登録.
	if (!RegisterClassEx(&wc)) {
//		MessageBox(nullptr,
//			"ｳｨﾝﾄﾞｳｸﾗｽの登録に失敗", "ｴﾗｰ", MB_OK);
		_ASSERT_EXPR(false, L"ｳｨﾝﾄﾞｳｸﾗｽの登録に失敗");
		return E_FAIL;
	}

	//ｳｨﾝﾄﾞｳの作成.
	m_hWnd = CreateWindow(
		APP_NAME,		//ｱﾌﾟﾘ名.
		WindowName,		//ｳｨﾝﾄﾞｳﾀｲﾄﾙ.
		 WS_POPUP|WS_VISIBLE,	//ｳｨﾝﾄﾞｳ種別(普通).//WS_OVERLAPPEDWINDOW:通常.
		x, y,			//表示位置x,y座標.
		width, height,	//ｳｨﾝﾄﾞｳ幅,高さ.
		nullptr,		//親ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
		nullptr,		//ﾒﾆｭｰ設定.
		hInstance,		//ｲﾝｽﾀﾝｽ番号.
		nullptr);		//ｳｨﾝﾄﾞｳ作成時に発生するｲﾍﾞﾝﾄに渡すﾃﾞｰﾀ.
	if (!m_hWnd) {
		_ASSERT_EXPR(false, "ｳｨﾝﾄﾞｳ作成失敗");
		return E_FAIL;
	}

	//ｳｨﾝﾄﾞｳの表示.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//ｳｨﾝﾄﾞｳ関数(ﾒｯｾｰｼﾞ毎の処理).
LRESULT CMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY://ｳｨﾝﾄﾞｳが破棄されたとき.
		//ｱﾌﾟﾘｹｰｼｮﾝの終了をWindowsに通知する.
		PostQuitMessage(0);
		break;
	
	case WM_KEYDOWN://ｷｰﾎﾞｰﾄﾞが押されたとき.
		//ｷｰ別の処理.
		switch (static_cast<char>(wParam)) {
		case VK_ESCAPE:	//ESCｷｰ.
			if (MessageBox(nullptr,
				"ｹﾞｰﾑを終了しますか？", "警告", MB_YESNO) == IDYES)
			{
				//ｳｨﾝﾄﾞｳを破棄する.
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	}

	//ﾒｲﾝに返す情報.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
void CMain::Loop()
{
	//ﾒｯｼｭ読み込み関数.
	LoadMesh();
	//UIの読み込み関数.
	LoadUI();
	//サウンドの読み込み関数.
	LoadSound();

	//------------------------------------------------
	//	ﾌﾚｰﾑﾚｰﾄ調整準備.
	//------------------------------------------------
	float Rate	= 0.0f;	//ﾚｰﾄ.
	float FPS = 60.0f;	//FPS値.
	DWORD sync_old = timeGetTime();	//過去時間.
	DWORD sync_now;					//現在時間.

	//時間処理のため、最小単位を1ﾐﾘ秒に変更.
	timeBeginPeriod(1);
	Rate = 1000.0f / FPS; //理想時間を算出.

	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		sync_now = timeGetTime();	//現在の時間を取得.

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if( sync_now - sync_old >= Rate)
		{
			sync_old = sync_now;	//現在時間に置き換え.

			//ｱﾌﾟﾘｹｰｼｮﾝの処理はここから飛ぶ.
			AppMain();


			Render();

		}
	}
	//ｱﾌﾟﾘｹｰｼｮﾝの終了.
}

//ｱﾌﾟﾘｹｰｼｮﾝﾒｲﾝ処理.
void CMain::AppMain()
{
	m_pMouse->SetMouse(m_hWnd);
	//合計数を計算.
	m_TotalBearCnt = m_BearCleanCnt + m_BossCleanCnt;
	////#if Debug
	////	//デバッグ用
	//if (GetAsyncKeyState(VK_BACK) & 0x8000) {
	//	//m_enScene = enScene::enGameClear;
	//	m_enStageScene = enStageScene::enBoss;
	//}
	//#endif	//#if Debug

//==========================================================================================================================
//		小松が手を加えている。(switch文の所{処理だよ}をとってね!！)	吉田のすること	:	必要なところに処理を書いてね!
//==========================================================================================================================
	switch (m_enScene)
	{
	//タイトル時
	case CMain::enScene::enTitle:
		//タイトルのクリックしたときの処理.
		GetKyeAppMain();	
		break;
		//ゲームメイン時
	case CMain::enScene::enGameMain:
		//----------------------------------------------------
		//7_19更新(吉田).
		//m_CursorParam.m_Posをセットする.

		//時間を管理する.
		m_pTime->TimeControl();
		
		//===================================================================
		if (m_HaniParam.m_Pos.z <= 51.5f) {
			if (m_P_Moveflag == true) {
				m_HaniParam.m_Pos.z += 0.1f;
			}
		}
		else  {
			m_P_Moveflag = false;
		}

		//========================移動処理===================================
		m_pPlayer->Move(m_P_Moveflag);
		m_pBackground->Move(m_P_Moveflag);
		m_pGunParts->Move(m_P_Moveflag);
		m_pPump->Move(m_P_Moveflag);


		//===================================================================

		//水風船処理.
		WaterBalloonProcessAppMain();

		//水鉄砲処理.
		WaterGunProcessAppMain();
	

		//くまさんの表示時間を管理.
		//もし、くまさんの表示カウントがカウントタイムを超えると.
		//メインで処理しないとtrueが入り続けて消せなくなる.
		for (int i = 0; i < BEAR_MAX; i++) {
			if (m_pTime->GetMissionTime() >= m_KumaParam[i].m_AppearCnt) {
				m_KumaParam[i].m_DispFlag = true;
			}
		}
		//==============================================================.
		//くまさんと弾の当たり判定.
		for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
			for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
				if (m_ShotParam[SNo].m_ShotFlag == true && m_KumaParam[KNo].m_DispFlag == true && m_KumaParam[KNo].m_anim_no != KEscape 
					&& m_KumaParam[KNo].m_anim_no != KRejoice) {
					m_KumaParam[KNo].m_OldPos = m_KumaParam[KNo].m_Pos;
					if (Collision(m_pShot, m_ShotParam[SNo].m_Pos, m_pEnemy, m_KumaParam[KNo].m_Pos) == true) {
						m_ShotParam[SNo].m_ShotFlag = false;
						m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
						SESoundDrowBlaze(m_pBearDamageSE, m_KumaParam->m_BearDamageSEflag = true);
						if (m_pHideTex[KNo].GetHideTexFlag() == false) {
							m_KumaParam[KNo].m_SkinCnt--;
							m_E_HideParam[KNo].m_Pos = m_KumaParam[KNo].m_Pos;
							m_E_HideParam[KNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);//隠すテクスチャの位置調節用.
							//くまさん用の隠すテクスチャにくまさんの座標を入れる.
							m_pHideTex[KNo].HideTex_Disp(m_E_HideParam[KNo].m_Pos);
							
						}
					}
					if (Collision(m_pShot, m_ShotParam[SNo].m_Pos, m_pEnemy, m_KumaParam[KNo].m_OldPos += D3DXVECTOR3(0.0f, 0.5f, 0.0f)) == true) {
						m_ShotParam[SNo].m_ShotFlag = false;
						m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
						SESoundDrowBlaze(m_pBearDamageSE, m_KumaParam->m_BearDamageSEflag = true);

						if (m_pHideTex[KNo].GetHideTexFlag() == false) {
							m_KumaParam[KNo].m_SkinCnt--;
							m_E_HideParam[KNo].m_Pos = m_KumaParam[KNo].m_Pos;
							m_E_HideParam[KNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);//隠すテクスチャの位置調節用.
							//くまさん用の隠すテクスチャにくまさんの座標を入れる.
							m_pHideTex[KNo].HideTex_Disp(m_E_HideParam[KNo].m_Pos);
							
						}
					}
				}
			}
		}

		//水風船当たり判定範囲(仮).
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_KumaParam[KNo].m_Pos.z <= m_pPlayer->GetPosition().z + BOM_AREA && m_KumaParam[KNo].m_HitBomFlag == false 
				&& m_KumaParam[KNo].m_anim_no != KEscape ) {
				m_KumaParam[KNo].m_HitBomFlag = true;
			}
		
		
			if (m_BomParam.m_ExpFlag == true )
			{
				if (m_pBomHideTex->GetHideTexFlag() == false)
				{
					m_Bom_HideParam.m_Pos = m_BomParam.m_Pos;
					m_Bom_HideParam.m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
					m_pBomHideTex->HideTex_Disp(m_Bom_HideParam.m_Pos);
					m_BomParam.m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
				}
			}
		}
		//-------------------くまさんの処理内容-------------------------.
		//この順番でしか動かない.

		//アニメーション時間の加算.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			//m_KumaParam[KNo].m_anim_time += m_pKumaSkin[KNo].GetAnimSpeed();
		}

		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			//移動アニメーション処理.
			if (m_KumaParam[KNo].m_TexChangeFlag == false) {//何もしていなければ.
				if (m_KumaParam[KNo].m_anim_no == KWait ) {
					m_KumaParam[KNo].m_anim_no = KWalk;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Move;
					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
			}	//当たった時の処理.
			else{
				if (m_KumaParam[KNo].m_anim_no == KWalk) {
					m_KumaParam[KNo].m_anim_no = KHit;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Wait;
					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
			}
			//あたったときのアニメーションが終わったら.
			if (m_pKumaSkin[KNo].GetAnimPeriod(m_KumaParam[KNo].m_anim_no) - m_pKumaSkin[KNo].GetAnimSpeed() 
				<= m_KumaParam[KNo].m_anim_time && m_KumaParam[KNo].m_anim_no == KHit ) {
				m_KumaParam[KNo].m_anim_no = KWait;
			}
		}

		//くまさんの動きの切り替え処理.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_KumaParam[KNo].m_TexChangeFlag == false && m_KumaParam[KNo].m_SkinCnt > CLEAN && m_P_Moveflag == false) {
				if (m_KumaParam[KNo].m_Pos.z <= m_pCamera->vPos.z + ATK_LENGTH && m_KumaParam[KNo].m_anim_no != KEscape 
					&& m_KumaParam[KNo].m_anim_no != KAttack) {	//画面手前まで来たら攻撃する.
					m_KumaParam[KNo].m_anim_no = KAttack;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Wait;
					m_KumaParam[KNo].m_anim_time = 0.0;
					m_pPlayer->UpDamage();		//ダメージを受けたとき.
					SESoundDrowBlaze(m_pPlayerDamageSE, m_DamageSEflag = true);

					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
				if (m_pKumaSkin[KNo].GetAnimPeriod(m_KumaParam[KNo].m_anim_no) - m_pKumaSkin[KNo].GetAnimSpeed() <=
					m_KumaParam[KNo].m_anim_time && m_KumaParam[KNo].m_anim_no == KAttack) {//アニメーションが終わったら動き出してね.
					m_KumaParam[KNo].m_anim_no = KEscape;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Escape;
					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
			}
		}

		//くまさんが綺麗になった時の処理.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_KumaParam[KNo].m_SkinCnt <= CLEAN && m_pHideTex->GetHideTexFlag() == false 
				&& m_KumaParam[KNo].m_anim_no != KRejoice && m_KumaParam[KNo].m_anim_no != KEscape) {  
				m_KumaParam[KNo].m_anim_no = KRejoice;
				m_KumaParam[KNo].m_enKMove = enKumaMove::Wait;
				m_KumaParam[KNo].m_anim_time = 0.0;
				//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				m_BearCleanCnt++;
			}
			//喜びアニメーションが終わったら.
			if (m_pKumaSkin[KNo].GetAnimPeriod(m_KumaParam[KNo].m_anim_no) - m_pKumaSkin[KNo].GetAnimSpeed()
				<= m_KumaParam[KNo].m_anim_time && m_KumaParam[KNo].m_anim_no == KRejoice) {
				m_KumaParam[KNo].m_anim_no = KEscape;
				m_KumaParam[KNo].m_enKMove = enKumaMove::Escape;
				//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
			}
		}

		//くまさんの動きの処理.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			switch (m_KumaParam[KNo].m_enKMove)
			{
			case enKumaMove::Wait:
				break;
			case enKumaMove::Move:
				//表示フラグがtrueの時移動開始.
				if (m_KumaParam[KNo].m_DispFlag == true) {
					//パターンで移動方法が変わる.
					switch (m_KumaParam[KNo].m_MovePattern)
					{
					case 1:
						//くまさんが左から出現した場合
						if (m_KumaParam[KNo].m_Pos.x < K_Left) {
							m_KumaParam[KNo].m_Pos.x += m_KumaParam[KNo].m_Speed;	
							m_KumaParam[KNo].m_Rot.y = -1.5f;
						}
						//くまさんが右から出現した場合
						else if (m_KumaParam[KNo].m_Pos.x > K_Right) {
							m_KumaParam[KNo].m_Pos.x -= m_KumaParam[KNo].m_Speed;
							m_KumaParam[KNo].m_Rot.y = 1.5f;
						}
						//両側から道に来た場合
						else
						{
							m_KumaParam[KNo].m_Rot.y = 0.0f;
							m_KumaParam[KNo].m_Pos.z -= m_KumaParam[KNo].m_Speed;
						}
						break;
					case 2:
						m_KumaParam[KNo].m_Pos.x += m_KumaParam[KNo].m_Speed;
						m_KumaParam[KNo].m_Rot.y = -1.5f;
						break;
					case 3:
						m_KumaParam[KNo].m_Pos.x -= m_KumaParam[KNo].m_Speed;
						m_KumaParam[KNo].m_Rot.y = 1.5f;
						break;
					case 4:
						m_KumaParam[KNo].m_Pos.x += 1.0f;
						m_KumaParam[KNo].m_Pos.z -= m_KumaParam[KNo].m_Speed;
						m_KumaParam[KNo].m_Rot.y = -1.5f;
						break;
					default:
						break;
					}
				}
				break;
			case enKumaMove::Attack:
				break;
			case enKumaMove::Stop:
				break;
			case enKumaMove::Escape:
				if (m_KumaParam[KNo].m_Pos.x <= CAM_CENTER) {
					m_KumaParam[KNo].m_Pos.x -= m_KumaParam[KNo].m_Speed;
					m_KumaParam[KNo].m_Rot.y = 1.5f;
					if (m_KumaParam[KNo].m_Pos.x <= -10.0f) {
						m_KumaParam[KNo].m_DispFlag = false;
					}
				}
				else if (m_KumaParam[KNo].m_Pos.x >= CAM_CENTER)
				{
					m_KumaParam[KNo].m_Pos.x += m_KumaParam[KNo].m_Speed;
					m_KumaParam[KNo].m_Rot.y = -1.5f;
					if (m_KumaParam[KNo].m_Pos.x >= 10.0f) {
						m_KumaParam[KNo].m_DispFlag = false;
					}
				}
				break;
			default:
				break;
			}
		}
		//==============================================================.

		//水鉄砲を表示してゲージを回復する処理.
		WaterGunGaugeChargeAppMain();


		switch (m_enStageScene)
		{
		case enStageScene::enMission:
			BGMSoundDrow(m_pGameOverBGM, m_BGMflag = false);
			BGMSoundDrow(m_TitleBGM, m_BGMflag = false);
			BGMSoundDrow(m_pMainBGM, m_BGMflag = true);

			if (m_BearCleanCnt >= CLEARMISSION) {
				m_enStageScene = enStageScene::enBoss;
				m_pTime->SetBossTime(0);
				m_pTime->SetMissionTime(MISSION_TIME);
				m_P_Moveflag = true;
			}

			//もし、カウントタイムが120以上になるとボスステージに飛ぶ.
			if (m_pTime->GetMissionTime() >= MISSION_TIME) {//1646大体終わる時間.
				if (m_pFade->GetFadeFlag() == 0) {
					m_pFade->FadeIn_Disp();
				}
				if (m_pFade->GetFadeFlag() == 2)
				{
					m_enStageScene = enStageScene::enBoss;
					m_pTime->SetBossTime(0);
					m_P_Moveflag = true;
				}
			}

			//負けてしまった時.
			if (m_pPlayer->GetDamage() >= LAST_DAMAGE) {
				
				if (m_pFade->GetFadeFlag() == 0) 
				{
					m_pFade->FadeIn_Disp();
				}
				if (m_pFade->GetFadeFlag() == 2)
				{
					m_pPlayer->DamageInit();
					//負けてしまった時.
					m_pTime->SetMissionTime(0);
					m_BearCleanCnt = 0;
					m_GunDispFlag = false;
					//コンテニューへ.
					m_enScene = enScene::enGameOver;
				}
			}
			break;
		case enStageScene::enBonus:
			if (GetAsyncKeyState('G') & 0x8000)
			{
				m_enStageScene = enStageScene::enBoss;
				m_pTime->SetBossTime(0);
			}

			break;
		case enStageScene::enBoss:
			BGMSoundDrow(m_pGameOverBGM, m_BGMflag = false);
			BGMSoundDrow(m_pMainBGM, m_BGMflag = false);
			BGMSoundDrow(m_pBossBGM, m_BGMflag = true);

			for (int KNo = 0; KNo < 10; KNo++) {
				if (m_KumaParam[KNo].m_anim_no != KEscape) {
					m_KumaParam[KNo].m_anim_no = KEscape;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Escape;
					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
			}
			//-------------------ボスくまさんの処理内容---------------------.
			m_pTime->TimeCnt();
			//アニメーション時間の加算.
			m_BossKumaParam.m_anim_time += m_pB_KumaSkin->GetAnimSpeed();

			if (m_BossKumaParam.m_anim_no != BRejoice) {
				//カウントが超えたら投げるアニメーションにはいってね.
				if (m_pTime->GetBossTime() >= 900 && m_pTime->GetBossTime() <= APPROACH_TIME) {
					if (m_BossKumaParam.m_anim_no == BWait) {
						m_BossKumaParam.m_anim_no = BKnead;
						m_BossKumaParam.m_anim_time = 0.0;
						//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
					}	//こねるアニメーションが終わったら振り上げるに変更.
					else if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed() 
						<= m_BossKumaParam.m_anim_time && m_BossKumaParam.m_anim_no == BKnead) 
					{
						m_BossKumaParam.m_anim_no = BShakeUp;
						m_BossKumaParam.m_anim_time = 0.0;
						//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
					}	//振り上げるが終わったら振り下ろすへ
					else if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed()
							<= m_BossKumaParam.m_anim_time && m_BossKumaParam.m_anim_no == BShakeUp)
					{
							m_BossKumaParam.m_anim_no = BShakeDown;
							//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
							

							//泥を投げる処理.
							//泥初期値.
							for (int MNo = 0; MNo < MUD_MAX; MNo++) {
								m_MudParam[MNo].m_Pos = m_BossKumaParam.m_Pos;
								m_MudParam[MNo].m_Pos.y += 0.7f;
								m_MudParam[MNo].m_Pos.x -= 0.3f;
								UnitMove(&m_MudParam[MNo].m_VecUnit, m_BossKumaParam.m_Pos, m_pCamera->vPos, MUD_SPD);
								if (m_MudParam[MNo].m_ShotFlag == false) {
									m_MudParam[MNo].m_ShotFlag = true;
									m_MudParam[MNo].m_fAcc = 0.0f;
								}
							}
					}	//振り下ろすが終わったら戻してまた投げる処理へ.
					else if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed()
						<= m_BossKumaParam.m_anim_time-0.4 && m_BossKumaParam.m_anim_no == BShakeDown) 
					{
						m_BossKumaParam.m_anim_no = BWait;
						m_BossKumaParam.m_anim_time = 0.0;
						//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
					}
				}
			}
			//近づくを有効したとき.
			if (m_pTime->GetBossTime() >= APPROACH_TIME) {
				if (m_BossKumaParam.m_anim_no != BApproach && m_BossKumaParam.m_anim_no != BWinch && m_BossKumaParam.m_anim_no != BAttack && m_BossKumaParam.m_anim_no != BRejoice) {
					m_BossKumaParam.m_anim_no = BApproach;
					m_BossKumaParam.m_enBKMove = enBossKumaMove::Approach;
					//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
				}
			}


			//ボスくまさんが近づくとき弾が3発当たったら.
			if (m_BossKumaParam.m_HitCnt >= HIT_CNT_MAX && m_BossKumaParam.m_anim_no == BApproach) {

				m_BossKumaParam.m_anim_no = BWinch;
				m_BossKumaParam.m_enBKMove = enBossKumaMove::Winch;
				m_BossKumaParam.m_HitCnt = 0;
				m_BossKumaParam.m_anim_time = 0.0;	
				//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
			}

			//ボスくまさんが近づくとき水風船があたったら.
			if (m_BossKumaParam.m_HitBomFlag == true && m_BossKumaParam.m_anim_no == BApproach) {

				m_BossKumaParam.m_anim_no = BWinch;
				m_BossKumaParam.m_enBKMove = enBossKumaMove::Winch;
				m_BossKumaParam.m_HitCnt = 0;
				m_BossKumaParam.m_anim_time = 0.0;
				m_BossKumaParam.m_HitBomFlag = false;
				//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
			}


			//ひるむアニメーションが終わったら近づくに戻す.
			if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed() 
				<= m_BossKumaParam.m_anim_time  && m_BossKumaParam.m_anim_no == BWinch) {
				m_BossKumaParam.m_anim_no = BWait;
				//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
			}

			//ボスくまさんが近づいてくるとき手前まで来たらなすりつけてくる処理.
			if (m_BossKumaParam.m_anim_no == BApproach && m_BossKumaParam.m_Pos.z <=  m_pCamera->vPos.z +ATK_LENGTH ) {
				m_BossKumaParam.m_anim_no = BAttack;
				m_BossKumaParam.m_enBKMove = enBossKumaMove::Attack;
				m_BossKumaParam.m_anim_time = 0.0;
				//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
				
			}

			//攻撃アニメーションが終わってから処理する
			if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed() <=
				m_BossKumaParam.m_anim_time&& m_BossKumaParam.m_anim_no == BAttack) {
				m_pPlayer->UpDamage();
				SESoundDrowBlaze(m_pPlayerDamageSE, m_DamageSEflag = true);

				m_BossKumaParam.m_anim_no = BApproach;
			}

			//ボスくまさんが綺麗になったら.
			if (m_BossKumaParam.m_anim_no != BRejoice && m_BossKumaParam.m_anim_no != BWinch ) {
				if (m_BossKumaParam.m_SkinChangeCnt[0] == 0) {
					if (m_BossKumaParam.m_SkinChangeCnt[1] == 0) {
						if (m_BossKumaParam.m_SkinChangeCnt[2] == 0) {
							if (m_BossKumaParam.m_SkinChangeCnt[3] == 0) {
								m_BossKumaParam.m_anim_no = BRejoice;
								m_BossKumaParam.m_enBKMove = enBossKumaMove::Wait;
								m_BossKumaParam.m_anim_time = 0.0;
								//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
								m_pFade->FadeIn_Disp();
								for (int MNo = 0; MNo < MUD_MAX; MNo++) {
									m_MudParam[MNo].m_ShotFlag = false;
								}
							}
						}
					}
				}
			}

			//喜びアニメーションが終わってから処理する
			if (m_pB_KumaSkin->GetAnimPeriod(5) - m_pB_KumaSkin->GetAnimSpeed() <=
				m_BossKumaParam.m_anim_time && m_BossKumaParam.m_anim_no == BRejoice) {

				if (m_pFade->GetFadeFlag() == 2)
				{
					m_BossCleanCnt += 10;
					m_GunDispFlag = false;
					m_enScene = enScene::enGameClear;
				}
			}

			//ボスくまさんの動き処理.
			switch (m_BossKumaParam.m_enBKMove)
			{
			case enBossKumaMove::Wait:
				break;
			case enBossKumaMove::Approach:
				m_BossKumaParam.m_Pos.z -= 0.01f;
				break;
			case enBossKumaMove::Attack:
				break;
			case enBossKumaMove::Throw:
				break;
			case enBossKumaMove::Winch:
				m_BossKumaParam.m_Pos.z += 0.03f;
				//初期位置を入れる.
   				if (m_BossKumaParam.m_Pos.z >= 56.0f) {
					m_BossKumaParam.m_Pos.z -= 0.03f;
				}
				break;
			default:
				break;
			}

			//泥の移動処理.
			for (int MNo = 0; MNo < MUD_MAX; MNo++) {
				if (m_MudParam[MNo].m_ShotFlag == true) {
			
					m_MudParam[MNo].m_Pos.x -= m_MudParam[MNo].m_VecUnit.x;
					m_MudParam[MNo].m_Pos.z -= m_MudParam[MNo].m_VecUnit.z;

					m_MudParam[MNo].m_Pos.y += MUD_JUMP_POWER - m_MudParam[MNo].m_fAcc;
					m_MudParam[MNo].m_fAcc += GREVITY;

					if (m_MudParam[MNo].m_Pos.y < 0.0f) {
						m_MudParam[MNo].m_ShotFlag = false;
						m_MudParam[MNo].m_Pos.y = 0.0f;
					}
				}
			}

			//負けてしまった時.
			if (m_pPlayer->GetDamage() >= LAST_DAMAGE) {
				if (m_pFade->GetFadeFlag() == 0)
				{
					m_pFade->FadeIn_Disp();
				}
				if (m_pFade->GetFadeFlag() == 2)
				{
					m_pTime->SetMissionTime(MISSION_TIME);
					m_pPlayer->DamageInit();
					m_pTime->SetBossTime(0);
					m_enScene = enScene::enGameOver;
				}
			}


			break;
		default:
			break;
		}

	//---------------------------------------------
	// スキンメッシュのボーン座標を取得する.
	//---------------------------------------------
	//----------------------------------------------------------------
	//頭.
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_atama_boon", &vHeadBonePos))
	{
		m_pBHeadSphere->SetPosition(vHeadBonePos);

	}
	//----------------------------------------------------------------
	//体.
	//お腹.
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_kosi_boon", &vBodyBonePos[0]))
	{
		m_pBBodySphere->SetPosition(vBodyBonePos[0]);

	}
	//首.
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_kubi_boon", &vBodyBonePos[1]))
	{
		m_pBBodySphere->SetPosition(vBodyBonePos[1]);

	}


	//----------------------------------------------------------------
	//腕.
	//右.
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_R_te_boon", &vArmRBonePos[0]))
	{
		m_pBArmSphere->SetPosition(vArmRBonePos[0]);
		
	}
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_R_kata_boon", &vArmRBonePos[1]))
	{
		m_pBArmSphere->SetPosition(vArmRBonePos[1]);

	}
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_R_hizi_boon", &vArmRBonePos[2]))
	{
		m_pBArmSphere->SetPosition(vArmRBonePos[2]);
	}

	//左.
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_L_te_boon", &vArmLBonePos[0]))
	{
		m_pBArmSphere->SetPosition(vArmLBonePos[0]);

	}
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_L_kata_boon", &vArmLBonePos[1]))
	{
		m_pBArmSphere->SetPosition(vArmLBonePos[1]);

	}
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_L_hizi_boon", &vArmLBonePos[2]))
	{
		m_pBArmSphere->SetPosition(vArmLBonePos[2]);
	}
		break;
		//コンテニュー時
	case CMain::enScene::enGameOver:
		GetKyeOver();
		break;
		//ゲームクリア時.
	case CMain::enScene::enGameClear:
		GetKyeClear();
		break;
	//それ以外の事が起きた時
	default:
		break;
	}
	//===============================================================================================
	
}

//ﾚﾝﾀﾞﾘﾝｸﾞ(描画).
void CMain::Render()
{
	//画面のｸﾘｱ.
	float ClearColor[4] = { 0.0f,0.0f,0.5f,1.0f };	//ｸﾘｱ色(RGBAの順).
	//ｶﾗｰﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pContext11->ClearRenderTargetView(
		m_pBackBuffer_TexRTV, ClearColor);
	//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pContext11->ClearDepthStencilView(
		m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	//ﾜｰﾙﾄﾞ変換(表示位置・回転・拡縮等を設定する).
	//Y軸回転.
	D3DXMatrixRotationY(
		&mWorld,	//(out)計算結果.
		timeGetTime()/1000.0f);		//単純にY軸回転させる(ﾗｼﾞｱﾝ指定).

	//==============================================
	// 追従カメラ設定.
	//==============================================
	//カメラ位置(自機の背中から)の設定.
	m_pCamera->vPos = m_pCamera->vLook = m_pPlayer->GetPosition();//自機の位置をコピー.
	m_pCamera->fYaw = m_pPlayer->GetRotation().y;//Y軸回転値をコピー.

	//Y軸回転行列の作成.
	D3DXMatrixRotationY(&m_pCamera->mRot, m_pCamera->fYaw);
	//Z軸ベクトルを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);
	//Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord(&vecAxisZ,
		&vecAxisZ, &m_pCamera->mRot);
	//----------------------------------------
	//　吉田変更.
	m_pCamera->vPos -= vecAxisZ * 4.0f;	//自機の背中側.
	//m_pCamera.vLook += vecAxisZ * 2.0f;	//自機の前側.


	//----------------------------------------
	//ﾋﾞｭｰ(ｶﾒﾗ)変換.
	D3DXVECTOR3	vUpVec(0.0f, 1.0f, 0.0f);	//上方(ﾍﾞｸﾄﾙ).
	D3DXMatrixLookAtLH(
		&mView,	//(out)ﾋﾞｭｰ計算結果.
		&m_pCamera->vPos, &m_pCamera->vLook, &vUpVec);

	//ﾌﾟﾛｼﾞｪｸｼｮﾝ(射影)変換.
	D3DXMatrixPerspectiveFovLH(
		&mProj,	//(out)ﾌﾟﾛｼﾞｪｸｼｮﾝ計算結果.
		static_cast<FLOAT>(D3DX_PI) / 4.0,	//y方向の視野(ﾗｼﾞｱﾝ指定)数値を大きくしたら視野が狭くなる.
		static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H),//ｱｽﾍﾟｸﾄ比(幅÷高さ).
		0.1f,		//近いﾋﾞｭｰ平面のz値.
		100.0f);	//遠いﾋﾞｭｰ平面のz値.
	
	//背景を表示.
	m_pBackground->Render(mView, mProj, m_vLight, m_pCamera->vPos);

	//地面を表示.
	for (int i = 0; i < GROUND_MAX; i++) {
		m_pGround->SetPosition(m_pGround->GetGroundPos(i));
		m_pGround->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}
	//草の地面.
	for (int i = GRASS_GROUND_MIDDLE; i < GRASS_GROUND_MAX; i++) {
		for (int j = 0; j < GRASS_GROUND_MIDDLE; j++) {
			m_pGrassGround->SetPosition(m_pGrassGround->GetGrassGroundPos(j));
			m_pGrassGround->Render(mView, mProj, m_vLight, m_pCamera->vPos);
		}
		m_pGrassGround->SetPosition(m_pGrassGround->GetGrassGroundPos(i));
		m_pGrassGround->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}

	//カーソルUIの表示.
	m_pCursor->SetPattern(0, 10);
	m_pCursor->SetPosition(D3DXVECTOR3(m_pMouse->GetCursorPosX(), m_pMouse->GetCursorPosY(), 0.30f));
	m_pCursor->Render();

	//=================================================================================================================================
	//		小松が手を加えている。(switch文の所{処理だよ}をとってね!！)	吉田のすること:必要なところに下のようなRenderを入れる.
	//=================================================================================================================================
	switch (m_enScene)
	{
	case CMain::enScene::enTitle:
		//スタート(進める)UI
		m_pStartUI->SetPosition(WND_W_HALF - (185.0f / 2), CLICK_START_Y, 0.40f);
		m_pStartUI->Render();
		//ゲームをやめるUI.
		m_pEndUI->SetPosition(WND_W - 110.0f, 0.0f, 0.41f);
		m_pEndUI->Render();
		//タイトルUI
		m_pTitle->SetPosition(WND_W_HALF - (534.0f /2), 20.0f, 0.42f);
		m_pTitle->Render();
		break;
	case CMain::enScene::enGameMain:
		//水風船の表示.
		if (m_BomParam.m_ExpFlag == true)
		{
			if (m_pBomHideTex->GetHideTexFlag() == false)
			{
				m_Bom_HideParam.m_Pos = m_BomParam.m_Pos;
				m_Bom_HideParam.m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
				m_pBomHideTex->HideTex_Disp(m_Bom_HideParam.m_Pos);
				m_BomParam.m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
			}
		}
		//水風船が当たっているときの隠すエフェクト.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_BomParam.m_ExpFlag == true && m_KumaParam[KNo].m_HitBomFlag == true) {
				m_KumaParam[KNo].m_SkinCnt -= 2;
				m_E_HideParam[KNo].m_Pos = m_KumaParam[KNo].m_Pos;
				m_E_HideParam[KNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);//隠すテクスチャの位置調節用.
				//くまさん用の隠すテクスチャにくまさんの座標を入れる.
				m_pHideTex[KNo].HideTex_Disp(m_E_HideParam[KNo].m_Pos);
				m_KumaParam[KNo].m_HitBomFlag = false;
			}
		}
		m_BomParam.m_ExpFlag = false;

		//くまさんの表示.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			//m_KumaParam[KNo].m_DispFlag = false;
			if (m_KumaParam[KNo].m_DispFlag == true) {
				m_pKumaSkin[KNo].SetTexChangeSystem(m_KumaParam[KNo].m_SkinCnt);
				m_pKumaSkin[KNo].SetPosition(m_KumaParam[KNo].m_Pos);//表示位置を更新.
				m_pKumaSkin[KNo].Render(mView, mProj, m_vLight, m_pCamera->vPos);
				m_pKumaSkin[KNo].SetRotation(m_KumaParam[KNo].m_Rot);
			}
		}

		//「弾」の表示.
		for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
			if (m_ShotParam[SNo].m_ShotFlag == true) {
				m_pShot->SetScale( 0.04f);
				m_pShot->SetPosition(m_ShotParam[SNo].m_Pos);//表示位置を更新.
				m_pShot->Render(mView, mProj, m_vLight, m_pCamera->vPos);
			}
		}

		//「水風船」の表示.
		if (m_BomParam.m_ShotFlag == true && WaterBallonDispCnt >= 0) {
			m_pBom->SetPosition(m_BomParam.m_Pos);//表示位置の更新.
			m_pBom->Render(mView, mProj, m_vLight, m_pCamera->vPos);
		}

		//「水風船範囲」の表示.
		m_pBomArea->SetPosition(m_HaniParam.m_Pos);
		m_pBomArea->Render(mView, mProj, m_vLight, m_pCamera->vPos);
		m_pBomArea->SetScale(0.4f);

		//エリアの表示.
		if (m_pArea->GetFadeFlag() == true) {
			m_pArea->AreaFade();
			m_pArea->Attach(m_pBomArea);
			m_pArea->Load(mView, mProj, m_vLight, m_pCamera->vPos);
			m_pArea->Render(m_HaniParam.m_Pos);
		}

		if (m_enStageScene == enStageScene::enBoss)
		{
			if (m_pFade->GetFadeFlag() == 0)
			{
				m_P_Moveflag = true;	//移動用.

			}

			//----------------------------------------
			//	ボスくまさんのボーン頭位置調整用.
			//----------------------------------------

			vHeadBoneOldPos = vHeadBonePos;
			vHeadBoneOldPos.y -= 0.4f;	//頭の真ん中に来るよう調整.

			vBodyBoneOldPos = vBodyBonePos[0];

			vNeckBoneOldPos = vBodyBonePos[1];
			vNeckBoneOldPos.y -= 0.2f;	//首の位置調整.

			

			//「ボスくまさん」の表示
			m_pB_KumaSkin->SetPosition(m_BossKumaParam.m_Pos);
			m_pB_KumaSkin->Render(mView, mProj, m_vLight, m_pCamera->vPos);

			//「泥」の表示.
			for (int MNo = 0; MNo < MUD_MAX; MNo++) {
				if (m_MudParam[MNo].m_ShotFlag == true) {
					m_pMud->SetPosition(m_MudParam[MNo].m_Pos);//表示位置を更新.
					m_pMud->Render(mView, mProj, m_vLight, m_pCamera->vPos);
					m_pMud->SetScale(0.5f);
				}
			}

			//ボスくまさんと弾の当たり判定.
			for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
				for (int BONo = 0; BONo < 3; BONo++) {
					if (m_ShotParam[SNo].m_ShotFlag == true /*&& m_BossKumaParam.m_WinchFlag == true*/) {
						//頭と弾の当たり判定.
						if (Collision(m_pBHeadSphere, vHeadBoneOldPos, m_pShot, m_ShotParam[SNo].m_Pos) == true && m_BossKumaParam.m_SkinChangeCnt[HEAD] > 0) {
							m_ShotParam[SNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
							if (m_pBHideTex[HEAD].GetHideTexFlag() == false) {
								m_BossKumaParam.m_SkinChangeCnt[HEAD]--;
								m_B_HideParam[HEAD].m_Pos = vHeadBoneOldPos;	//隠すエフェクト位置調整用.
								m_B_HideParam[HEAD].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);		//位置調整.
								m_pBHideTex[HEAD].HideTex_Disp(m_B_HideParam[HEAD].m_Pos);//隠すエフェクトの位置セット.
								if (m_BossKumaParam.m_anim_no == BApproach) {
									m_BossKumaParam.m_HitCnt++;						//当たった数.
								}
							}
						}
						//体と弾の当たり判定.
						if (Collision(m_pBBodySphere, vBodyBoneOldPos, m_pShot, m_ShotParam[SNo].m_Pos) == true || Collision(m_pBNeckSphere, vNeckBoneOldPos, m_pShot, m_ShotParam[SNo].m_Pos) == true) {
							if (m_BossKumaParam.m_SkinChangeCnt[BODY] > 0) {
								m_ShotParam[SNo].m_ShotFlag = false;
								m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
								if (m_pBHideTex[BODY].GetHideTexFlag() == false) {
									m_BossKumaParam.m_SkinChangeCnt[BODY]--;
									m_B_HideParam[BODY].m_Pos = vBodyBoneOldPos;
									m_B_HideParam[BODY].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
									m_pBHideTex[BODY].HideTex_Disp(m_B_HideParam[BODY].m_Pos);//隠すエフェクトの位置セット.
									if (m_BossKumaParam.m_anim_no == BApproach) {
										m_BossKumaParam.m_HitCnt++;						//当たった数.
									}
								}
							}
						}
						//右手と弾の当たり判定.
						if (Collision(m_pBArmSphere, vArmRBonePos[BONo], m_pShot, m_ShotParam[SNo].m_Pos) == true && m_BossKumaParam.m_SkinChangeCnt[ARM_R] > 0) {
							m_ShotParam[SNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
							if (m_pBHideTex[ARM_R].GetHideTexFlag() == false) {
								m_BossKumaParam.m_SkinChangeCnt[ARM_R]--;//0:右腕.
								m_B_HideParam[ARM_R].m_Pos = vArmRBonePos[1];
								m_B_HideParam[ARM_R].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
								m_pBHideTex[ARM_R].HideTex_Disp(m_B_HideParam[ARM_R].m_Pos);//隠すエフェクトの位置セット.
								if (m_BossKumaParam.m_anim_no == BApproach) {
									m_BossKumaParam.m_HitCnt++;						//当たった数.
								}
							}
						}
						//左手と弾の当たり判定.
						if (Collision(m_pBArmSphere, vArmLBonePos[BONo], m_pShot, m_ShotParam[SNo].m_Pos) == true && m_BossKumaParam.m_SkinChangeCnt[ARM_L] > 0) {
							m_ShotParam[SNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
							if (m_pBHideTex[ARM_L].GetHideTexFlag() == false) {
								m_BossKumaParam.m_SkinChangeCnt[ARM_L]--;//1:左腕.
								m_B_HideParam[ARM_L].m_Pos = vArmLBonePos[1];
								m_B_HideParam[ARM_L].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
								m_pBHideTex[ARM_L].HideTex_Disp(m_B_HideParam[ARM_L].m_Pos);//隠すエフェクトの位置セット.					
								if (m_BossKumaParam.m_anim_no == BApproach) {
									m_BossKumaParam.m_HitCnt++;						//当たった数.
								}
							}
						}
					}
				}
			}

			//水風船とボスくまさんの当たり判定//追加.

			if (m_BomParam.m_ShotFlag == true && m_BossKumaParam.m_BomHitFlag == false) {
				if (m_BossKumaParam.m_Pos.z <= m_pGunParts->GetPosition().z + BOM_AREA) {
					m_BossKumaParam.m_BomHitFlag = true;
				}
			}
			if (m_BomParam.m_BExpFlag == true && m_BossKumaParam.m_BomHitFlag == true) {
				m_B_HideParam[HEAD].m_Pos = vHeadBoneOldPos;
				m_B_HideParam[BODY].m_Pos = vBodyBoneOldPos;
				m_B_HideParam[ARM_R].m_Pos = vArmRBonePos[1];
				m_B_HideParam[ARM_L].m_Pos = vArmLBonePos[1];
				for(int i=0;i< TEX_NO;i++){
					m_BossKumaParam.m_SkinChangeCnt[i]--;
					m_B_HideParam[i].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
					m_pBHideTex[i].HideTex_Disp(m_B_HideParam[i].m_Pos);
				}
				m_BossKumaParam.m_BomHitFlag = false;
				m_BossKumaParam.m_HitBomFlag = true;
			}
			if (m_BomParam.m_BExpFlag == true) {
				if (m_pBomHideTex->GetHideTexFlag() == false)
				{
					m_Bom_HideParam.m_Pos = m_BomParam.m_Pos;
					m_Bom_HideParam.m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
					m_pBomHideTex->HideTex_Disp(m_Bom_HideParam.m_Pos);
					m_BomParam.m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
				}
			}


			//水風船と泥の当たり判定.
			for (int MNo = 0; MNo < MUD_MAX; MNo++) {
				if (m_BomParam.m_BExpFlag == true && m_MudParam[MNo].m_ShotFlag == true) {
					//泥用の隠すテクスチャに泥の座標を入れる.
					m_M_HideParam[MNo].m_Pos = D3DXVECTOR3(m_MudParam[MNo].m_Pos);
					m_M_HideParam[MNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
					m_pHideTex[MNo].HideTex_Disp(m_M_HideParam[MNo].m_Pos);
					//当たったら見えないところへ.
					m_MudParam[MNo].m_ShotFlag = false;
					m_MudParam[MNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
				}
			}


			m_BomParam.m_BExpFlag = false;

			//弾と泥の当たり判定.
			for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
				for (int MNo = 0; MNo < MUD_MAX; MNo++) {
					if (m_ShotParam[SNo].m_ShotFlag == true && m_MudParam[MNo].m_ShotFlag == true) {
						if (Collision(m_pShot, m_ShotParam[SNo].m_Pos, m_pMud, m_MudParam[MNo].m_Pos) == true) {
							//泥用の隠すテクスチャに泥の座標を入れる.
							m_M_HideParam[MNo].m_Pos = D3DXVECTOR3(m_MudParam[MNo].m_Pos);
							m_M_HideParam[MNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
							m_pHideTex[MNo].HideTex_Disp(m_M_HideParam[MNo].m_Pos);
							//当たったら見えないところへ.
							m_MudParam[MNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
							m_MudParam[MNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);

						}
					}
				}
			}

			//泥とカメラの当たり判定.
			for (int MNo = 0; MNo < MUD_MAX; MNo++) {
				//泥の位置がカメラを超えたら.
				if (m_MudParam[MNo].m_Pos.z <= m_pCamera->vPos.z + 1.0f && m_MudParam[MNo].m_ShotFlag == true) {
					m_MudParam[MNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
					m_MudParam[MNo].m_ShotFlag = false;
					m_pPlayer->UpDamage();
					SESoundDrowBlaze(m_pPlayerDamageSE, m_DamageSEflag = true);
				}
			}	


		}

		//用意しているテクスチャ数以上に行かないようにする.
		for (int TNo = 0; TNo < TEX_NO; TNo++) {
			if (m_BossKumaParam.m_SkinChangeCnt[TNo] <= 0) {
				m_BossKumaParam.m_SkinChangeCnt[TNo] = 0;
			}
		}

		//-------------------------------------------
		//現在ぼすくまのテクスチャをセットする.
		//-------------------------------------------
		//頭.
		m_pCDxB_KumaMesh->SetChangeTexttureSystem_Atama(m_BossKumaParam.m_SkinChangeCnt[HEAD]);
		//体.
		m_pCDxB_KumaMesh->SetChangeTexttureSystem_Karada(m_BossKumaParam.m_SkinChangeCnt[BODY]);
		//右腕.
		m_pCDxB_KumaMesh->SetChangeTexttureSystem_UdeR(m_BossKumaParam.m_SkinChangeCnt[ARM_R]);
		//左腕.
		m_pCDxB_KumaMesh->SetChangeTexttureSystem_UdeL(m_BossKumaParam.m_SkinChangeCnt[ARM_L]);

		//===========================================

		//--------------------------------------------------------------------------------
		//	7_19更新(吉田).
		//スクリーン座標からワールド座標へ.
		m_pMouse->CalcScreenToWorld(
			&m_WMousePos,
			static_cast<FLOAT>(m_pMouse->GetMousePosX()),static_cast<FLOAT>(m_pMouse->GetMousePosY()),
			0.0f, &mView, &mProj);
		//--------------------------------------------------------------------------------
		//	7_26更新(吉田).
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//レイでカーソルからオブジェクト（地面とくまさん）との当たり判定をとってあたったらその座標をかえしてくれるよ.
			CalcScreenToXZ(
				&m_HitPos,
			static_cast<FLOAT>(m_pMouse->GetMousePosX() + 8), static_cast<FLOAT>(m_pMouse->GetMousePosY() + 22),
				&mView, &mProj);
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{

			if (WaterBallonDispCnt > 0) {


				//レイでカーソルからオブジェクト（地面とくまさん）との当たり判定をとってあたったらその座標をかえしてくれるよ.
				CalcScreenToXZ(
					&m_HitPos,
					static_cast<FLOAT>(m_pMouse->GetMousePosX() + 8), static_cast<FLOAT>(m_pMouse->GetMousePosY() + 22),
					&mView, &mProj);
			}
		}
		//--------------------------------------------------------------------------------




		//くまさん用の隠すエフェクト表示.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_pHideTex[KNo].GetHideTexFlag() == true) {
				m_KumaParam[KNo].m_TexChangeFlag = true;

				SetDepth(false);//Zﾃｽﾄ:OFF.
				m_pHideTex[KNo].Move();
				m_pHideTex[KNo].Load(mView, mProj, m_pCamera->vPos);
				m_pHideTex[KNo].Attach(m_pHide);
				m_pHideTex[KNo].Render();
				SetDepth(true);	//Zﾃｽﾄ:ON.
			}
			if (m_pHideTex[KNo].GetHideTexFlag() == false) {
				m_KumaParam[KNo].m_TexChangeFlag = false;
			}
		}

		//ボスくまさん用隠すエフェクト表示.
		for (int TNo = 0; TNo < TEX_NO; TNo++) {
			if (m_pBHideTex[TNo].GetHideTexFlag() == true) {
				SetDepth(false);//Zﾃｽﾄ:OFF.
				m_pBHideTex[TNo].Move();
				m_pBHideTex[TNo].Load(mView, mProj, m_pCamera->vPos);
				m_pBHideTex[TNo].Attach(m_pHide);
				m_pBHideTex[TNo].Render();
				SetDepth(true);	//Zﾃｽﾄ:ON.
			}
		}

		//水風船用隠すエフェクト表示
		if (m_pBomHideTex->GetHideTexFlag() == true)
		{
			SetDepth(false);
			m_pBomHideTex->Move();
			m_pBomHideTex->Load(mView, mProj, m_pCamera->vPos);
			m_pBomHideTex->Attach(m_pHide);
			m_pBomHideTex->Render();
			SetDepth(true);
		}
		//自身のダメージUIの表示.
		P_DamageRender();//プレイヤーダメージでレンダリングに置いてある.

		//===========================================================================================
		//		小松が手を加えている.
		//===========================================================================================
		//吉田へ	SetPositionかSetPatternのどちらかを使って頑張って

		//水風船UIの表示,
		for (int i = 0; i < WaterBallonDispCnt; i++) {
			float value_z = 0.16f - (i * 0.01f);
			m_pWaterBalloon[i]->SetPosition(900.0f + (120.0f * i), 550.0f, value_z);
			m_pWaterBalloon[i]->Render();
		}

		//ゲージの表示.
		GaugeRender();		//ゲージでレンダリングに置いてある.


		if (m_enStageScene == enStageScene::enMission)
		{
			//くまさんのミッション.
			m_pBearMission->SetPosition(0.0f, 0.0f, 0.13f);
			m_pBearMission->Render();

			//制限時間.
			for (int i = 0; i < 2; i++) {
				m_TimeCnt[i] = m_pTime->SetDigits();
				m_pTime->PutDigits();
				m_pTimeUI->SetPattern(m_TimeCnt[i], 1);
				m_pTimeUI->SetPosition(1230 - (i * 45.0f), 30.0f, 0.12f);
				m_pTimeUI->Render();
			}
			//ミッションクリアするための数.
			m_pMissionUI->SetPattern(CLEARMISSION, 1);
			m_pMissionUI->SetPosition(350.0f, 120.0f, 0.11f);
			m_pMissionUI->Render();

			//綺麗にした数.
			m_BearCleanDigit = m_BearCleanCnt;
			m_pInGameCleanUI->SetPattern(m_BearCleanDigit, 1);
			m_pInGameCleanUI->SetPosition(200.0f, 120.0f, 0.10f);
			m_pInGameCleanUI->Render();
		}
		if (m_enStageScene == enStageScene::enBoss)
		{
			//ボスくまさんのミッション.
			m_pBossMission->SetPosition(0.0f, 0.0f, 0.10f);
			m_pBossMission->Render();
		}

		break;
	case CMain::enScene::enGameOver:
		//ゲームオーバーロゴ.
		m_pOverUI->SetPosition(WND_W_HALF - (520.0f / 2), 50.0f, 0.40f);
		m_pOverUI->Render();
		//ゲーム文字.
		m_pOverWord->SetPosition(WND_W_HALF - (1112.0f / 2), 300.0f, 0.41f);
		m_pOverWord->Render();
		//ゲームに戻るボタン.
		m_pRetryButton->SetPosition(300.0f, 500.0f, 0.42f);
		m_pRetryButton->Render();
		//タイトルに戻るボタン.
		m_pBackButton->SetPosition(800.0f, 500.0f, 0.43f);
		m_pBackButton->Render();
		break;
	case CMain::enScene::enGameClear:
		//綺麗にした数.
		m_BearCleanDigit = m_BearCleanCnt;
		for (int i = 0; i < 2; i++) {
			m_InsideBearClean[i] = m_BearCleanDigit % 10;
			m_BearCleanDigit /= 10;
			m_pCleanUI->SetPattern(m_InsideBearClean[i], 1);
			m_pCleanUI->SetPosition(700.0f - (i * 55.0f), 210.0f, 0.40f);
			m_pCleanUI->Render();
		}

		//ボスくまさんを綺麗にした数.
		m_BossCleanDigit = m_BossCleanCnt;
		for (int i = 0; i < 2; i++) {
			m_InsideBossClean[i] = m_BossCleanDigit % 10;
			m_BossCleanDigit /= 10;
			m_pBossCleanUI->SetPattern(m_InsideBossClean[i], 1);
			m_pBossCleanUI->SetPosition(700.0f - (i * 55.0f), 290.0f, 0.41f);
			m_pBossCleanUI->Render();
		}
		//綺麗にした合計数.
		m_TotalBearDigit = m_TotalBearCnt;
		for (int i = 0; i < 2; i++) {
			m_InsideTotalBear[i] = m_TotalBearDigit % 10;
			m_TotalBearDigit /= 10;
			m_pTotalBearUI->SetPattern(m_InsideTotalBear[i], 1);
			m_pTotalBearUI->SetPosition(700.0f - (i * 55.0f), 370.0f, 0.42f);
			m_pTotalBearUI->Render();
		}

		//クリアロゴ.
		m_pClearUI->SetPosition(WND_W_HALF - (984.0f/2), WND_H_HALF - (686.0f/2), 0.43f);
		m_pClearUI->Render();
		//タイトルに戻るボタン.
		m_pReturnButton->SetPosition(WND_W - 112.0f,0.0f, 0.44f);
		m_pReturnButton->Render();

		break;
	default:
		break;
	}


	//石の表示.
	for (int i = 0; i < STONE_MAX; i++) {
		m_pStone->SetPosition(m_pStone->GetStonePos(i));
		m_pStone->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}
	//草の表示.
	for (int i = 0; i < GRASS_MAX; i++) {
		m_pGrass->SetPosition(m_pGrass->GetGrassPos(i));
		m_pGrass->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}
	//水鉄砲の表示.
	if (m_GunDispFlag == true) {
		//水鉄砲のパーツを表示.
		m_pGunParts->Render(mView, mProj, m_vLight, m_pCamera->vPos);
		//ポンプの表示.
		m_pPump->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}

	//フェイドの表示.
	if (m_pFade->GetFadeFlag() >= 1) {
		m_pFade->Fade_in();
		m_pFade->Attach(m_pUIFade);
		m_pFade->Render();
	}


	//ﾚﾝﾀﾞﾘﾝｸﾞされたｲﾒｰｼﾞを表示.
	m_pSwapChain->Present(0, 0);
}

/************************************************************
*	ここから3D.
*/
//DirectX初期化.
HRESULT CMain::InitD3D()
{
	//---------------------------------------------------------
	//	ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝ関係.
	//---------------------------------------------------------

	//ｽﾜｯﾌﾟﾁｪｰﾝ構造体.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));//0で初期化.
	sd.BufferCount = 1;	//ﾊﾞｯｸﾊﾞｯﾌｧの数.
	sd.BufferDesc.Width = WND_W;//ﾊﾞｯｸﾊﾞｯﾌｧの幅.
	sd.BufferDesc.Height = WND_H;//ﾊﾞｯｸﾊﾞｯﾌｧの高さ.
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//ﾌｫｰﾏｯﾄ(32ﾋﾞｯﾄｶﾗｰ).
	sd.BufferDesc.RefreshRate.Numerator = 60;//ﾘﾌﾚｯｼｭﾚｰﾄ(分母) ※FPS:60.
	sd.BufferDesc.RefreshRate.Denominator = 1;//ﾘﾌﾚｯｼｭﾚｰﾄ(分子).
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//使い方(表示先).
	sd.OutputWindow = m_hWnd;//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	sd.SampleDesc.Count = 1;//ﾏﾙﾁｻﾝﾌﾟﾙの数.
	sd.SampleDesc.Quality = 0;//ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	sd.Windowed = TRUE;//ｳｨﾝﾄﾞｳﾓｰﾄﾞ(ﾌﾙｽｸﾘｰﾝ時はFALSE).

	//作成を試みる機能ﾚﾍﾞﾙの優先を指定.
	// (GPUがｻﾎﾟｰﾄする機能ｾｯﾄの定義).
	// D3D_FEATURE_LEVEL列挙型の配列.
	// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 の GPUﾚﾍﾞﾙ.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;//配列の要素数.

	//ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝの作成.
	//ﾊｰﾄﾞｳｪｱ(GPU)ﾃﾞﾊﾞｲｽでの作成.
	if (FAILED(
		D3D11CreateDeviceAndSwapChain(
			nullptr,		//ﾋﾞﾃﾞｵｱﾀﾞﾌﾟﾀへのﾎﾟｲﾝﾀ.
			D3D_DRIVER_TYPE_HARDWARE,//作成するﾃﾞﾊﾞｲｽの種類.
			nullptr,		//ｿﾌﾄｳｪｱ ﾗｽﾀﾗｲｻﾞを実装するDLLのﾊﾝﾄﾞﾙ.
			0,				//有効にするﾗﾝﾀｲﾑﾚｲﾔｰ.
			&pFeatureLevels,//作成を試みる機能ﾚﾍﾞﾙの順序を指定する配列へのﾎﾟｲﾝﾀ.
			1,				//↑の要素数.
			D3D11_SDK_VERSION,//SDKのﾊﾞｰｼﾞｮﾝ.
			&sd,			//ｽﾜｯﾌﾟﾁｪｰﾝの初期化ﾊﾟﾗﾒｰﾀのﾎﾟｲﾝﾀ.
			&m_pSwapChain,	//(out)ﾚﾝﾀﾞﾘﾝｸﾞに使用するｽﾜｯﾌﾟﾁｪｰﾝ.
			&m_pDevice11,	//(out)作成されたﾃﾞﾊﾞｲｽ.
			pFeatureLevel,	//機能ﾚﾍﾞﾙの配列にある最初の要素を表すﾎﾟｲﾝﾀ.
			&m_pContext11)))//(out)ﾃﾞﾊﾞｲｽ ｺﾝﾃｷｽﾄ.
	{
		//WARPﾃﾞﾊﾞｲｽの作成.
		// D3D_FEATURE_LEVEL_9_1～D3D_FEATURE_LEVEL_10_1.
		if (FAILED(
			D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
				0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
				&sd, &m_pSwapChain, &m_pDevice11,
				pFeatureLevel, &m_pContext11)))
		{
			//ﾘﾌｧﾚﾝｽﾃﾞﾊﾞｲｽの作成.
			// DirectX SDKがｲﾝｽﾄｰﾙされていないと使えない.
			if (FAILED(
				D3D11CreateDeviceAndSwapChain(
					nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr,
					0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
					&sd, &m_pSwapChain, &m_pDevice11,
					pFeatureLevel, &m_pContext11)))
			{
				MessageBox(nullptr,
					"ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝ作成失敗",
					"Error", MB_OK);
				return E_FAIL;
			}
		}
	}

	//各種ﾃｸｽﾁｬとそれに付帯する各種ﾋﾞｭｰ(画面)を作成.

	//----------------------------------------------------------
	//	ﾊﾞｯｸﾊﾞｯﾌｧ準備：ｶﾗｰﾊﾞｯﾌｧ設定.
	//----------------------------------------------------------
	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬを取得(既にあるので作成ではない).
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:式に関連付けされたGUIDを取得
									//         Texture2Dの唯一の物として扱う
		(LPVOID*)&pBackBuffer_Tex);	//(out)ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬ.

	//そのﾃｸｽﾁｬに対してﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ(RTV)を作成.
	m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV);	//(out)RTV.
	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬを解放.
	SAFE_RELEASE(pBackBuffer_Tex);

	//----------------------------------------------------------
	//	ﾊﾞｯｸﾊﾞｯﾌｧ準備：ﾃﾞﾌﾟｽ(深度)ｽﾃﾝｼﾙ関係.
	//----------------------------------------------------------
	//ﾃﾞﾌﾟｽ(深さor深度)ｽﾃﾝｼﾙﾋﾞｭｰ用のﾃｸｽﾁｬを作成.
	D3D11_TEXTURE2D_DESC	descDepth;
	descDepth.Width = WND_W;	//幅.
	descDepth.Height = WND_H;	//高さ.
	descDepth.MipLevels = 1;	//ﾐｯﾌﾟﾏｯﾌﾟﾚﾍﾞﾙ:1.
	descDepth.ArraySize = 1;	//配列数:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;//32ﾋﾞｯﾄﾌｫｰﾏｯﾄ.
	descDepth.SampleDesc.Count = 1;		//ﾏﾙﾁｻﾝﾌﾟﾙの数.
	descDepth.SampleDesc.Quality = 0;	//ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	descDepth.Usage = D3D11_USAGE_DEFAULT;//使用方法:ﾃﾞﾌｫﾙﾄ.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//深度(ｽﾃﾝｼﾙとして使用).
	descDepth.CPUAccessFlags = 0;	//CPUからはｱｸｾｽしない.
	descDepth.MiscFlags = 0;		//その他の設定なし.

	m_pDevice11->CreateTexture2D(
		&descDepth,
		nullptr,
		&m_pBackBuffer_DSTex);	//(out)ﾃﾞﾌﾟｽｽﾃﾝｼﾙ用ﾃｸｽﾁｬ.

	//そのﾃｸｽﾁｬに対してﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ(DSV)を作成.
	m_pDevice11->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		nullptr,
		&m_pBackBuffer_DSTexDSV);	//(out)DSV.

	//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰとﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰをﾊﾟｲﾌﾟﾗｲﾝにｾｯﾄ.
	m_pContext11->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV);

	//深度ﾃｽﾄ(Zﾃｽﾄ)を有効にする.
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable		= TRUE;//有効.
	dsDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable	= FALSE;
	dsDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	//深度設定作成.
	if (FAILED(	
		m_pDevice11->CreateDepthStencilState(
			&dsDesc, &m_pDepthStencilState)))
	{
		_ASSERT_EXPR(false, L"深度設定作成失敗");
		return E_FAIL;
	}
	//深度ﾃｽﾄ(Z)ﾃｽﾄを無効にする.
	dsDesc.DepthEnable = FALSE;//無効.
	//深度設定作成.
	if (FAILED(
		m_pDevice11->CreateDepthStencilState(
			&dsDesc, &m_pDepthStencilStateOff)))
	{
		_ASSERT_EXPR(false, L"深度設定作成失敗");
		return E_FAIL;
	}

	//------------------------------------------------
	//	ﾋﾞｭｰﾎﾟｰﾄ設定.
	//------------------------------------------------
	D3D11_VIEWPORT vp;
	vp.Width = WND_W;	//幅.
	vp.Height = WND_H;	//高さ.
	vp.MinDepth = 0.0f;	//最小深度(手前).
	vp.MaxDepth = 1.0f;	//最大深度(奥).
	vp.TopLeftX = 0.0f;	//左上位置x.
	vp.TopLeftY = 0.0f;	//左上位置y.

	m_pContext11->RSSetViewports(1, &vp);


	//------------------------------------------------
	//	ﾗｽﾀﾗｲｽﾞ(面の塗りつぶし方)の設定.
	//------------------------------------------------
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;//塗りつぶし(ｿﾘｯﾄﾞ).
	rdc.CullMode = D3D11_CULL_NONE;	//D3D11_CULL_BACK:背面を描画しない.
									//D3D11_CULL_FRONT:正面を描画しない.
									//D3D11_CULL_NONE:ｶﾘﾝｸﾞを切る(正背面を描画する).
	rdc.FrontCounterClockwise = FALSE;//ﾎﾟﾘｺﾞﾝの表裏を決定するﾌﾗｸﾞ.
									//TRUE:左回りなら前向き。右回りなら後ろ向き。
									//FALSE:逆になる.
	rdc.DepthClipEnable = FALSE;	//距離についてのｸﾘｯﾋﾟﾝｸﾞ有効.

	ID3D11RasterizerState* pRs = nullptr;
	m_pDevice11->CreateRasterizerState(&rdc, &pRs);
	m_pContext11->RSSetState(pRs);
	SAFE_RELEASE(pRs);




	return S_OK;
}

//Direct3D終了処理.
void CMain::DestroyD3D()
{
	m_pSwishSE->Close();
	SAFE_DELETE(m_pSwishSE);
	m_pPumpSE->Close();
	SAFE_DELETE(m_pPumpSE);
	m_pBearDamageSE->Close();
	SAFE_DELETE(m_pBearDamageSE);
	m_pPlayerDamageSE->Close();
	SAFE_DELETE(m_pPlayerDamageSE);
	m_pPlayerMoveSE->Close();
	SAFE_DELETE(m_pPlayerMoveSE);
	m_WaterBalloonSE->Close();
	SAFE_DELETE(m_WaterBalloonSE);
	m_WaterShotSE->Close();
	SAFE_DELETE(m_WaterShotSE);
	m_pCircleSE->Close();
	SAFE_DELETE(m_pCircleSE);
	m_pClickSE->Close();
	SAFE_DELETE(m_pClickSE);

	m_pGameClearBGM->Close();
	SAFE_DELETE(m_pGameClearBGM);
	m_pGameOverBGM->Close();
	SAFE_DELETE(m_pGameOverBGM);
	m_pBossBGM->Close();
	SAFE_DELETE(m_pBossBGM);
	m_pMainBGM->Close();
	SAFE_DELETE(m_pMainBGM);
	m_TitleBGM->Close();
	SAFE_DELETE(m_TitleBGM);

	//メッシュの解放.
	SAFE_DELETE(m_pBomArea);
	//UIの解放.
	SAFE_DELETE(m_pTimeUI);
	SAFE_DELETE(m_pTotalBearUI);
	SAFE_DELETE(m_pBossCleanUI);
	SAFE_DELETE(m_pCleanUI);
	SAFE_DELETE(m_pMissionUI);
	SAFE_DELETE(m_pInGameCleanUI);
	SAFE_DELETE(m_pBossMission);
	SAFE_DELETE(m_pBearMission);
	for (int i = WATER_BALLOON-1; i >= 0; i--) 
	{
		SAFE_DELETE(m_pWaterBalloon[i]);

	}
	SAFE_DELETE(m_pSpaceLogo);
	SAFE_DELETE(m_pSpaceBar);
	SAFE_DELETE(m_pBossBearIcon);
	SAFE_DELETE(m_pGaugeBar);
	SAFE_DELETE(m_pDecremetGauge);
	SAFE_DELETE(m_pMudDamage_3);
	SAFE_DELETE(m_pMudDamage_2);
	SAFE_DELETE(m_pMudDamage_1);
	SAFE_DELETE(m_pTime);
	SAFE_DELETE(m_pPump);
	SAFE_DELETE(m_pGunParts);
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE_ARRAY(m_pGrass);
	SAFE_DELETE_ARRAY(m_pStone);
	SAFE_DELETE_ARRAY(m_pGrassGround);
	SAFE_DELETE_ARRAY(m_pGround);
	SAFE_DELETE(m_pCursor);
	SAFE_DELETE(m_pMouse);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pPlayer);

	SAFE_DELETE_ARRAY(m_pBChangeTextures);
	SAFE_DELETE(m_pReturnButton);
	SAFE_DELETE(m_pClearUI);
	SAFE_DELETE(m_pBackButton);
	SAFE_DELETE(m_pRetryButton);
	SAFE_DELETE(m_pOverWord);
	SAFE_DELETE(m_pOverUI);
	SAFE_DELETE(m_pEndUI);
	SAFE_DELETE(m_pStartUI);
	SAFE_DELETE(m_pTitle);
	SAFE_DELETE(m_pArea);
	SAFE_DELETE(m_pUIFade);
	SAFE_DELETE(m_pFade);
	
	SAFE_DELETE(m_pBomHideTex);
	SAFE_DELETE_ARRAY(m_pBHideTex);
	SAFE_DELETE_ARRAY(m_pHideTex);
	SAFE_DELETE(m_pHide);
	SAFE_DELETE(m_pMud);
	SAFE_DELETE(m_pBom);
	SAFE_DELETE(m_pShot);
	SAFE_DELETE(m_pEnemy);

	//スフィアの解放.
	SAFE_DELETE(m_pBArmSphere);
	SAFE_DELETE(m_pBNeckSphere);
	SAFE_DELETE(m_pBBodySphere);
	SAFE_DELETE(m_pBHeadSphere);
	SAFE_DELETE(m_pSphere);

	//スキンメッシュの解放.
	SAFE_DELETE(m_pB_KumaSkin);
	SAFE_DELETE(m_pCDxB_KumaMesh);
	SAFE_DELETE_ARRAY(m_pKumaSkin);
	SAFE_DELETE(m_pCDxKumaMesh);
		

	

	//ﾓﾃﾞﾙの解放.
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexBuffer);

	//ｼｪｰﾀﾞを解放.
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);

	//Direct3Dｵﾌﾞｼﾞｪｸﾄを解放.
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pContext11);
	SAFE_RELEASE(m_pDevice11);
}

//ﾒｯｼｭの読み込み関数.
HRESULT CMain::LoadMesh()
{	
	//確認用にｽﾌｨｱを読み込む.
	m_pSphere = new CDX9Mesh();
	m_pSphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x",true);
	InitSphere(m_pSphere);	//ｽﾌｨｱのﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ作成.

	//-------------------------------------------
	//ボスくまさんテクスチャ切り替え用
	//-------------------------------------------
	//確認用にｽﾌｨｱを読み込む.
	//腕.
	m_pBArmSphere = new CDX9Mesh();
	m_pBArmSphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x",true);
	InitSphere(m_pBArmSphere);	//ｽﾌｨｱのﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ作成.
	m_pBArmSphere->m_Sphere.fRadius = 0.15f;	//当たり判定の半径.
	
	//確認用にｽﾌｨｱを読み込む.
	//頭.
	m_pBHeadSphere = new CDX9Mesh();
	m_pBHeadSphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x", true);
	InitSphere(m_pBHeadSphere);	//ｽﾌｨｱのﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ作成.
	m_pBHeadSphere->m_Sphere.fRadius = 0.3f; //当たり判定の半径.//半径なのでScaleの値の大きさにしたい場合２分の1しないといけない.
	
	

	//確認用にｽﾌｨｱを読み込む.
	//お腹.
	m_pBBodySphere = new CDX9Mesh();
	m_pBBodySphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x", true);
	InitSphere(m_pBBodySphere);	//ｽﾌｨｱのﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ作成.
	m_pBBodySphere->m_Sphere.fRadius = 0.3f; //当たり判定の半径.

	//首.
	m_pBNeckSphere = new CDX9Mesh();
	m_pBNeckSphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x", true);
	InitSphere(m_pBNeckSphere);	//ｽﾌｨｱのﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ作成.
	m_pBNeckSphere->m_Sphere.fRadius = 0.20f; //当たり判定の半径.

	//--------------------------------------
	//	ﾒｯｼｭ読み込み.
	//--------------------------------------
	//「敵機」.
	m_pEnemy = new CDX9Mesh();
	m_pEnemy->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Grass\\kusa.x", true);
	//「弾」.
	m_pShot = new CDX9Mesh();
	m_pShot->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Bullet\\mizu.x", true);
	//「泥」.
	m_pMud = new CDX9Mesh();
	m_pMud->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Bullet\\doro.x", true);
	//----------------------------------
	// 吉田更新 2019_10_17.

	//「水風船」.
	m_pBom = new CDX9Mesh();
	m_pBom->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\WaterBom\\mizuhuusen.x", true);

	//基準となるﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱの情報を各ﾓﾃﾞﾙにｾｯﾄする.
	m_pEnemy->m_Sphere = m_pSphere->m_Sphere;
	m_pEnemy->m_Sphere.fRadius = 1.0f;	
	m_pMud->m_Sphere = m_pSphere->m_Sphere;
	//弾は別ｻｲｽﾞを使用する.
	InitSphere(m_pShot);
	m_pShot->m_Sphere.fRadius = 0.15f;

	////ﾒｯｼｭ毎にﾊﾞｳﾝﾃﾞｨﾝｸﾞﾎﾞｯｸｽの作成.
	//InitBBox(m_pPlayer);
	//InitBBox(m_pEnemy);

	m_pHideTex = new CHideTex[BEAR_MAX]();

	
	m_pBHideTex = new CHideTex[TEX_NO]();

	m_pBomHideTex = new CHideTex();
	//-------------------------------------------
	//吉田.
	//ｽﾌﾟﾗｲﾄUIの初期化.
	//隠すエフェクトの初期化.
	m_pHide = new CSprite();
	//元画像大きさ、　一コマ当たりの大きさ、スケール　　　　
	Sprite(900.0f, 600.0f, 900.0f / 3, 600.0f / 2, 4.0f, 4.0f, "Data\\Texture\\mizu.png", m_pHide);
	
	//-----------------------------------------

	//----------------------------
	// 吉田　更新 2019_10_17.

	//-----------------------------------------------------
	//	くまさん読み込み.
	//-----------------------------------------------------
	CD3DXSKINMESH_INIT si;
	si.hWnd = m_hWnd;
	si.pDevice = m_pDevice11;
	si.pDeviceContext = m_pContext11;
	
	
	m_pCDxKumaMesh = new CDX9SkinMesh();

	m_pCDxKumaMesh->Init(&si, "Data\\ModelSkin\\Kuma\\Kuma.X");
	
	

	//m_pKumaSkin = new CCharacter[BEAR_MAX]();


	//for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
	//	m_pKumaSkin[KNo].SetScale(0.02f);
	//	m_pKumaSkin[KNo].SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//	m_pKumaSkin[KNo].AttachModel(m_pCDxKumaMesh);
	//	m_pKumaSkin[KNo].SetAnimSpeed(0.02);//アニメーション速度設定.
	//}
		
	//---------------------------------------------------
	//	くまさん用テクスチャ切り替え部分.
	//---------------------------------------------------
	
	m_pChangeTextures = std::make_shared<CDX9SkinMesh::CHANGE_TEXTURES>();
	
	strcpy_s(m_pChangeTextures->szTextureName_2, "Data\\ModelSkin\\Kuma\\kumasan_b_3.png");		//綺麗.
	strcpy_s(m_pChangeTextures->szTextureName, "Data\\ModelSkin\\Kuma\\kumasan_b_2.png");	//少し汚い.
	strcpy_s(m_pChangeTextures->szTargetTextureName, "kumasan_b_1.png");		//汚い(デフォルト).			
	//テクスチャ作成.
	if (m_pChangeTextures->szTargetTextureName != 0 &&
		FAILED(
			D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11, m_pChangeTextures->szTextureName,
				nullptr, nullptr, &m_pChangeTextures->pTexture, nullptr)))
	{
		MessageBox(NULL, "テクスチャ読み込み失敗",
			"Error", MB_OK);
		return E_FAIL;
	}
	//テクスチャ作成2.
	if (m_pChangeTextures->szTargetTextureName != 0 &&
		FAILED(
			D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11, m_pChangeTextures->szTextureName_2,
				nullptr, nullptr, &m_pChangeTextures->pTextureTest, nullptr)))
	{
		MessageBox(NULL, "テクスチャ読み込み失敗",
			"Error", MB_OK);
		return E_FAIL;
	}

	m_pCDxKumaMesh->SetChangeTextures(m_pChangeTextures);

	
	


	//---------------------------------------
	//	ボスくまさん読み込み.
	//---------------------------------------
	//----------------------------------
	// 吉田更新 2019_10_17.
	m_pCDxB_KumaMesh = new CDX9SkinMesh();
	m_pCDxB_KumaMesh->Init(&si, "Data\\ModelSkin\\BossKuma\\BossKuma.X");

	//m_pCDxSkinMesh->m_vPos.y = 0.01f;//表示位置を地面より上にしておく.

	//m_pB_KumaSkin = new CCharacter();
	//m_pB_KumaSkin->SetScale(0.05f);
	//m_pB_KumaSkin->AttachModel(m_pCDxB_KumaMesh);
	//m_pB_KumaSkin->SetAnimSpeed(0.01);//アニメーション速度設定.

				


	m_pBChangeTextures = new CDX9SkinMesh::CHANGE_TEXTURES[TEX_NO];
	

	//頭.
	strcpy_s(m_pBChangeTextures[HEAD].szTextureName_2, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_atama_1.png");		//綺麗.
	strcpy_s(m_pBChangeTextures[HEAD].szTextureName, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_atama_2.png");	//少し汚い.
	strcpy_s(m_pBChangeTextures[HEAD].szTargetTextureName, "bosskuma_uv_atama_3.png");		//汚い(デフォルト).			
	
	//体.
	strcpy_s(m_pBChangeTextures[BODY].szTextureName_2, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_karada_1.png");		//綺麗.
	strcpy_s(m_pBChangeTextures[BODY].szTextureName, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_karada_2.png");	//少し汚い.
	strcpy_s(m_pBChangeTextures[BODY].szTargetTextureName, "bosskuma_uv_karada_3.png");		//汚い(デフォルト).			
	
	//右腕.
	strcpy_s(m_pBChangeTextures[ARM_R].szTextureName_2, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_asi_1.png");		//綺麗.
	strcpy_s(m_pBChangeTextures[ARM_R].szTextureName, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_asi_2.png");	//少し汚い.
	strcpy_s(m_pBChangeTextures[ARM_R].szTargetTextureName, "bosskuma_uv_asi_3.png");		//汚い(デフォルト).			
	
	//左腕.
	strcpy_s(m_pBChangeTextures[ARM_L].szTextureName_2,	 "Data\\ModelSkin\\BossKuma\\bosskuma_uv_asi_1.png");		//綺麗.
	strcpy_s(m_pBChangeTextures[ARM_L].szTextureName,		 "Data\\ModelSkin\\BossKuma\\bosskuma_uv_asi_2.png");	//少し汚い.
	strcpy_s(m_pBChangeTextures[ARM_L].szTargetTextureName, "bosskuma_uv_asi_3.png");		//汚い(デフォルト).		


	for (int BNo = 0; BNo < TEX_NO; BNo++) {
		//テクスチャ作成.
		if (m_pBChangeTextures[BNo].szTargetTextureName != 0 &&
			FAILED(
				D3DX11CreateShaderResourceViewFromFileA(
					m_pDevice11, m_pBChangeTextures[BNo].szTextureName,
					nullptr, nullptr, &m_pBChangeTextures[BNo].pTexture, nullptr)))
		{
			MessageBox(NULL, "テクスチャ読み込み失敗",
				"Error", MB_OK);
			return E_FAIL;
		}
		//テクスチャ作成2.
		if (m_pBChangeTextures[BNo].szTargetTextureName != 0 &&
			FAILED(
				D3DX11CreateShaderResourceViewFromFileA(
					m_pDevice11, m_pBChangeTextures[BNo].szTextureName_2,
					nullptr, nullptr, &m_pBChangeTextures[BNo].pTextureTest, nullptr)))
		{
			MessageBox(NULL, "テクスチャ読み込み失敗",
				"Error", MB_OK);
			return E_FAIL;
		}
	}
	m_pCDxB_KumaMesh->SetBChangeTextures(&m_pBChangeTextures[HEAD]);
	m_pCDxB_KumaMesh->SetBChangeTextures1(&m_pBChangeTextures[BODY]);
	m_pCDxB_KumaMesh->SetBChangeTextures2(&m_pBChangeTextures[ARM_L]);
	m_pCDxB_KumaMesh->SetBChangeTextures3(&m_pBChangeTextures[ARM_R]);


	
	//--------------------------------------------------

	
	//各ｵﾌﾞｼﾞｪｸﾄﾊﾟﾗﾒｰﾀの初期化.

	m_pPlayer = new CPlayer();
	m_pPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.9f, 4.0f));
	m_pPlayer->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//「カメラ」.
	m_pCamera = new CCamera();
	//「マウス」.
	m_pMouse = new CMouse();
	//「フェイド」.
	m_pFade = new CFade();
	//「時間」.
	m_pTime = new CTime();
	//「範囲」.
	m_pArea = new CArea();
	

	//	m_PlayerParam.m_ShotFlag = false;
	//for (int KNo = 0; KNo > ENMY_MAX; KNo++) {
	//	m_KumaParam[KNo].m_Pos = D3DXVECTOR3(1.0f*KNo, 0.7f, 20.0f);
	//}
	
	//地面(メッシュオブジェクトクラス).
	m_pGround = new CGround[GROUND_MAX]();
	m_pGround->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);

	//草の地面.
	m_pGrassGround = new CGrassGround[GRASS_GROUND_MAX]();
	m_pGrassGround->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);

	//石.
	m_pStone = new CStone[STONE_MAX]();
	m_pStone->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);

	//草.
	m_pGrass = new CGrass[GRASS_MAX]();
	m_pGrass->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);

	//背景(メッシュオブジェクトクラス).
	m_pBackground = new CBackground();
	m_pBackground->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);
	
	//水鉄砲のパーツ.
	m_pGunParts = new CGunParts();
	m_pGunParts->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);
	
	//水鉄砲のポンプ.
	m_pPump = new CPump();
	m_pPump->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);
	//「範囲」.
	m_pBomArea = new CDX9Mesh();
	m_pBomArea->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Way\\mizutamri.x", true);

	FileDataBear();
	
	return S_OK;
}

//UIの読み込み関数.
HRESULT CMain::LoadUI()
{
	//減少ゲージUI.
	m_pDecremetGauge = new CSpriteUI();
	UI_Display(368.0f, 88.0f, 368.0f, 88.0f, 368.0f, 88.0f, "Data\\Texture\\PlayerGauge\\ge-zi.png", m_pDecremetGauge);
	//ゲージバーUI.
	m_pGaugeBar = new CSpriteUI();
	UI_Display(398.0f, 88.0f, 398.0f, 88.0f, 398.0f, 88.0f, "Data\\Texture\\PlayerGauge\\waku.png", m_pGaugeBar);
	//くまさんのアイコン.
	m_pBossBearIcon = new CSpriteUI();
	UI_Display(142.0f, 238.0f, 142.0f, 238.0f, 142.0f, 238.0f, "Data\\Texture\\PlayerGauge\\ge-zi_bosskuma.png", m_pBossBearIcon);
	
	//スペースキーUI.
	m_pSpaceBar = new CSpriteUI();
	UI_Display(380.0f, 96.0f, 380.0f, 96.0f, 380.0f, 96.0f, "Data\\Texture\\PlayerGauge\\supe-su_2.png", m_pSpaceBar);
	//スペースキーロゴ.
	m_pSpaceLogo = new CSpriteUI();
	UI_Display(62.0f, 70.0f, 62.0f, 70.0f, 62.0f, 70.0f, "Data\\Texture\\PlayerGauge\\riro-do.png", m_pSpaceLogo);

	//水風船UIの初期化.
	for (int i = 0; i < WATER_BALLOON; i++) {
		m_pWaterBalloon[i] = new CSpriteUI();
		UI_Display(118.0f, 132.0f, 118.0f, 132.0f, 118.0f, 132.0f, "Data\\Texture\\WaterBalloon.png", m_pWaterBalloon[i]);
	}
	//ミッション(くまさん用).
	m_pBearMission = new CSpriteUI();
	UI_Display(585.0f, 180.0f, 585.0f, 180.0f, 585.0f, 180.0f, "Data\\Texture\\TextMisiion\\kumasanwokireinisite.png", m_pBearMission);
	//ミッション(ボスくまさん用).
	m_pBossMission = new CSpriteUI();
	UI_Display(585.0f, 180.0f, 585.0f, 180.0f, 585.0f, 180.0f, "Data\\Texture\\TextMisiion\\bosskumasanwokireinisite.png", m_pBossMission);
	//綺麗になった数(ゲーム中のUI).
	m_pInGameCleanUI = new CSpriteUI();
	UI_Display(378.0f, 40.0f, 378.0f / 10, 40.0f, 378.0f / 10, 40.0f, "Data\\Texture\\DebugText\\suuzi_kumasanyou.png", m_pInGameCleanUI);
	//ミッションクリアするための数.
	m_pMissionUI = new CSpriteUI();
	UI_Display(378.0f, 40.0f, 378.0f / 10, 40.0f, 378.0f / 10, 40.0f, "Data\\Texture\\DebugText\\suuzi_kumasanyou.png", m_pMissionUI);
	//綺麗になった数UI.
	m_pCleanUI = new CSpriteUI();
	UI_Display(520.0f, 55.0f, 520.0f / 10, 55.0f, 520.0f / 10, 55.0f, "Data\\Texture\\GameClear\\suuzi_taimu_ge-mukuriayou.png", m_pCleanUI);
	//綺麗になった数UI.
	m_pBossCleanUI = new CSpriteUI();
	UI_Display(520.0f, 55.0f, 520.0f / 10, 55.0f, 520.0f / 10, 55.0f, "Data\\Texture\\GameClear\\suuzi_taimu_ge-mukuriayou.png", m_pBossCleanUI);
	//綺麗になった数UI.
	m_pTotalBearUI = new CSpriteUI();
	UI_Display(520.0f, 55.0f, 520.0f / 10, 55.0f, 520.0f / 10, 55.0f, "Data\\Texture\\GameClear\\suuzi_taimu_ge-mukuriayou.png", m_pTotalBearUI);
	//綺麗になった数UI.
	m_pTimeUI = new CSpriteUI();
	UI_Display(378.0f, 40.0f, 378.0f / 10, 40.0f, 378.0f / 10, 40.0f, "Data\\Texture\\DebugText\\suuzi_kumasanyou.png", m_pTimeUI);

	//======================自身の汚れ状況=================================.
	//泥の汚れパターン1の初期化.
	m_pMudDamage_1 = new CSpriteUI();
	UI_Display(3508.0f, 2480.0f, 3508.0f, 2480.0f, 1280.0f, 720.0f, "Data\\Texture\\PlayerDamage\\MudDamage_1.png", m_pMudDamage_1);
	//泥の汚れパターン2の初期化.
	m_pMudDamage_2 = new CSpriteUI();
	UI_Display(3508.0f, 2480.0f, 3508.0f, 2480.0f, 1280.0f, 720.0f, "Data\\Texture\\PlayerDamage\\MudDamage_2.png", m_pMudDamage_2);
	//泥の汚れパターン3の初期化.
	m_pMudDamage_3 = new CSpriteUI();
	UI_Display(3508.0f, 2480.0f, 3508.0f, 2480.0f, 1280.0f, 720.0f, "Data\\Texture\\PlayerDamage\\MudDamage_3.png", m_pMudDamage_3);

	//===================================タイトル画面で使う↓================================================
	//タイトルUI(ロゴ).
	m_pTitle = new CSpriteUI();
	UI_Display(534.0f, 428.0f, 534.0f, 428.0f, 534.0f, 428.0f, "Data\\Texture\\Title\\taitoru.png", m_pTitle);
	//タイトルを進める時クリックする所.
	m_pStartUI = new CSpriteUI();
	UI_Display(185.0f, 185.0f, 185.0f, 185.0f, 185.0f, 185.0f, "Data\\Texture\\Title\\suta-tobatan.png", m_pStartUI);
	//タイトル画面でゲームをやめる時クリックする所.
	m_pEndUI = new CSpriteUI();
	UI_Display(110.0f, 110.0f, 110.0f, 110.0f, 110.0f, 110.0f, "Data\\Texture\\Title\\batubotan.png", m_pEndUI);

	//===================================クリア画面で使う↓=============================================.
	//掲示板.
	m_pClearUI = new CSpriteUI();
	UI_Display(984.0f, 686.0f, 984.0f, 686.0f, 984.0f, 686.0f, "Data\\Texture\\GameClear\\ge-mukuria.png", m_pClearUI);
	//戻るボタン.
	m_pReturnButton = new CSpriteUI();
	UI_Display(112.0f, 112.0f, 112.0f, 112.0f, 112.0f, 112.0f, "Data\\Texture\\GameClear\\yazirusi.png", m_pReturnButton);

	//===================================ゲームオーバー画面で使う↓=============================================.
	//ゲームオーバーロゴ.
	m_pOverUI = new CSpriteUI();
	UI_Display(520.0f, 228.0f, 520.0f, 228.0f, 520.0f, 228.0f, "Data\\Texture\\GameOver\\doronkokumasan.png", m_pOverUI);
	//ゲームオーバー文字.
	m_pOverWord = new CSpriteUI();
	UI_Display(1112.0f, 122.0f, 1112.0f, 122.0f, 1112.0f, 122.0f, "Data\\Texture\\GameOver\\doronkoninattyatta-.png", m_pOverWord);
	//ゲームに戻るボタン.
	m_pRetryButton = new CSpriteUI();
	UI_Display(205.0f, 205.0f, 205.0f, 205.0f, 205.0f, 205.0f, "Data\\Texture\\GameOver\\ritorai.png", m_pRetryButton);
	//タイトルに戻るボタン.
	m_pBackButton = new CSpriteUI();
	UI_Display(205.0f, 205.0f, 205.0f, 205.0f, 205.0f, 205.0f, "Data\\Texture\\GameOver\\batubotan_ge-muo-ba-.png", m_pBackButton);
	//===============================================================================================================================
	//----------------------------
	// 吉田
	//カーソル用.
	m_pCursor = new CSpriteUI();
	UI_Display(CURSOR_SIZE, CURSOR_SIZE, CURSOR_SIZE, CURSOR_SIZE, CURSOR_SIZE, CURSOR_SIZE,"Data\\Texture\\Cursor.png",m_pCursor);

	//フェイド用.
	m_pUIFade = new CSpriteUI();
	UI_Display(1.0f, 1.0f,1.0f, 1.0f,WND_W, WND_H,"Data\\Texture\\Fade.png",m_pUIFade);

	return S_OK;
}

//サウンドの読み込み関数.
HRESULT CMain::LoadSound()
{
	//タイトルサウンド(BGM).
	m_TitleBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM");
	//音声ﾌｧｲﾙを開く.
	if (!m_TitleBGM->Open("Data\\Sound\\BGM\\Title.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//メインサウンド(BGM).
	m_pMainBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM2");
	//音声ﾌｧｲﾙを開く.
	if (!m_pMainBGM->Open("Data\\Sound\\BGM\\GameMain.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//ボスサウンド(BGM).
	m_pBossBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM3");
	//音声ﾌｧｲﾙを開く.
	if (!m_pBossBGM->Open("Data\\Sound\\BGM\\BossBGM.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//ゲームオーバーサウンド(BGM).
	m_pGameOverBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM4");
	//音声ﾌｧｲﾙを開く.
	if (!m_pGameOverBGM->Open("Data\\Sound\\BGM\\GameOver.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//ゲームクリアサウンド(BGM).
	m_pGameClearBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM5");
	//音声ﾌｧｲﾙを開く.
	if (!m_pGameClearBGM->Open("Data\\Sound\\BGM\\GameClear.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}

	//クリックのSE.
	m_pClickSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_1");
	//音声ﾌｧｲﾙを開く.
	if (!m_pClickSE->Open("Data\\Sound\\SE\\Click.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//円のSE.
	m_pCircleSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_2");
	//音声ﾌｧｲﾙを開く.
	if (!m_pCircleSE->Open("Data\\Sound\\SE\\Circle.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//水の発射ｻｳﾝﾄﾞ(SE).
	m_WaterShotSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_3");
	//音声ﾌｧｲﾙを開く.
	if (!m_WaterShotSE->Open("Data\\Sound\\SE\\shot.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}

	//水風船の発射ｻｳﾝﾄﾞ(SE).
	m_WaterBalloonSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_4");
	//音声ﾌｧｲﾙを開く.
	if (!m_WaterBalloonSE->Open("Data\\Sound\\SE\\WaterBalloon.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//プレイヤーの移動SE.
	m_pPlayerMoveSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_5");
	//音声ﾌｧｲﾙを開く.
	if (!m_pPlayerMoveSE->Open("Data\\Sound\\SE\\PlayerMove.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//プレイヤーのダメージSE.
	m_pPlayerDamageSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_6");
	//音声ﾌｧｲﾙを開く.
	if (!m_pPlayerDamageSE->Open("Data\\Sound\\SE\\PlayerDamage.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//くまさんのダメージSE.
	m_pBearDamageSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_7");
	//音声ﾌｧｲﾙを開く.
	if (!m_pBearDamageSE->Open("Data\\Sound\\SE\\BearDamage.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}

	//くまさんのダメージSE.
	m_pPumpSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_8");
	//音声ﾌｧｲﾙを開く.
	if (!m_pPumpSE->Open("Data\\Sound\\SE\\Pump.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}

	//くまさんのダメージSE.
	m_pSwishSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_9");
	//音声ﾌｧｲﾙを開く.
	if (!m_pSwishSE->Open("Data\\Sound\\SE\\Swish.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}



	return S_OK;
}



//===========================================================
//	HLSLﾌｧｲﾙを読み込みｼｪｰﾀﾞを作成する.
//	HLSL: High Level Shading Language の略.
//===========================================================
HRESULT CMain::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			"Mesh.hlsl",	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ).
			nullptr,		//ﾏｸﾛ定義の配列へのﾎﾟｲﾝﾀ(未使用).
			nullptr,		//ｲﾝｸﾙｰﾄﾞﾌｧｲﾙを扱うｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			"VS_Main",		//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"vs_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ).
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,				//ｴﾌｪｸﾄｺﾝﾊﾟｲﾙﾌﾗｸﾞ(未使用).
			nullptr,		//ｽﾚｯﾄﾞ ﾎﾟﾝﾌﾟ ｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,		//ｴﾗｰと警告一覧を格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			nullptr)))		//戻り値へのﾎﾟｲﾝﾀ(未使用).
	{
		_ASSERT_EXPR(false, L"hlsl読み込み失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ.
	{
		_ASSERT_EXPR(false, L"ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						//位置.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのﾌｫｰﾏｯﾄ(32bit float型*3).
			0,
			0,								//ﾃﾞｰﾀの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL",						//法線.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGIのﾌｫｰﾏｯﾄ(32bit float型*3).
			0,
			12,								//ﾃﾞｰﾀの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",						//ﾃｸｽﾁｬ位置.
			0,
			DXGI_FORMAT_R32G32_FLOAT,		//DXGIのﾌｫｰﾏｯﾄ(32bit float型*2).
			0,
			24,								//ﾃﾞｰﾀの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ.
	{
		_ASSERT_EXPR(false, L"頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			"Mesh.hlsl",	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ).
			nullptr,		//ﾏｸﾛ定義の配列へのﾎﾟｲﾝﾀ(未使用).
			nullptr,		//ｲﾝｸﾙｰﾄﾞﾌｧｲﾙを扱うｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			"PS_Main",		//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"ps_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ).
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,				//ｴﾌｪｸﾄｺﾝﾊﾟｲﾙﾌﾗｸﾞ(未使用).
			nullptr,		//ｽﾚｯﾄﾞ ﾎﾟﾝﾌﾟ ｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用).
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,		//ｴﾗｰと警告一覧を格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			nullptr)))		//戻り値へのﾎﾟｲﾝﾀ(未使用).
	{
		_ASSERT_EXPR(false, L"hlsl読み込み失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾋﾟｸｾﾙｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	{
		_ASSERT_EXPR(false, L"ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//ｺﾝｽﾀﾝﾄ(定数)ﾊﾞｯﾌｧ作成.
	//ｼｪｰﾀﾞに特定の数値を送るﾊﾞｯﾌｧ.
	//ここでは変換行列渡し用.
	//ｼｪｰﾀﾞに World, View, Projection 行列を渡す.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;	//その他のﾌﾗｸﾞ(未使用).
	cb.StructureByteStride = 0;	//構造体のｻｲｽﾞ(未使用).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//使用方法:直接書き込み.

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer)))
	{
		_ASSERT_EXPR(false, L"ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗");
		return E_FAIL;
	}


	return S_OK;
}

//深度(Z)ﾃｽﾄON/OFF切り替え.
void CMain::SetDepth(bool flag)
{
	ID3D11DepthStencilState* pTmp
		= (flag == true) ? m_pDepthStencilState : m_pDepthStencilStateOff;

	//深度設定をｾｯﾄ.
	m_pContext11->OMSetDepthStencilState(pTmp, 1);
}

//ｽﾌｨｱ作成.
HRESULT CMain::InitSphere(CDX9Mesh* pMesh)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;//頂点ﾊﾞｯﾌｧ.
	void* pVertices = nullptr;	//頂点.
	D3DXVECTOR3 vCenter;		//中心.
	float fRadius;				//半径.

	//頂点ﾊﾞｯﾌｧを取得.
	if (FAILED(
		pMesh->GetMesh()->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}
	//ﾒｯｼｭの頂点ﾊﾞｯﾌｧをﾛｯｸする.
	if (FAILED(
		pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}
	//ﾒｯｼｭの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>(pVertices),
		pMesh->GetMesh()->GetNumVertices(),	//頂点の数.
		D3DXGetFVFVertexSize(pMesh->GetMesh()->GetFVF()),//頂点の情報.
		&vCenter,	//(out)中心座標.
		&fRadius);	//(out)半径.

	//ｱﾝﾛｯｸ.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//中心と半径を構造体に設定.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.fRadius = fRadius;

	return S_OK;
}

//-----------------------------------------------
//	吉田変更.
//衝突判定(ﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ).
bool CMain::Collision(
	CDX9Mesh* pAttack, D3DXVECTOR3 AtkPos,	//攻撃側.
	CDX9Mesh* pTarget, D3DXVECTOR3 TgtPos)	//標的側.
{
	//２つの物体の中心間(２点間)の距離を求める.
	D3DXVECTOR3 vLength
		= TgtPos - AtkPos;
	//長さに変換する.
	float Length = D3DXVec3Length(&vLength);

	//２点間の距離が、それぞれの物体の半径を足したものより.
	//小さいということは、ｽﾌｨｱ同士が重なっている(衝突している)ということ.
	if (Length <=
		pAttack->m_Sphere.fRadius + pTarget->m_Sphere.fRadius)
	{
		return true;	//衝突.
	}
	return false;	//衝突していない.
}

// オブジェクト(地面とくま)とスクリーン座標の交点算出関数.
D3DXVECTOR3 * CMain::CalcScreenToXZ(D3DXVECTOR3 * Pos, float Sx, float Sy, D3DXMATRIX * View, D3DXMATRIX * Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	D3DXVECTOR3 center;
	m_pMouse->CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, View, Prj);
	m_pMouse->CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, View, Prj);
	//if(farpos とnearposの間にオブジェクトがあるなら).
	//ray = オブジェクトpos - nearpos;
	//center = pTarget->GetPosition();
	//pTarget->m_Sphere.fRadius;
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// 床との交差が起きている場合は交点を.
	// 起きていない場合は遠くの壁との交点を出力.
	//ベクトルをとってnearposとfarposを角度を渡すとその方向に水飛ばせる.
	D3DXVECTOR3 HitPos;
	//------------------------------------------------------------------------
	//	7_26 更新　吉田.
	for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
		if (RaySphere(&HitPos, &nearpos, &ray, &m_KumaParam[KNo].m_Pos) == true)
		{
			*Pos = HitPos;
			return Pos;
		}
		else {
			if (ray.y <= 0) {
				// 床交点
				float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
				float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
				*Pos = nearpos + (LP0 / Lray) *ray;//(LP0 / Lray)向き.
			}
			else {
				*Pos = farpos;
			}
		}
	}
	//------------------------------------------------------------------------

	return Pos;
}

//初期座標と交点座標間の単位ベクトル取得関数.
D3DXVECTOR3 * CMain::UnitMove(D3DXVECTOR3 * Pos, D3DXVECTOR3 HitPos, D3DXVECTOR3 InitPos, float SHOT_SPD)
{
	D3DXVECTOR3 Vec_HitInit;
	Vec_HitInit = HitPos - InitPos;

	D3DXVec3Normalize(&Vec_HitInit, &Vec_HitInit);

	*Pos = Vec_HitInit * SHOT_SPD;
	return Pos;
}

//球とレイの当たり判定.	//交点座標.		//レイの開始位置. //レイの長さ.   //当たるもの.
bool CMain::RaySphere(D3DXVECTOR3* Pos, D3DXVECTOR3* x, D3DXVECTOR3* v, D3DXVECTOR3* pTgtPos)
{
	// 線分と球の判定
	D3DXVECTOR3* center = pTgtPos;          // 中心位置.
	FLOAT radius_sq = 1.0f;

	D3DXVECTOR3 xc = *x - *center;// 中心からの相対位置.
	FLOAT xc2 = D3DXVec3Dot(v, v);
	FLOAT vxc = D3DXVec3Dot(v, &xc);
	FLOAT c = D3DXVec3Dot(&xc, &xc) - radius_sq * radius_sq;
	FLOAT D = vxc * vxc - xc2 * c;// 判別式.

	// 交点が存在しない外れた.
	if (D < 0)
	{
		return false;
	}
	else
	{
		float t = (-vxc - sqrt(vxc * vxc - xc2 * c)) / xc2;

		*Pos = *x + *v * t;
		return Pos;
		return true;

	}
}

//==================================================
//	小松が手を加えている.
//==================================================
//スクリプトUIの座標.
void CMain::UI_Display(
	float Base_W, float Base_H,		//元画像の大きさ
	float Stride_W, float Stride_H,	//1コマ当たりの幅,高さ
	float Disp_W, float Disp_H,		//表示する大きさ.
	const char* fileName,			//ファイルの名前(画像名)
	CSpriteUI*	UIName)				//SpriteUIの名前.
{
	CSpriteUI::SPRITE_STATE ss;
	ss.Base.w = Base_W;
	ss.Base.h = Base_H;
	ss.Stride.w = Stride_W;//1コマ当たりの幅を算出.
	ss.Stride.h = Stride_H;//1コマ当たりの高さを算出.
	ss.Disp.w = Disp_W;
	ss.Disp.h = Disp_H;
	UIName->Init(m_pDevice11, m_pContext11,
		fileName, &ss);
}
//==================================================


//スプライトセット関数.
void CMain::Sprite(
	float Base_W, float Base_H,		//元画像の大きさ
	float Stride_W, float Stride_H,	//1コマ当たりの幅,高さ
	float Disp_W, float Disp_H,		//表示する大きさ.
	const char* fileName,			//ファイルの名前(画像名)					
	CSprite*	UIName)				//SpriteUIの名前.
{
	CSprite::SPRITE_STATE ss;
	ss.Base.w = Base_W;
	ss.Base.h = Base_H;
	ss.Stride.w = Stride_W;//1コマ当たりの幅を算出.
	ss.Stride.h = Stride_H;//1コマ当たりの高さを算出.
	ss.Disp.w = Disp_W;
	ss.Disp.h = Disp_H;
	UIName->Init(m_pDevice11, m_pContext11,
		fileName, &ss);
}

//アニメーション更新関数.
void CMain::UpdeAnim()
{
	m_BossKumaParam.m_anim_time = 0.0;
	//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);

}

//数値初期化関数.
void CMain::Init()
{
	//くまさん関係初期化.
	for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
		m_KumaParam[KNo].m_Pos = m_KumaParam[KNo].m_InitPos;
		m_KumaParam[KNo].m_anim_no = 0;
		m_KumaParam[KNo].m_anim_time = 0.0;
		m_KumaParam[KNo].m_DispFlag = false;
		m_KumaParam[KNo].m_AppearCnt = m_KumaParam[KNo].m_InitDispCnt;
		m_KumaParam[KNo].m_TexChangeFlag = false;
		m_KumaParam[KNo].m_SkinCnt = m_KumaParam[KNo].m_InitSkinCnt;
		m_KumaParam[KNo].m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_KumaParam[KNo].m_OldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_pTime->Init();

	//ダメージの初期化.
	m_Damage = CPlayer::enDamageParam::enNoDamage;
	m_pPlayer->Init();
	m_pGunParts->Init();
	m_pPump->Init();
	//撃つものの初期化
	//水鉄砲初期化
	ShotWait = 0.0f;
	for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
		m_ShotParam[SNo].m_Pos.y = -10.0f;
		m_ShotParam[SNo].m_ShotFlag = false;
	}
	//泥の初期化
	for (int MNo = 0; MNo < MUD_MAX; MNo++) {
		m_MudParam[MNo].m_Pos.y = -5.0f;
		m_MudParam[MNo].m_ShotFlag = false;
	}
	//水風船の初期化
	m_BomParam.m_Pos.y = -10.0f;
	m_BomParam.m_ShotFlag = false;
	for (int TNo = 0; TNo < TEX_NO; TNo++) {
		m_BossKumaParam.m_SkinChangeCnt[TNo] = 2;
	}

	//ボスくまさん関係初期化.
	m_BossKumaParam.m_Pos.z = BKUMA_INIT;
	for (int TNo = 0; TNo < TEX_NO; TNo++) {
		m_BossKumaParam.m_SkinChangeCnt[TNo];
	}
	m_BossKumaParam.m_enBKMove = enBossKumaMove::Wait;
	m_BossKumaParam.m_anim_no = 0;
	m_BossKumaParam.m_anim_time = 0.0;
	m_BossKumaParam.m_HitCnt = 0;
	m_BossKumaParam.m_CleanCnt = 0;
	//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);

	
	m_SpaceDispflag = false;

	//m_pFade->SetEndFlag(false);

	m_alphaFlag = false;

	WaterBallonCnt = 3;
	WaterBallonDispCnt = 3;

	m_P_Moveflag = false;
	
	m_pBackground->Init();
	m_pHideTex->Init();
	m_pBHideTex->Init();
	for (int i = 0; i < MUD_MAX; i++) {
		m_M_HideParam[i].m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int i = 0; i < BEAR_MAX; i++) {
		m_E_HideParam[i].m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int i = 0; i < TEX_NO; i++) {
		m_B_HideParam[i].m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_HaniParam.m_Pos.z = 6.0f;
	
	m_BomParam.m_BExpFlag = false;
	m_BomParam.m_ExpFlag = false;

	m_ContinueFlag = false;
}
//==========================================================================================================================================.
//													小松が見やすいように関数化しました.
//==========================================================================================================================================.

//タイトルをクリックしたときの処理.
void CMain::GetKyeAppMain()
{
	//マウスと円の当たり判定をとっている.
	if (m_pMouse->PointAndCircle(
		WND_W_HALF, CLICK_START_Y + (205.0f / 2), (185.0f / 2))
		&& m_pFade->GetFadeFlag() == 0) {
		SESoundDrow(m_pCircleSE, m_BomParam.m_SoundFlag = true);	//音
		//左クリックするとゲームを進める.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//弾を飛ばす時のサウンド.
			SESoundDrowBlaze(m_pClickSE, m_ShotParam->m_SoundFlag = true);
			Init();
			m_pTime->SetMissionTime(0);
			m_pTime->Init();
			m_enStageScene = enStageScene::enMission;
			m_pFade->FadeIn_Disp();
			m_pArea->AreaFade_Disp();
		}
	}
	else
	{
		m_pCircleSE->SeekToStart();
	}
	//マウスと円の当たり判定をとっている.
	if (m_pMouse->PointAndCircle(
		WND_W - (110.0f / 2), (110.0f / 2), (110.0f / 2))) {
		SESoundDrowBlaze(m_pCircleSE, m_BomParam.m_SoundFlag = true);
		//左クリックするとゲームを進める.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//ｳｨﾝﾄﾞｳを破棄する.
			DestroyWindow(m_hWnd);
		}
	}
	if (m_pFade->GetFadeFlag() == 2)
	{
		//ゲームメインへ.
		m_enScene = enScene::enGameMain;
		m_pTime->SetBossTime(0);
		m_BearCleanCnt = 0;
		m_BossCleanCnt = 0;
	}
	BGMSoundDrow(m_TitleBGM, m_BGMflag = true);
	BGMSoundDrow(m_pGameOverBGM, m_BGMflag = false);
	BGMSoundDrow(m_pGameClearBGM, m_BGMflag = false);
}

//ゲームオーバーをクリックしたときの処理.
void CMain::GetKyeOver()
{
	//マウスと円の当たり判定をとっている.
	if (m_pMouse->PointAndCircle(
		300.0f + (205.0f / 2), 510.0f + (205.0f / 2), (205.0f / 2)) &&
		m_pFade->GetFadeFlag() == 0) {
		SESoundDrowBlaze(m_pCircleSE, m_BomParam.m_SoundFlag = true);
		//左クリックするとゲームを進める.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//弾を飛ばす時のサウンド.
			SESoundDrowBlaze(m_pClickSE, m_ShotParam->m_SoundFlag = true);
			m_pFade->FadeIn_Disp();
			m_ContinueFlag = true;
		}
	}
	//マウスと円の当たり判定をとっている.
	else if (m_pMouse->PointAndCircle(
		800.0f + (205.0f / 2), 510.0f + (205.0f / 2), (205.0f / 2)) &&
		m_pFade->GetFadeFlag() == 0){
		SESoundDrowBlaze(m_pCircleSE, m_BomParam.m_SoundFlag = true);
		//左クリックするとゲームを進める.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//弾を飛ばす時のサウンド.
			SESoundDrowBlaze(m_pClickSE, m_ShotParam->m_SoundFlag = true);
			m_pFade->FadeIn_Disp();
		}
	}

	if (m_pFade->GetFadeFlag() == 2)
	{
		if (m_ContinueFlag == true) {
			//ゲームメインへ.
			Init();
			m_enScene = enScene::enGameMain;
			m_pArea->AreaFade_Disp();
			m_pTime->Init();
		}
		else  {
			//タイトルへ.
			Init();
			m_enScene = enScene::enTitle;
		}
	}
	BGMSoundDrow(m_pMainBGM, m_BGMflag = false);
	BGMSoundDrow(m_pBossBGM, m_BGMflag = false);
	BGMSoundDrow(m_pGameOverBGM, m_BGMflag = true);
}
//ゲームクリアをクリックしたときの処理.
void CMain::GetKyeClear()
{
	//マウスと円の当たり判定をとっている.
	if (m_pMouse->PointAndCircle(
		WND_W - (112.0f / 2), (112.0f / 2), (112.0f / 2))
		&& m_pFade->GetFadeFlag() == 0) {
		SESoundDrowBlaze(m_pCircleSE, m_BomParam.m_SoundFlag = true);
		//左クリックするとゲームを進める.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//弾を飛ばす時のサウンド.
			SESoundDrowBlaze(m_pClickSE, m_ShotParam->m_SoundFlag = true);
			m_pFade->FadeIn_Disp();
		}
	}
	if (m_pFade->GetFadeFlag() == 2)
	{
		//タイトルへ.
		Init();
		m_enScene = enScene::enTitle;
	}
	BGMSoundDrow(m_pBossBGM, m_BGMflag = false);
	BGMSoundDrow(m_pGameClearBGM, m_BGMflag = true);
}

//アプリメインの水風船の処理.
void CMain::WaterBalloonProcessAppMain()
{

	//水風船処理.
	if (WaterBallonCnt > 0) {
		//	2019_7_31 更新　吉田.
		if (GetAsyncKeyState('M') & 0x8000 && m_P_Moveflag == false) {
			//------------------------------------
			//吉田更新_ クリックすると空気入れをやめる処理.
			if (m_BomParam.m_ShotFlag == false) {
				SESoundDrowBlaze(m_WaterBalloonSE, m_BomParam.m_SoundFlag = true);
				m_GunDispFlag = false;
				WaterBallonDispCnt--;

				m_BomParam.m_Pos = m_pPlayer->GetPosition();
				m_BomParam.m_Pos.x += 0.5f;
				m_BomParam.m_Pos.z -= 2.0f;
				m_BomParam.m_HitPos = m_pPlayer->GetPosition();
				m_BomParam.m_HitPos.y += 0.5f;
				m_BomParam.m_HitPos.z += 5.0f;
				UnitMove(&m_BomParam.m_VecUnit, m_BomParam.m_HitPos, m_BomParam.m_Pos, BOM_SPD);
				m_BomParam.m_ShotFlag = true;
				m_BomParam.m_fAcc = 0.0f;
				

			}
		}
		
		if (m_BomParam.m_ShotFlag == true) {
			m_BomParam.m_Pos.x += m_BomParam.m_VecUnit.x;
			m_BomParam.m_Pos.z += m_BomParam.m_VecUnit.z;
			
			m_BomParam.m_Pos.y += BOM_JUMP_POWER - m_BomParam.m_fAcc;
			m_BomParam.m_fAcc += BOM_GREVITY;
		}
		if (m_BomParam.m_Pos.z >= m_pCamera->vPos.z + 10.0f) {
			m_BomParam.m_ShotFlag = false;
			WaterBallonCnt--;
			m_BomParam.m_ExpFlag = true;
			m_BomParam.m_BExpFlag = true;
		}

	}
}

//アプリメインの水鉄砲の処理.
void CMain::WaterGunProcessAppMain()
{
	//もし、m_GaugeBarCntが0.01f以上なら撃てて、そうじゃないなら撃てない.
	if (m_pPump->GetGauge() >= 0.01f) {
		ShotWait -= 0.1f;
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && m_P_Moveflag == false) {
			//------------------------------------
			//吉田更新_ クリックすると空気入れをやめる処理.
			m_GunDispFlag = false;
			//------------------------------------
			m_pPump->DecrementGauge();
			for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
				if (ShotWait <= 0) {//発射間隔.
					if (m_ShotParam[SNo].m_ShotFlag == false) {
						m_ShotParam[SNo].m_Pos = m_WMousePos;//カーソルの位置に弾を持ってくる.
						UnitMove(&m_ShotParam[SNo].m_VecUnit, m_HitPos, m_WMousePos, SHOT_SPD);//単位ベクトル取得.
						m_ShotParam[SNo].m_ShotFlag = true;
						//弾を飛ばす時のサウンド.
						SESoundDrowBlaze(m_WaterShotSE, m_ShotParam->m_SoundFlag = true);
						break;
					}
				}
			}
		}
	}
	for (int SNo = 0; SNo < SHOT_MAX; SNo++) {

		if (m_ShotParam[SNo].m_ShotFlag == true)
		{
			m_ShotParam[SNo].m_Pos += m_ShotParam[SNo].m_VecUnit;//単位ベクトルを足して弾移動.
			m_ShotParam[SNo].m_DispCount++;
		}
		if (m_ShotParam[SNo].m_DispCount >= 120) {//時間がたったら弾を消す.
			m_ShotParam[SNo].m_ShotFlag = false;
			m_ShotParam[SNo].m_DispCount = 0;
			m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		}
	}
	if (ShotWait <= 0) {
		ShotWait = SHOT_WAIT;//弾の発射間隔.
	}

}

//水鉄砲を表示してゲージを回復する処理.
void CMain::WaterGunGaugeChargeAppMain()
{
	//リロード処理(水鉄砲メッシュ).
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		m_SpeceKey++;	//押されている.
	}
	else
	{
		if (m_SpeceKey > 0) {
			m_SpeceKey = -1;	//離れた瞬間.
		}
		else
		{
			m_SpeceKey = 0;	//離れている状態.
		}
	}

	//長押しで反応しなくする.
	if (m_SpeceKey == 1) {
		m_GunDispFlag = true;
		m_pPump->SetReload();
		SESoundDrowBlaze(m_pPumpSE, m_PumpSEflag = true);
	}
	m_pPump->Charge(m_pGunParts->GetPosition());
}

//ゲージでレンダリングに置いてある.
void CMain::GaugeRender()
{
	//スペースキーで回復するUI表示.
	if (m_SpaceDispflag == true) {
		//スペースを押すのロゴ.
		m_pSpaceLogo->SetPosition(WND_W_HALF - 62.0f / 2, 460.0f, 0.14f);
		m_pSpaceLogo->Render();
		//スペースを押すのバー.
		m_pSpaceBar->SetPosition(WND_W_HALF - 380.0f / 2, 450.0f, 0.15f);	//xの-10.0fは減少ゲージのサイズと同じため小さくしたときに少しずれるためにぎりぎりにしている.
		m_pSpaceBar->Render();
	}
	//ボスくまさんのアイコン.
	m_pBossBearIcon->SetPosition(10.0f, 460.0f, 0.16f);
	m_pBossBearIcon->Render();
	//ゲージバー.
	m_pGaugeBar->SetPosition(90.0f, UI_MOSTLOWER, 0.17f);
	m_pGaugeBar->Render();
	//減少ゲージのUIの表示.
	m_pDecremetGauge->SetPosition(104.6f, UI_MOSTLOWER, 0.18f);
	m_pDecremetGauge->GaugeRender(m_pPump->GetGauge());
	//ゲージの管理.
	if (m_pPump->GetGauge() >= MAXGAUGE) {
		m_pPump->SetGauge(MAXGAUGE);
	}
	else if (m_pPump->GetGauge() <= MINIMUMGAUGE) {
		m_SpaceDispflag = true;
	}
	else {
		m_SpaceDispflag = false;
	}
}

//プレイヤーダメージでレンダリングに置いてある.
void CMain::P_DamageRender()
{
	if (m_pPlayer->GetDamage() >= BIG_DAMAGE) {
		m_pMudDamage_3->SetPosition(0.0f, 0.0f, 0.20f);
		m_pMudDamage_3->Render();
	}
	else if (m_pPlayer->GetDamage() >= MIDDLE_DAMAGE) {
		m_pMudDamage_2->SetPosition(0.0f, 0.0f, 0.20f);
		m_pMudDamage_2->Render();
	}
	else if (m_pPlayer->GetDamage() >= SMALL_DAMAGE) {
		m_pMudDamage_1->SetPosition(0.0f, 0.0f, 0.20f);
		m_pMudDamage_1->Render();
	}
}

//データの読み込み(くまさんのデータ)&ロードメッシュでしている.
void CMain::FileDataBear()
{
	//ファイルを開く.
	fstream	filestream("Data\\ExcelData\\Bear.csv");
	if (!filestream.is_open()) {
		return;
	}

	bool	isNotSkip = false;
	//ファイルの読み込み.
	while (!filestream.eof())
	{
		string	buffer;
		filestream >> buffer;
		if (!isNotSkip) {
			isNotSkip = true;
			continue;	//一行目をスキップ.
		}
		//ファイルから読み込
		vector<string>	record;
		istringstream	streambuffer(buffer);
		string			token;
		enENEMY_PARAM_DATA enBearLineNo = enENEMY_PARAM_DATA::enAPPEARANCECNT;
		while (getline(streambuffer, token, ','))
		{
			record.push_back(token);
			switch (enBearLineNo)
			{
			case CMain::enENEMY_PARAM_DATA::enAPPEARANCECNT:
				m_KumaParam[BearDataCnt].m_AppearCnt = atoi(token.c_str());
				m_KumaParam[BearDataCnt].m_InitDispCnt = m_KumaParam[BearDataCnt].m_AppearCnt;
				enBearLineNo = enENEMY_PARAM_DATA::enPOSITION_X;

				break;
			case CMain::enENEMY_PARAM_DATA::enPOSITION_X:
				m_KumaParam[BearDataCnt].m_Pos.x = static_cast<float>(atof(token.c_str()));
				m_KumaParam[BearDataCnt].m_InitPos.x = m_KumaParam[BearDataCnt].m_Pos.x;
				enBearLineNo = enENEMY_PARAM_DATA::enPOSITION_Y;

				break;
			case CMain::enENEMY_PARAM_DATA::enPOSITION_Y:
				m_KumaParam[BearDataCnt].m_Pos.y = static_cast<float>(atof(token.c_str()));
				m_KumaParam[BearDataCnt].m_InitPos.y = m_KumaParam[BearDataCnt].m_Pos.y;
				enBearLineNo = enENEMY_PARAM_DATA::enPOSITION_Z;

				break;
			case CMain::enENEMY_PARAM_DATA::enPOSITION_Z:
				m_KumaParam[BearDataCnt].m_Pos.z = static_cast<float>(atof(token.c_str()));
				m_KumaParam[BearDataCnt].m_InitPos.z = m_KumaParam[BearDataCnt].m_Pos.z;
				enBearLineNo = enENEMY_PARAM_DATA::enSPEED;
				break;
			case CMain::enENEMY_PARAM_DATA::enSPEED:
				m_KumaParam[BearDataCnt].m_Speed = static_cast<float>(atof(token.c_str()));
				enBearLineNo = enENEMY_PARAM_DATA::enDAMAGECNT;
				break;
			case CMain::enENEMY_PARAM_DATA::enDAMAGECNT:
				m_KumaParam[BearDataCnt].m_SkinCnt = atoi(token.c_str());
				m_KumaParam[BearDataCnt].m_InitSkinCnt = m_KumaParam[BearDataCnt].m_SkinCnt;
				enBearLineNo = enENEMY_PARAM_DATA::enMOVEPATTERN;
				break;
			case CMain::enENEMY_PARAM_DATA::enMOVEPATTERN:
				m_KumaParam[BearDataCnt].m_MovePattern = atoi(token.c_str());
				enBearLineNo = enENEMY_PARAM_DATA::NONE;
				break;
			}
		}
		if (enBearLineNo == enENEMY_PARAM_DATA::NONE) {
			BearDataCnt++;
		}
	}
}

//BGMサウンドの流す(レンダリング処理).
void CMain::BGMSoundDrow(clsSound* m_pSound, bool m_SoundFlag)
{
	if (m_SoundFlag == true)
	{
		if (m_pSound->IsStopped())
		{
			m_pSound->SeekToStart();
			m_pSound->Play();
		}
	}
	else if(m_SoundFlag == false){
		m_pSound->Stop();
	}
}

//SEサウンドの流す(レンダリング処理連射用).
void CMain::SESoundDrowBlaze(clsSound* m_pSound, bool m_SoundFlag)
{
	//吉田変更.
	if (m_SoundFlag == true)
	{
		m_pSound->SeekToStart();
		m_pSound->Play();
		m_SoundFlag = false;
	}
}

//SEサウンドの流す(レンダリング処理).
void CMain::SESoundDrow(clsSound* m_pSound, bool m_SoundFlag)
{
	if (m_SoundFlag == true)
	{
		if (m_pSound->IsStopped())
		{
			
			m_pSound->Play();
			
		}
	}
}

