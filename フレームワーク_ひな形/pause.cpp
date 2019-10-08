//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "pause.h"
#include "scene.h"
#include "scene2D.h"
#include "input.h"
#include "gamepad.h"
#include "manager.h"
#include "game.h"
#include "Fade.h"
#include "sound.h"

//=============================================================================
// 生成処理
//=============================================================================
CPause * CPause::Create(void)
{
	//インスタンスの生成
	CPause * pPause = new CPause;

	//初期化処理
	pPause->Init();
	return pPause;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause()
{
	//ポインタの初期化
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		m_pScene2D[nCntPolygon] = NULL;
	}

	m_state = STATE_CONTINUE;				//選択されているボタン設定
	m_StateBottun = STATEBOTTUN_NORMAL;		//ボタンの選択状態の設定
	m_bDeth = false;						//死亡状態かどうか
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{
	//ポリゴンの生成
	if (m_pScene2D[0] == NULL)
	{
		/*m_pScene2D[0] = CScene2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f),
			D3DXVECTOR3(640.0f, 370.0f, 0.0f),
			CScene2D::TEX_MAX,
			CScene2D::TYPE_UI);*/

		if (m_pScene2D[0] != NULL)
		{
			m_pScene2D[0]->SetColoer(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (m_pScene2D[1] == NULL)
	{
		/*m_pScene2D[1] = CScene2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f),
										  D3DXVECTOR3(275.0f, 350.0f, 0.0f),
										  CScene2D::TEX_PAUSEBACE,
										  CScene2D::TYPE_UI);*/
	}

	if (m_pScene2D[2] == NULL)
	{
		/*m_pScene2D[2] = CScene2D::Create(D3DXVECTOR3(640.0f, 250.0f, 0.0f),
										 D3DXVECTOR3(150.0f, 75.0f, 0.0f),
										 CScene2D::TEX_PAUSE,
										 CScene2D::TYPE_UI);*/

		if (m_pScene2D[2] != NULL)
		{
			m_pScene2D[2]->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
		}
	}

	if (m_pScene2D[3] == NULL)
	{
		/*m_pScene2D[3] = CScene2D::Create(D3DXVECTOR3(640.0f, 380.0f, 0.0f),
			D3DXVECTOR3(150.0f, 75.0f, 0.0f),
			CScene2D::TEX_PAUSE,
			CScene2D::TYPE_UI);*/

		if (m_pScene2D[3] != NULL)
		{
			m_pScene2D[3]->SetTexUV(0.0f, 1.0f, 0.25f, 0.5f);
			m_pScene2D[3]->SetColoer(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		}
	}

	if (m_pScene2D[4] == NULL)
	{
		/*m_pScene2D[4] = CScene2D::Create(D3DXVECTOR3(640.0f, 510.0f, 0.0f),
			D3DXVECTOR3(150.0f, 75.0f, 0.0f),
			CScene2D::TEX_PAUSE,
			CScene2D::TYPE_UI);*/

		if (m_pScene2D[4] != NULL)
		{
			m_pScene2D[4]->SetTexUV(0.0f, 1.0f, 0.5f, 0.75f);
			m_pScene2D[4]->SetColoer(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	//2Dポリゴンのメモリの破棄
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (m_pScene2D[nCntPolygon] != NULL)
		{
			m_pScene2D[nCntPolygon]->Uninit();
			m_pScene2D[nCntPolygon] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();		//キーボード情報の取得
	CGamePad * pGamePad = CManager::GetInputGamePad();					//ゲームパッドの取得

	//ボタンの選択
	if (pInputKeyboard->GetKeyboardTrigger(DIK_UP) == true || pGamePad->GetGamePadLeftStickTrigger(CGamePad::ANALOG_STICK_UP) == true)
	{
		m_state = (STATE)((m_state + (STATE_MAX - 1)) % STATE_MAX);
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_DOWN) == true || pGamePad->GetGamePadLeftStickTrigger(CGamePad::ANALOG_STICK_DOWN) == true)
	{
		m_state = (STATE)((m_state + 1) % STATE_MAX);
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pGamePad->GetGamePadTrigger(CGamePad::ELECOM_A) == true)
	{
		m_StateBottun = STATEBOTTUN_SELECT;
	}

	//2Dポリゴンの更新
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (m_pScene2D[nCntPolygon] != NULL)
		{
			//色の設定
			if (m_state == (STATE)(nCntPolygon - 2))
			{
				if (m_StateBottun == STATEBOTTUN_NORMAL)
				{
					m_pScene2D[nCntPolygon]->SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{
					m_pScene2D[nCntPolygon]->SetColoer(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));	//色の設定
					BottunUpdate();
				}
			}
			else if(nCntPolygon != 0 && nCntPolygon != 1)
			{
				m_pScene2D[nCntPolygon]->SetColoer(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f));
			}

			//更新処理
			m_pScene2D[nCntPolygon]->Update();
		}
	}

	//死亡状態だったら
	if (m_bDeth == true)
	{
		Uninit();		//終了処理
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{
	//2Dポリゴンの描画
	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (m_pScene2D[nCntPolygon] != NULL)
		{
			m_pScene2D[nCntPolygon]->Draw();
		}
	}
}

//=============================================================================
// ボタンの更新処理
//=============================================================================
void CPause::BottunUpdate(void)
{
	//ゲームポインタの取得
	CGame * pGame = CManager::GetGame();

	//フェード状態の取得
	CFade::FADE FadeMode = CFade::GetFadeMode();

	pGame->SetPause(false);	//ポーズ状態をもとに戻す
	m_bDeth = true;			//死亡状態にする

	switch (m_state)
	{
	case STATE_CONTINUE:
		break;

	case STATE_RETRY:
		if (FadeMode != CFade::FADE_OUT)
		{
			//ボタンを押した状態にする
			CFade::SetFade(CManager::MODE_GAME);	//チュートリアル画面に移動する
		}
		break;

	case STATE_QUIT:
		if (FadeMode != CFade::FADE_OUT)
		{
			//ボタンを押した状態にする
			CFade::SetFade(CManager::MODE_TITLE);	//チュートリアル画面に移動する
		}
		break;
	}
}