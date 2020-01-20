//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : 佐藤　安純
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "fieldmanager.h"
#include "charResult.h"
#include "scene2D.h"
#include "moveUI.h"
#include "resultCharUI.h"
#include "select.h"
#include "RawMouse.h"
#include "score.h"
#include "number2D.h"
#include "select.h"
#include "audience.h"
#include "barun.h"
#include "sky.h"
#include "RawMouse.h"
#include "shadow.h" //安

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_OUT_TIME		(60  * 5)		// フェードするまでの時間
#define UI_FILENAME			("data\\TEXT\\UI\\result.txt")
#define LOAD_OBJ			("data/TEXT/Stage.txt")
#define LOAD_TERRITORY		("data/TEXT/Territory.txt")
#define FLAG_JAPAN			("data/TEXTURE/Select/NationalFlag/Japan.png")
#define FLAG_RUSSIA			("data/TEXTURE/Select/NationalFlag/Russia.png")
#define FLAG_AMERICA		("data/TEXTURE/Select/NationalFlag/America.png")
#define FLAG_BRAZIL			("data/TEXTURE/Select/NationalFlag/Brazil.png")
#define FLAG_ENGLAND		("data/TEXTURE/Select/NationalFlag/Engrand.png")
#define FLAG_ITALY			("data/TEXTURE/Select/NationalFlag/Italy.png")
#define FLAG_NEWZEALAND		("data/TEXTURE/Select/NationalFlag/Newzealand.png")
#define FLAG_SOUTHAFRICA	("data/TEXTURE/Select/NationalFlag/SouthAfrica.png")
#define PLAYERNAME			("data/TEXTURE/result/PLAYER_ALL.png")
#define BACK_LINE			("data/TEXTURE/result/BackLine.png")
#define RANK_FLAM			("data/TEXTURE/result/ResultCharFlam.png")
#define RANK_ALLNUM			("data/TEXTURE/result/Rank_ALL.png")
#define COM_NAME			("data/TEXTURE/result/COMPUTER_ALL.png")
#define RANKING_FLAM		("data/TEXTURE/result/Result_Shape.png")
#define CPU_TEX				("data/TEXTURE/result/CPU.png")
#define P_TEX				("data/TEXTURE/result/P.png")
#define CHR_JAPAN			("data/TEXTURE/result/Character/ResultChar_Japan.png")
#define CHR_RUSSIA			("data/TEXTURE/result/Character/ResultChar_Russia.png")
#define CHR_AMERICA			("data/TEXTURE/result/Character/ResultChar_America.png")
#define CHR_BRAZIL			("data/TEXTURE/result/Character/ResultChar_Brazil.png")
#define CHR_ENGLAND			("data/TEXTURE/result/Character/ResultChar_England.png")
#define CHR_ITALY			("data/TEXTURE/result/Character/ResultChar_Italy.png")
#define CHR_NEWZEALAND		("data/TEXTURE/result/Character/ResultChar_Newzeland.png")
#define CHR_SOUTHAFRICA		("data/TEXTURE/result/Character/ResultChar_SouthAfrica.png")
#define RESULT_FLAG			("data/TEXTURE/result/ResultFlag.png")
#define RESULT_TABLE		("data/TEXTURE/result/result_bg000.png")
#define NAME_FLAM			("data/TEXTURE/result/NameFlam.png")
#define NUMBER_TEX			("data/TEXTURE/number/number.png")
#define PLAYER_TEX			("data/TEXTURE/result/プレイヤー.png")
#define COM_TEX				("data/TEXTURE/result/コンピューター.png")
#define TOTAL_TEX			("data/TEXTURE/result/合計.png")
#define GETNUM_TEX			("data/TEXTURE/result/獲得.png")
#define MAKENUM_TEX			("data/TEXTURE/result/領地作成.png")
#define ROBBED_TEX			("data/TEXTURE/result/奪われた数.png")
#define THANKSBACK_TEX		("data/TEXTURE/result/ThanksBack.png")
#define THANKSBG_TEX		("data/TEXTURE/result/ThankYouForPlaying_BG.png")
#define THANKS_TEX			("data/TEXTURE/result/ThankYouForPlaying.png")

//画面が白い状態時のマクロ
#define FLAG_WHIGHTSIZE		(D3DXVECTOR3(130.0f, 100.0f, 0.0f))
#define FLAG_AIMSIZE		(D3DXVECTOR3(90.0f, 70.0f, 0.0f))

//ズーム状態のマクロ
#define CHANGE_TIME	(90)

//風船
#define MOVE_XZ					  (0.2f);
#define MIDLLE					  (60.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CResult::TYPE CResult::m_type = TYPE_MAX;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CResult::CResult(){}

