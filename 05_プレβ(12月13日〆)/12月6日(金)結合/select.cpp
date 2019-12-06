//=============================================================================
//
// セレクトの処理 [select.cpp]
// Author : 佐藤 安純
//
//=============================================================================
#include "select.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "loadText.h"
#include "fieldmanager.h"
#include "playericon.h"
#include "moveUI.h"
#include "charSelect.h"
#include "RawMouse.h"
#include "collision.h"
#include "sky.h"
#include "audience.h"
#include "ringRender.h"
#include "territoryRender.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOAD_UI				("data\\TEXT\\UI\\Select.txt")
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
#define SELECT_1P			("data/TEXTURE/Select/NumPlayer/1P.png")
#define SELECT_2P			("data/TEXTURE/Select/NumPlayer/2P.png")
#define SELECT_3P			("data/TEXTURE/Select/NumPlayer/3P.png")
#define SELECT_4P			("data/TEXTURE/Select/NumPlayer/4P.png")
#define NAME_JAPAN			("data/TEXTURE/Select/CountryName/JPN.png")
#define NAME_RUSSIA			("data/TEXTURE/Select/CountryName/RUS.png")
#define NAME_AMERICA		("data/TEXTURE/Select/CountryName/USA.png")
#define NAME_BRAZIL			("data/TEXTURE/Select/CountryName/BRA.png")
#define NAME_ENGLAND		("data/TEXTURE/Select/CountryName/GBR.png")
#define NAME_ITALY			("data/TEXTURE/Select/CountryName/ITA.png")
#define NAME_NEWZEALAND		("data/TEXTURE/Select/CountryName/NGL.png")
#define NAME_SOUTHAFRICA	("data/TEXTURE/Select/CountryName/ZAF.png")
#define NO_ENTRY			("data/TEXTURE/Select/NoEntry.png")
#define LOGO				("data/TEXTURE/Select/CountrySelect_Title.png")
#define FLAM				("data/TEXTURE/Select/select_window.png")
#define LOGO2				("data/TEXTURE/Select/PlayerSelect_title.png")
#define NO_ENTRY_CHR		("data/TEXTURE/Select/NoEntryChar.png")
#define BACK_CUNTRY			("data/TEXTURE/Select/BackArrow.png")
#define RADY_TO_FIGHT		("data/TEXTURE/Select/READY_TO_FIGHT.png")
#define FLAG_SIZE			(D3DXVECTOR3(100.0f, 70.0f, 0.0f))
#define PLAYER_SIZE			(D3DXVECTOR3(130.0f, 75.0f, 0.0f))
#define NAME_SIZE			(D3DXVECTOR3(80.0f, 20.0f, 0.0f))
#define NAME_POS			(D3DXVECTOR3(150.0f, 527.0f, 0.0f))
#define NAME_MOVE			(320.0f)
#define MOVE_X				(280.0f)
#define MOVE_Y				(200.0f)
#define NOENTRY_SIZE		(D3DXVECTOR3(150.0f, 100.0f, 0.0f))
#define NOENTRY_POS			(D3DXVECTOR3(170.0f, 605.0f, 0.0f))
#define NOENTRY_POS_REMAKE	(D3DXVECTOR3(-475.0f, 605.0f, 0.0f))
#define NOENTRY_MOVE		(315.0f)
#define MOVE_UP				(2.5f)
#define MOVE_DOWN			(3.4f)
#define MOVE_TIME			(70)
#define CHAR_BACK_SIZE		(D3DXVECTOR3(150.0f, 170.0f, 0.0f))
#define CHAR_BACK_POS		(D3DXVECTOR3(170.0f, 340.0f, 0.0f))
#define CHAR_BACK_MOVE		(313.0f)
#define BACKBUTTON_SIZE		(D3DXVECTOR3(30.0f, 30.0f, 0.0f))
#define TIME_BACKBOTTUN		(30)
#define CHANGE_CHARBACK		(D3DXVECTOR2(2.5f, 3.0f))
#define CHANGE_CHARBACK2	(D3DXVECTOR2(2.3f, 2.5f))
#define CHANGE_FLAM			(D3DXVECTOR2(5.0f, 4.0f))
#define CHANGE_BACKBUTTON	(D3DXVECTOR2(0.3f, 0.3f))
#define FIRST_SIZE			(D3DXVECTOR3(40.0f, 40.0f, 0.0f))


//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CMoveUI * CSelect::m_pNationalFlag[MAX_COUNTRY] = {};
int CSelect::m_nEntryPlayer = 0;
int CSelect::m_anPlayeCuntry[NUMPLAYER] = { -1, -1, -1, -1 };
int CSelect::m_anPlayerType[NUMPLAYER] = { -1, -1, -1, -1 };
int CSelect::m_anControler[NUMPLAYER] = { -1, -1, -1, -1 };
CSelect::INPUTTYPE CSelect::m_anInputType[NUMPLAYER] = { INPUTTYPE_MAX, INPUTTYPE_MAX, INPUTTYPE_MAX, INPUTTYPE_MAX };

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSelect::CSelect(){}

