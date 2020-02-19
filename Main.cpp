#include "Main.h"

//=======================================================
//��������������Ă���(�t�@�C���̓��o�͂ɕK�v).
//=======================================================
#include <iostream>
#include <fstream>	//���͂�ifstream�A�o�͂�ofstream�ɕK�v.
#include <string>	//std::getline�֐����g�����߂ɕK�v.
#include <sstream>	//����͕K�v.
#include <vector>	//����͕K�v.
using namespace std;
//=======================================================
//��۰��ٕϐ�.
CMain*	g_pCMain = nullptr;

//================================================
//	Ҳ݊֐�.
//================================================
INT WINAPI WinMain(
	HINSTANCE hInstance,	//�ݽ�ݽ�ԍ�(����޳�̔ԍ�).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow)
{
	//�o�͂Ƀf���[�g���Ă��Ȃ�����\��������ׂ̂���.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	g_pCMain = new CMain();	//���������׽�錾.

	if (g_pCMain != nullptr) {

		//����޳�쐬����������.
		if (SUCCEEDED(
			g_pCMain->InitWindow(
				hInstance,
				0, 0,
				WND_W, WND_H,
				WND_TITLE)))
		{
			//Dx11�p�̏�����.
			if (SUCCEEDED(g_pCMain->InitD3D()))
			{
				//ү����ٰ��.
				g_pCMain->Loop();
			}
		}
		//�I��.
		g_pCMain->DestroyD3D();//Direct3D�̉��.

		SAFE_DELETE(g_pCMain);//�׽�̔j��.
	}

	return 0;
}

//================================================
//	����޳��ۼ��ެ�.
//================================================
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT uMsg,
	WPARAM wPara, LPARAM lParam)
{
	//��ۼ��ެ�.
	return g_pCMain->MsgProc(hWnd, uMsg, wPara, lParam);
}

/**********************************************************************
*	Ҳݸ׽.
**/
//=================================================
//	�ݽ�׸�.
//=================================================
CMain::CMain()
	: m_hWnd(nullptr)//������ؽĂƌĂ�.
	, m_pDevice11(nullptr)
	, m_pContext11(nullptr)
	, m_pSwapChain(nullptr)
	, m_pBackBuffer_TexRTV(nullptr)
	, m_pBackBuffer_DSTex(nullptr)
	, m_pBackBuffer_DSTexDSV(nullptr)
	, m_pDepthStencilState(nullptr)
	, m_pDepthStencilStateOff(nullptr)

	, m_vLight(0.0f, 0.5f, -1.0f)	//ײĕ���.
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
//	�޽�׸�.
//=================================================
CMain::~CMain()
{
}

//����޳�������֐�.
HRESULT CMain::InitWindow(
	HINSTANCE hInstance,	//�ݽ�ݽ.
	INT x, INT y,			//����޳x,y���W.
	INT width, INT height,	//����޳��,����.
	const char* WindowName)	//����޳��.
{
	//����޳�̒�`.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));//������(0��ݒ�).

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//�����}�E�X�̕\�������Ȃ��悤
	ShowCursor(FALSE);	//�J�����͏����Ƃ��Ƃ���.
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = APP_NAME;
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	//����޳�׽��Windows�ɓo�^.
	if (!RegisterClassEx(&wc)) {
//		MessageBox(nullptr,
//			"����޳�׽�̓o�^�Ɏ��s", "�װ", MB_OK);
		_ASSERT_EXPR(false, L"����޳�׽�̓o�^�Ɏ��s");
		return E_FAIL;
	}

	//����޳�̍쐬.
	m_hWnd = CreateWindow(
		APP_NAME,		//���ؖ�.
		WindowName,		//����޳����.
		 WS_POPUP|WS_VISIBLE,	//����޳���(����).//WS_OVERLAPPEDWINDOW:�ʏ�.
		x, y,			//�\���ʒux,y���W.
		width, height,	//����޳��,����.
		nullptr,		//�e����޳�����.
		nullptr,		//�ƭ��ݒ�.
		hInstance,		//�ݽ�ݽ�ԍ�.
		nullptr);		//����޳�쐬���ɔ����������Ăɓn���ް�.
	if (!m_hWnd) {
		_ASSERT_EXPR(false, "����޳�쐬���s");
		return E_FAIL;
	}

	//����޳�̕\��.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//����޳�֐�(ү���ޖ��̏���).
LRESULT CMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY://����޳���j�����ꂽ�Ƃ�.
		//���ع���݂̏I����Windows�ɒʒm����.
		PostQuitMessage(0);
		break;
	
	case WM_KEYDOWN://���ް�ނ������ꂽ�Ƃ�.
		//���ʂ̏���.
		switch (static_cast<char>(wParam)) {
		case VK_ESCAPE:	//ESC��.
			if (MessageBox(nullptr,
				"�ްт��I�����܂����H", "�x��", MB_YESNO) == IDYES)
			{
				//����޳��j������.
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	}

	//Ҳ݂ɕԂ����.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//ү����ٰ��.
void CMain::Loop()
{
	//ү���ǂݍ��݊֐�.
	LoadMesh();
	//UI�̓ǂݍ��݊֐�.
	LoadUI();
	//�T�E���h�̓ǂݍ��݊֐�.
	LoadSound();

	//------------------------------------------------
	//	�ڰ�ڰĒ�������.
	//------------------------------------------------
	float Rate	= 0.0f;	//ڰ�.
	float FPS = 60.0f;	//FPS�l.
	DWORD sync_old = timeGetTime();	//�ߋ�����.
	DWORD sync_now;					//���ݎ���.

	//���ԏ����̂��߁A�ŏ��P�ʂ�1�ؕb�ɕύX.
	timeBeginPeriod(1);
	Rate = 1000.0f / FPS; //���z���Ԃ��Z�o.

	//ү����ٰ��.
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		sync_now = timeGetTime();	//���݂̎��Ԃ��擾.

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if( sync_now - sync_old >= Rate)
		{
			sync_old = sync_now;	//���ݎ��Ԃɒu������.

			//���ع���݂̏����͂���������.
			AppMain();


			Render();

		}
	}
	//���ع���݂̏I��.
}

//���ع����Ҳݏ���.
void CMain::AppMain()
{
	m_pMouse->SetMouse(m_hWnd);
	//���v�����v�Z.
	m_TotalBearCnt = m_BearCleanCnt + m_BossCleanCnt;
	////#if Debug
	////	//�f�o�b�O�p
	//if (GetAsyncKeyState(VK_BACK) & 0x8000) {
	//	//m_enScene = enScene::enGameClear;
	//	m_enStageScene = enStageScene::enBoss;
	//}
	//#endif	//#if Debug

//==========================================================================================================================
//		��������������Ă���B(switch���̏�{��������}���Ƃ��Ă�!�I)	�g�c�̂��邱��	:	�K�v�ȂƂ���ɏ����������Ă�!
//==========================================================================================================================
	switch (m_enScene)
	{
	//�^�C�g����
	case CMain::enScene::enTitle:
		//�^�C�g���̃N���b�N�����Ƃ��̏���.
		GetKyeAppMain();	
		break;
		//�Q�[�����C����
	case CMain::enScene::enGameMain:
		//----------------------------------------------------
		//7_19�X�V(�g�c).
		//m_CursorParam.m_Pos���Z�b�g����.

		//���Ԃ��Ǘ�����.
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

		//========================�ړ�����===================================
		m_pPlayer->Move(m_P_Moveflag);
		m_pBackground->Move(m_P_Moveflag);
		m_pGunParts->Move(m_P_Moveflag);
		m_pPump->Move(m_P_Moveflag);


		//===================================================================

		//�����D����.
		WaterBalloonProcessAppMain();

		//���S�C����.
		WaterGunProcessAppMain();
	

		//���܂���̕\�����Ԃ��Ǘ�.
		//�����A���܂���̕\���J�E���g���J�E���g�^�C���𒴂����.
		//���C���ŏ������Ȃ���true�����葱���ď����Ȃ��Ȃ�.
		for (int i = 0; i < BEAR_MAX; i++) {
			if (m_pTime->GetMissionTime() >= m_KumaParam[i].m_AppearCnt) {
				m_KumaParam[i].m_DispFlag = true;
			}
		}
		//==============================================================.
		//���܂���ƒe�̓����蔻��.
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
							m_E_HideParam[KNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);//�B���e�N�X�`���̈ʒu���ߗp.
							//���܂���p�̉B���e�N�X�`���ɂ��܂���̍��W������.
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
							m_E_HideParam[KNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);//�B���e�N�X�`���̈ʒu���ߗp.
							//���܂���p�̉B���e�N�X�`���ɂ��܂���̍��W������.
							m_pHideTex[KNo].HideTex_Disp(m_E_HideParam[KNo].m_Pos);
							
						}
					}
				}
			}
		}

		//�����D�����蔻��͈�(��).
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
		//-------------------���܂���̏������e-------------------------.
		//���̏��Ԃł��������Ȃ�.

		//�A�j���[�V�������Ԃ̉��Z.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			//m_KumaParam[KNo].m_anim_time += m_pKumaSkin[KNo].GetAnimSpeed();
		}

		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			//�ړ��A�j���[�V��������.
			if (m_KumaParam[KNo].m_TexChangeFlag == false) {//�������Ă��Ȃ����.
				if (m_KumaParam[KNo].m_anim_no == KWait ) {
					m_KumaParam[KNo].m_anim_no = KWalk;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Move;
					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
			}	//�����������̏���.
			else{
				if (m_KumaParam[KNo].m_anim_no == KWalk) {
					m_KumaParam[KNo].m_anim_no = KHit;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Wait;
					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
			}
			//���������Ƃ��̃A�j���[�V�������I�������.
			if (m_pKumaSkin[KNo].GetAnimPeriod(m_KumaParam[KNo].m_anim_no) - m_pKumaSkin[KNo].GetAnimSpeed() 
				<= m_KumaParam[KNo].m_anim_time && m_KumaParam[KNo].m_anim_no == KHit ) {
				m_KumaParam[KNo].m_anim_no = KWait;
			}
		}

		//���܂���̓����̐؂�ւ�����.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_KumaParam[KNo].m_TexChangeFlag == false && m_KumaParam[KNo].m_SkinCnt > CLEAN && m_P_Moveflag == false) {
				if (m_KumaParam[KNo].m_Pos.z <= m_pCamera->vPos.z + ATK_LENGTH && m_KumaParam[KNo].m_anim_no != KEscape 
					&& m_KumaParam[KNo].m_anim_no != KAttack) {	//��ʎ�O�܂ŗ�����U������.
					m_KumaParam[KNo].m_anim_no = KAttack;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Wait;
					m_KumaParam[KNo].m_anim_time = 0.0;
					m_pPlayer->UpDamage();		//�_���[�W���󂯂��Ƃ�.
					SESoundDrowBlaze(m_pPlayerDamageSE, m_DamageSEflag = true);

					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
				if (m_pKumaSkin[KNo].GetAnimPeriod(m_KumaParam[KNo].m_anim_no) - m_pKumaSkin[KNo].GetAnimSpeed() <=
					m_KumaParam[KNo].m_anim_time && m_KumaParam[KNo].m_anim_no == KAttack) {//�A�j���[�V�������I������瓮���o���Ă�.
					m_KumaParam[KNo].m_anim_no = KEscape;
					m_KumaParam[KNo].m_enKMove = enKumaMove::Escape;
					//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				}
			}
		}

		//���܂����Y��ɂȂ������̏���.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_KumaParam[KNo].m_SkinCnt <= CLEAN && m_pHideTex->GetHideTexFlag() == false 
				&& m_KumaParam[KNo].m_anim_no != KRejoice && m_KumaParam[KNo].m_anim_no != KEscape) {  
				m_KumaParam[KNo].m_anim_no = KRejoice;
				m_KumaParam[KNo].m_enKMove = enKumaMove::Wait;
				m_KumaParam[KNo].m_anim_time = 0.0;
				//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
				m_BearCleanCnt++;
			}
			//��уA�j���[�V�������I�������.
			if (m_pKumaSkin[KNo].GetAnimPeriod(m_KumaParam[KNo].m_anim_no) - m_pKumaSkin[KNo].GetAnimSpeed()
				<= m_KumaParam[KNo].m_anim_time && m_KumaParam[KNo].m_anim_no == KRejoice) {
				m_KumaParam[KNo].m_anim_no = KEscape;
				m_KumaParam[KNo].m_enKMove = enKumaMove::Escape;
				//m_pKumaSkin[KNo].ChangeAnimSet(m_KumaParam[KNo].m_anim_no);
			}
		}

		//���܂���̓����̏���.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			switch (m_KumaParam[KNo].m_enKMove)
			{
			case enKumaMove::Wait:
				break;
			case enKumaMove::Move:
				//�\���t���O��true�̎��ړ��J�n.
				if (m_KumaParam[KNo].m_DispFlag == true) {
					//�p�^�[���ňړ����@���ς��.
					switch (m_KumaParam[KNo].m_MovePattern)
					{
					case 1:
						//���܂��񂪍�����o�������ꍇ
						if (m_KumaParam[KNo].m_Pos.x < K_Left) {
							m_KumaParam[KNo].m_Pos.x += m_KumaParam[KNo].m_Speed;	
							m_KumaParam[KNo].m_Rot.y = -1.5f;
						}
						//���܂��񂪉E����o�������ꍇ
						else if (m_KumaParam[KNo].m_Pos.x > K_Right) {
							m_KumaParam[KNo].m_Pos.x -= m_KumaParam[KNo].m_Speed;
							m_KumaParam[KNo].m_Rot.y = 1.5f;
						}
						//�������瓹�ɗ����ꍇ
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

		//���S�C��\�����ăQ�[�W���񕜂��鏈��.
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

			//�����A�J�E���g�^�C����120�ȏ�ɂȂ�ƃ{�X�X�e�[�W�ɔ��.
			if (m_pTime->GetMissionTime() >= MISSION_TIME) {//1646��̏I��鎞��.
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

			//�����Ă��܂�����.
			if (m_pPlayer->GetDamage() >= LAST_DAMAGE) {
				
				if (m_pFade->GetFadeFlag() == 0) 
				{
					m_pFade->FadeIn_Disp();
				}
				if (m_pFade->GetFadeFlag() == 2)
				{
					m_pPlayer->DamageInit();
					//�����Ă��܂�����.
					m_pTime->SetMissionTime(0);
					m_BearCleanCnt = 0;
					m_GunDispFlag = false;
					//�R���e�j���[��.
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
			//-------------------�{�X���܂���̏������e---------------------.
			m_pTime->TimeCnt();
			//�A�j���[�V�������Ԃ̉��Z.
			m_BossKumaParam.m_anim_time += m_pB_KumaSkin->GetAnimSpeed();

			if (m_BossKumaParam.m_anim_no != BRejoice) {
				//�J�E���g���������瓊����A�j���[�V�����ɂ͂����Ă�.
				if (m_pTime->GetBossTime() >= 900 && m_pTime->GetBossTime() <= APPROACH_TIME) {
					if (m_BossKumaParam.m_anim_no == BWait) {
						m_BossKumaParam.m_anim_no = BKnead;
						m_BossKumaParam.m_anim_time = 0.0;
						//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
					}	//���˂�A�j���[�V�������I�������U��グ��ɕύX.
					else if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed() 
						<= m_BossKumaParam.m_anim_time && m_BossKumaParam.m_anim_no == BKnead) 
					{
						m_BossKumaParam.m_anim_no = BShakeUp;
						m_BossKumaParam.m_anim_time = 0.0;
						//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
					}	//�U��グ�邪�I�������U�艺�낷��
					else if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed()
							<= m_BossKumaParam.m_anim_time && m_BossKumaParam.m_anim_no == BShakeUp)
					{
							m_BossKumaParam.m_anim_no = BShakeDown;
							//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
							

							//�D�𓊂��鏈��.
							//�D�����l.
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
					}	//�U�艺�낷���I�������߂��Ă܂������鏈����.
					else if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed()
						<= m_BossKumaParam.m_anim_time-0.4 && m_BossKumaParam.m_anim_no == BShakeDown) 
					{
						m_BossKumaParam.m_anim_no = BWait;
						m_BossKumaParam.m_anim_time = 0.0;
						//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
					}
				}
			}
			//�߂Â���L�������Ƃ�.
			if (m_pTime->GetBossTime() >= APPROACH_TIME) {
				if (m_BossKumaParam.m_anim_no != BApproach && m_BossKumaParam.m_anim_no != BWinch && m_BossKumaParam.m_anim_no != BAttack && m_BossKumaParam.m_anim_no != BRejoice) {
					m_BossKumaParam.m_anim_no = BApproach;
					m_BossKumaParam.m_enBKMove = enBossKumaMove::Approach;
					//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
				}
			}


			//�{�X���܂��񂪋߂Â��Ƃ��e��3������������.
			if (m_BossKumaParam.m_HitCnt >= HIT_CNT_MAX && m_BossKumaParam.m_anim_no == BApproach) {

				m_BossKumaParam.m_anim_no = BWinch;
				m_BossKumaParam.m_enBKMove = enBossKumaMove::Winch;
				m_BossKumaParam.m_HitCnt = 0;
				m_BossKumaParam.m_anim_time = 0.0;	
				//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
			}

			//�{�X���܂��񂪋߂Â��Ƃ������D������������.
			if (m_BossKumaParam.m_HitBomFlag == true && m_BossKumaParam.m_anim_no == BApproach) {

				m_BossKumaParam.m_anim_no = BWinch;
				m_BossKumaParam.m_enBKMove = enBossKumaMove::Winch;
				m_BossKumaParam.m_HitCnt = 0;
				m_BossKumaParam.m_anim_time = 0.0;
				m_BossKumaParam.m_HitBomFlag = false;
				//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
			}


			//�Ђ�ރA�j���[�V�������I�������߂Â��ɖ߂�.
			if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed() 
				<= m_BossKumaParam.m_anim_time  && m_BossKumaParam.m_anim_no == BWinch) {
				m_BossKumaParam.m_anim_no = BWait;
				//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
			}

			//�{�X���܂��񂪋߂Â��Ă���Ƃ���O�܂ŗ�����Ȃ�����Ă��鏈��.
			if (m_BossKumaParam.m_anim_no == BApproach && m_BossKumaParam.m_Pos.z <=  m_pCamera->vPos.z +ATK_LENGTH ) {
				m_BossKumaParam.m_anim_no = BAttack;
				m_BossKumaParam.m_enBKMove = enBossKumaMove::Attack;
				m_BossKumaParam.m_anim_time = 0.0;
				//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);
				
			}

			//�U���A�j���[�V�������I����Ă��珈������
			if (m_pB_KumaSkin->GetAnimPeriod(m_BossKumaParam.m_anim_no) - m_pB_KumaSkin->GetAnimSpeed() <=
				m_BossKumaParam.m_anim_time&& m_BossKumaParam.m_anim_no == BAttack) {
				m_pPlayer->UpDamage();
				SESoundDrowBlaze(m_pPlayerDamageSE, m_DamageSEflag = true);

				m_BossKumaParam.m_anim_no = BApproach;
			}

			//�{�X���܂����Y��ɂȂ�����.
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

			//��уA�j���[�V�������I����Ă��珈������
			if (m_pB_KumaSkin->GetAnimPeriod(5) - m_pB_KumaSkin->GetAnimSpeed() <=
				m_BossKumaParam.m_anim_time && m_BossKumaParam.m_anim_no == BRejoice) {

				if (m_pFade->GetFadeFlag() == 2)
				{
					m_BossCleanCnt += 10;
					m_GunDispFlag = false;
					m_enScene = enScene::enGameClear;
				}
			}

			//�{�X���܂���̓�������.
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
				//�����ʒu������.
   				if (m_BossKumaParam.m_Pos.z >= 56.0f) {
					m_BossKumaParam.m_Pos.z -= 0.03f;
				}
				break;
			default:
				break;
			}

			//�D�̈ړ�����.
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

			//�����Ă��܂�����.
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
	// �X�L�����b�V���̃{�[�����W���擾����.
	//---------------------------------------------
	//----------------------------------------------------------------
	//��.
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_atama_boon", &vHeadBonePos))
	{
		m_pBHeadSphere->SetPosition(vHeadBonePos);

	}
	//----------------------------------------------------------------
	//��.
	//����.
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_kosi_boon", &vBodyBonePos[0]))
	{
		m_pBBodySphere->SetPosition(vBodyBonePos[0]);

	}
	//��.
	if (m_pCDxB_KumaMesh->GetPosFromBone("bosskuma_modeoru_kubi_boon", &vBodyBonePos[1]))
	{
		m_pBBodySphere->SetPosition(vBodyBonePos[1]);

	}


	//----------------------------------------------------------------
	//�r.
	//�E.
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

	//��.
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
		//�R���e�j���[��
	case CMain::enScene::enGameOver:
		GetKyeOver();
		break;
		//�Q�[���N���A��.
	case CMain::enScene::enGameClear:
		GetKyeClear();
		break;
	//����ȊO�̎����N������
	default:
		break;
	}
	//===============================================================================================
	
}