//=============================================================================
// デストラクタ
//=============================================================================
CResult::~CResult(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CResult::Init(void)
{
	//ベースの初期化
	CModeBase::Init();

	//変数の初期化
	m_pCamera = NULL;
	m_pLoadTerritory = NULL;
	m_pLoadTextObj = NULL;
	m_pFieldManager = NULL;
	m_pWinChar = NULL;
	m_pWhiteBack = NULL;
	m_pNatinalFlag = NULL;
	m_pWinerName = NULL;
	m_pBackLine = NULL;
	m_pRankNo1 = NULL;
	m_pTotalLogo = NULL;
	m_pGetNumLogo = NULL;
	m_pMakeNum = NULL;
	m_pRobbedNum = NULL;
	m_state = STATE_WHITE;
	m_bWhiteIn = false;
	m_nMaxPlayer = 0;
	m_nCountID = 0;
	m_nCounter = 0;
	m_nTime = 0;
	m_nFadeOutTime = 0;
	m_nCountSoundTime = 0;

	m_Color[0] = D3DXCOLOR(1.0f, 0.25f, 0.25f, 1.0f);	//１P
	m_Color[1] = D3DXCOLOR(0.25f, 0.5f, 1.0f, 1.0f);	//２P
	m_Color[2] = D3DXCOLOR(0.4f, 0.9f, 0.4f, 1.0f);		//３P
	m_Color[3] = D3DXCOLOR(1.0f, 1.0f, 0.25f, 1.0f);	//４P
	m_Color[4] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);		//COM

	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		m_pCuntryTex[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < RESULT_TEX; nCnt++)
	{
		m_pTexture[nCnt] = NULL;
	}
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pResultCharUI[nCnt] = NULL;
	}

	//キャラ情報
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		m_CharInfo[nCnt] = { 0, 0, 0, 7, -1, CSelect::INPUTTYPE_MAX };
		m_CharResultScore[nCnt] = { 0, 0, 0, 7, -1, CSelect::INPUTTYPE_MAX };
		m_pCharImage[nCnt] = NULL;
		m_pCharaTex[nCnt] = NULL;
		m_bCharFlagDown[nCnt] = false;
		m_pRankNum[nCnt] = NULL;
		m_pFlag[nCnt] = NULL;
		m_pTable[nCnt] = NULL;
		m_pNameFlam[nCnt] = NULL;
		m_pName[nCnt] = NULL;
		m_pNameJapanese[nCnt] = NULL;
		m_pNumber[nCnt] = NULL;
		m_pBackCharImage[nCnt] = NULL;
		m_pTotalScore[nCnt] = NULL;
		m_pGetScore[nCnt] = NULL;
		m_pMakeScore[nCnt] = NULL;
		m_pRobotedScore[nCnt] = NULL;
		m_pThanksBack[nCnt] = NULL;
		m_pThanksBG[nCnt] = NULL;
		m_pThanks[nCnt] = NULL;
		m_bThanksFlagDown[nCnt] = false;
		m_nCountMakeShape[nCnt] = 0;
		m_nCountGetTerritory[nCnt] = 0;
		m_nCountRobbotedTerritory[nCnt] = 0;
	}

	//スコアテクスチャの読み込み
	CNumber2D::Load();

	//テクスチャの読み込み
	LoadTex();
	CBarun::LoadTex();
	CShadow::LoadTex();		//影の読み込み　安

	//コントローラー情報取得
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//コントローラーの番号取得
		m_CharInfo[nCnt].nControlerNo = CSelect::GetControlerNum(nCnt);
		m_CharResultScore[nCnt].nControlerNo = m_CharInfo[nCnt].nControlerNo;

		//コントローラーのタイプ取得
		m_CharInfo[nCnt].m_InputType = CSelect::GetInputType(nCnt);
		m_CharResultScore[nCnt].m_InputType = m_CharInfo[nCnt].m_InputType;
	}

	//地面の生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//オブジェクトの生成
	//if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリーの読み込み

	//スコア順にソートする
	SortScore();

	//スコアの取得
	GetScoreResult();

	//キャラテクスチャを読み込む
	InitCharImage();

	//勝者キャラの生成
	InitWinerChar(m_CharInfo[0].nCuntry, m_CharInfo[0].nType, m_CharInfo[0].nCharNo);

	//カメラの生成
	if (m_pCamera == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;		//ビューポートの左上X座標
		viewport.Y = 0;		//ビューポートの左上Y座標
		viewport.Width = SCREEN_WIDTH;	//幅
		viewport.Height = SCREEN_HEIGHT;//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pCamera = new CResultCamera;
		if (m_pWinChar != NULL) { m_pCamera->SetTargetPos(m_pWinChar->GetPos()); }
		m_pCamera->Init();
		m_pCamera->SetViewport(viewport);
		m_pCamera->SetPosV(D3DXVECTOR3(-40.0f, 60.0f, -100.0f));
		m_pCamera->SetPosR(D3DXVECTOR3(-70.0f, 90.0f, 50.0f));
	}

	//白背景の生成
	if (m_pWhiteBack == NULL) { m_pWhiteBack = CScene2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(640.0f, 360.0f, 0.0f), CScene2D::TYPE_LOGO); }

	//国旗の生成
	if (m_pNatinalFlag == NULL) { m_pNatinalFlag = CMoveUI::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(140.0f, 110.0f, 0.0f), m_pCuntryTex[m_CharInfo[0].nCuntry]); }

	//キャラ表示の初期化
	InitCharUI();

	// 観客の生成
	CAudience::Create();

	// 空の生成
	CSky::Load();
	CSky::Create();
}

//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//ベースの破棄
	CModeBase::Uninit();

	//カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//地面マネージャーの破棄
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	//オブジェクトの破棄
	if (m_pLoadTextObj != NULL)
	{
		m_pLoadTextObj->Uninit();
		delete m_pLoadTextObj;
		m_pLoadTextObj = NULL;
	}

	//テリトリーの破棄
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	//キャラ表示UIの破棄
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pResultCharUI[nCnt] != NULL)
		{
			m_pResultCharUI[nCnt]->Uninit();
			delete m_pResultCharUI[nCnt];
			m_pResultCharUI[nCnt] = NULL;
		}
	}

	m_pWinChar = NULL;
	m_pNatinalFlag = NULL;
	m_pBackLine = NULL;
	m_pWhiteBack = NULL;
	m_pNatinalFlag = NULL;
	m_pWinerName = NULL;
	m_pRankNo1 = NULL;
	m_pTotalLogo = NULL;
	m_pGetNumLogo = NULL;
	m_pMakeNum = NULL;
	m_pRobbedNum = NULL;

	//キャラテクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		if (m_pCharaTex[nCnt] != NULL)
		{
			m_pCharaTex[nCnt]->Release();
			m_pCharaTex[nCnt] = NULL;
		}

		m_pCharImage[nCnt] = NULL;
		m_pRankNum[nCnt] = NULL;
		m_pFlag[nCnt] = NULL;
		m_pTable[nCnt] = NULL;
		m_pNameFlam[nCnt] = NULL;
		m_pName[nCnt] = NULL;
		m_pNameJapanese[nCnt] = NULL;
		m_pNumber[nCnt] = NULL;
		m_pBackCharImage[nCnt] = NULL;
		m_pTotalScore[nCnt] = NULL;
		m_pGetScore[nCnt] = NULL;
		m_pMakeScore[nCnt] = NULL;
		m_pRobotedScore[nCnt] = NULL;
		m_pThanksBack[nCnt] = NULL;
		m_pThanksBG[nCnt] = NULL;
		m_pThanks[nCnt] = NULL;
	}

	//テクスチャの破棄
	UnloadTex();
	CNumber2D::Unload();
	CBarun::UnloadTex();
	CSky::UnLoad();
	CShadow::UnloadTex();	//影

	//フェード以外を削除
	CScene::ReleaseFade();
}

//=============================================================================
// 更新処理
//=============================================================================
void CResult::Update(void)
{
	//インプットの取得　
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得
	//サウンドの取得
	CSound *pSound = CManager::GetSound();

	m_nCounter++;
	if (m_nCounter % 60 == 0)
	{
		m_nCountSoundTime++;
		if (m_nCountSoundTime <= 1)
		{
			pSound->PlaySoundA(CSound::SOUND_LABEL_BGM003);
		}
		if (m_nCountSoundTime == 5)
		{
			pSound->PlaySoundA(CSound::SOUND_LABEL_BGM002);
		}
	}
#ifdef _DEBUG
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_1) == true)
		{
			if (CFade::FADE_OUT != CFade::GetFadeMode())
			{
				//決定音
				CSound::PlaySound(CSound::SOUND_LABEL_SE007);
				CFade::SetFade(CManager::MODE_RANKING);
			}
		}
	}
