//=============================================================================
//
// ボタンの処理 [bottun.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "bottun.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "scene2D.h"
#include "RawMouse.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//ベクトルの初期化
#define CHANGE_COL		(0.02f)							//α値の変化量

//=============================================================================
// コンストラクタ
//=============================================================================
CBottun::CBottun(int nPriority, CScene::OBJTYPE type) : CScene2D(nPriority, type){}

//=============================================================================
// デストラクタ
//=============================================================================
CBottun::~CBottun(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBottun::Init(void)
{
	//変数の初期化
	m_State = STATE_NORMAL;
	m_fCol_a = 1.0f;
	m_fChangeCol = CHANGE_COL;

	CScene2D::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBottun::Uninit()
{
	CScene2D::Uninit();

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBottun::Update()
{
	//インプットの取得　
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CRawMouse *pRawMouse = CManager::GetRawMouse();

	//キーを押したら
	CInputXPad * pXPad = CManager::GetXPad();
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) || pXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) || 
			pRawMouse->GetTrigger(CRawMouse::RIMS_BUTTON_LEFT, nCnt))
		{
			m_State = STATE_ENTER;
		}
	}

	switch (m_State)
	{
	case STATE_NORMAL:

		//色の変化量の更新
		m_fCol_a += m_fChangeCol;

		//色の修正
		if (m_fCol_a < 0.0f)
		{
			m_fCol_a = 0.0f;
			m_fChangeCol *= -1;
		}
		else if (m_fCol_a > 1.0f)
		{
			m_fCol_a = 1.0f;
			m_fChangeCol *= -1;
		}

		//色を設定する
		CScene2D::SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fCol_a));
		break;

	case STATE_ENTER:
		CScene2D::SetColoer(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));		//赤色に設定する
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBottun::Draw()
{
	CScene2D::Draw();
}