//�����ݸ�(�`��).
void CMain::Render()
{
	//��ʂ̸ر.
	float ClearColor[4] = { 0.0f,0.0f,0.5f,1.0f };	//�ر�F(RGBA�̏�).
	//�װ�ޯ��ޯ̧.
	m_pContext11->ClearRenderTargetView(
		m_pBackBuffer_TexRTV, ClearColor);
	//���߽��ݼ��ޯ��ޯ̧.
	m_pContext11->ClearDepthStencilView(
		m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	//ܰ��ޕϊ�(�\���ʒu�E��]�E�g�k����ݒ肷��).
	//Y����].
	D3DXMatrixRotationY(
		&mWorld,	//(out)�v�Z����.
		timeGetTime()/1000.0f);		//�P����Y����]������(׼ޱݎw��).

	//==============================================
	// �Ǐ]�J�����ݒ�.
	//==============================================
	//�J�����ʒu(���@�̔w������)�̐ݒ�.
	m_pCamera->vPos = m_pCamera->vLook = m_pPlayer->GetPosition();//���@�̈ʒu���R�s�[.
	m_pCamera->fYaw = m_pPlayer->GetRotation().y;//Y����]�l���R�s�[.

	//Y����]�s��̍쐬.
	D3DXMatrixRotationY(&m_pCamera->mRot, m_pCamera->fYaw);
	//Z���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);
	//Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(&vecAxisZ,
		&vecAxisZ, &m_pCamera->mRot);
	//----------------------------------------
	//�@�g�c�ύX.
	m_pCamera->vPos -= vecAxisZ * 4.0f;	//���@�̔w����.
	//m_pCamera.vLook += vecAxisZ * 2.0f;	//���@�̑O��.


	//----------------------------------------
	//�ޭ�(���)�ϊ�.
	D3DXVECTOR3	vUpVec(0.0f, 1.0f, 0.0f);	//���(�޸��).
	D3DXMatrixLookAtLH(
		&mView,	//(out)�ޭ��v�Z����.
		&m_pCamera->vPos, &m_pCamera->vLook, &vUpVec);

	//��ۼު����(�ˉe)�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&mProj,	//(out)��ۼު���݌v�Z����.
		static_cast<FLOAT>(D3DX_PI) / 4.0,	//y�����̎���(׼ޱݎw��)���l��傫�������王�삪�����Ȃ�.
		static_cast<FLOAT>(WND_W) / static_cast<FLOAT>(WND_H),//���߸Ĕ�(��������).
		0.1f,		//�߂��ޭ����ʂ�z�l.
		100.0f);	//�����ޭ����ʂ�z�l.
	
	//�w�i��\��.
	m_pBackground->Render(mView, mProj, m_vLight, m_pCamera->vPos);

	//�n�ʂ�\��.
	for (int i = 0; i < GROUND_MAX; i++) {
		m_pGround->SetPosition(m_pGround->GetGroundPos(i));
		m_pGround->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}
	//���̒n��.
	for (int i = GRASS_GROUND_MIDDLE; i < GRASS_GROUND_MAX; i++) {
		for (int j = 0; j < GRASS_GROUND_MIDDLE; j++) {
			m_pGrassGround->SetPosition(m_pGrassGround->GetGrassGroundPos(j));
			m_pGrassGround->Render(mView, mProj, m_vLight, m_pCamera->vPos);
		}
		m_pGrassGround->SetPosition(m_pGrassGround->GetGrassGroundPos(i));
		m_pGrassGround->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}

	//�J�[�\��UI�̕\��.
	m_pCursor->SetPattern(0, 10);
	m_pCursor->SetPosition(D3DXVECTOR3(m_pMouse->GetCursorPosX(), m_pMouse->GetCursorPosY(), 0.30f));
	m_pCursor->Render();

	//=================================================================================================================================
	//		��������������Ă���B(switch���̏�{��������}���Ƃ��Ă�!�I)	�g�c�̂��邱��:�K�v�ȂƂ���ɉ��̂悤��Render������.
	//=================================================================================================================================
	switch (m_enScene)
	{
	case CMain::enScene::enTitle:
		//�X�^�[�g(�i�߂�)UI
		m_pStartUI->SetPosition(WND_W_HALF - (185.0f / 2), CLICK_START_Y, 0.40f);
		m_pStartUI->Render();
		//�Q�[������߂�UI.
		m_pEndUI->SetPosition(WND_W - 110.0f, 0.0f, 0.41f);
		m_pEndUI->Render();
		//�^�C�g��UI
		m_pTitle->SetPosition(WND_W_HALF - (534.0f /2), 20.0f, 0.42f);
		m_pTitle->Render();
		break;
	case CMain::enScene::enGameMain:
		//�����D�̕\��.
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
		//�����D���������Ă���Ƃ��̉B���G�t�F�N�g.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_BomParam.m_ExpFlag == true && m_KumaParam[KNo].m_HitBomFlag == true) {
				m_KumaParam[KNo].m_SkinCnt -= 2;
				m_E_HideParam[KNo].m_Pos = m_KumaParam[KNo].m_Pos;
				m_E_HideParam[KNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);//�B���e�N�X�`���̈ʒu���ߗp.
				//���܂���p�̉B���e�N�X�`���ɂ��܂���̍��W������.
				m_pHideTex[KNo].HideTex_Disp(m_E_HideParam[KNo].m_Pos);
				m_KumaParam[KNo].m_HitBomFlag = false;
			}
		}
		m_BomParam.m_ExpFlag = false;

		//���܂���̕\��.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			//m_KumaParam[KNo].m_DispFlag = false;
			if (m_KumaParam[KNo].m_DispFlag == true) {
				m_pKumaSkin[KNo].SetTexChangeSystem(m_KumaParam[KNo].m_SkinCnt);
				m_pKumaSkin[KNo].SetPosition(m_KumaParam[KNo].m_Pos);//�\���ʒu���X�V.
				m_pKumaSkin[KNo].Render(mView, mProj, m_vLight, m_pCamera->vPos);
				m_pKumaSkin[KNo].SetRotation(m_KumaParam[KNo].m_Rot);
			}
		}

		//�u�e�v�̕\��.
		for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
			if (m_ShotParam[SNo].m_ShotFlag == true) {
				m_pShot->SetScale( 0.04f);
				m_pShot->SetPosition(m_ShotParam[SNo].m_Pos);//�\���ʒu���X�V.
				m_pShot->Render(mView, mProj, m_vLight, m_pCamera->vPos);
			}
		}

		//�u�����D�v�̕\��.
		if (m_BomParam.m_ShotFlag == true && WaterBallonDispCnt >= 0) {
			m_pBom->SetPosition(m_BomParam.m_Pos);//�\���ʒu�̍X�V.
			m_pBom->Render(mView, mProj, m_vLight, m_pCamera->vPos);
		}

		//�u�����D�͈́v�̕\��.
		m_pBomArea->SetPosition(m_HaniParam.m_Pos);
		m_pBomArea->Render(mView, mProj, m_vLight, m_pCamera->vPos);
		m_pBomArea->SetScale(0.4f);

		//�G���A�̕\��.
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
				m_P_Moveflag = true;	//�ړ��p.

			}

			//----------------------------------------
			//	�{�X���܂���̃{�[�����ʒu�����p.
			//----------------------------------------

			vHeadBoneOldPos = vHeadBonePos;
			vHeadBoneOldPos.y -= 0.4f;	//���̐^�񒆂ɗ���悤����.

			vBodyBoneOldPos = vBodyBonePos[0];

			vNeckBoneOldPos = vBodyBonePos[1];
			vNeckBoneOldPos.y -= 0.2f;	//��̈ʒu����.

			

			//�u�{�X���܂���v�̕\��
			m_pB_KumaSkin->SetPosition(m_BossKumaParam.m_Pos);
			m_pB_KumaSkin->Render(mView, mProj, m_vLight, m_pCamera->vPos);

			//�u�D�v�̕\��.
			for (int MNo = 0; MNo < MUD_MAX; MNo++) {
				if (m_MudParam[MNo].m_ShotFlag == true) {
					m_pMud->SetPosition(m_MudParam[MNo].m_Pos);//�\���ʒu���X�V.
					m_pMud->Render(mView, mProj, m_vLight, m_pCamera->vPos);
					m_pMud->SetScale(0.5f);
				}
			}

			//�{�X���܂���ƒe�̓����蔻��.
			for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
				for (int BONo = 0; BONo < 3; BONo++) {
					if (m_ShotParam[SNo].m_ShotFlag == true /*&& m_BossKumaParam.m_WinchFlag == true*/) {
						//���ƒe�̓����蔻��.
						if (Collision(m_pBHeadSphere, vHeadBoneOldPos, m_pShot, m_ShotParam[SNo].m_Pos) == true && m_BossKumaParam.m_SkinChangeCnt[HEAD] > 0) {
							m_ShotParam[SNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
							if (m_pBHideTex[HEAD].GetHideTexFlag() == false) {
								m_BossKumaParam.m_SkinChangeCnt[HEAD]--;
								m_B_HideParam[HEAD].m_Pos = vHeadBoneOldPos;	//�B���G�t�F�N�g�ʒu�����p.
								m_B_HideParam[HEAD].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);		//�ʒu����.
								m_pBHideTex[HEAD].HideTex_Disp(m_B_HideParam[HEAD].m_Pos);//�B���G�t�F�N�g�̈ʒu�Z�b�g.
								if (m_BossKumaParam.m_anim_no == BApproach) {
									m_BossKumaParam.m_HitCnt++;						//����������.
								}
							}
						}
						//�̂ƒe�̓����蔻��.
						if (Collision(m_pBBodySphere, vBodyBoneOldPos, m_pShot, m_ShotParam[SNo].m_Pos) == true || Collision(m_pBNeckSphere, vNeckBoneOldPos, m_pShot, m_ShotParam[SNo].m_Pos) == true) {
							if (m_BossKumaParam.m_SkinChangeCnt[BODY] > 0) {
								m_ShotParam[SNo].m_ShotFlag = false;
								m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
								if (m_pBHideTex[BODY].GetHideTexFlag() == false) {
									m_BossKumaParam.m_SkinChangeCnt[BODY]--;
									m_B_HideParam[BODY].m_Pos = vBodyBoneOldPos;
									m_B_HideParam[BODY].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
									m_pBHideTex[BODY].HideTex_Disp(m_B_HideParam[BODY].m_Pos);//�B���G�t�F�N�g�̈ʒu�Z�b�g.
									if (m_BossKumaParam.m_anim_no == BApproach) {
										m_BossKumaParam.m_HitCnt++;						//����������.
									}
								}
							}
						}
						//�E��ƒe�̓����蔻��.
						if (Collision(m_pBArmSphere, vArmRBonePos[BONo], m_pShot, m_ShotParam[SNo].m_Pos) == true && m_BossKumaParam.m_SkinChangeCnt[ARM_R] > 0) {
							m_ShotParam[SNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
							if (m_pBHideTex[ARM_R].GetHideTexFlag() == false) {
								m_BossKumaParam.m_SkinChangeCnt[ARM_R]--;//0:�E�r.
								m_B_HideParam[ARM_R].m_Pos = vArmRBonePos[1];
								m_B_HideParam[ARM_R].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
								m_pBHideTex[ARM_R].HideTex_Disp(m_B_HideParam[ARM_R].m_Pos);//�B���G�t�F�N�g�̈ʒu�Z�b�g.
								if (m_BossKumaParam.m_anim_no == BApproach) {
									m_BossKumaParam.m_HitCnt++;						//����������.
								}
							}
						}
						//����ƒe�̓����蔻��.
						if (Collision(m_pBArmSphere, vArmLBonePos[BONo], m_pShot, m_ShotParam[SNo].m_Pos) == true && m_BossKumaParam.m_SkinChangeCnt[ARM_L] > 0) {
							m_ShotParam[SNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
							if (m_pBHideTex[ARM_L].GetHideTexFlag() == false) {
								m_BossKumaParam.m_SkinChangeCnt[ARM_L]--;//1:���r.
								m_B_HideParam[ARM_L].m_Pos = vArmLBonePos[1];
								m_B_HideParam[ARM_L].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
								m_pBHideTex[ARM_L].HideTex_Disp(m_B_HideParam[ARM_L].m_Pos);//�B���G�t�F�N�g�̈ʒu�Z�b�g.					
								if (m_BossKumaParam.m_anim_no == BApproach) {
									m_BossKumaParam.m_HitCnt++;						//����������.
								}
							}
						}
					}
				}
			}

			//�����D�ƃ{�X���܂���̓����蔻��//�ǉ�.

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


			//�����D�ƓD�̓����蔻��.
			for (int MNo = 0; MNo < MUD_MAX; MNo++) {
				if (m_BomParam.m_BExpFlag == true && m_MudParam[MNo].m_ShotFlag == true) {
					//�D�p�̉B���e�N�X�`���ɓD�̍��W������.
					m_M_HideParam[MNo].m_Pos = D3DXVECTOR3(m_MudParam[MNo].m_Pos);
					m_M_HideParam[MNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
					m_pHideTex[MNo].HideTex_Disp(m_M_HideParam[MNo].m_Pos);
					//���������猩���Ȃ��Ƃ����.
					m_MudParam[MNo].m_ShotFlag = false;
					m_MudParam[MNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
				}
			}


			m_BomParam.m_BExpFlag = false;

			//�e�ƓD�̓����蔻��.
			for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
				for (int MNo = 0; MNo < MUD_MAX; MNo++) {
					if (m_ShotParam[SNo].m_ShotFlag == true && m_MudParam[MNo].m_ShotFlag == true) {
						if (Collision(m_pShot, m_ShotParam[SNo].m_Pos, m_pMud, m_MudParam[MNo].m_Pos) == true) {
							//�D�p�̉B���e�N�X�`���ɓD�̍��W������.
							m_M_HideParam[MNo].m_Pos = D3DXVECTOR3(m_MudParam[MNo].m_Pos);
							m_M_HideParam[MNo].m_Pos -= D3DXVECTOR3(2.0f, 1.5f, 1.0f);
							m_pHideTex[MNo].HideTex_Disp(m_M_HideParam[MNo].m_Pos);
							//���������猩���Ȃ��Ƃ����.
							m_MudParam[MNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
							m_MudParam[MNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_ShotFlag = false;
							m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);

						}
					}
				}
			}

			//�D�ƃJ�����̓����蔻��.
			for (int MNo = 0; MNo < MUD_MAX; MNo++) {
				//�D�̈ʒu���J�����𒴂�����.
				if (m_MudParam[MNo].m_Pos.z <= m_pCamera->vPos.z + 1.0f && m_MudParam[MNo].m_ShotFlag == true) {
					m_MudParam[MNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
					m_MudParam[MNo].m_ShotFlag = false;
					m_pPlayer->UpDamage();
					SESoundDrowBlaze(m_pPlayerDamageSE, m_DamageSEflag = true);
				}
			}	


		}

		//�p�ӂ��Ă���e�N�X�`�����ȏ�ɍs���Ȃ��悤�ɂ���.
		for (int TNo = 0; TNo < TEX_NO; TNo++) {
			if (m_BossKumaParam.m_SkinChangeCnt[TNo] <= 0) {
				m_BossKumaParam.m_SkinChangeCnt[TNo] = 0;
			}
		}

		//-------------------------------------------
		//���݂ڂ����܂̃e�N�X�`�����Z�b�g����.
		//-------------------------------------------
		//��.
		m_pCDxB_KumaMesh->SetChangeTexttureSystem_Atama(m_BossKumaParam.m_SkinChangeCnt[HEAD]);
		//��.
		m_pCDxB_KumaMesh->SetChangeTexttureSystem_Karada(m_BossKumaParam.m_SkinChangeCnt[BODY]);
		//�E�r.
		m_pCDxB_KumaMesh->SetChangeTexttureSystem_UdeR(m_BossKumaParam.m_SkinChangeCnt[ARM_R]);
		//���r.
		m_pCDxB_KumaMesh->SetChangeTexttureSystem_UdeL(m_BossKumaParam.m_SkinChangeCnt[ARM_L]);

		//===========================================

		//--------------------------------------------------------------------------------
		//	7_19�X�V(�g�c).
		//�X�N���[�����W���烏�[���h���W��.
		m_pMouse->CalcScreenToWorld(
			&m_WMousePos,
			static_cast<FLOAT>(m_pMouse->GetMousePosX()),static_cast<FLOAT>(m_pMouse->GetMousePosY()),
			0.0f, &mView, &mProj);
		//--------------------------------------------------------------------------------
		//	7_26�X�V(�g�c).
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//���C�ŃJ�[�\������I�u�W�F�N�g�i�n�ʂƂ��܂���j�Ƃ̓����蔻����Ƃ��Ă��������炻�̍��W���������Ă�����.
			CalcScreenToXZ(
				&m_HitPos,
			static_cast<FLOAT>(m_pMouse->GetMousePosX() + 8), static_cast<FLOAT>(m_pMouse->GetMousePosY() + 22),
				&mView, &mProj);
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{

			if (WaterBallonDispCnt > 0) {


				//���C�ŃJ�[�\������I�u�W�F�N�g�i�n�ʂƂ��܂���j�Ƃ̓����蔻����Ƃ��Ă��������炻�̍��W���������Ă�����.
				CalcScreenToXZ(
					&m_HitPos,
					static_cast<FLOAT>(m_pMouse->GetMousePosX() + 8), static_cast<FLOAT>(m_pMouse->GetMousePosY() + 22),
					&mView, &mProj);
			}
		}
		//--------------------------------------------------------------------------------




		//���܂���p�̉B���G�t�F�N�g�\��.
		for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
			if (m_pHideTex[KNo].GetHideTexFlag() == true) {
				m_KumaParam[KNo].m_TexChangeFlag = true;

				SetDepth(false);//Zý�:OFF.
				m_pHideTex[KNo].Move();
				m_pHideTex[KNo].Load(mView, mProj, m_pCamera->vPos);
				m_pHideTex[KNo].Attach(m_pHide);
				m_pHideTex[KNo].Render();
				SetDepth(true);	//Zý�:ON.
			}
			if (m_pHideTex[KNo].GetHideTexFlag() == false) {
				m_KumaParam[KNo].m_TexChangeFlag = false;
			}
		}

		//�{�X���܂���p�B���G�t�F�N�g�\��.
		for (int TNo = 0; TNo < TEX_NO; TNo++) {
			if (m_pBHideTex[TNo].GetHideTexFlag() == true) {
				SetDepth(false);//Zý�:OFF.
				m_pBHideTex[TNo].Move();
				m_pBHideTex[TNo].Load(mView, mProj, m_pCamera->vPos);
				m_pBHideTex[TNo].Attach(m_pHide);
				m_pBHideTex[TNo].Render();
				SetDepth(true);	//Zý�:ON.
			}
		}

		//�����D�p�B���G�t�F�N�g�\��
		if (m_pBomHideTex->GetHideTexFlag() == true)
		{
			SetDepth(false);
			m_pBomHideTex->Move();
			m_pBomHideTex->Load(mView, mProj, m_pCamera->vPos);
			m_pBomHideTex->Attach(m_pHide);
			m_pBomHideTex->Render();
			SetDepth(true);
		}
		//���g�̃_���[�WUI�̕\��.
		P_DamageRender();//�v���C���[�_���[�W�Ń����_�����O�ɒu���Ă���.

		//===========================================================================================
		//		��������������Ă���.
		//===========================================================================================
		//�g�c��	SetPosition��SetPattern�̂ǂ��炩���g���Ċ撣����

		//�����DUI�̕\��,
		for (int i = 0; i < WaterBallonDispCnt; i++) {
			float value_z = 0.16f - (i * 0.01f);
			m_pWaterBalloon[i]->SetPosition(900.0f + (120.0f * i), 550.0f, value_z);
			m_pWaterBalloon[i]->Render();
		}

		//�Q�[�W�̕\��.
		GaugeRender();		//�Q�[�W�Ń����_�����O�ɒu���Ă���.


		if (m_enStageScene == enStageScene::enMission)
		{
			//���܂���̃~�b�V����.
			m_pBearMission->SetPosition(0.0f, 0.0f, 0.13f);
			m_pBearMission->Render();

			//��������.
			for (int i = 0; i < 2; i++) {
				m_TimeCnt[i] = m_pTime->SetDigits();
				m_pTime->PutDigits();
				m_pTimeUI->SetPattern(m_TimeCnt[i], 1);
				m_pTimeUI->SetPosition(1230 - (i * 45.0f), 30.0f, 0.12f);
				m_pTimeUI->Render();
			}
			//�~�b�V�����N���A���邽�߂̐�.
			m_pMissionUI->SetPattern(CLEARMISSION, 1);
			m_pMissionUI->SetPosition(350.0f, 120.0f, 0.11f);
			m_pMissionUI->Render();

			//�Y��ɂ�����.
			m_BearCleanDigit = m_BearCleanCnt;
			m_pInGameCleanUI->SetPattern(m_BearCleanDigit, 1);
			m_pInGameCleanUI->SetPosition(200.0f, 120.0f, 0.10f);
			m_pInGameCleanUI->Render();
		}
		if (m_enStageScene == enStageScene::enBoss)
		{
			//�{�X���܂���̃~�b�V����.
			m_pBossMission->SetPosition(0.0f, 0.0f, 0.10f);
			m_pBossMission->Render();
		}

		break;
	case CMain::enScene::enGameOver:
		//�Q�[���I�[�o�[���S.
		m_pOverUI->SetPosition(WND_W_HALF - (520.0f / 2), 50.0f, 0.40f);
		m_pOverUI->Render();
		//�Q�[������.
		m_pOverWord->SetPosition(WND_W_HALF - (1112.0f / 2), 300.0f, 0.41f);
		m_pOverWord->Render();
		//�Q�[���ɖ߂�{�^��.
		m_pRetryButton->SetPosition(300.0f, 500.0f, 0.42f);
		m_pRetryButton->Render();
		//�^�C�g���ɖ߂�{�^��.
		m_pBackButton->SetPosition(800.0f, 500.0f, 0.43f);
		m_pBackButton->Render();
		break;
	case CMain::enScene::enGameClear:
		//�Y��ɂ�����.
		m_BearCleanDigit = m_BearCleanCnt;
		for (int i = 0; i < 2; i++) {
			m_InsideBearClean[i] = m_BearCleanDigit % 10;
			m_BearCleanDigit /= 10;
			m_pCleanUI->SetPattern(m_InsideBearClean[i], 1);
			m_pCleanUI->SetPosition(700.0f - (i * 55.0f), 210.0f, 0.40f);
			m_pCleanUI->Render();
		}

		//�{�X���܂�����Y��ɂ�����.
		m_BossCleanDigit = m_BossCleanCnt;
		for (int i = 0; i < 2; i++) {
			m_InsideBossClean[i] = m_BossCleanDigit % 10;
			m_BossCleanDigit /= 10;
			m_pBossCleanUI->SetPattern(m_InsideBossClean[i], 1);
			m_pBossCleanUI->SetPosition(700.0f - (i * 55.0f), 290.0f, 0.41f);
			m_pBossCleanUI->Render();
		}
		//�Y��ɂ������v��.
		m_TotalBearDigit = m_TotalBearCnt;
		for (int i = 0; i < 2; i++) {
			m_InsideTotalBear[i] = m_TotalBearDigit % 10;
			m_TotalBearDigit /= 10;
			m_pTotalBearUI->SetPattern(m_InsideTotalBear[i], 1);
			m_pTotalBearUI->SetPosition(700.0f - (i * 55.0f), 370.0f, 0.42f);
			m_pTotalBearUI->Render();
		}

		//�N���A���S.
		m_pClearUI->SetPosition(WND_W_HALF - (984.0f/2), WND_H_HALF - (686.0f/2), 0.43f);
		m_pClearUI->Render();
		//�^�C�g���ɖ߂�{�^��.
		m_pReturnButton->SetPosition(WND_W - 112.0f,0.0f, 0.44f);
		m_pReturnButton->Render();

		break;
	default:
		break;
	}


	//�΂̕\��.
	for (int i = 0; i < STONE_MAX; i++) {
		m_pStone->SetPosition(m_pStone->GetStonePos(i));
		m_pStone->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}
	//���̕\��.
	for (int i = 0; i < GRASS_MAX; i++) {
		m_pGrass->SetPosition(m_pGrass->GetGrassPos(i));
		m_pGrass->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}
	//���S�C�̕\��.
	if (m_GunDispFlag == true) {
		//���S�C�̃p�[�c��\��.
		m_pGunParts->Render(mView, mProj, m_vLight, m_pCamera->vPos);
		//�|���v�̕\��.
		m_pPump->Render(mView, mProj, m_vLight, m_pCamera->vPos);
	}

	//�t�F�C�h�̕\��.
	if (m_pFade->GetFadeFlag() >= 1) {
		m_pFade->Fade_in();
		m_pFade->Attach(m_pUIFade);
		m_pFade->Render();
	}


	//�����ݸނ��ꂽ�Ұ�ނ�\��.
	m_pSwapChain->Present(0, 0);
}

