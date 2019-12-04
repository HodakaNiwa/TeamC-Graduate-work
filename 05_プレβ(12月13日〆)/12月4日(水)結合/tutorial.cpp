//=============================================================================
//
// チュートリアルの処理 [tutorial.cpp]
// Author : 佐藤 安純
//
//=============================================================================
#include "tutorial.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "camera.h"
#include "fieldmanager.h"
#include "loadText.h"
#include "territory.h"
#include "movie.h"
#include "RawMouse.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define LOAD_OBJ			("data/TEXT/Stage.txt")
#define LOAD_TERRITORY		("data/TEXT/Territory.txt")
#define MOVIE_NAME			(L"data/MOVIE/Tutorial.avi")

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTutorial::CTutorial(){}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CTutorial::Init(void)
{
	//ベースの初期化
	CModeBase::Init();

	//変数の初期化
	m_pMovie = NULL;

	//動画の生成
	if (m_pMovie == NULL)
	{
		m_pMovie = CMovie::Create(MOVIE_NAME);
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	//ベースの破棄
	CModeBase::Uninit();

	if (m_pMovie != NULL)
	{
		m_pMovie->Uninit();
		delete m_pMovie;
		m_pMovie = NULL;
	}

	//フェード以外削除
	CScene::ReleaseFade();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
	//インプットの取得　
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true ||
			pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
		{
			if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_TITLE); }
		}
	}

	//動画の更新処理
	if (m_pMovie != NULL)
	{
		m_pMovie->Update();
		bool bEndMovie = m_pMovie->GetEndMovie();

		if (bEndMovie)
		{
			//動画の破棄
			m_pMovie->Uninit();
			delete m_pMovie;
			m_pMovie = NULL;

			//タイトルへ
			if (CFade::GetFadeMode() != CFade::FADE_OUT) { CFade::SetFade(CManager::MODE_TITLE); }
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{
	CScene::DrawAll();
}