//=============================================================================
// デストラクタ
//=============================================================================
CSelect::~CSelect(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CSelect::Init(void)
{
	//ベースの初期化
	CModeBase::Init();

	//変数の初期化
	m_pCamera = NULL;
	m_pFieldManager = NULL;
	m_pLoadTextObj = NULL;
	m_pLoadTerritory = NULL;
	m_pTexNoEntry = NULL;
	m_pLogo = NULL;
	m_pBackButton = NULL;
	m_pRedyFight = NULL;
	m_nEntryPlayer = 0;
	m_state = STATE_CUNTRY_SELECT;
	m_nCountTime = 0;
	m_nNumControler = 0;
	m_nCounter = 0;

	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		m_apTextureCountry[nCnt] = NULL;	//国旗テクスチャ
		m_apTexCuntoryName[nCnt] = NULL;	//国名テクスチャ
		m_pNationalFlag[nCnt] = NULL;		//国旗ポインタ
	}

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		m_apTexPlayer[nCnt] = NULL;				//プレイヤーアイコンテクスチャ
		m_pNumPlayer[nCnt] = NULL;				//プレイヤーアイコンポインタ
		m_pNoEntry[nCnt] = NULL;				//エントリーしているかどうか
		m_pFram[nCnt] = NULL;					//フレーム
		m_pCharBack[nCnt] = NULL;				//キャラ背景
		m_pCuntoryName[nCnt] = NULL;			//国名
		m_nCntPressTime[nCnt] = 0;				//ボタンを押した時間
		m_nCntPressTime2[nCnt] = 0;				//ボタンを押した時間
		m_anPlayeCuntry[nCnt] = -1;				//国
		m_anPlayerType[nCnt] = -1;				//タイプ
		m_pSelectChar[nCnt] = NULL;				//キャラ選択UI
		m_bPlayerEnter[nCnt] = false;			//エントリーしているかどうか
		m_bEntryGamePad[nCnt] = false;			//コントローラーがエントリーしているかどうか
		m_bEntryMouse[nCnt] = false;			//マウスがエントリーしているかどうか
		m_anControler[nCnt] = -1;				//コントローラーの番号
		m_anInputType[nCnt] = INPUTTYPE_MAX;	//入力デバイスのタイプ
	}
	m_bEntryKeyboard = false;					//キーボードがエントリーしてるかどうか

	for (int nCnt = 0; nCnt < SELECT_TEX; nCnt++)
	{
		m_pTex[nCnt] = NULL;
	}

	//テクスチャの読み込み処理
	LoadTexture();
	CSelectCharacter::LoadTex();

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

		m_pCamera = new CRotationCamera;
		m_pCamera->Init(800.0f, D3DXVECTOR3(0.0f, 400.0f, 0.0f), 0.001f, -600.0f);
		m_pCamera->SetViewport(viewport);
	}

	//地面の生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//オブジェクトの生成
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリーの読み込み

	//国選択モードのUI生成処理
	InitCuntrySelect();

	// 観客の生成
	CAudience::Create();

	// 空の生成
	CSky::Load();
	CSky::Create();

	// リング描画クラスを生成
	CRingRender::Create();

	// テリトリー描画クラスを生成
	CTerritoryRender::Create();
}

//=============================================================================
// 終了処理
//=============================================================================
void CSelect::Uninit(void)
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

	for (int nCntUI = 0; nCntUI < NUMPLAYER; nCntUI++)
	{
		if (m_pSelectChar[nCntUI] != NULL)
		{
			m_pSelectChar[nCntUI]->Uninit();
			delete m_pSelectChar[nCntUI];
			m_pSelectChar[nCntUI] = NULL;
		}
	}

	//テクスチャの破棄
	UnloadTexture();
	CSelectCharacter::UnloadTex();

	// 空テクスチャの破棄
	CSky::UnLoad();

	//フェード以外削除
	CScene::ReleaseFade();

	//死亡フラグ
	CScene::DeathCheck();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSelect::Update(void)
{
	//サウンドの取得
	CSound *pSound = CManager::GetSound();

	//インプットの取得　
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	//国名のテクスチャの切り替え
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pCuntoryName[nCnt] != NULL)
		{
			m_pCuntoryName[nCnt]->BindTexture(m_apTexCuntoryName[m_pNumPlayer[nCnt]->GetNumCuntry()]);
		}
	}

	//カメラの更新処理
	if (m_pCamera != NULL) { m_pCamera->Update(); }

	//状態ごとの更新処理
	switch (m_state)
	{
	case STATE_CUNTRY_SELECT: UpdateCuntrySelect(); break;			//国選択
	case STATE_CHARACTER_SELECT: UpdateCharacterSelect(); break;	//キャラ選択
	case STATE_CHANGE: UpdateChange(); break;						//モード切替
	case STATE_BACKCHANGE: UpdateBackChange(); break;				//国選択画面に戻る
	case STATE_DICISION: UpdateDicision(); break;					//決定
	}

	// 地面の更新処理
	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	CScene::DeathCheck();
	CCollider::DeathCheck();
}