#endif

	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	//カメラの更新処理
	if (m_pCamera != NULL) { m_pCamera->Update(); }

	//状態ごとの更新処理
	switch (m_state)
	{
	case STATE_WHITE: UpdateWhite(); break;
	case STATE_ZOOM: UpdateZoom();	break;
	case STATE_RANKING: UpdateRanking(); break;
	case STATE_SCORERESULT: UpdateScoreResult(); break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
	//カメラの設定
	if (m_pCamera != NULL) { m_pCamera->Set(); }

	CScene::DrawAll();
}

//=============================================================================
// 画面が白い状態の更新処理
//=============================================================================
void CResult::UpdateWhite(void)
{
	if (m_pNatinalFlag == NULL) { return; }

	if (!m_bWhiteIn)
	{
		D3DXVECTOR3 Size = m_pNatinalFlag->GetSize();

		if (Size.x == FLAG_WHIGHTSIZE.x)
		{
			m_bWhiteIn = true;
		}
		else
		{
			m_pNatinalFlag->UpdateScaleDown(FLAG_WHIGHTSIZE, D3DXVECTOR2(0.1f, 0.1f));
		}
	}

	if (!m_bWhiteIn) { return; }

	//国旗を左上に移動する
	m_pNatinalFlag->UpdateScaleDown(FLAG_AIMSIZE, D3DXVECTOR2(1.0f, 0.7f));
	m_pNatinalFlag->UpdateMoveDest(D3DXVECTOR3(140.0f, 95.0f, 0.0f), 8.0f);

	if (m_pWhiteBack != NULL)
	{//背景を透明にする
		//死亡フラグ
		bool bDeth = false;

		//色を取得
		D3DXCOLOR col = m_pWhiteBack->GetColor();
		col.a -= 0.05f;

		if (col.a < 0.0f)
		{
			col.a = 0.0f;
			bDeth = true;
		}
		else if (col.a < 0.5f)
		{
			if (m_pWinChar != NULL) { m_pWinChar->SetMotionType(CCharResult::MOTION_TYPE_WIN); }
			CreateBarun();	//風船の破棄
		}

		//色を設定する
		m_pWhiteBack->SetColoer(col);

		if (bDeth)
		{//白背景を破棄する
			m_pWhiteBack->Uninit();
			m_pWhiteBack = NULL;
		}
	}

	//カメラをキャラクターを移す状態にする
	if (m_pCamera != NULL)
	{
		if (m_pCamera->GetState() != CResultCamera::STATE_CHAR_ZOOM)
		{
			m_pCamera->SetState(CResultCamera::STATE_CHAR_IN);
		}
		else
		{
			m_state = STATE_ZOOM;	//ズーム状態にする
		}
	}
}

//=============================================================================
// キャラクターズーム時の処理
//=============================================================================
void CResult::UpdateZoom(void)
{
	//勝者名の生成・移動処理
	if (m_pWinerName == NULL)
	{
		if (m_CharInfo[0].nCharNo < m_nMaxPlayer)
		{
			m_pWinerName = CMoveUI::Create(D3DXVECTOR3(350.0f, 600.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), m_pTexture[0]);
			m_pWinerName->SetTexUV(0.0f, 1.0f, 0.25f * m_CharInfo[0].nCharNo, (0.25f * m_CharInfo[0].nCharNo) + 0.25f);
		}
		else
		{
			m_pWinerName = CMoveUI::Create(D3DXVECTOR3(350.0f, 600.0f, 0.0f), D3DXVECTOR3(300.0f, 100.0f, 0.0f), m_pTexture[4]);
			m_pWinerName->SetTexUV(0.0f, 1.0f, 0.125f * (m_CharInfo[0].nCharNo - m_nMaxPlayer), (0.125f * (m_CharInfo[0].nCharNo - m_nMaxPlayer)) + 0.125f);
		}
	}
	else
	{
		m_pWinerName->UpdateMoveDest(D3DXVECTOR3(300.0f, 600.0f, 0.0f), 1.0f);
	}

	//時間の加算
	m_nCounter++;

	if (CHANGE_TIME < m_nCounter) { m_state = STATE_RANKING; }
}

//=============================================================================
// ランキングを表示する
//=============================================================================
void CResult::UpdateRanking(void)
{
	if (m_pWinerName != NULL) { m_pWinerName->UpdateMoveDest(D3DXVECTOR3(300.0f, 350.0f, 0.0f), 25.0f); }	//名前

	//ランキングの表示
	if (m_pRankNo1 == NULL)
	{
		m_pRankNo1 = CMoveUI::Create(D3DXVECTOR3(350.0f, -150.0f, 0.0f), D3DXVECTOR3(100.0f, 100.0f, 0.0f), m_pTexture[3]);
		m_pRankNo1->SetTex(0.125f, 0);
	}
	else
	{
		m_pRankNo1->UpdateMoveDest(D3DXVECTOR3(350.0f, 100.0f, 0.0f), 20.0f);
	}

	//背景のライン
	if (m_pBackLine != NULL) { m_pBackLine->UpdateMoveDest(D3DXVECTOR3(640.0f, 640.0f, 0.0f), 25.0f); }

	//キャラUIの表示
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pResultCharUI[nCnt] != NULL)
		{
			if (nCnt < 2)
			{
				m_pResultCharUI[nCnt]->UpdateMoveDest(D3DXVECTOR3(170.0f + (nCnt * 290.0f), 600.0f + (nCnt * 8.0f), 0.0f), 25.0f);
			}
			else if (nCnt == 2)
			{
				m_pResultCharUI[nCnt]->UpdateMoveDest(D3DXVECTOR3(720.0f, 600.0f + (nCnt * 8.0f), 0.0f), 25.0f);
			}
			else
			{
				int nCounter = nCnt;
				nCounter -= 3;
				m_pResultCharUI[nCnt]->UpdateMoveDest(D3DXVECTOR3(895.0f + (nCounter * 108.0f), 620.0f, 0.0f), 25.0f);
			}
		}
	}

	//入力処理
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pInputXPad = CManager::GetXPad();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{//安
		m_state = STATE_SCORERESULT;	//スコア表示状態にする
		InitScoreResult();				//スコア表の初期化
	}

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true)
		{
			m_state = STATE_SCORERESULT;	//スコア表示状態にする
			InitScoreResult();				//スコア表の初期化
		}

		if (pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
		{
			m_state = STATE_SCORERESULT;	//スコア表示状態にする
			InitScoreResult();				//スコア表の初期化
		}
	}
}

