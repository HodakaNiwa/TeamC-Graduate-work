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
#define FLAG_SIZE			(D3DXVECTOR3(100.0f, 70.0f, 0.0f))
#define PLAYER_SIZE			(D3DXVECTOR3(130.0f, 75.0f, 0.0f))
#define NAME_SIZE			(D3DXVECTOR3(80.0f, 20.0f, 0.0f))
#define NAME_POS			(D3DXVECTOR3(150.0f, 527.0f, 0.0f))
#define NAME_MOVE			(320.0f)
#define MOVE_X				(280.0f)
#define MOVE_Y				(200.0f)
#define NOENTRY_SIZE		(D3DXVECTOR3(150.0f, 100.0f, 0.0f))
#define NOENTRY_POS			(D3DXVECTOR3(170.0f, 605.0f, 0.0f))
#define NOENTRY_MOVE		(315.0f)


//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CMoveUI * CSelect::m_pNationalFlag[MAX_COUNTRY] = {};
int CSelect::m_nEntryPlayer = 0;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSelect::CSelect() {}

//=============================================================================
// デストラクタ
//=============================================================================
CSelect::~CSelect() {}

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
	m_nEntryPlayer = 0;
	m_state = STATE_CUNTRY_SELECT;

	for (int nCnt = 0; nCnt < MAX_COUNTRY; nCnt++)
	{
		m_apTextureCountry[nCnt] = NULL;	//国旗テクスチャ
		m_apTexCuntoryName[nCnt] = NULL;	//国名テクスチャ
		m_pNationalFlag[nCnt] = NULL;		//国旗ポインタ
	}

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		m_apTexPlayer[nCnt] = NULL;	//プレイヤーアイコンテクスチャ
		m_pNumPlayer[nCnt] = NULL;	//プレイヤーアイコンポインタ
		m_pNoEntry[nCnt] = NULL;	//エントリーしているかどうか
		m_pFram[nCnt] = NULL;		//フレーム
	}

	for (int nCnt = 0; nCnt < SELECT_TEX; nCnt++)
	{
		m_pTex[nCnt] = NULL;
	}

	//テクスチャの読み込み処理
	LoadTexture();

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

		m_pCamera = new CCamera;
		m_pCamera->Init();
		m_pCamera->SetViewport(viewport);
		m_pCamera->SetPosV(D3DXVECTOR3(14.0f, 1000.0f, -600.0f));
		m_pCamera->SetPosR(D3DXVECTOR3(900.0f, 255.0f, 600.0f));
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

	//テクスチャの破棄
	UnloadTexture();

	//フェード以外削除
	CScene::ReleaseFade();
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
	CInputXPad * pXPad = CManager::GetXPad();

	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{//セレクトからゲームへ
		
		if (CFade::GetFadeMode() != CFade::FADE_OUT) 
		{ 
			//決定音
			pSound->PlaySound(CSound::SOUND_LABEL_SE007);
			CFade::SetFade(CManager::MODE_GAME); 
		}
	}

	//国名のテクスチャの切り替え
	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (m_pCuntoryName[nCnt] != NULL)
		{
			m_pCuntoryName[nCnt]->BindTexture(m_apTexCuntoryName[m_pNumPlayer[nCnt]->GetNumCuntry()]);
		}
	}

	//状態ごとの更新処理
	switch (m_state)
	{
	case STATE_CUNTRY_SELECT: UpdateCuntrySelect(); break;			//国選択
	case STATE_CHARACTER_SELECT: UpdateCharacterSelect(); break;	//キャラ選択
	case STATE_CHANGE: UpdateChange(); break;						//モード切替
	case STATE_DICISION: UpdateDicision(); break;					//決定
	}

	//pSound->SetVolume(CSound::SOUND_LABEL_SE007, 10.0f);	//ボリュームの設定
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
	//サウンドの取得
	CSound *pSound = CManager::GetSound();

	CGamePad * pGamePad = CManager::GetInputGamePad();	//ゲームパッドの取得
	CInputXPad * pXPad = CManager::GetXPad();

	for (int nCnt = 0; nCnt < NUMPLAYER; nCnt++)
	{
		if (pXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) == true)
		{
			if (CFade::FADE_OUT != CFade::GetFadeMode())
			{//フェードアウトの時じゃなかったら音を鳴らす
				//決定音
				pSound->PlaySound(CSound::SOUND_LABEL_SE007);
				//pSound->SetVolume(CSound::SOUND_LABEL_SE007, 10.0f);	//ボリュームの設定
			}
			m_pNumPlayer[nCnt]->SetDraw(true);

			//国名の生成
			if (m_pCuntoryName[nCnt] == NULL)
			{
				m_pCuntoryName[nCnt] = CMoveUI::Create(D3DXVECTOR3(NAME_POS.x + (nCnt * NAME_MOVE), NAME_POS.y, 0.0f), NAME_SIZE, m_apTexCuntoryName[m_pNumPlayer[nCnt]->GetNumCuntry()]);

				//エントリー数の加算
				m_nEntryPlayer++;

				//NO ENTRYの破棄
				if (m_pNoEntry[nCnt] != NULL)
				{
					m_pNoEntry[nCnt]->Uninit();
					m_pNoEntry[nCnt] = NULL;
				}
			}
		}
	}
}

//=============================================================================
// キャラ選択の初期化処理
//=============================================================================
void CSelect::InitCharacterSelect(void)
{

}

//=============================================================================
// キャラ選択の更新処理
//=============================================================================
void CSelect::UpdateCharacterSelect(void)
{

}

//=============================================================================
// キャラ選択の破棄処理
//=============================================================================
void CSelect::UninitCharacterSelect(void)
{

}

//=============================================================================
// モード切り替え処理
//=============================================================================
void CSelect::UpdateChange(void)
{

}

//=============================================================================
// 決定処理
//=============================================================================
void CSelect::UpdateDicision(void)
{

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
	D3DXCreateTextureFromFile(pDevice, LOGO, &m_pTex[0]);	//ロゴ
	D3DXCreateTextureFromFile(pDevice, FLAM, &m_pTex[1]);	//フレーム
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