//=============================================================================
// 描画処理
//=============================================================================
void CSelect::Draw(void)
{
	//カメラの設定
	if (m_pCamera != NULL) { m_pCamera->Set(); }

	//すべて描画する
	CScene::DrawAll();
}

//=============================================================================
// 国選択の初期化処理
//=============================================================================
void CSelect::InitCuntrySelect(void)
{
	//ロゴの設定
	if (m_pLogo == NULL) { m_pLogo = CMoveUI::Create(D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR3(640.0f, 50.0f, 0.0f), m_pTex[0]); }

	//プレイヤーの選択表示生成
	for (int nCnt = NUMPLAYER - 1; nCnt >= 0; nCnt--)
	{
		m_pNumPlayer[nCnt] = CPlayerIcon::Create(D3DXVECTOR3(255.0f, 200.0f, 0.0f), nCnt);
		m_pNumPlayer[nCnt]->BindTexture(m_apTexPlayer[nCnt]);
		m_pNumPlayer[nCnt]->SetDraw(false);
	}

	//国旗の表示
	int nCountFlag = 0;;
	for (int nCntY = 0; nCntY < 2; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			m_pNationalFlag[nCountFlag] = CMoveUI::Create(D3DXVECTOR3(230.0f + (nCntX * MOVE_X), 200.0f + (nCntY * MOVE_Y), 0.0f), FLAG_SIZE, m_apTextureCountry[nCountFlag]);
			nCountFlag++;
		}
	}

	//フレーム・エントリーしていないテクスチャの表示
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pFram[nCnt] == NULL)
		{
			m_pFram[nCnt] = CMoveUI::Create(D3DXVECTOR3(NOENTRY_POS.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS.y, 0.0f), NOENTRY_SIZE, m_pTex[1]);
		}

		if (m_pNoEntry[nCnt] == NULL)
		{
			m_pNoEntry[nCnt] = CMoveUI::Create(D3DXVECTOR3(NOENTRY_POS.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS.y, 0.0f), NOENTRY_SIZE, m_pTexNoEntry);
		}
	}
}

//=============================================================================
// 国選択の更新処理
//=============================================================================
void CSelect::UpdateCuntrySelect(void)
{
	//入力デバイスの取得
	CGamePad * pGamePad = CManager::GetInputGamePad();
	CInputXPad * pXPad = CManager::GetXPad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true)
		{
			PlayerEnter(nCnt, INPUTTYPE_GAMEPAD);
		}

		//マウスの入力処理
		if (pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
		{
			PlayerEnter(nCnt, INPUTTYPE_MOUSE);
		}
	}

	//キーボード入力
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		PlayerEnter(0, INPUTTYPE_KEYBOARD);
		m_nNumControler = 0;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_2) == true)
	{
		PlayerEnter(1, INPUTTYPE_GAMEPAD);
		m_nNumControler = 1;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_3) == true)
	{
		PlayerEnter(2, INPUTTYPE_GAMEPAD);
		m_nNumControler = 2;
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_4) == true)
	{
		PlayerEnter(3, INPUTTYPE_GAMEPAD);
		m_nNumControler = 3;
	}

	//国名の表示処理
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//国名の生成
		if ((m_pCuntoryName[nCnt] == NULL) && (m_pNumPlayer[nCnt] != NULL))
		{
			if (m_pNumPlayer[nCnt]->GetDraw() == false) { continue; }

			//テクスチャ番号の設定
			int nTexNum = m_pNumPlayer[nCnt]->GetNumCuntry();
			if (nTexNum == -1) { nTexNum = 0; }
			m_pNumPlayer[nCnt]->SetNumCuntry(nTexNum);

			m_pCuntoryName[nCnt] = CMoveUI::Create(D3DXVECTOR3(NAME_POS.x + (nCnt * NAME_MOVE), NAME_POS.y, 0.0f), NAME_SIZE, m_apTexCuntoryName[nTexNum]);

			//NO ENTRYの破棄
			if (m_pNoEntry[nCnt] != NULL)
			{
				m_pNoEntry[nCnt]->Uninit();
				m_pNoEntry[nCnt] = NULL;
			}
		}
	}
}