//=============================================================================
// キャラランキングUIの破棄
//=============================================================================
void CResult::UninitCharRanking(void)
{
	if (m_pWinerName != NULL)
	{//勝者名の破棄
		m_pWinerName->Uninit();
		m_pWinerName = NULL;
	}

	if (m_pRankNo1 != NULL)
	{//一位のロゴを破棄
		m_pRankNo1->Uninit();
		m_pRankNo1 = NULL;
	}

	if (m_pBackLine != NULL)
	{//バックラインの破棄
		m_pBackLine->Uninit();
		m_pBackLine = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{//キャラUIの破棄
		if (m_pResultCharUI[nCnt] != NULL)
		{
			m_pResultCharUI[nCnt]->UninitUI();
			m_pResultCharUI[nCnt]->Uninit();
			delete m_pResultCharUI[nCnt];
			m_pResultCharUI[nCnt] = NULL;
		}
	}
}

//=============================================================================
// スコア表示の初期化処理
//=============================================================================
void CResult::InitScoreResult(void)
{
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		int nNo = m_CharInfo[nCnt].nCharNo;
		if (m_pBackCharImage[nNo] == NULL)
		{//キャラ背景
			m_pBackCharImage[nNo] = CMoveUI::Create(D3DXVECTOR3(80.0f + (nNo * 160.0f), -550.0f, 0.0f), D3DXVECTOR3(80.0f, 275.0f, 0.0f), NULL);

			//色の設定
			if (nNo < m_nMaxPlayer)
			{
				m_pBackCharImage[nNo]->SetColoer(m_Color[nNo]);
			}
			else
			{
				m_pBackCharImage[nNo]->SetColoer(D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));
			}
		}

		if (m_pCharImage[nNo] == NULL)
		{//キャラ画像
			int nCharType = m_CharInfo[nCnt].nType;
			int nCuntry = m_CharInfo[nCnt].nCuntry;
			m_pCharImage[nNo] = CMoveUI::Create(D3DXVECTOR3(80.0f + (nNo * 160.0f), -550.0f, 0.0f), D3DXVECTOR3(80.0f, 275.0f, 0.0f), m_pCharaTex[nCuntry]);
			m_pCharImage[nNo]->SetTexUV(0.0f + (nCharType * 0.25f), 0.25f + (nCharType * 0.25f), 0.0f, 1.0f);
		}

		if (m_pRankNum[nNo] == NULL)
		{//ランキングのナンバー生成
			m_pRankNum[nNo] = CMoveUI::Create(D3DXVECTOR3(110.0f + (nNo * 160.0f), -650.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), m_pTexture[3]);
			m_pRankNum[nNo]->SetTexUV(0.0f + (nCnt * 0.125f), 0.125f + (nCnt * 0.125f), 0.0f, 1.0f);
		}

		if (m_pFlag[nNo] == NULL)
		{//国旗
			int nCuntry = m_CharInfo[nCnt].nCuntry;
			m_pFlag[nNo] = CMoveUI::Create(D3DXVECTOR3(80.0f + (nNo * 160.0f), -865.0f, 0.0f), D3DXVECTOR3(81.0, 75.0f, 0.0f), m_pTexture[8]);
			m_pFlag[nNo]->SetTexUV(0.0f, 1.0f, 0.0f + (nCuntry * 0.125f), 0.125f + (nCuntry * 0.125f));
		}

		if (m_pTable[nNo] == NULL)
		{//表
			m_pTable[nNo] = CMoveUI::Create(D3DXVECTOR3(80.0f + (nNo * 160.0f), -450.0f, 0.0f), D3DXVECTOR3(80.0f, 175.0f, 0.0f), m_pTexture[9]);

			if (nNo < m_nMaxPlayer)
			{//プレイヤーの色にする
				m_pTable[nNo]->SetColoer(m_Color[nNo]);
			}
			else
			{//灰色にする
				m_pTable[nNo]->SetColoer(m_Color[4]);
			}
		}

		if (m_pNameFlam[nNo] == NULL)
		{//名前のフレーム
			m_pNameFlam[nNo] = CMoveUI::Create(D3DXVECTOR3(75.0f + (nNo * 160.0f), -810.0f, 0.0f), D3DXVECTOR3(75.0f, 40.0f, 0.0f), m_pTexture[10]);

			if (nNo < m_nMaxPlayer)
			{//プレイヤーの色にする
				m_pNameFlam[nNo]->SetColoer(m_Color[nNo]);
			}
			else
			{//灰色にする
				m_pNameFlam[nNo]->SetColoer(m_Color[4]);
			}
		}

		if (m_pName[nNo] == NULL)
		{//名前
			if (nNo < m_nMaxPlayer)
			{//プレイヤーの色にする
				m_pName[nNo] = CMoveUI::Create(D3DXVECTOR3(20.0f + (nNo * 160.0f), -830.0f, 0.0f), D3DXVECTOR3(35.0f, 20.0f, 0.0f), m_pTexture[7]);
				m_pName[nNo]->SetTexUV(0.0f, 1.0f, 0.0f + (nNo * 0.25f), 0.25f + (nNo * 0.25f));
				m_pName[nNo]->SetColoer(m_Color[nNo]);
			}
			else
			{//CPUの色
				m_pName[nNo] = CMoveUI::Create(D3DXVECTOR3(35.0f + (nNo * 160.0f), -830.0f, 0.0f), D3DXVECTOR3(35.0f, 20.0f, 0.0f), m_pTexture[6]);
				m_pName[nNo]->SetColoer(m_Color[4]);
			}
		}

		if (m_pNameJapanese[nNo] == NULL)
		{//日本語表記の名前
			if (nNo < m_nMaxPlayer)
			{
				m_pNameJapanese[nNo] = CMoveUI::Create(D3DXVECTOR3(55.0f + (nNo * 160.0f), -805.0f, 0.0f), D3DXVECTOR3(50.0f, 15.0f, 0.0f), m_pTexture[12]);
			}
			else
			{
				m_pNameJapanese[nNo] = CMoveUI::Create(D3DXVECTOR3(55.0f + (nNo * 160.0f), -805.0f, 0.0f), D3DXVECTOR3(50.0f, 20.0f, 0.0f), m_pTexture[13]);
			}
		}

		if (m_pNumber[nNo] == NULL)
		{//ナンバー
			if (nNo < m_nMaxPlayer)
			{
				int nPattern = nNo + 1;
				m_pNumber[nNo] = CMoveUI::Create(D3DXVECTOR3(125.0f + (nNo * 160.0f), -819.0f, 0.0f), D3DXVECTOR3(15.0f, 20.0f, 0.0f), m_pTexture[11]);
				m_pNumber[nNo]->SetTexUV(0.0f + (0.1f * nPattern), 0.1f + (0.1f * nPattern), 0.0f, 1.0f);
			}
			else
			{
				int nPattern = (nNo + 1) - m_nMaxPlayer;
				m_pNumber[nNo] = CMoveUI::Create(D3DXVECTOR3(125.0f + (nNo * 160.0f), -819.0f, 0.0f), D3DXVECTOR3(15.0f, 20.0f, 0.0f), m_pTexture[11]);
				m_pNumber[nNo]->SetTexUV(0.0f + (0.1f * nPattern), 0.1f + (0.1f * nPattern), 0.0f, 1.0f);
			}
		}
	}
}

