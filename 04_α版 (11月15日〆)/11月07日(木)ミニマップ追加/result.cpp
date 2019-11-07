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

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_OUT_TIME			(60  * 5)		// フェードするまでの時間
#define UI_FILENAME				("data\\TEXT\\UI\\result.txt")

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

	//UIの読み込み
	LoadUI(UI_FILENAME);

	//初期化
	m_nCounter = 0;
	m_nTime = 0;
}
//=============================================================================
// 終了処理
//=============================================================================
void CResult::Uninit(void)
{
	//ベースの破棄
	CModeBase::Uninit();

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
	
	m_nCounter++;
	if (m_nCounter % 60 == 0)
	{
		m_nTime++;
		if (m_nTime <= 1)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM003);
		}
		if(m_nTime == 5)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_BGM002);
		}
	}
	
	//CSound::PlaySound(CSound::SOUND_LABEL_SE015);
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{
			//決定音
			CSound::PlaySound(CSound::SOUND_LABEL_SE007);
			CFade::SetFade(CManager::MODE_TITLE);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CResult::Draw(void)
{
	CScene::DrawAll();
}