//=============================================================================
// 国選択の破棄
//===========================================================================
void CSelect::UninitCuntrySelect(void)
{
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//フレーム
		if(m_pFram[nCnt] != NULL)
		{
			m_pFram[nCnt]->Uninit();
			m_pFram[nCnt] = NULL;
		}

		//NO ENTRY
		if (m_pNoEntry[nCnt] != NULL)
		{
			m_pNoEntry[nCnt]->Uninit();
			m_pNoEntry[nCnt] = NULL;
		}

		//国名
		if (m_pCuntoryName[nCnt] != NULL)
		{
			m_pCuntoryName[nCnt]->Uninit();
			m_pCuntoryName[nCnt] = NULL;
		}

		//プレイヤーのアイコン
		if (m_pNumPlayer[nCnt] != NULL)
		{
			m_pNumPlayer[nCnt]->Uninit();
			m_pNumPlayer[nCnt] = NULL;
		}
	}

	//国旗の破棄
	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		if (m_pNationalFlag[nCnt] != NULL)
		{
			m_pNationalFlag[nCnt]->Uninit();
			m_pNationalFlag[nCnt] = NULL;
		}
	}
}

//=============================================================================
// キャラ選択の初期化処理
//=============================================================================
void CSelect::InitCharacterSelect(void)
{
	//プレイヤータイプ選択ロゴに設定
	if (m_pLogo != NULL) { m_pLogo->BindTexture(m_pTex[2]); }


	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//フレーム・エントリーしていないテクスチャの表示
		if (m_pFram[nCnt] == NULL)
		{
			m_pFram[nCnt] = CMoveUI::Create(D3DXVECTOR3(NOENTRY_POS.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS.y, 0.0f), D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_pTex[1]);
		}

		//キャラ背景の生成
		if (m_pCharBack[nCnt] == NULL)
		{
			D3DXVECTOR3 Pos = D3DXVECTOR3(CHAR_BACK_POS.x + (nCnt * CHAR_BACK_MOVE), CHAR_BACK_POS.y, 0.0f);		//位置計算

			if (m_bPlayerEnter[nCnt] == true)
			{//エントリーしている場合
				m_pCharBack[nCnt] = CMoveUI::Create(Pos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_apTextureCountry[m_anPlayeCuntry[nCnt]]);
				m_pCharBack[nCnt]->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f));	//色の設定

				if (m_pSelectChar[nCnt] == NULL)
				{//キャラ選択UIの生成
					Pos.y -= 5.0f;
					m_pSelectChar[nCnt] = CSelectCharacter::Create(Pos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_anPlayeCuntry[nCnt], nCnt, m_anControler[nCnt]);
					m_pSelectChar[nCnt]->SetInputType(m_anInputType[nCnt]);
				}
			}
			else
			{//エントリーしていない場合
				Pos.y -= 5.0f;
				m_pCharBack[nCnt] = CMoveUI::Create(Pos, D3DXVECTOR3(40.0f, 40.0f, 0.0f), m_pTex[3]);
			}
		}
	}

	//バックボタンの生成
	if (m_pBackButton == NULL) { m_pBackButton = CMoveUI::Create(D3DXVECTOR3(50.0f, 140.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), m_pTex[4]); }
}

//=============================================================================
// キャラ選択の更新処理
//=============================================================================
void CSelect::UpdateCharacterSelect(void)
{
	//入力デバイスの取得
	CGamePad * pGamePad = CManager::GetInputGamePad();
	CInputXPad * pXPad = CManager::GetXPad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//ゲームパッド入力処理
		if (pXPad->GetPress(XINPUT_GAMEPAD_B, nCnt) == true)
		{
			m_nCntPressTime[nCnt]++;

			if ((m_nCntPressTime[nCnt] % TIME_BACKBOTTUN) == 0)
			{
				SetState(STATE_BACKCHANGE);	//前回の画面に戻る
			}
		}
		else
		{
			m_nCntPressTime[nCnt] = 0;
		}

		//マウスの入力処理
		if (pRawMouse->GetPress(nCnt, CRawMouse::RIMS_BUTTON_LEFT) == true)
		{
			m_nCntPressTime2[nCnt]++;

			if ((m_nCntPressTime2[nCnt] % TIME_BACKBOTTUN) == 0)
			{
				SetState(STATE_BACKCHANGE);	//前回の画面に戻る
			}
		}
		else
		{
			m_nCntPressTime2[nCnt] = 0;
		}
	}
	

	//キーボードの入力処理
	if (pKeyboard->GetKeyboardTrigger(DIK_B) == true)
	{ SetState(STATE_BACKCHANGE); }	//前回の画面に戻る

	for (int nCntEntry = 0; nCntEntry < m_nEntryPlayer; nCntEntry++)
	{
		if (pXPad->GetTrigger(XINPUT_GAMEPAD_START, nCntEntry) == true)
		{
			SetState(STATE_BACKCHANGE);
		}

		//マウスの入力処理
		if (pRawMouse->GetPress(nCntEntry, CRawMouse::RIMS_BUTTON_LEFT) == true)
		{
			SetState(STATE_BACKCHANGE);

		}
	}

	int nCntEnter = 0;;
	for (int nCntNumPlayer = 0; nCntNumPlayer < NUMPLAYER; nCntNumPlayer++)
	{
		if (m_pSelectChar[nCntNumPlayer] != NULL)
		{
			m_pSelectChar[nCntNumPlayer]->Update();

			bool bEnter = m_pSelectChar[nCntNumPlayer]->GetbEnter();
			if (bEnter == true) { nCntEnter++; }	//決定した人数を加算
		}
	}

	//決定数が同じになったら決定状態にする
	if (nCntEnter == m_nEntryPlayer) { SetState(STATE_DICISION); }
}