/************************************************************
*	��������3D.
*/
//DirectX������.
HRESULT CMain::InitD3D()
{
	//---------------------------------------------------------
	//	���޲��ƽܯ�����݊֌W.
	//---------------------------------------------------------

	//�ܯ�����ݍ\����.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));//0�ŏ�����.
	sd.BufferCount = 1;	//�ޯ��ޯ̧�̐�.
	sd.BufferDesc.Width = WND_W;//�ޯ��ޯ̧�̕�.
	sd.BufferDesc.Height = WND_H;//�ޯ��ޯ̧�̍���.
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//̫�ϯ�(32�ޯĶװ).
	sd.BufferDesc.RefreshRate.Numerator = 60;//��گ��ڰ�(����) ��FPS:60.
	sd.BufferDesc.RefreshRate.Denominator = 1;//��گ��ڰ�(���q).
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//�g����(�\����).
	sd.OutputWindow = m_hWnd;//����޳�����.
	sd.SampleDesc.Count = 1;//�������ق̐�.
	sd.SampleDesc.Quality = 0;//�������ق̸��è.
	sd.Windowed = TRUE;//����޳Ӱ��(�ٽ�ذݎ���FALSE).

	//�쐬�����݂�@�\���ق̗D����w��.
	// (GPU����߰Ă���@�\��Ă̒�`).
	// D3D_FEATURE_LEVEL�񋓌^�̔z��.
	// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 �� GPU����.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;//�z��̗v�f��.

	//���޲��ƽܯ�����݂̍쐬.
	//ʰ�޳��(GPU)���޲��ł̍쐬.
	if (FAILED(
		D3D11CreateDeviceAndSwapChain(
			nullptr,		//���޵�������ւ��߲��.
			D3D_DRIVER_TYPE_HARDWARE,//�쐬�������޲��̎��.
			nullptr,		//��ĳ�� ׽�ײ�ނ���������DLL�������.
			0,				//�L���ɂ��������ڲ԰.
			&pFeatureLevels,//�쐬�����݂�@�\���ق̏������w�肷��z��ւ��߲��.
			1,				//���̗v�f��.
			D3D11_SDK_VERSION,//SDK���ް�ޮ�.
			&sd,			//�ܯ�����݂̏��������Ұ����߲��.
			&m_pSwapChain,	//(out)�����ݸނɎg�p����ܯ������.
			&m_pDevice11,	//(out)�쐬���ꂽ���޲�.
			pFeatureLevel,	//�@�\���ق̔z��ɂ���ŏ��̗v�f��\���߲��.
			&m_pContext11)))//(out)���޲� ��÷��.
	{
		//WARP���޲��̍쐬.
		// D3D_FEATURE_LEVEL_9_1�`D3D_FEATURE_LEVEL_10_1.
		if (FAILED(
			D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
				0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
				&sd, &m_pSwapChain, &m_pDevice11,
				pFeatureLevel, &m_pContext11)))
		{
			//�̧�ݽ���޲��̍쐬.
			// DirectX SDK���ݽİق���Ă��Ȃ��Ǝg���Ȃ�.
			if (FAILED(
				D3D11CreateDeviceAndSwapChain(
					nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr,
					0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
					&sd, &m_pSwapChain, &m_pDevice11,
					pFeatureLevel, &m_pContext11)))
			{
				MessageBox(nullptr,
					"���޲��ƽܯ�����ݍ쐬���s",
					"Error", MB_OK);
				return E_FAIL;
			}
		}
	}

	//�e��ø����Ƃ���ɕt�т���e���ޭ�(���)���쐬.

	//----------------------------------------------------------
	//	�ޯ��ޯ̧�����F�װ�ޯ̧�ݒ�.
	//----------------------------------------------------------
	//�ޯ��ޯ̧ø������擾(���ɂ���̂ō쐬�ł͂Ȃ�).
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:���Ɋ֘A�t�����ꂽGUID���擾
									//         Texture2D�̗B��̕��Ƃ��Ĉ���
		(LPVOID*)&pBackBuffer_Tex);	//(out)�ޯ��ޯ̧ø���.

	//����ø����ɑ΂������ް���ޯ��ޭ�(RTV)���쐬.
	m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV);	//(out)RTV.
	//�ޯ��ޯ̧ø��������.
	SAFE_RELEASE(pBackBuffer_Tex);

	//----------------------------------------------------------
	//	�ޯ��ޯ̧�����F���߽(�[�x)��ݼي֌W.
	//----------------------------------------------------------
	//���߽(�[��or�[�x)��ݼ��ޭ��p��ø������쐬.
	D3D11_TEXTURE2D_DESC	descDepth;
	descDepth.Width = WND_W;	//��.
	descDepth.Height = WND_H;	//����.
	descDepth.MipLevels = 1;	//Я��ϯ������:1.
	descDepth.ArraySize = 1;	//�z��:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;//32�ޯ�̫�ϯ�.
	descDepth.SampleDesc.Count = 1;		//�������ق̐�.
	descDepth.SampleDesc.Quality = 0;	//�������ق̸��è.
	descDepth.Usage = D3D11_USAGE_DEFAULT;//�g�p���@:��̫��.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//�[�x(��ݼقƂ��Ďg�p).
	descDepth.CPUAccessFlags = 0;	//CPU����ͱ������Ȃ�.
	descDepth.MiscFlags = 0;		//���̑��̐ݒ�Ȃ�.

	m_pDevice11->CreateTexture2D(
		&descDepth,
		nullptr,
		&m_pBackBuffer_DSTex);	//(out)���߽��ݼٗpø���.

	//����ø����ɑ΂������߽��ݼ��ޭ�(DSV)���쐬.
	m_pDevice11->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		nullptr,
		&m_pBackBuffer_DSTexDSV);	//(out)DSV.

	//���ް���ޯ��ޭ������߽��ݼ��ޭ����߲��ײ݂ɾ��.
	m_pContext11->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV);

	//�[�xý�(Zý�)��L���ɂ���.
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable		= TRUE;//�L��.
	dsDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable	= FALSE;
	dsDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	//�[�x�ݒ�쐬.
	if (FAILED(	
		m_pDevice11->CreateDepthStencilState(
			&dsDesc, &m_pDepthStencilState)))
	{
		_ASSERT_EXPR(false, L"�[�x�ݒ�쐬���s");
		return E_FAIL;
	}
	//�[�xý�(Z)ýĂ𖳌��ɂ���.
	dsDesc.DepthEnable = FALSE;//����.
	//�[�x�ݒ�쐬.
	if (FAILED(
		m_pDevice11->CreateDepthStencilState(
			&dsDesc, &m_pDepthStencilStateOff)))
	{
		_ASSERT_EXPR(false, L"�[�x�ݒ�쐬���s");
		return E_FAIL;
	}

	//------------------------------------------------
	//	�ޭ��߰Đݒ�.
	//------------------------------------------------
	D3D11_VIEWPORT vp;
	vp.Width = WND_W;	//��.
	vp.Height = WND_H;	//����.
	vp.MinDepth = 0.0f;	//�ŏ��[�x(��O).
	vp.MaxDepth = 1.0f;	//�ő�[�x(��).
	vp.TopLeftX = 0.0f;	//����ʒux.
	vp.TopLeftY = 0.0f;	//����ʒuy.

	m_pContext11->RSSetViewports(1, &vp);


	//------------------------------------------------
	//	׽�ײ��(�ʂ̓h��Ԃ���)�̐ݒ�.
	//------------------------------------------------
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;//�h��Ԃ�(�د��).
	rdc.CullMode = D3D11_CULL_NONE;	//D3D11_CULL_BACK:�w�ʂ�`�悵�Ȃ�.
									//D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
									//D3D11_CULL_NONE:��ݸނ�؂�(���w�ʂ�`�悷��).
	rdc.FrontCounterClockwise = FALSE;//��غ�݂̕\�������肷���׸�.
									//TRUE:�����Ȃ�O�����B�E���Ȃ�������B
									//FALSE:�t�ɂȂ�.
	rdc.DepthClipEnable = FALSE;	//�����ɂ��Ă̸د��ݸޗL��.

	ID3D11RasterizerState* pRs = nullptr;
	m_pDevice11->CreateRasterizerState(&rdc, &pRs);
	m_pContext11->RSSetState(pRs);
	SAFE_RELEASE(pRs);




	return S_OK;
}

