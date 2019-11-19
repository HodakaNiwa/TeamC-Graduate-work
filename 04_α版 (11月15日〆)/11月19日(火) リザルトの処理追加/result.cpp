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
#define CHR_JAPAN			("data/TEXTURE/result/Char.png")

//画面が白い状態時のマクロ
#define FLAG_WHIGHTSIZE		(D3DXVECTOR3(130.0f, 100.0f, 0.0f))
#define FLAG_AIMSIZE		(D3DXVECTOR3(90.0f, 70.0f, 0.0f))

//ズーム状態のマクロ
#define CHANGE_TIME	(90)

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
	m_state = STATE_WHITE;
	m_bWhiteIn = false;
	m_nCharFlagDown = false;
	m_nMaxPlayer = 0;
	m_nCountID = 0;
	m_nCounter = 0;
	m_nTime = 0;

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
		m_CharInfo[nCnt] = { 0, 0, 0, 7 };
		m_CharResultScore[nCnt] = { 0, 0, 0, 7 };
		m_pCharImage[nCnt] = NULL;
		m_pCharaTex[nCnt] = NULL;
	}

	//テクスチャの読み込み
	LoadTex();

	//地面の生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//オブジェクトの生成
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリーの読み込み

	//スコア順にソートする
	SortScore();

	//キャラテクスチャを読み込む
	InitCharImage();

	//勝者キャラの生成
	InitWinerChar(m_CharInfo[0].nCuntry, m_CharInfo[0].nType);

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

	//キャラテクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		if (m_pCharaTex[nCnt] != NULL)
		{
			m_pCharaTex[nCnt]->Release();
			m_pCharaTex[nCnt] = NULL;
		}
	}

	//テクスチャの破棄
	UnloadTex();

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
	
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{
			//決定音
			CSound::PlaySound(CSound::SOUND_LABEL_SE007);
			CFade::SetFade(CManager::MODE_TITLE);
		}
	}

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

	if (pKeyboard->GetKeyboardTrigger(DIK_SPACE) == true)
	{
		m_state = STATE_SCORERESULT;	//スコア表示状態にする
		InitScoreResult();				//スコア表の初期化
	}

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) == true)
		{
			m_state = STATE_SCORERESULT;	//スコア表示状態にする
			InitScoreResult();				//スコア表の初期化
		}

		if (pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_LEFT) == true)
		{
			m_state = STATE_SCORERESULT;	//スコア表示状態にする
			InitScoreResult();				//スコア表の初期化
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
		if (m_pCharImage[nCnt] == NULL)
		{
			m_pCharImage[nCnt] = CMoveUI::Create(D3DXVECTOR3(80.0f + (nCnt * 160.0f), -550.0f, 0.0f), D3DXVECTOR3(80.0f, 275.0f, 0.0f), m_pCharaTex[nCnt]);
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
		m_nCountID++;
		
		//値の修正
		if (m_nCountID > MAX_CHARACTER) { m_nCountID = MAX_CHARACTER; }
	}

	for (int nCnt = 0; nCnt < m_nCountID; nCnt++)
	{
		if (m_pCharImage[nCnt] != NULL)
		{
			m_pCharImage[nCnt]->UpdateMoveDest(D3DXVECTOR3(80.0f + (nCnt * 160.0f), 400.0f, 0.0f), 30.0f);
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
	int nMaxEnemy = pGame->GetEnemyNum();		//エネミーの総数を取得

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

	for (int nCnt = m_nMaxPlayer; nCnt < MAX_CHARACTER; nCnt++)
	{
		//キャラタイプの取得
		m_CharInfo[nCnt].nType = 0;
		m_CharResultScore[nCnt].nType = 0;

		//キャラの国を取得
		m_CharInfo[nCnt].nCuntry = 0;
		m_CharResultScore[nCnt].nCuntry = 0;

		//スコアの取得
		m_CharInfo[nCnt].nScore = 0;
		m_CharResultScore[nCnt].nScore = 0;
	}

	//スコアをソートする
	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		for (int nCnt2 = nCnt; nCnt2 < MAX_CHARACTER - 1; nCnt2++)
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
}

//=============================================================================
// 勝利キャラの生成
//=============================================================================
void CResult::InitWinerChar(int nCuntry, int nType)
{
	switch (nCuntry)
	{
	case CCharacter::TYPE_JAPAN:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_RUSSIA:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_AMERICA:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_BRAZIL:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_ENGLAND:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_ITALY:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_NEWZEALAND:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;

	case CCharacter::TYPE_SOUTHAFRICA:
		if (nType == 0)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		}
		else if (nType == 1)
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		}
		else
		{
			m_pWinChar = CCharResult::Create(INITIALIZE_VECTOR3, "data\\TEXT\\ModelLoad\\PLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\PLAYER_MOTION.txt");
		}
		break;
	}
}

//=============================================================================
// キャライメージ画像の読み込み処理
//=============================================================================
void CResult::InitCharImage(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_CHARACTER; nCnt++)
	{
		switch (m_CharResultScore[nCnt].nCuntry)
		{
		case CCharacter::TYPE_JAPAN:	//日本
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_RUSSIA:	//ロシア
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_AMERICA:	//アメリカ
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_BRAZIL:	//ブラジル
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_ENGLAND:	//イギリス
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_ITALY:	//イタリア
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_NEWZEALAND:	//ニュージーランド
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;

		case CCharacter::TYPE_SOUTHAFRICA:	//南アフリカ
			if (m_CharResultScore[nCnt].nType == 0)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else if (m_CharResultScore[nCnt].nType == 1)
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			else
			{
				D3DXCreateTextureFromFile(pDevice, CHR_JAPAN, &m_pCharaTex[nCnt]);
			}
			break;
		}
	}
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