//=============================================================================
// キャラ選択の破棄処理
//=============================================================================
void CSelect::UninitCharacterSelect(void)
{
	//拡大処理
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		//キャラフレーム
		if (m_pCharBack[nCnt] != NULL)
		{
			m_pCharBack[nCnt]->Uninit();
			m_pCharBack[nCnt] = NULL;
		}

		//キャラ選択UI
		if (m_pSelectChar[nCnt] != NULL)
		{
			m_pSelectChar[nCnt]->UninitUI();	//UIを消す
			m_pSelectChar[nCnt]->Uninit();
			delete m_pSelectChar[nCnt];
			m_pSelectChar[nCnt] = NULL;
		}

		//フレーム
		if (m_pFram[nCnt] != NULL)
		{
			m_pFram[nCnt]->Uninit();
			m_pFram[nCnt] = NULL;
		}
	}

	//ボタン
	if (m_pBackButton != NULL)
	{
		m_pBackButton->Uninit();
		m_pBackButton = NULL;
	}

	//ロゴの破棄
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}
}

//=============================================================================
// モード切り替え処理
//=============================================================================
void CSelect::UpdateChange(void)
{
	m_nCountTime++;

	if (m_nCountTime < MOVE_TIME)
	{
		//上移動
		if (m_pLogo != NULL) { m_pLogo->UpdateMoveUp(MOVE_UP); }

		//左右移動
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			//フレーム
			if (m_pFram[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//左
					m_pFram[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT);
				}
				else
				{//右
					m_pFram[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT);
				}
			}

			//NO ENTRY
			if (m_pNoEntry[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//左
					m_pNoEntry[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT);
				}
				else
				{//右
					m_pNoEntry[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT);
				}
			}

			//国名
			if (m_pCuntoryName[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//左
					m_pCuntoryName[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT);
				}
				else
				{//右
					m_pCuntoryName[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT);
				}
			}

			//選択された国を保存
			if (m_pNumPlayer[nCnt] != NULL) { m_anPlayeCuntry[nCnt] = m_pNumPlayer[nCnt]->GetNumCuntry(); }
		}

		//国旗左右移動
		for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
		{
			if (m_pNationalFlag[nCnt] != NULL)
			{
				if (nCnt == 0 || nCnt == 1 || nCnt == 4 || nCnt == 5)
				{//左
					m_pNationalFlag[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT);
				}
				else
				{//右
					m_pNationalFlag[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT);
				}
			}
		}
	}
	else if(m_nCountTime ==  MOVE_TIME)
	{
		UninitCuntrySelect();				//国選択の破棄
	}

	if ((m_nCountTime > MOVE_TIME) && (m_nCountTime < 120))
	{
		InitCharacterSelect();				//キャラクターの選択処理

		//下移動
		if (m_pLogo != NULL) { m_pLogo->UpdateMoveDown(MOVE_DOWN); }

		//拡大処理
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			//キャラフレーム
			if (m_bPlayerEnter[nCnt] == true)
			{
				m_pCharBack[nCnt]->UpdateScaleUp(CHAR_BACK_SIZE, CHANGE_CHARBACK);
			}
			else
			{
				m_pCharBack[nCnt]->UpdateScaleUp(D3DXVECTOR3(145.0f, 150.0f, 0.0f), CHANGE_CHARBACK2);
			}

			//フレーム
			if (m_pFram[nCnt] != NULL) { m_pFram[nCnt]->UpdateScaleUp(NOENTRY_SIZE, CHANGE_FLAM); }

			//キャラ選択UI
			if (m_pSelectChar[nCnt] != NULL) { m_pSelectChar[nCnt]->UpdateScaleUp(D3DXVECTOR3(145.0f, 150.0f, 0.0f), CHANGE_CHARBACK2); }
		}

		if (m_pBackButton != NULL) { m_pBackButton->UpdateScaleUp(BACKBUTTON_SIZE, CHANGE_BACKBUTTON); }
	}
	else if (m_nCountTime > 120)
	{//キャラ選択モードにする
		SetState(STATE_CHARACTER_SELECT);
		m_nCountTime = 0;
	}
}