//Direct3D�I������.
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

	//���b�V���̉��.
	SAFE_DELETE(m_pBomArea);
	//UI�̉��.
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

	//�X�t�B�A�̉��.
	SAFE_DELETE(m_pBArmSphere);
	SAFE_DELETE(m_pBNeckSphere);
	SAFE_DELETE(m_pBBodySphere);
	SAFE_DELETE(m_pBHeadSphere);
	SAFE_DELETE(m_pSphere);

	//�X�L�����b�V���̉��.
	SAFE_DELETE(m_pB_KumaSkin);
	SAFE_DELETE(m_pCDxB_KumaMesh);
	SAFE_DELETE_ARRAY(m_pKumaSkin);
	SAFE_DELETE(m_pCDxKumaMesh);
		

	

	//���ق̉��.
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexBuffer);

	//����ނ����.
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);

	//Direct3D��޼ު�Ă����.
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pContext11);
	SAFE_RELEASE(m_pDevice11);
}

//ү���̓ǂݍ��݊֐�.
HRESULT CMain::LoadMesh()
{	
	//�m�F�p�ɽ̨���ǂݍ���.
	m_pSphere = new CDX9Mesh();
	m_pSphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x",true);
	InitSphere(m_pSphere);	//�̨����޳��ިݸ޽̨��쐬.

	//-------------------------------------------
	//�{�X���܂���e�N�X�`���؂�ւ��p
	//-------------------------------------------
	//�m�F�p�ɽ̨���ǂݍ���.
	//�r.
	m_pBArmSphere = new CDX9Mesh();
	m_pBArmSphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x",true);
	InitSphere(m_pBArmSphere);	//�̨����޳��ިݸ޽̨��쐬.
	m_pBArmSphere->m_Sphere.fRadius = 0.15f;	//�����蔻��̔��a.
	
	//�m�F�p�ɽ̨���ǂݍ���.
	//��.
	m_pBHeadSphere = new CDX9Mesh();
	m_pBHeadSphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x", true);
	InitSphere(m_pBHeadSphere);	//�̨����޳��ިݸ޽̨��쐬.
	m_pBHeadSphere->m_Sphere.fRadius = 0.3f; //�����蔻��̔��a.//���a�Ȃ̂�Scale�̒l�̑傫���ɂ������ꍇ�Q����1���Ȃ��Ƃ����Ȃ�.
	
	

	//�m�F�p�ɽ̨���ǂݍ���.
	//����.
	m_pBBodySphere = new CDX9Mesh();
	m_pBBodySphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x", true);
	InitSphere(m_pBBodySphere);	//�̨����޳��ިݸ޽̨��쐬.
	m_pBBodySphere->m_Sphere.fRadius = 0.3f; //�����蔻��̔��a.

	//��.
	m_pBNeckSphere = new CDX9Mesh();
	m_pBNeckSphere->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Sphere.x", true);
	InitSphere(m_pBNeckSphere);	//�̨����޳��ިݸ޽̨��쐬.
	m_pBNeckSphere->m_Sphere.fRadius = 0.20f; //�����蔻��̔��a.

	//--------------------------------------
	//	ү���ǂݍ���.
	//--------------------------------------
	//�u�G�@�v.
	m_pEnemy = new CDX9Mesh();
	m_pEnemy->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Grass\\kusa.x", true);
	//�u�e�v.
	m_pShot = new CDX9Mesh();
	m_pShot->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Bullet\\mizu.x", true);
	//�u�D�v.
	m_pMud = new CDX9Mesh();
	m_pMud->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Bullet\\doro.x", true);
	//----------------------------------
	// �g�c�X�V 2019_10_17.

	//�u�����D�v.
	m_pBom = new CDX9Mesh();
	m_pBom->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\WaterBom\\mizuhuusen.x", true);

	//��ƂȂ��޳��ިݸ޽̨��̏����e���قɾ�Ă���.
	m_pEnemy->m_Sphere = m_pSphere->m_Sphere;
	m_pEnemy->m_Sphere.fRadius = 1.0f;	
	m_pMud->m_Sphere = m_pSphere->m_Sphere;
	//�e�͕ʻ��ނ��g�p����.
	InitSphere(m_pShot);
	m_pShot->m_Sphere.fRadius = 0.15f;

	////ү�������޳��ިݸ��ޯ���̍쐬.
	//InitBBox(m_pPlayer);
	//InitBBox(m_pEnemy);

	m_pHideTex = new CHideTex[BEAR_MAX]();

	
	m_pBHideTex = new CHideTex[TEX_NO]();

	m_pBomHideTex = new CHideTex();
	//-------------------------------------------
	//�g�c.
	//���ײ�UI�̏�����.
	//�B���G�t�F�N�g�̏�����.
	m_pHide = new CSprite();
	//���摜�傫���A�@��R�}������̑傫���A�X�P�[���@�@�@�@
	Sprite(900.0f, 600.0f, 900.0f / 3, 600.0f / 2, 4.0f, 4.0f, "Data\\Texture\\mizu.png", m_pHide);
	
	//-----------------------------------------

	//----------------------------
	// �g�c�@�X�V 2019_10_17.

	//-----------------------------------------------------
	//	���܂���ǂݍ���.
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
	//	m_pKumaSkin[KNo].SetAnimSpeed(0.02);//�A�j���[�V�������x�ݒ�.
	//}
		
	//---------------------------------------------------
	//	���܂���p�e�N�X�`���؂�ւ�����.
	//---------------------------------------------------
	
	m_pChangeTextures = std::make_shared<CDX9SkinMesh::CHANGE_TEXTURES>();
	
	strcpy_s(m_pChangeTextures->szTextureName_2, "Data\\ModelSkin\\Kuma\\kumasan_b_3.png");		//�Y��.
	strcpy_s(m_pChangeTextures->szTextureName, "Data\\ModelSkin\\Kuma\\kumasan_b_2.png");	//��������.
	strcpy_s(m_pChangeTextures->szTargetTextureName, "kumasan_b_1.png");		//����(�f�t�H���g).			
	//�e�N�X�`���쐬.
	if (m_pChangeTextures->szTargetTextureName != 0 &&
		FAILED(
			D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11, m_pChangeTextures->szTextureName,
				nullptr, nullptr, &m_pChangeTextures->pTexture, nullptr)))
	{
		MessageBox(NULL, "�e�N�X�`���ǂݍ��ݎ��s",
			"Error", MB_OK);
		return E_FAIL;
	}
	//�e�N�X�`���쐬2.
	if (m_pChangeTextures->szTargetTextureName != 0 &&
		FAILED(
			D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11, m_pChangeTextures->szTextureName_2,
				nullptr, nullptr, &m_pChangeTextures->pTextureTest, nullptr)))
	{
		MessageBox(NULL, "�e�N�X�`���ǂݍ��ݎ��s",
			"Error", MB_OK);
		return E_FAIL;
	}

	m_pCDxKumaMesh->SetChangeTextures(m_pChangeTextures);

	
	


	//---------------------------------------
	//	�{�X���܂���ǂݍ���.
	//---------------------------------------
	//----------------------------------
	// �g�c�X�V 2019_10_17.
	m_pCDxB_KumaMesh = new CDX9SkinMesh();
	m_pCDxB_KumaMesh->Init(&si, "Data\\ModelSkin\\BossKuma\\BossKuma.X");

	//m_pCDxSkinMesh->m_vPos.y = 0.01f;//�\���ʒu��n�ʂ���ɂ��Ă���.

	//m_pB_KumaSkin = new CCharacter();
	//m_pB_KumaSkin->SetScale(0.05f);
	//m_pB_KumaSkin->AttachModel(m_pCDxB_KumaMesh);
	//m_pB_KumaSkin->SetAnimSpeed(0.01);//�A�j���[�V�������x�ݒ�.

				


	m_pBChangeTextures = new CDX9SkinMesh::CHANGE_TEXTURES[TEX_NO];
	

	//��.
	strcpy_s(m_pBChangeTextures[HEAD].szTextureName_2, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_atama_1.png");		//�Y��.
	strcpy_s(m_pBChangeTextures[HEAD].szTextureName, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_atama_2.png");	//��������.
	strcpy_s(m_pBChangeTextures[HEAD].szTargetTextureName, "bosskuma_uv_atama_3.png");		//����(�f�t�H���g).			
	
	//��.
	strcpy_s(m_pBChangeTextures[BODY].szTextureName_2, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_karada_1.png");		//�Y��.
	strcpy_s(m_pBChangeTextures[BODY].szTextureName, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_karada_2.png");	//��������.
	strcpy_s(m_pBChangeTextures[BODY].szTargetTextureName, "bosskuma_uv_karada_3.png");		//����(�f�t�H���g).			
	
	//�E�r.
	strcpy_s(m_pBChangeTextures[ARM_R].szTextureName_2, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_asi_1.png");		//�Y��.
	strcpy_s(m_pBChangeTextures[ARM_R].szTextureName, "Data\\ModelSkin\\BossKuma\\bosskuma_uv_asi_2.png");	//��������.
	strcpy_s(m_pBChangeTextures[ARM_R].szTargetTextureName, "bosskuma_uv_asi_3.png");		//����(�f�t�H���g).			
	
	//���r.
	strcpy_s(m_pBChangeTextures[ARM_L].szTextureName_2,	 "Data\\ModelSkin\\BossKuma\\bosskuma_uv_asi_1.png");		//�Y��.
	strcpy_s(m_pBChangeTextures[ARM_L].szTextureName,		 "Data\\ModelSkin\\BossKuma\\bosskuma_uv_asi_2.png");	//��������.
	strcpy_s(m_pBChangeTextures[ARM_L].szTargetTextureName, "bosskuma_uv_asi_3.png");		//����(�f�t�H���g).		


	for (int BNo = 0; BNo < TEX_NO; BNo++) {
		//�e�N�X�`���쐬.
		if (m_pBChangeTextures[BNo].szTargetTextureName != 0 &&
			FAILED(
				D3DX11CreateShaderResourceViewFromFileA(
					m_pDevice11, m_pBChangeTextures[BNo].szTextureName,
					nullptr, nullptr, &m_pBChangeTextures[BNo].pTexture, nullptr)))
		{
			MessageBox(NULL, "�e�N�X�`���ǂݍ��ݎ��s",
				"Error", MB_OK);
			return E_FAIL;
		}
		//�e�N�X�`���쐬2.
		if (m_pBChangeTextures[BNo].szTargetTextureName != 0 &&
			FAILED(
				D3DX11CreateShaderResourceViewFromFileA(
					m_pDevice11, m_pBChangeTextures[BNo].szTextureName_2,
					nullptr, nullptr, &m_pBChangeTextures[BNo].pTextureTest, nullptr)))
		{
			MessageBox(NULL, "�e�N�X�`���ǂݍ��ݎ��s",
				"Error", MB_OK);
			return E_FAIL;
		}
	}
	m_pCDxB_KumaMesh->SetBChangeTextures(&m_pBChangeTextures[HEAD]);
	m_pCDxB_KumaMesh->SetBChangeTextures1(&m_pBChangeTextures[BODY]);
	m_pCDxB_KumaMesh->SetBChangeTextures2(&m_pBChangeTextures[ARM_L]);
	m_pCDxB_KumaMesh->SetBChangeTextures3(&m_pBChangeTextures[ARM_R]);


	
	//--------------------------------------------------

	
	//�e��޼ު�����Ұ��̏�����.

	m_pPlayer = new CPlayer();
	m_pPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.9f, 4.0f));
	m_pPlayer->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�u�J�����v.
	m_pCamera = new CCamera();
	//�u�}�E�X�v.
	m_pMouse = new CMouse();
	//�u�t�F�C�h�v.
	m_pFade = new CFade();
	//�u���ԁv.
	m_pTime = new CTime();
	//�u�͈́v.
	m_pArea = new CArea();
	

	//	m_PlayerParam.m_ShotFlag = false;
	//for (int KNo = 0; KNo > ENMY_MAX; KNo++) {
	//	m_KumaParam[KNo].m_Pos = D3DXVECTOR3(1.0f*KNo, 0.7f, 20.0f);
	//}
	
	//�n��(���b�V���I�u�W�F�N�g�N���X).
	m_pGround = new CGround[GROUND_MAX]();
	m_pGround->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);

	//���̒n��.
	m_pGrassGround = new CGrassGround[GRASS_GROUND_MAX]();
	m_pGrassGround->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);

	//��.
	m_pStone = new CStone[STONE_MAX]();
	m_pStone->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);

	//��.
	m_pGrass = new CGrass[GRASS_MAX]();
	m_pGrass->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);

	//�w�i(���b�V���I�u�W�F�N�g�N���X).
	m_pBackground = new CBackground();
	m_pBackground->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);
	
	//���S�C�̃p�[�c.
	m_pGunParts = new CGunParts();
	m_pGunParts->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);
	
	//���S�C�̃|���v.
	m_pPump = new CPump();
	m_pPump->LoadMesh(m_hWnd, m_pDevice11, m_pContext11);
	//�u�͈́v.
	m_pBomArea = new CDX9Mesh();
	m_pBomArea->Init(m_hWnd, m_pDevice11, m_pContext11,
		"Data\\Model\\Way\\mizutamri.x", true);

	FileDataBear();
	
	return S_OK;
}