//=============================================================================
// スコア表を表示する
//=============================================================================
void CResult::UpdateScoreResult(void)
{
	m_nTime++;

	if ((m_nTime % 10) == 0)
	{
		m_bCharFlagDown[m_nCountID] = true;
		m_nCountID++;

		//値の修正
		if (m_nCountID > MAX_CHARACTER - 1) { m_nCountID = MAX_CHARACTER - 1; }
	}

	//背景の移動処理
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		if (m_bCharFlagDown[nCnt] == false) { continue; }

		int nNo = m_CharInfo[nCnt].nCharNo;
		if (m_pBackCharImage[nNo] != NULL)
		{//キャラ背景の移動
			m_pBackCharImage[nNo]->UpdateMoveDest(D3DXVECTOR3(80.0f + (nNo * 160.0f), 400.0f, 0.0f), 30.0f);
		}

		if (m_pCharImage[nNo] != NULL)
		{//キャラ画像の移動
			m_pCharImage[nNo]->UpdateMoveDest(D3DXVECTOR3(80.0f + (nNo * 160.0f), 400.0f, 0.0f), 30.0f);
		}

		if (m_pRankNum[nNo] != NULL)
		{//ランキングのナンバー生成
			m_pRankNum[nNo]->UpdateMoveDest(D3DXVECTOR3(110.0f + (nNo * 160.0f), 300.0f, 0.0f), 30.0f);
		}

		if (m_pFlag[nNo] != NULL)
		{//国旗の表示
			m_pFlag[nNo]->UpdateMoveDest(D3DXVECTOR3(80.0f + (nNo * 160.0f), 85.0f, 0.0f), 30.0f);
		}

		if (m_pTable[nNo] != NULL)
		{//表の表示
			m_pTable[nNo]->UpdateMoveDest(D3DXVECTOR3(80.0f + (nNo * 160.0f), 500.0f, 0.0f), 30.0f);
		}

		if (m_pNameFlam[nNo] != NULL)
		{//名前のフレーム
			m_pNameFlam[nNo]->UpdateMoveDest(D3DXVECTOR3(75.0f + (nNo * 160.0f), 140.0f, 0.0f), 30.0f);
		}

		if (m_pName[nNo] != NULL)
		{//名前
			if (nNo < m_nMaxPlayer)
			{
				m_pName[nNo]->UpdateMoveDest(D3DXVECTOR3(20.0f + (nNo * 160.0f), 120.0f, 0.0f), 30.0f);
			}
			else
			{
				m_pName[nNo]->UpdateMoveDest(D3DXVECTOR3(35.0f + (nNo * 160.0f), 120.0f, 0.0f), 30.0f);
			}
		}

		if (m_pNameJapanese[nNo] != NULL)
		{//日本語表記の名前
			m_pNameJapanese[nNo]->UpdateMoveDest(D3DXVECTOR3(55.0f + (nNo * 160.0f), 145.0f, 0.0f), 30.0f);
		}

		if (m_pNumber[nNo] != NULL)
		{//ナンバー
			m_pNumber[nNo]->UpdateMoveDest(D3DXVECTOR3(125.0f + (nNo * 160.0f), 131.0f, 0.0f), 30.0f);
		}
	}

	//一定時間超えたら
	if (m_nTime == 100) { UninitCharRanking(); }//ランキングUIの破棄
	if (m_nTime > 120)
	{
		if (m_pTotalLogo == NULL)
		{//トータルロゴの表示
			m_pTotalLogo = CMoveUI::Create(D3DXVECTOR3(40.0f, 380.0f, 0.0f), D3DXVECTOR3(30.0f, 18.0f, 0.0f), m_pTexture[14]);
			m_pTotalLogo->SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}

		for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
		{//スコアの表示
			if (m_pTotalScore[nCnt] == NULL)
			{
				m_pTotalScore[nCnt] = CScore::Create(CScore::TYPE_GAME, 10, D3DXVECTOR3(40.0f + (160.0f * nCnt), 430.0f, 0.0f), D3DXVECTOR3(25.0f, 40.0f, 0.0f));
				m_pTotalScore[nCnt]->AddScore(m_CharResultScore[nCnt].nScore);
			}
		}
	}
	if (m_nTime > 140)
	{
		if (m_pGetNumLogo == NULL)
		{//獲得数ロゴの表示
			m_pGetNumLogo = CMoveUI::Create(D3DXVECTOR3(35.0f, 480.0f, 0.0f), D3DXVECTOR3(27.0f, 12.0f, 0.0f), m_pTexture[15]);
			m_pGetNumLogo->SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}

		for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
		{//獲得数の表示
			if (m_pGetScore[nCnt] == NULL)
			{
				m_pGetScore[nCnt] = CScore::Create(CScore::TYPE_GAME, 10, D3DXVECTOR3(40.0f + (160.0f * nCnt), 510.0f, 0.0f), D3DXVECTOR3(20.0f, 32.0f, 0.0f));
				m_pGetScore[nCnt]->AddScore(m_nCountGetTerritory[nCnt]);
			}
		}
	}
	if (m_nTime > 160)
	{
		if (m_pMakeNum == NULL)
		{//作成数ロゴの表示
			m_pMakeNum = CMoveUI::Create(D3DXVECTOR3(65.0f, 552.0f, 0.0f), D3DXVECTOR3(60.0f, 12.0f, 0.0f), m_pTexture[16]);
			m_pMakeNum->SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}

		for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
		{//作成数の表示
			if (m_pMakeScore[nCnt] == NULL)
			{
				m_pMakeScore[nCnt] = CScore::Create(CScore::TYPE_GAME, 10, D3DXVECTOR3(40.0f + (160.0f * nCnt), 582.0f, 0.0f), D3DXVECTOR3(20.0f, 32.0f, 0.0f));
				m_pMakeScore[nCnt]->AddScore(m_nCountMakeShape[nCnt]);
			}
		}
	}
	if (m_nTime > 180)
	{
		if (m_pRobbedNum == NULL)
		{//奪われた数ロゴの表示
			m_pRobbedNum = CMoveUI::Create(D3DXVECTOR3(65.0f, 622.0f, 0.0f), D3DXVECTOR3(60.0f, 12.0f, 0.0f), m_pTexture[17]);
			m_pRobbedNum->SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}

		for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
		{//奪われた数の表示
			if (m_pRobotedScore[nCnt] == NULL)
			{
				m_pRobotedScore[nCnt] = CScore::Create(CScore::TYPE_GAME, 10, D3DXVECTOR3(40.0f + (160.0f * nCnt), 652.0f, 0.0f), D3DXVECTOR3(20.0f, 32.0f, 0.0f));
				m_pRobotedScore[nCnt]->AddScore(m_nCountRobbotedTerritory[nCnt]);
			}
		}
	}
	if (m_nTime > 181)
	{
		//入力処理
		CInputXPad * pXPad = CManager::GetXPad();
		CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
		CRawMouse * pRawMouse = CManager::GetRawMouse();

		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
		{
			for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
			{
				CreateThankyou(nCnt);
			}
		}

		bool bEnter = false;
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true)
			{
				for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
				{
					if ((nCnt == m_CharResultScore[nCntPlayer].nControlerNo) && (CSelect::INPUTTYPE_GAMEPAD == m_CharResultScore[nCntPlayer].m_InputType))
					{
						CreateThankyou(m_CharResultScore[nCntPlayer].nCharNo);
						bEnter = true;
					}
				}
			}

			if (pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
			{
				for (int nCntPlayer = 0; nCntPlayer < m_nMaxPlayer; nCntPlayer++)
				{
					if ((nCnt == m_CharResultScore[nCntPlayer].nControlerNo) && (CSelect::INPUTTYPE_MOUSE == m_CharResultScore[nCntPlayer].m_InputType))
					{
						CreateThankyou(m_CharResultScore[nCntPlayer].nCharNo);
						bEnter = true;
					}
				}
			}
		}

		if (bEnter == true)
		{//コンピューターの移動処理
			for (int nCnt = m_nMaxPlayer; nCnt < MAX_CHARACTER; nCnt++)
			{
				CreateThankyou(nCnt);
			}
		}

		//画像の移動処理
		int nCountFlag = 0;
		for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
		{
			if (m_pThanksBack[nCnt] != NULL)
			{
				m_pThanksBack[nCnt]->UpdateMoveDest(D3DXVECTOR3(80.0f + (nCnt * 160.0f), 340.0f, 0.0f), 30.0f);
			}

			if (m_pThanksBG[nCnt] != NULL)
			{
				m_pThanksBG[nCnt]->UpdateMoveDest(D3DXVECTOR3(80.0f + (nCnt * 160.0f), 340.0f, 0.0f), 30.0f);
			}

			if (m_pThanks[nCnt] != NULL)
			{
				D3DXVECTOR3 Pos = D3DXVECTOR3(95.0f + (nCnt * 160.0f), 340.0f, 0.0f);

				//奇数だったら位置を修正する
				if ((nCnt % 2) != 0) { Pos.x += -20.0f; }
				m_pThanks[nCnt]->UpdateMoveDest(Pos, 30.0f);

				//奇数だったら位置を修正する
				m_pThanks[nCnt]->SetRotPos(0.95f);
				if ((nCnt % 2) != 0) { m_pThanks[nCnt]->SetRotPos(0.6f); }
			}

			//フラグをカウント
			if (m_bThanksFlagDown[nCnt] == true) { nCountFlag++; }
		}

		if (nCountFlag == MAX_CHARACTER)
		{//ランキング画面に移動する
			m_nFadeOutTime++;

			if (m_nFadeOutTime > 90)
			{
				if (CFade::FADE_OUT != CFade::GetFadeMode())
				{
					//決定音
					CSound::PlaySound(CSound::SOUND_LABEL_SE007);
					CFade::SetFade(CManager::MODE_RANKING);
				}
			}
		}
	}

}