//=============================================================================
// 国の選択処理に戻る
//=============================================================================
void CSelect::UpdateBackChange(void)
{
	//時間の可算
	m_nCountTime++;

	if (m_nCountTime < 30)
	{
		//ロゴの上移動
		if (m_pLogo != NULL) { m_pLogo->UpdateMoveUp(4.0f); }

		//縮小処理
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			//キャラフレーム
			if (m_bPlayerEnter[nCnt] == true)
			{
				m_pCharBack[nCnt]->UpdateScaleDown(FIRST_SIZE, CHANGE_CHARBACK);
			}
			else
			{
				m_pCharBack[nCnt]->UpdateScaleDown(FIRST_SIZE, CHANGE_CHARBACK2);
			}

			//キャラ選択UI
			if (m_pSelectChar[nCnt] != NULL) { m_pSelectChar[nCnt]->UpdateScaleDown(FIRST_SIZE, CHANGE_CHARBACK2); }

			//フレーム
			if (m_pFram[nCnt] != NULL) { m_pFram[nCnt]->UpdateScaleDown(FIRST_SIZE, CHANGE_FLAM); }
		}

		//ボタンの縮小処理
		if (m_pBackButton != NULL) { m_pBackButton->UpdateScaleDown(D3DXVECTOR3(10.0f, 10.0f, 0.0f), CHANGE_BACKBUTTON); }
	}
	else if (m_nCountTime == 30)
	{
		UninitCharacterSelect();	//キャラクター選択画面の破棄
		BackCuntryInit();			//国選択のポリゴンを生成
	}
	else if (m_nCountTime > 30 && m_nCountTime < 70)
	{
		//下移動
		if (m_pLogo != NULL) { m_pLogo->UpdateMoveDown(MOVE_UP); }

		//左右移動
		for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
		{
			//フレーム
			if (m_pFram[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//左
					m_pFram[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT_REMAKE);
				}
				else
				{//右
					m_pFram[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT_REMAKE);
				}
			}

			//NO ENTRY
			if (m_pNoEntry[nCnt] != NULL)
			{
				if (nCnt < NUMPLAYER / 2)
				{//左
					m_pNoEntry[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT_REMAKE);
				}
				else
				{//右
					m_pNoEntry[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT_REMAKE);
				}
			}

			//プレイヤーのアイコンの移動
			if ((m_pNumPlayer[nCnt] != NULL) && (true == m_pNumPlayer[nCnt]->GetDraw()))
			{
				D3DXVECTOR3 Pos = m_pNumPlayer[nCnt]->GetPos();

				if (Pos.x < SCREEN_WIDTH / 2.0f)
				{
					m_pNumPlayer[nCnt]->UpdateMoveRight(17.0f);
				}
				else
				{
					m_pNumPlayer[nCnt]->UpdateMoveLeft(15.7f);
				}
			}
		}

		//国旗の左右移動
		for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
		{
			if (m_pNationalFlag[nCnt] != NULL)
			{
				if (nCnt == 0 || nCnt == 1 || nCnt == 4 || nCnt == 5)
				{//右
					m_pNationalFlag[nCnt]->UpdateMoveRight(MOVE_RIGHT_LEFT_REMAKE);
				}
				else
				{//左
					m_pNationalFlag[nCnt]->UpdateMoveLeft(MOVE_RIGHT_LEFT_REMAKE);
				}
			}
		}
	}
	else if (m_nCountTime == 70)
	{
		SetState(STATE_CUNTRY_SELECT);	//国の選択状態にする
		m_nCountTime = 0;				//時間の初期化
	}
}

//=============================================================================
// 国の選択画面の生成
//=============================================================================
void CSelect::BackCuntryInit(void)
{
	//ロゴの設定
	if (m_pLogo == NULL) { m_pLogo = CMoveUI::Create(D3DXVECTOR3(640.0f, -55.0f, 0.0f), D3DXVECTOR3(640.0f, 50.0f, 0.0f), m_pTex[0]); }

	//プレイヤーの選択表示生成
	for (int nCnt = NUMPLAYER - 1; nCnt >= 0; nCnt--)
	{
		m_pNumPlayer[nCnt] = CPlayerIcon::Create(D3DXVECTOR3(255.0f, 200.0f, 0.0f), nCnt);
		m_pNumPlayer[nCnt]->BindTexture(m_apTexPlayer[nCnt]);		//テクスチャの割当て
		m_pNumPlayer[nCnt]->SetNumCuntry(m_anPlayeCuntry[nCnt]);	//選択されている国を設定
		m_pNumPlayer[nCnt]->SetDraw(false);

		//描画状態にする
		if (m_bPlayerEnter[nCnt] == true)
		{
			m_pNumPlayer[nCnt]->SetNumInput(m_anControler[nCnt]);		//コントローラーの取得
			m_pNumPlayer[nCnt]->SetInputType(m_anInputType[nCnt]);		//コントローラーのタイプ取得
			m_pNumPlayer[nCnt]->SetDraw(true);
		}
	}

	//国旗の表示
	int nCountFlag = 0;;
	for (int nCntY = 0; nCntY < 2; nCntY++)
	{
		for (int nCntX = 0; nCntX < 4; nCntX++)
		{
			//位置設定
			D3DXVECTOR3 Pos = D3DXVECTOR3(-410.0f + (nCntX * MOVE_X), 200.0f + (nCntY * MOVE_Y), 0.0f);
			if (nCntX >= (NUMPLAYER / 2.0f)) { Pos.x += SCREEN_WIDTH; }

			m_pNationalFlag[nCountFlag] = CMoveUI::Create(Pos, FLAG_SIZE, m_apTextureCountry[nCountFlag]);

			//プレイヤーのアイコン位置を合わせる
			for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
			{
				if (m_anPlayeCuntry[nCnt] != nCountFlag) { continue; }
				if (m_pNumPlayer[nCnt] != NULL)
				{
					m_pNumPlayer[nCnt]->SetPos(Pos, m_pNumPlayer[nCnt]->GetOtherPos());
				}
			}

			nCountFlag++;
		}
	}

	//フレーム・エントリーしていないテクスチャの表示
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		D3DXVECTOR3 Pos = D3DXVECTOR3(NOENTRY_POS_REMAKE.x + (nCnt * NOENTRY_MOVE), NOENTRY_POS_REMAKE.y, 0.0f);

		if (nCnt >= (NUMPLAYER / 2.0f)) { Pos.x += SCREEN_WIDTH; }

		if (m_pFram[nCnt] == NULL)
		{
			m_pFram[nCnt] = CMoveUI::Create(Pos, NOENTRY_SIZE, m_pTex[1]);
		}

		if ((m_pNoEntry[nCnt] == NULL) && (m_bPlayerEnter[nCnt] == false))
		{//エントリー状態じゃなかったら
			m_pNoEntry[nCnt] = CMoveUI::Create(Pos, NOENTRY_SIZE, m_pTexNoEntry);
		}
	}
}