//UI�̓ǂݍ��݊֐�.
HRESULT CMain::LoadUI()
{
	//�����Q�[�WUI.
	m_pDecremetGauge = new CSpriteUI();
	UI_Display(368.0f, 88.0f, 368.0f, 88.0f, 368.0f, 88.0f, "Data\\Texture\\PlayerGauge\\ge-zi.png", m_pDecremetGauge);
	//�Q�[�W�o�[UI.
	m_pGaugeBar = new CSpriteUI();
	UI_Display(398.0f, 88.0f, 398.0f, 88.0f, 398.0f, 88.0f, "Data\\Texture\\PlayerGauge\\waku.png", m_pGaugeBar);
	//���܂���̃A�C�R��.
	m_pBossBearIcon = new CSpriteUI();
	UI_Display(142.0f, 238.0f, 142.0f, 238.0f, 142.0f, 238.0f, "Data\\Texture\\PlayerGauge\\ge-zi_bosskuma.png", m_pBossBearIcon);
	
	//�X�y�[�X�L�[UI.
	m_pSpaceBar = new CSpriteUI();
	UI_Display(380.0f, 96.0f, 380.0f, 96.0f, 380.0f, 96.0f, "Data\\Texture\\PlayerGauge\\supe-su_2.png", m_pSpaceBar);
	//�X�y�[�X�L�[���S.
	m_pSpaceLogo = new CSpriteUI();
	UI_Display(62.0f, 70.0f, 62.0f, 70.0f, 62.0f, 70.0f, "Data\\Texture\\PlayerGauge\\riro-do.png", m_pSpaceLogo);

	//�����DUI�̏�����.
	for (int i = 0; i < WATER_BALLOON; i++) {
		m_pWaterBalloon[i] = new CSpriteUI();
		UI_Display(118.0f, 132.0f, 118.0f, 132.0f, 118.0f, 132.0f, "Data\\Texture\\WaterBalloon.png", m_pWaterBalloon[i]);
	}
	//�~�b�V����(���܂���p).
	m_pBearMission = new CSpriteUI();
	UI_Display(585.0f, 180.0f, 585.0f, 180.0f, 585.0f, 180.0f, "Data\\Texture\\TextMisiion\\kumasanwokireinisite.png", m_pBearMission);
	//�~�b�V����(�{�X���܂���p).
	m_pBossMission = new CSpriteUI();
	UI_Display(585.0f, 180.0f, 585.0f, 180.0f, 585.0f, 180.0f, "Data\\Texture\\TextMisiion\\bosskumasanwokireinisite.png", m_pBossMission);
	//�Y��ɂȂ�����(�Q�[������UI).
	m_pInGameCleanUI = new CSpriteUI();
	UI_Display(378.0f, 40.0f, 378.0f / 10, 40.0f, 378.0f / 10, 40.0f, "Data\\Texture\\DebugText\\suuzi_kumasanyou.png", m_pInGameCleanUI);
	//�~�b�V�����N���A���邽�߂̐�.
	m_pMissionUI = new CSpriteUI();
	UI_Display(378.0f, 40.0f, 378.0f / 10, 40.0f, 378.0f / 10, 40.0f, "Data\\Texture\\DebugText\\suuzi_kumasanyou.png", m_pMissionUI);
	//�Y��ɂȂ�����UI.
	m_pCleanUI = new CSpriteUI();
	UI_Display(520.0f, 55.0f, 520.0f / 10, 55.0f, 520.0f / 10, 55.0f, "Data\\Texture\\GameClear\\suuzi_taimu_ge-mukuriayou.png", m_pCleanUI);
	//�Y��ɂȂ�����UI.
	m_pBossCleanUI = new CSpriteUI();
	UI_Display(520.0f, 55.0f, 520.0f / 10, 55.0f, 520.0f / 10, 55.0f, "Data\\Texture\\GameClear\\suuzi_taimu_ge-mukuriayou.png", m_pBossCleanUI);
	//�Y��ɂȂ�����UI.
	m_pTotalBearUI = new CSpriteUI();
	UI_Display(520.0f, 55.0f, 520.0f / 10, 55.0f, 520.0f / 10, 55.0f, "Data\\Texture\\GameClear\\suuzi_taimu_ge-mukuriayou.png", m_pTotalBearUI);
	//�Y��ɂȂ�����UI.
	m_pTimeUI = new CSpriteUI();
	UI_Display(378.0f, 40.0f, 378.0f / 10, 40.0f, 378.0f / 10, 40.0f, "Data\\Texture\\DebugText\\suuzi_kumasanyou.png", m_pTimeUI);

	//======================���g�̉����=================================.
	//�D�̉���p�^�[��1�̏�����.
	m_pMudDamage_1 = new CSpriteUI();
	UI_Display(3508.0f, 2480.0f, 3508.0f, 2480.0f, 1280.0f, 720.0f, "Data\\Texture\\PlayerDamage\\MudDamage_1.png", m_pMudDamage_1);
	//�D�̉���p�^�[��2�̏�����.
	m_pMudDamage_2 = new CSpriteUI();
	UI_Display(3508.0f, 2480.0f, 3508.0f, 2480.0f, 1280.0f, 720.0f, "Data\\Texture\\PlayerDamage\\MudDamage_2.png", m_pMudDamage_2);
	//�D�̉���p�^�[��3�̏�����.
	m_pMudDamage_3 = new CSpriteUI();
	UI_Display(3508.0f, 2480.0f, 3508.0f, 2480.0f, 1280.0f, 720.0f, "Data\\Texture\\PlayerDamage\\MudDamage_3.png", m_pMudDamage_3);

	//===================================�^�C�g����ʂŎg����================================================
	//�^�C�g��UI(���S).
	m_pTitle = new CSpriteUI();
	UI_Display(534.0f, 428.0f, 534.0f, 428.0f, 534.0f, 428.0f, "Data\\Texture\\Title\\taitoru.png", m_pTitle);
	//�^�C�g����i�߂鎞�N���b�N���鏊.
	m_pStartUI = new CSpriteUI();
	UI_Display(185.0f, 185.0f, 185.0f, 185.0f, 185.0f, 185.0f, "Data\\Texture\\Title\\suta-tobatan.png", m_pStartUI);
	//�^�C�g����ʂŃQ�[������߂鎞�N���b�N���鏊.
	m_pEndUI = new CSpriteUI();
	UI_Display(110.0f, 110.0f, 110.0f, 110.0f, 110.0f, 110.0f, "Data\\Texture\\Title\\batubotan.png", m_pEndUI);

	//===================================�N���A��ʂŎg����=============================================.
	//�f����.
	m_pClearUI = new CSpriteUI();
	UI_Display(984.0f, 686.0f, 984.0f, 686.0f, 984.0f, 686.0f, "Data\\Texture\\GameClear\\ge-mukuria.png", m_pClearUI);
	//�߂�{�^��.
	m_pReturnButton = new CSpriteUI();
	UI_Display(112.0f, 112.0f, 112.0f, 112.0f, 112.0f, 112.0f, "Data\\Texture\\GameClear\\yazirusi.png", m_pReturnButton);

	//===================================�Q�[���I�[�o�[��ʂŎg����=============================================.
	//�Q�[���I�[�o�[���S.
	m_pOverUI = new CSpriteUI();
	UI_Display(520.0f, 228.0f, 520.0f, 228.0f, 520.0f, 228.0f, "Data\\Texture\\GameOver\\doronkokumasan.png", m_pOverUI);
	//�Q�[���I�[�o�[����.
	m_pOverWord = new CSpriteUI();
	UI_Display(1112.0f, 122.0f, 1112.0f, 122.0f, 1112.0f, 122.0f, "Data\\Texture\\GameOver\\doronkoninattyatta-.png", m_pOverWord);
	//�Q�[���ɖ߂�{�^��.
	m_pRetryButton = new CSpriteUI();
	UI_Display(205.0f, 205.0f, 205.0f, 205.0f, 205.0f, 205.0f, "Data\\Texture\\GameOver\\ritorai.png", m_pRetryButton);
	//�^�C�g���ɖ߂�{�^��.
	m_pBackButton = new CSpriteUI();
	UI_Display(205.0f, 205.0f, 205.0f, 205.0f, 205.0f, 205.0f, "Data\\Texture\\GameOver\\batubotan_ge-muo-ba-.png", m_pBackButton);
	//===============================================================================================================================
	//----------------------------
	// �g�c
	//�J�[�\���p.
	m_pCursor = new CSpriteUI();
	UI_Display(CURSOR_SIZE, CURSOR_SIZE, CURSOR_SIZE, CURSOR_SIZE, CURSOR_SIZE, CURSOR_SIZE,"Data\\Texture\\Cursor.png",m_pCursor);

	//�t�F�C�h�p.
	m_pUIFade = new CSpriteUI();
	UI_Display(1.0f, 1.0f,1.0f, 1.0f,WND_W, WND_H,"Data\\Texture\\Fade.png",m_pUIFade);

	return S_OK;
}