//=============================================================================
// キャラUIの初期化処理
//=============================================================================
void CResult::InitCharUI(void)
{
	//背景のライン
	if (m_pBackLine == NULL)
	{
		m_pBackLine = CMoveUI::Create(D3DXVECTOR3(640.0f, 890.0f, 0.0f), D3DXVECTOR3(640.0f, 83.0f, 0.0f), m_pTexture[1]);
	}

	//キャラ表示
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pResultCharUI[nCnt] == NULL)
		{
			if (nCnt < 2)
			{
				m_pResultCharUI[nCnt] = CResultCharUI::Create(D3DXVECTOR3(170.0f + (nCnt * 290.0f), 850.0f + (nCnt * 8.0f), 0.0f), D3DXVECTOR3(150.0f + (nCnt * -15.0f), 200.0f + (nCnt * -15.0f), 0.0f), m_CharInfo[nCnt + 1].nCharNo, nCnt, m_CharInfo[nCnt + 1].nCuntry, m_CharInfo[nCnt + 1].nType);
			}
			else if (nCnt == 2)
			{
				m_pResultCharUI[nCnt] = CResultCharUI::Create(D3DXVECTOR3(720.0f, 850.0f + (nCnt * 8.0f), 0.0f), D3DXVECTOR3(150.0f + (nCnt * -15.0f), 200.0f + (nCnt * -15.0f), 0.0f), m_CharInfo[nCnt + 1].nCharNo, nCnt, m_CharInfo[nCnt + 1].nCuntry, m_CharInfo[nCnt + 1].nType);
			}
			else
			{
				int nCounter = nCnt;
				nCounter -= 3;
				m_pResultCharUI[nCnt] = CResultCharUI::Create(D3DXVECTOR3(895.0f + (nCounter * 108.0f), 870.0f, 0.0f), D3DXVECTOR3(52.0f, 160.0f, 0.0f), m_CharInfo[nCnt + 1].nCharNo, nCnt, m_CharInfo[nCnt + 1].nCuntry, m_CharInfo[nCnt + 1].nType);
			}
		}
	}
}