//=============================================================================
// 決定処理
//=============================================================================
void CSelect::UpdateDicision(void)
{
	if (m_pRedyFight == NULL)
	{ //レディートゥファイトを生成
		m_pRedyFight = CMoveUI::Create(D3DXVECTOR3(0.0f, 340.0f, 0.0f), D3DXVECTOR3(640.0f, 150.0f, 0.0f), m_pTex[5]);
	}
	else
	{
		m_pRedyFight->UpdateMoveRight(50.0f);
		D3DXVECTOR3 Pos = m_pRedyFight->GetPos();

		if (Pos.x > 680.0f)
		{
			//位置の再設定
			Pos.x = 640.0f;
			m_pRedyFight->SetPos(Pos, m_pRedyFight->GetOtherPos());

			//入力デバイスの取得
			CInputXPad * pXPad = CManager::GetXPad();
			CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
			CRawMouse * pRawMouse = CManager::GetRawMouse();

			for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
			{
				if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true)
				{
					SetPlayerInfo();	//プレイヤー情報の保存
					if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_GAME); }
				}

				if (pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
				{
					SetPlayerInfo();	//プレイヤー情報の保存
					if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_GAME); }
				}
			}

			if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
			{
				if (CFade::GetFadeMode() != CFade::FADE_OUT)
				{
					SetPlayerInfo();	//プレイヤー情報の保存
					CFade::SetFade(CManager::MODE_GAME);
				}
			}
		}
	}
}

//=============================================================================
// エントリー処理
//=============================================================================
void CSelect::PlayerEnter(int nCntPlayer, INPUTTYPE InputType)
{
	//デバイスがエントリーしているかどうか
	if (InputType == INPUTTYPE_GAMEPAD)
	{
		if (m_bEntryGamePad[nCntPlayer]) { return; }
		m_bEntryGamePad[nCntPlayer] = true;
	}
	else if (InputType == INPUTTYPE_MOUSE)
	{
		if (m_bEntryMouse[nCntPlayer]) { return; }
		m_bEntryMouse[nCntPlayer] = true;
	}
	else if (InputType == INPUTTYPE_KEYBOARD)
	{
		if (m_bEntryKeyboard) { return; }
		m_bEntryKeyboard = true;
	}

	//エントリー数の加算
	if (false == m_pNumPlayer[m_nEntryPlayer]->GetDraw())
	{
		//エントリーしている状態にする
		m_bPlayerEnter[m_nEntryPlayer] = true;

		//コントローラーの番号を保存
		m_anControler[m_nEntryPlayer] = nCntPlayer;

		//入力デバイスのタイプ設定
		m_anInputType[m_nEntryPlayer] = InputType;

		//プレイヤーアイコン・国名の表示
		m_pNumPlayer[m_nEntryPlayer]->SetDraw(true);
		m_pNumPlayer[m_nEntryPlayer]->SetNumInput(nCntPlayer);
		m_pNumPlayer[m_nEntryPlayer]->SetInputType(InputType);

		//総数の加算
		m_nEntryPlayer++;
	}
}