//�T�E���h�̓ǂݍ��݊֐�.
HRESULT CMain::LoadSound()
{
	//�^�C�g���T�E���h(BGM).
	m_TitleBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM");
	//����̧�ق��J��.
	if (!m_TitleBGM->Open("Data\\Sound\\BGM\\Title.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//���C���T�E���h(BGM).
	m_pMainBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM2");
	//����̧�ق��J��.
	if (!m_pMainBGM->Open("Data\\Sound\\BGM\\GameMain.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//�{�X�T�E���h(BGM).
	m_pBossBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM3");
	//����̧�ق��J��.
	if (!m_pBossBGM->Open("Data\\Sound\\BGM\\BossBGM.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//�Q�[���I�[�o�[�T�E���h(BGM).
	m_pGameOverBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM4");
	//����̧�ق��J��.
	if (!m_pGameOverBGM->Open("Data\\Sound\\BGM\\GameOver.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//�Q�[���N���A�T�E���h(BGM).
	m_pGameClearBGM = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "BGM5");
	//����̧�ق��J��.
	if (!m_pGameClearBGM->Open("Data\\Sound\\BGM\\GameClear.mp3", m_Sound, m_hWnd)) {
		return E_FAIL;
	}

	//�N���b�N��SE.
	m_pClickSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_1");
	//����̧�ق��J��.
	if (!m_pClickSE->Open("Data\\Sound\\SE\\Click.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//�~��SE.
	m_pCircleSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_2");
	//����̧�ق��J��.
	if (!m_pCircleSE->Open("Data\\Sound\\SE\\Circle.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//���̔��˻����(SE).
	m_WaterShotSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_3");
	//����̧�ق��J��.
	if (!m_WaterShotSE->Open("Data\\Sound\\SE\\shot.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}

	//�����D�̔��˻����(SE).
	m_WaterBalloonSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_4");
	//����̧�ق��J��.
	if (!m_WaterBalloonSE->Open("Data\\Sound\\SE\\WaterBalloon.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//�v���C���[�̈ړ�SE.
	m_pPlayerMoveSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_5");
	//����̧�ق��J��.
	if (!m_pPlayerMoveSE->Open("Data\\Sound\\SE\\PlayerMove.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//�v���C���[�̃_���[�WSE.
	m_pPlayerDamageSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_6");
	//����̧�ق��J��.
	if (!m_pPlayerDamageSE->Open("Data\\Sound\\SE\\PlayerDamage.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}
	//���܂���̃_���[�WSE.
	m_pBearDamageSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_7");
	//����̧�ق��J��.
	if (!m_pBearDamageSE->Open("Data\\Sound\\SE\\BearDamage.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}

	//���܂���̃_���[�WSE.
	m_pPumpSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_8");
	//����̧�ق��J��.
	if (!m_pPumpSE->Open("Data\\Sound\\SE\\Pump.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}

	//���܂���̃_���[�WSE.
	m_pSwishSE = new clsSound();
	strcpy_s(m_Sound, sizeof(m_Sound), "SE_9");
	//����̧�ق��J��.
	if (!m_pSwishSE->Open("Data\\Sound\\SE\\Swish.wav", m_Sound, m_hWnd)) {
		return E_FAIL;
	}



	return S_OK;
}



//===========================================================
//	HLSĻ�ق�ǂݍ��ݼ���ނ��쐬����.
//	HLSL: High Level Shading Language �̗�.
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

	//HLSL�����ްï������ނ�����ނ��쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			"Mesh.hlsl",	//�����̧�ٖ�(HLSĻ��).
			nullptr,		//ϸے�`�̔z��ւ��߲��(���g�p).
			nullptr,		//�ݸٰ��̧�ق���������̪���ւ��߲��(���g�p).
			"VS_Main",		//����޴��ذ�߲�Ċ֐��̖��O.
			"vs_5_0",		//����ނ����ق��w�肷�镶����(���̧��).
			uCompileFlag,	//����޺��߲��׸�.
			0,				//�̪�ĺ��߲��׸�(���g�p).
			nullptr,		//�گ�� ����� ����̪���ւ��߲��(���g�p).
			&pCompiledShader,//����ނ��i�[������؂ւ��߲��.
			&pErrors,		//�װ�ƌx���ꗗ���i�[������؂ւ��߲��.
			nullptr)))		//�߂�l�ւ��߲��(���g�p).
	{
		_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬��������ނ���u�ްï������ށv���쐬.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)�ްï�������.
	{
		_ASSERT_EXPR(false, L"�ްï������ލ쐬���s");
		return E_FAIL;
	}

	//���_���߯�ڲ��Ă��`.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						//�ʒu.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI��̫�ϯ�(32bit float�^*3).
			0,
			0,								//�ް��̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"NORMAL",						//�@��.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI��̫�ϯ�(32bit float�^*3).
			0,
			12,								//�ް��̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD",						//ø����ʒu.
			0,
			DXGI_FORMAT_R32G32_FLOAT,		//DXGI��̫�ϯ�(32bit float�^*2).
			0,
			24,								//�ް��̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//���_���߯�ڲ��Ă̔z��v�f�����Z�o.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_���߯�ڲ��Ă��쐬.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)���_���߯�ڲ���.
	{
		_ASSERT_EXPR(false, L"���_���߯�ڲ��č쐬���s");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSL�����߸�ټ���ނ�����ނ��쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			"Mesh.hlsl",	//�����̧�ٖ�(HLSĻ��).
			nullptr,		//ϸے�`�̔z��ւ��߲��(���g�p).
			nullptr,		//�ݸٰ��̧�ق���������̪���ւ��߲��(���g�p).
			"PS_Main",		//����޴��ذ�߲�Ċ֐��̖��O.
			"ps_5_0",		//����ނ����ق��w�肷�镶����(���̧��).
			uCompileFlag,	//����޺��߲��׸�.
			0,				//�̪�ĺ��߲��׸�(���g�p).
			nullptr,		//�گ�� ����� ����̪���ւ��߲��(���g�p).
			&pCompiledShader,//����ނ��i�[������؂ւ��߲��.
			&pErrors,		//�װ�ƌx���ꗗ���i�[������؂ւ��߲��.
			nullptr)))		//�߂�l�ւ��߲��(���g�p).
	{
		_ASSERT_EXPR(false, L"hlsl�ǂݍ��ݎ��s");
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬��������ނ���u�߸�ټ���ށv���쐬.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)�߸�ټ����.
	{
		_ASSERT_EXPR(false, L"�߸�ټ���ލ쐬���s");
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�ݽ���(�萔)�ޯ̧�쐬.
	//����ނɓ���̐��l�𑗂��ޯ̧.
	//�����ł͕ϊ��s��n���p.
	//����ނ� World, View, Projection �s���n��.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof(SHADER_CONSTANT_BUFFER);//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p).
	cb.StructureByteStride = 0;	//�\���̻̂���(���g�p).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�̍쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer)))
	{
		_ASSERT_EXPR(false, L"�ݽ����ޯ̧�쐬���s");
		return E_FAIL;
	}


	return S_OK;
}

//�[�x(Z)ý�ON/OFF�؂�ւ�.
void CMain::SetDepth(bool flag)
{
	ID3D11DepthStencilState* pTmp
		= (flag == true) ? m_pDepthStencilState : m_pDepthStencilStateOff;

	//�[�x�ݒ���.
	m_pContext11->OMSetDepthStencilState(pTmp, 1);
}

//�̨��쐬.
HRESULT CMain::InitSphere(CDX9Mesh* pMesh)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;//���_�ޯ̧.
	void* pVertices = nullptr;	//���_.
	D3DXVECTOR3 vCenter;		//���S.
	float fRadius;				//���a.

	//���_�ޯ̧���擾.
	if (FAILED(
		pMesh->GetMesh()->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}
	//ү���̒��_�ޯ̧��ۯ�����.
	if (FAILED(
		pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}
	//ү���̊O�ډ~�̒��S�Ɣ��a���v�Z����.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>(pVertices),
		pMesh->GetMesh()->GetNumVertices(),	//���_�̐�.
		D3DXGetFVFVertexSize(pMesh->GetMesh()->GetFVF()),//���_�̏��.
		&vCenter,	//(out)���S���W.
		&fRadius);	//(out)���a.

	//��ۯ�.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//���S�Ɣ��a���\���̂ɐݒ�.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.fRadius = fRadius;

	return S_OK;
}

//-----------------------------------------------
//	�g�c�ύX.
//�Փ˔���(�޳��ިݸ޽̨�).
bool CMain::Collision(
	CDX9Mesh* pAttack, D3DXVECTOR3 AtkPos,	//�U����.
	CDX9Mesh* pTarget, D3DXVECTOR3 TgtPos)	//�W�I��.
{
	//�Q�̕��̂̒��S��(�Q�_��)�̋��������߂�.
	D3DXVECTOR3 vLength
		= TgtPos - AtkPos;
	//�����ɕϊ�����.
	float Length = D3DXVec3Length(&vLength);

	//�Q�_�Ԃ̋������A���ꂼ��̕��̂̔��a�𑫂������̂��.
	//�������Ƃ������Ƃ́A�̨����m���d�Ȃ��Ă���(�Փ˂��Ă���)�Ƃ�������.
	if (Length <=
		pAttack->m_Sphere.fRadius + pTarget->m_Sphere.fRadius)
	{
		return true;	//�Փ�.
	}
	return false;	//�Փ˂��Ă��Ȃ�.
}

// �I�u�W�F�N�g(�n�ʂƂ���)�ƃX�N���[�����W�̌�_�Z�o�֐�.
D3DXVECTOR3 * CMain::CalcScreenToXZ(D3DXVECTOR3 * Pos, float Sx, float Sy, D3DXMATRIX * View, D3DXMATRIX * Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	D3DXVECTOR3 center;
	m_pMouse->CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, View, Prj);
	m_pMouse->CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, View, Prj);
	//if(farpos ��nearpos�̊ԂɃI�u�W�F�N�g������Ȃ�).
	//ray = �I�u�W�F�N�gpos - nearpos;
	//center = pTarget->GetPosition();
	//pTarget->m_Sphere.fRadius;
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��.
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��.
	//�x�N�g�����Ƃ���nearpos��farpos���p�x��n���Ƃ��̕����ɐ���΂���.
	D3DXVECTOR3 HitPos;
	//------------------------------------------------------------------------
	//	7_26 �X�V�@�g�c.
	for (int KNo = 0; KNo < BEAR_MAX; KNo++) {
		if (RaySphere(&HitPos, &nearpos, &ray, &m_KumaParam[KNo].m_Pos) == true)
		{
			*Pos = HitPos;
			return Pos;
		}
		else {
			if (ray.y <= 0) {
				// ����_
				float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
				float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
				*Pos = nearpos + (LP0 / Lray) *ray;//(LP0 / Lray)����.
			}
			else {
				*Pos = farpos;
			}
		}
	}
	//------------------------------------------------------------------------

	return Pos;
}

//�������W�ƌ�_���W�Ԃ̒P�ʃx�N�g���擾�֐�.
D3DXVECTOR3 * CMain::UnitMove(D3DXVECTOR3 * Pos, D3DXVECTOR3 HitPos, D3DXVECTOR3 InitPos, float SHOT_SPD)
{
	D3DXVECTOR3 Vec_HitInit;
	Vec_HitInit = HitPos - InitPos;

	D3DXVec3Normalize(&Vec_HitInit, &Vec_HitInit);

	*Pos = Vec_HitInit * SHOT_SPD;
	return Pos;
}

//���ƃ��C�̓����蔻��.	//��_���W.		//���C�̊J�n�ʒu. //���C�̒���.   //���������.
bool CMain::RaySphere(D3DXVECTOR3* Pos, D3DXVECTOR3* x, D3DXVECTOR3* v, D3DXVECTOR3* pTgtPos)
{
	// �����Ƌ��̔���
	D3DXVECTOR3* center = pTgtPos;          // ���S�ʒu.
	FLOAT radius_sq = 1.0f;

	D3DXVECTOR3 xc = *x - *center;// ���S����̑��Έʒu.
	FLOAT xc2 = D3DXVec3Dot(v, v);
	FLOAT vxc = D3DXVec3Dot(v, &xc);
	FLOAT c = D3DXVec3Dot(&xc, &xc) - radius_sq * radius_sq;
	FLOAT D = vxc * vxc - xc2 * c;// ���ʎ�.

	// ��_�����݂��Ȃ��O�ꂽ.
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
//	��������������Ă���.
//==================================================
//�X�N���v�gUI�̍��W.
void CMain::UI_Display(
	float Base_W, float Base_H,		//���摜�̑傫��
	float Stride_W, float Stride_H,	//1�R�}������̕�,����
	float Disp_W, float Disp_H,		//�\������傫��.
	const char* fileName,			//�t�@�C���̖��O(�摜��)
	CSpriteUI*	UIName)				//SpriteUI�̖��O.
{
	CSpriteUI::SPRITE_STATE ss;
	ss.Base.w = Base_W;
	ss.Base.h = Base_H;
	ss.Stride.w = Stride_W;//1�R�}������̕����Z�o.
	ss.Stride.h = Stride_H;//1�R�}������̍������Z�o.
	ss.Disp.w = Disp_W;
	ss.Disp.h = Disp_H;
	UIName->Init(m_pDevice11, m_pContext11,
		fileName, &ss);
}
//==================================================


//�X�v���C�g�Z�b�g�֐�.
void CMain::Sprite(
	float Base_W, float Base_H,		//���摜�̑傫��
	float Stride_W, float Stride_H,	//1�R�}������̕�,����
	float Disp_W, float Disp_H,		//�\������傫��.
	const char* fileName,			//�t�@�C���̖��O(�摜��)					
	CSprite*	UIName)				//SpriteUI�̖��O.
{
	CSprite::SPRITE_STATE ss;
	ss.Base.w = Base_W;
	ss.Base.h = Base_H;
	ss.Stride.w = Stride_W;//1�R�}������̕����Z�o.
	ss.Stride.h = Stride_H;//1�R�}������̍������Z�o.
	ss.Disp.w = Disp_W;
	ss.Disp.h = Disp_H;
	UIName->Init(m_pDevice11, m_pContext11,
		fileName, &ss);
}

//�A�j���[�V�����X�V�֐�.
void CMain::UpdeAnim()
{
	m_BossKumaParam.m_anim_time = 0.0;
	//m_pB_KumaSkin->ChangeAnimSet(m_BossKumaParam.m_anim_no);

}

//���l�������֐�.
void CMain::Init()
{
	//���܂���֌W������.
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

	//�_���[�W�̏�����.
	m_Damage = CPlayer::enDamageParam::enNoDamage;
	m_pPlayer->Init();
	m_pGunParts->Init();
	m_pPump->Init();
	//�����̂̏�����
	//���S�C������
	ShotWait = 0.0f;
	for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
		m_ShotParam[SNo].m_Pos.y = -10.0f;
		m_ShotParam[SNo].m_ShotFlag = false;
	}
	//�D�̏�����
	for (int MNo = 0; MNo < MUD_MAX; MNo++) {
		m_MudParam[MNo].m_Pos.y = -5.0f;
		m_MudParam[MNo].m_ShotFlag = false;
	}
	//�����D�̏�����
	m_BomParam.m_Pos.y = -10.0f;
	m_BomParam.m_ShotFlag = false;
	for (int TNo = 0; TNo < TEX_NO; TNo++) {
		m_BossKumaParam.m_SkinChangeCnt[TNo] = 2;
	}

	//�{�X���܂���֌W������.
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
//													���������₷���悤�Ɋ֐������܂���.
//==========================================================================================================================================.

//�^�C�g�����N���b�N�����Ƃ��̏���.
void CMain::GetKyeAppMain()
{
	//�}�E�X�Ɖ~�̓����蔻����Ƃ��Ă���.
	if (m_pMouse->PointAndCircle(
		WND_W_HALF, CLICK_START_Y + (205.0f / 2), (185.0f / 2))
		&& m_pFade->GetFadeFlag() == 0) {
		SESoundDrow(m_pCircleSE, m_BomParam.m_SoundFlag = true);	//��
		//���N���b�N����ƃQ�[����i�߂�.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//�e���΂����̃T�E���h.
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
	//�}�E�X�Ɖ~�̓����蔻����Ƃ��Ă���.
	if (m_pMouse->PointAndCircle(
		WND_W - (110.0f / 2), (110.0f / 2), (110.0f / 2))) {
		SESoundDrowBlaze(m_pCircleSE, m_BomParam.m_SoundFlag = true);
		//���N���b�N����ƃQ�[����i�߂�.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//����޳��j������.
			DestroyWindow(m_hWnd);
		}
	}
	if (m_pFade->GetFadeFlag() == 2)
	{
		//�Q�[�����C����.
		m_enScene = enScene::enGameMain;
		m_pTime->SetBossTime(0);
		m_BearCleanCnt = 0;
		m_BossCleanCnt = 0;
	}
	BGMSoundDrow(m_TitleBGM, m_BGMflag = true);
	BGMSoundDrow(m_pGameOverBGM, m_BGMflag = false);
	BGMSoundDrow(m_pGameClearBGM, m_BGMflag = false);
}

//�Q�[���I�[�o�[���N���b�N�����Ƃ��̏���.
void CMain::GetKyeOver()
{
	//�}�E�X�Ɖ~�̓����蔻����Ƃ��Ă���.
	if (m_pMouse->PointAndCircle(
		300.0f + (205.0f / 2), 510.0f + (205.0f / 2), (205.0f / 2)) &&
		m_pFade->GetFadeFlag() == 0) {
		SESoundDrowBlaze(m_pCircleSE, m_BomParam.m_SoundFlag = true);
		//���N���b�N����ƃQ�[����i�߂�.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//�e���΂����̃T�E���h.
			SESoundDrowBlaze(m_pClickSE, m_ShotParam->m_SoundFlag = true);
			m_pFade->FadeIn_Disp();
			m_ContinueFlag = true;
		}
	}
	//�}�E�X�Ɖ~�̓����蔻����Ƃ��Ă���.
	else if (m_pMouse->PointAndCircle(
		800.0f + (205.0f / 2), 510.0f + (205.0f / 2), (205.0f / 2)) &&
		m_pFade->GetFadeFlag() == 0){
		SESoundDrowBlaze(m_pCircleSE, m_BomParam.m_SoundFlag = true);
		//���N���b�N����ƃQ�[����i�߂�.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//�e���΂����̃T�E���h.
			SESoundDrowBlaze(m_pClickSE, m_ShotParam->m_SoundFlag = true);
			m_pFade->FadeIn_Disp();
		}
	}

	if (m_pFade->GetFadeFlag() == 2)
	{
		if (m_ContinueFlag == true) {
			//�Q�[�����C����.
			Init();
			m_enScene = enScene::enGameMain;
			m_pArea->AreaFade_Disp();
			m_pTime->Init();
		}
		else  {
			//�^�C�g����.
			Init();
			m_enScene = enScene::enTitle;
		}
	}
	BGMSoundDrow(m_pMainBGM, m_BGMflag = false);
	BGMSoundDrow(m_pBossBGM, m_BGMflag = false);
	BGMSoundDrow(m_pGameOverBGM, m_BGMflag = true);
}
//�Q�[���N���A���N���b�N�����Ƃ��̏���.
void CMain::GetKyeClear()
{
	//�}�E�X�Ɖ~�̓����蔻����Ƃ��Ă���.
	if (m_pMouse->PointAndCircle(
		WND_W - (112.0f / 2), (112.0f / 2), (112.0f / 2))
		&& m_pFade->GetFadeFlag() == 0) {
		SESoundDrowBlaze(m_pCircleSE, m_BomParam.m_SoundFlag = true);
		//���N���b�N����ƃQ�[����i�߂�.
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			//�e���΂����̃T�E���h.
			SESoundDrowBlaze(m_pClickSE, m_ShotParam->m_SoundFlag = true);
			m_pFade->FadeIn_Disp();
		}
	}
	if (m_pFade->GetFadeFlag() == 2)
	{
		//�^�C�g����.
		Init();
		m_enScene = enScene::enTitle;
	}
	BGMSoundDrow(m_pBossBGM, m_BGMflag = false);
	BGMSoundDrow(m_pGameClearBGM, m_BGMflag = true);
}

//�A�v�����C���̐����D�̏���.
void CMain::WaterBalloonProcessAppMain()
{

	//�����D����.
	if (WaterBallonCnt > 0) {
		//	2019_7_31 �X�V�@�g�c.
		if (GetAsyncKeyState('M') & 0x8000 && m_P_Moveflag == false) {
			//------------------------------------
			//�g�c�X�V_ �N���b�N����Ƌ�C�������߂鏈��.
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

//�A�v�����C���̐��S�C�̏���.
void CMain::WaterGunProcessAppMain()
{
	//�����Am_GaugeBarCnt��0.01f�ȏ�Ȃ猂�ĂāA��������Ȃ��Ȃ猂�ĂȂ�.
	if (m_pPump->GetGauge() >= 0.01f) {
		ShotWait -= 0.1f;
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && m_P_Moveflag == false) {
			//------------------------------------
			//�g�c�X�V_ �N���b�N����Ƌ�C�������߂鏈��.
			m_GunDispFlag = false;
			//------------------------------------
			m_pPump->DecrementGauge();
			for (int SNo = 0; SNo < SHOT_MAX; SNo++) {
				if (ShotWait <= 0) {//���ˊԊu.
					if (m_ShotParam[SNo].m_ShotFlag == false) {
						m_ShotParam[SNo].m_Pos = m_WMousePos;//�J�[�\���̈ʒu�ɒe�������Ă���.
						UnitMove(&m_ShotParam[SNo].m_VecUnit, m_HitPos, m_WMousePos, SHOT_SPD);//�P�ʃx�N�g���擾.
						m_ShotParam[SNo].m_ShotFlag = true;
						//�e���΂����̃T�E���h.
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
			m_ShotParam[SNo].m_Pos += m_ShotParam[SNo].m_VecUnit;//�P�ʃx�N�g���𑫂��Ēe�ړ�.
			m_ShotParam[SNo].m_DispCount++;
		}
		if (m_ShotParam[SNo].m_DispCount >= 120) {//���Ԃ���������e������.
			m_ShotParam[SNo].m_ShotFlag = false;
			m_ShotParam[SNo].m_DispCount = 0;
			m_ShotParam[SNo].m_Pos = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		}
	}
	if (ShotWait <= 0) {
		ShotWait = SHOT_WAIT;//�e�̔��ˊԊu.
	}

}

//���S�C��\�����ăQ�[�W���񕜂��鏈��.
void CMain::WaterGunGaugeChargeAppMain()
{
	//�����[�h����(���S�C���b�V��).
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		m_SpeceKey++;	//������Ă���.
	}
	else
	{
		if (m_SpeceKey > 0) {
			m_SpeceKey = -1;	//���ꂽ�u��.
		}
		else
		{
			m_SpeceKey = 0;	//����Ă�����.
		}
	}

	//�������Ŕ������Ȃ�����.
	if (m_SpeceKey == 1) {
		m_GunDispFlag = true;
		m_pPump->SetReload();
		SESoundDrowBlaze(m_pPumpSE, m_PumpSEflag = true);
	}
	m_pPump->Charge(m_pGunParts->GetPosition());
}

//�Q�[�W�Ń����_�����O�ɒu���Ă���.
void CMain::GaugeRender()
{
	//�X�y�[�X�L�[�ŉ񕜂���UI�\��.
	if (m_SpaceDispflag == true) {
		//�X�y�[�X�������̃��S.
		m_pSpaceLogo->SetPosition(WND_W_HALF - 62.0f / 2, 460.0f, 0.14f);
		m_pSpaceLogo->Render();
		//�X�y�[�X�������̃o�[.
		m_pSpaceBar->SetPosition(WND_W_HALF - 380.0f / 2, 450.0f, 0.15f);	//x��-10.0f�͌����Q�[�W�̃T�C�Y�Ɠ������ߏ����������Ƃ��ɏ�������邽�߂ɂ��肬��ɂ��Ă���.
		m_pSpaceBar->Render();
	}
	//�{�X���܂���̃A�C�R��.
	m_pBossBearIcon->SetPosition(10.0f, 460.0f, 0.16f);
	m_pBossBearIcon->Render();
	//�Q�[�W�o�[.
	m_pGaugeBar->SetPosition(90.0f, UI_MOSTLOWER, 0.17f);
	m_pGaugeBar->Render();
	//�����Q�[�W��UI�̕\��.
	m_pDecremetGauge->SetPosition(104.6f, UI_MOSTLOWER, 0.18f);
	m_pDecremetGauge->GaugeRender(m_pPump->GetGauge());
	//�Q�[�W�̊Ǘ�.
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

//�v���C���[�_���[�W�Ń����_�����O�ɒu���Ă���.
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

//�f�[�^�̓ǂݍ���(���܂���̃f�[�^)&���[�h���b�V���ł��Ă���.
void CMain::FileDataBear()
{
	//�t�@�C�����J��.
	fstream	filestream("Data\\ExcelData\\Bear.csv");
	if (!filestream.is_open()) {
		return;
	}

	bool	isNotSkip = false;
	//�t�@�C���̓ǂݍ���.
	while (!filestream.eof())
	{
		string	buffer;
		filestream >> buffer;
		if (!isNotSkip) {
			isNotSkip = true;
			continue;	//��s�ڂ��X�L�b�v.
		}
		//�t�@�C������ǂݍ�
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

//BGM�T�E���h�̗���(�����_�����O����).
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

//SE�T�E���h�̗���(�����_�����O�����A�˗p).
void CMain::SESoundDrowBlaze(clsSound* m_pSound, bool m_SoundFlag)
{
	//�g�c�ύX.
	if (m_SoundFlag == true)
	{
		m_pSound->SeekToStart();
		m_pSound->Play();
		m_SoundFlag = false;
	}
}

//SE�T�E���h�̗���(�����_�����O����).
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