//=============================================================================
// スコア順にソートする
//=============================================================================
void CResult::SortScore(void)
{
	CGame *pGame = CManager::GetGame();			//ゲームの取得
	m_nMaxPlayer = CSelect::GetEntryPlayer();	//プレイヤーの総数を取得
	int nMaxEnemy = pGame->GetEnemyNumResult();	//エネミーの総数を取得

	//プレイヤーとエネミーの総数分まわす
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		//キャラクターの取得
		CCharacter *pCharacter = pGame->GetChara(nCnt);

		//キャラタイプの取得
		m_CharInfo[nCnt].nType = pCharacter->GetCharcterTypeResult(nCnt);
		m_CharResultScore[nCnt].nType = m_CharInfo[nCnt].nType;

		//キャラの国を取得
		m_CharInfo[nCnt].nCuntry = pCharacter->GetCuntryResult(nCnt);
		m_CharResultScore[nCnt].nCuntry = m_CharInfo[nCnt].nCuntry;

		//スコアの取得
		m_CharInfo[nCnt].nScore = pGame->GetScore(nCnt);
		m_CharResultScore[nCnt].nScore = m_CharInfo[nCnt].nScore;

		//キャラ番号の保存
		m_CharInfo[nCnt].nCharNo = nCnt;
		m_CharResultScore[nCnt].nCharNo = nCnt;
	}

	//スコアをソートする
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < MAX_CHARACTER; nCnt2++)
		{
			if (m_CharInfo[nCnt].nScore < m_CharInfo[nCnt2].nScore)
			{
				//スコアの入替え
				int nSubScore = m_CharInfo[nCnt].nScore;
				m_CharInfo[nCnt].nScore = m_CharInfo[nCnt2].nScore;
				m_CharInfo[nCnt2].nScore = nSubScore;

				//キャラタイプの入れ替え
				int nSubType = m_CharInfo[nCnt].nType;
				m_CharInfo[nCnt].nType = m_CharInfo[nCnt2].nType;
				m_CharInfo[nCnt2].nType = nSubType;

				//国タイプの入替え
				int nSubCuntry = m_CharInfo[nCnt].nCuntry;
				m_CharInfo[nCnt].nCuntry = m_CharInfo[nCnt2].nCuntry;
				m_CharInfo[nCnt2].nCuntry = nSubCuntry;

				//キャラ番号の入れ替え
				int nSubCharNo = m_CharInfo[nCnt].nCharNo;
				m_CharInfo[nCnt].nCharNo = m_CharInfo[nCnt2].nCharNo;
				m_CharInfo[nCnt2].nCharNo = nSubCharNo;
			}
		}
	}

	//マネージャーにスコア情報を渡す
	for (int nCnt = 0; nCnt < m_nMaxPlayer; nCnt++)
	{
		CManager::SetScore(nCnt, m_CharResultScore[nCnt].nScore);		//スコアの保存
		CManager::SetCuntry(nCnt, m_CharResultScore[nCnt].nCuntry);		//国番号の保存
		CManager::SetCharType(nCnt, m_CharResultScore[nCnt].nType);		//タイプの保存
	}
}

//=============================================================================
// 勝利キャラの生成
//=============================================================================
void CResult::InitWinerChar(int nCuntry, int nType, int nNumPlayer)
{
	if (nType == 0)
	{
		m_pWinChar = CCharResult::Create(D3DXVECTOR3(500.0f, 0.0f, 1000.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt",
										 CCharacter::CHARCTERTYPE_SPEED, nNumPlayer, nCuntry);
	}
	else if (nType == 1)
	{
		m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt",
										 CCharacter::CHARCTERTYPE_POWER, nNumPlayer, nCuntry);
	}
	else
	{
		m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt",
										 CCharacter::CHARCTERTYPE_TECHNIQUE, nNumPlayer, nCuntry);
	}
}

//=============================================================================
// Thankyouの画像生成処理
//=============================================================================
void CResult::CreateThankyou(int nCnt)
{
	if (m_pThanksBack[nCnt] == NULL)
	{//Thanks背景の生成
		m_pThanksBack[nCnt] = CMoveUI::Create(D3DXVECTOR3(80.0f + (nCnt * 160.0f), -610.0f, 0.0f), D3DXVECTOR3(80.0f, 335.0f, 0.0f), m_pTexture[18]);

		//色の設定
		if (nCnt < m_nMaxPlayer)
		{
			m_pThanksBack[nCnt]->SetColoer(CCharacter::m_CountryColor[nCnt]);
		}
		else
		{
			m_pThanksBack[nCnt]->SetColoer(m_Color[4]);
		}
	}

	if (m_pThanksBG[nCnt] == NULL)
	{//Thanks背景の生成
		m_pThanksBG[nCnt] = CMoveUI::Create(D3DXVECTOR3(80.0f + (nCnt * 160.0f), -610.0f, 0.0f), D3DXVECTOR3(70.0f, 50.0f, 0.0f), m_pTexture[19]);

		//奇数だったら画像を反転させる
		if ((nCnt % 2) != 0) { m_pThanksBG[nCnt]->SetReflectRightLeft(); }

		//色の設定
		if (nCnt < m_nMaxPlayer)
		{
			m_pThanksBG[nCnt]->SetColoer(m_Color[nCnt]);
		}
		else
		{
			m_pThanksBG[nCnt]->SetColoer(m_Color[4]);
		}
	}

	if (m_pThanks[nCnt] == NULL)
	{//Thankyou for Playingを生成
		D3DXVECTOR3 Pos = D3DXVECTOR3(95.0f + (nCnt * 160.0f), -610.0f, 0.0f);

		//奇数だったら位置を修正する
		if ((nCnt % 2) != 0) { Pos.x += -20.0f; }

		m_pThanks[nCnt] = CMoveUI::Create(Pos, D3DXVECTOR3(45.0f, 30.0f, 0.0f), m_pTexture[20]);
		m_pThanks[nCnt]->SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_pThanks[nCnt]->SetRotPos(0.95f);

		//奇数だったら位置を修正する
		if ((nCnt % 2) != 0) { m_pThanks[nCnt]->SetRotPos(0.6f); }

	}

	//画像を下すフラグ
	if (m_bThanksFlagDown[nCnt] == false) { m_bThanksFlagDown[nCnt] = true; }
}