//=============================================================================
// プレイヤー情報の保存
//=============================================================================
void CSelect::SetPlayerInfo(void)
{
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pSelectChar[nCnt] != NULL)
		{
			m_anPlayeCuntry[nCnt] = m_pSelectChar[nCnt]->GetCuntry();
			m_anPlayerType[nCnt] = m_pSelectChar[nCnt]->GetType();
		}
	}
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void CSelect::LoadTexture(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//国旗のテクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, FLAG_JAPAN, &m_apTextureCountry[0]);			//日本
	D3DXCreateTextureFromFile(pDevice, FLAG_RUSSIA, &m_apTextureCountry[1]);		//ロシア
	D3DXCreateTextureFromFile(pDevice, FLAG_AMERICA, &m_apTextureCountry[2]);		//アメリカ
	D3DXCreateTextureFromFile(pDevice, FLAG_BRAZIL, &m_apTextureCountry[3]);		//ブラジル
	D3DXCreateTextureFromFile(pDevice, FLAG_ENGLAND, &m_apTextureCountry[4]);		//イギリス
	D3DXCreateTextureFromFile(pDevice, FLAG_ITALY, &m_apTextureCountry[5]);			//イタリア
	D3DXCreateTextureFromFile(pDevice, FLAG_NEWZEALAND, &m_apTextureCountry[6]);	//ニュージーランド
	D3DXCreateTextureFromFile(pDevice, FLAG_SOUTHAFRICA, &m_apTextureCountry[7]);	//南アフリカ

	//プレイヤーの選択画像
	D3DXCreateTextureFromFile(pDevice, SELECT_1P, &m_apTexPlayer[0]);	//1P
	D3DXCreateTextureFromFile(pDevice, SELECT_2P, &m_apTexPlayer[1]);	//2P
	D3DXCreateTextureFromFile(pDevice, SELECT_3P, &m_apTexPlayer[2]);	//3P
	D3DXCreateTextureFromFile(pDevice, SELECT_4P, &m_apTexPlayer[3]);	//4P

	//国名の読み込み処理
	D3DXCreateTextureFromFile(pDevice, NAME_JAPAN, &m_apTexCuntoryName[0]);			//日本
	D3DXCreateTextureFromFile(pDevice, NAME_RUSSIA, &m_apTexCuntoryName[1]);		//ロシア
	D3DXCreateTextureFromFile(pDevice, NAME_AMERICA, &m_apTexCuntoryName[2]);		//アメリカ
	D3DXCreateTextureFromFile(pDevice, NAME_BRAZIL, &m_apTexCuntoryName[3]);		//ブラジル
	D3DXCreateTextureFromFile(pDevice, NAME_ENGLAND, &m_apTexCuntoryName[4]);		//イギリス
	D3DXCreateTextureFromFile(pDevice, NAME_ITALY, &m_apTexCuntoryName[5]);			//イタリア
	D3DXCreateTextureFromFile(pDevice, NAME_NEWZEALAND, &m_apTexCuntoryName[6]);	//ニュージーランド
	D3DXCreateTextureFromFile(pDevice, NAME_SOUTHAFRICA, &m_apTexCuntoryName[7]);	//南アフリカ

	//エントリーしていないテクスチャの読み込み処理
	D3DXCreateTextureFromFile(pDevice, NO_ENTRY, &m_pTexNoEntry);			//日本

	//その他のテクスチャ
	D3DXCreateTextureFromFile(pDevice, LOGO, &m_pTex[0]);			//ロゴ
	D3DXCreateTextureFromFile(pDevice, FLAM, &m_pTex[1]);			//フレーム
	D3DXCreateTextureFromFile(pDevice, LOGO2, &m_pTex[2]);			//プレイヤータイプ選択ロゴ
	D3DXCreateTextureFromFile(pDevice, NO_ENTRY_CHR, &m_pTex[3]);	//キャラクターのNOENTRY
	D3DXCreateTextureFromFile(pDevice, BACK_CUNTRY, &m_pTex[4]);	//キャラクターのNOENTRY
	D3DXCreateTextureFromFile(pDevice, RADY_TO_FIGHT, &m_pTex[5]);	//レディートゥファイト
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CSelect::UnloadTexture(void)
{
	//国旗テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		if (m_apTextureCountry[nCnt] != NULL)
		{
			m_apTextureCountry[nCnt]->Release();
			m_apTextureCountry[nCnt] = NULL;
		}
	}

	//プレイヤーのアイコン
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_apTexPlayer[nCnt] != NULL)
		{
			m_apTexPlayer[nCnt]->Release();
			m_apTexPlayer[nCnt] = NULL;
		}
	}

	//国名テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		if (m_apTexCuntoryName[nCnt] != NULL)
		{
			m_apTexCuntoryName[nCnt]->Release();
			m_apTexCuntoryName[nCnt] = NULL;
		}
	}

	//エントリーしてしていない
	if (m_pTexNoEntry != NULL)
	{
		m_pTexNoEntry->Release();
		m_pTexNoEntry = NULL;
	}

	//ロゴテクスチャ
	for (int nCnt = 0; nCnt < SELECT_TEX; nCnt++)
	{
		if (m_pTex[nCnt] != NULL)
		{
			m_pTex[nCnt]->Release();
			m_pTex[nCnt] = NULL;
		}
	}
}