//=============================================================================
// 風船の生成
//=============================================================================
void CResult::CreateBarun(void)
{
	int nCntColor = 0;
	D3DXVECTOR3 move = INITIALIZE_VECTOR3;

	for (int nCnt = 0; nCnt < 30; nCnt++)
	{
		nCntColor = (nCntColor + 1) % 5;
		int nRand = rand() % 20 + 10;
		move.y = (float)nRand * 0.1f;
		move.z += MOVE_XZ;
		move.x += MOVE_XZ;
		if (move.z > 3.0f) { move.z = -3.0f; }
		if (move.x > 3.0f) { move.x = -3.0f; }

		nCntColor = (nCntColor + 1) % 5;
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), 300.0f, 150.0f), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * (MIDLLE + 5.0f)), 100.0f, 1500.0f), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * (MIDLLE + 10.0f)), 50.0f, 1400.0f), (nCntColor + 1) % 5, move);

		nRand = rand() % 20 + 10;
		move.y = (float)nRand * 0.1f;
		nCntColor = (nCntColor + 1) % 5;
		move.z += MOVE_XZ;
		move.x += MOVE_XZ;
		if (move.z > 3.0f) { move.z = -3.0f; }
		if (move.x > 3.0f) { move.x = -3.0f; }

		nCntColor = (nCntColor + 1) % 5;
		CBarun::Create(D3DXVECTOR3(-1400.0f, 50.0f, -1200.0f + (nCnt * MIDLLE)), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(-1500.0f, 150.0f, -1200.0f + (MIDLLE + 5.0f)), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(-1600.0f, 200.0f, -1200.0f + (MIDLLE + 10.0f)), (nCntColor + 1) % 5, move);

		CBarun::Create(D3DXVECTOR3(-1400.0f, -50.0f, -1200.0f + (nCnt * MIDLLE)), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(-1500.0f, -150.0f, -1200.0f + (MIDLLE + 5.0f)), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(-1600.0f, -200.0f, -1200.0f + (MIDLLE + 10.0f)), (nCntColor + 1) % 5, move);
	}
}

//=============================================================================
// キャライメージ画像の読み込み処理
//=============================================================================
void CResult::InitCharImage(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[0]);
	D3DXCreateTextureFromFile(pDevice, CHR_RUSSIA, &m_pCharaTex[1]);
	D3DXCreateTextureFromFile(pDevice, CHR_AMERICA, &m_pCharaTex[2]);
	D3DXCreateTextureFromFile(pDevice, CHR_BRAZIL, &m_pCharaTex[3]);
	D3DXCreateTextureFromFile(pDevice, CHR_ENGLAND, &m_pCharaTex[4]);
	D3DXCreateTextureFromFile(pDevice, CHR_ITALY, &m_pCharaTex[5]);
	D3DXCreateTextureFromFile(pDevice, CHR_NEWZEALAND, &m_pCharaTex[6]);
	D3DXCreateTextureFromFile(pDevice, CHR_SOUTHAFRICA, &m_pCharaTex[7]);
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void CResult::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//国旗テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, FLAG_JAPAN, &m_pCuntryTex[0]);		//日本
	D3DXCreateTextureFromFile(pDevice, FLAG_RUSSIA, &m_pCuntryTex[1]);		//ロシア
	D3DXCreateTextureFromFile(pDevice, FLAG_AMERICA, &m_pCuntryTex[2]);		//アメリカ
	D3DXCreateTextureFromFile(pDevice, FLAG_BRAZIL, &m_pCuntryTex[3]);		//ブラジル
	D3DXCreateTextureFromFile(pDevice, FLAG_ENGLAND, &m_pCuntryTex[4]);		//イギリス
	D3DXCreateTextureFromFile(pDevice, FLAG_ITALY, &m_pCuntryTex[5]);		//イタリア
	D3DXCreateTextureFromFile(pDevice, FLAG_NEWZEALAND, &m_pCuntryTex[6]);	//ニュージーランド
	D3DXCreateTextureFromFile(pDevice, FLAG_SOUTHAFRICA, &m_pCuntryTex[7]);	//南アフリカ

	//リザルトテクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PLAYERNAME, &m_pTexture[0]);		//プレイヤーネーム
	D3DXCreateTextureFromFile(pDevice, BACK_LINE, &m_pTexture[1]);		//バックライン
	D3DXCreateTextureFromFile(pDevice, RANK_FLAM, &m_pTexture[2]);		//ランキングフレーム
	D3DXCreateTextureFromFile(pDevice, RANK_ALLNUM, &m_pTexture[3]);	//番号テクスチャ
	D3DXCreateTextureFromFile(pDevice, COM_NAME, &m_pTexture[4]);		//コンピューターの名前
	D3DXCreateTextureFromFile(pDevice, RANKING_FLAM, &m_pTexture[5]);	//ランキングフレーム
	D3DXCreateTextureFromFile(pDevice, CPU_TEX, &m_pTexture[6]);		//CPUロゴ
	D3DXCreateTextureFromFile(pDevice, P_TEX, &m_pTexture[7]);			//Pロゴ
	D3DXCreateTextureFromFile(pDevice, RESULT_FLAG, &m_pTexture[8]);	//表用の国旗
	D3DXCreateTextureFromFile(pDevice, RESULT_TABLE, &m_pTexture[9]);	//表
	D3DXCreateTextureFromFile(pDevice, NAME_FLAM, &m_pTexture[10]);		//名前のフレーム
	D3DXCreateTextureFromFile(pDevice, NUMBER_TEX, &m_pTexture[11]);	//ナンバー
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &m_pTexture[12]);	//プレイヤー
	D3DXCreateTextureFromFile(pDevice, COM_TEX, &m_pTexture[13]);		//コンピューター
	D3DXCreateTextureFromFile(pDevice, TOTAL_TEX, &m_pTexture[14]);		//合計
	D3DXCreateTextureFromFile(pDevice, GETNUM_TEX, &m_pTexture[15]);	//獲得
	D3DXCreateTextureFromFile(pDevice, MAKENUM_TEX, &m_pTexture[16]);	//作成数
	D3DXCreateTextureFromFile(pDevice, ROBBED_TEX, &m_pTexture[17]);	//奪われた数
	D3DXCreateTextureFromFile(pDevice, THANKSBACK_TEX, &m_pTexture[18]); //Thankyouの背景
	D3DXCreateTextureFromFile(pDevice, THANKSBG_TEX, &m_pTexture[19]);	//Thankyouの背景
	D3DXCreateTextureFromFile(pDevice, THANKS_TEX, &m_pTexture[20]);	//Thankyou
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CResult::UnloadTex(void)
{
	//国旗のテクスチャの破棄
	for (int nCnt = 0; nCnt < CUNTRY; nCnt++)
	{
		if (m_pCuntryTex[nCnt] != NULL)
		{
			m_pCuntryTex[nCnt]->Release();
			m_pCuntryTex[nCnt] = NULL;
		}
	}

	//リザルトテクスチャの破棄
	for (int nCnt = 0; nCnt < RESULT_TEX; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// キャラクターのスコアを計算する
//=============================================================================
void CResult::GetScoreResult(void)
{
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		m_nCountMakeShape[nCnt] = CGame::GetCountMakeShape(nCnt);				//図形の作った数を取得
		m_nCountGetTerritory[nCnt] = CGame::GetCountGetTerritory(nCnt);			//テリトリーの取得数
		m_nCountRobbotedTerritory[nCnt] = CGame::GetRobbotedTerritory(nCnt);	//テリトリーの奪われた数
	}
}