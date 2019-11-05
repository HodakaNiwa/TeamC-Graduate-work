//=============================================================================
//
// プレイヤーのアイコンの処理 [playericon.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "playericon.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "scene2D.h"
#include "select.h"
#include "moveUI.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//ベクトルの初期化
#define SIZE			(D3DXVECTOR3(130.0f, 75.0f, 0.0f))
#define WIDTH_DIVISION	(4)		//横の分割数
#define HIGHT_DIVISION	(2)		//縦の分割数
#define MAX_CUNTORY		(8)		//国の最大数
#define ADDPOS_X		(25.0f)	//Xの追加量
#define CUNTRY_SIZE		(D3DXVECTOR3(115.0f,  70.0f, 0.0f))	//国旗のサイズ
#define CUNTRY_POS		(D3DXVECTOR3(170.0f, 630.0f, 0.0f))
#define ADD_X			(310.0f)

//=============================================================================
// 生成処理
//=============================================================================
CPlayerIcon * CPlayerIcon::Create(D3DXVECTOR3 Pos, int nNumPlayer)
{
	CPlayerIcon * pPlayerIcon = new CPlayerIcon;
	pPlayerIcon->Init();
	pPlayerIcon->Set(Pos, SIZE);
	pPlayerIcon->SetNumPlayer(nNumPlayer);
	
	return pPlayerIcon;
}

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CPlayerIcon::CPlayerIcon() : CScene2D(6, CScene::OBJTYPE_UI) {}

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayerIcon::CPlayerIcon(int nPriority, CScene::OBJTYPE type) : CScene2D(nPriority, type){}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayerIcon::~CPlayerIcon(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayerIcon::Init(void)
{
	CScene2D::Init();
	m_nNumCuntry = 0;
	m_nOldNumCuntry = 0;
	m_pNationalFlag = NULL;
	m_nCountInput = 0;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayerIcon::Uninit()
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayerIcon::Update()
{
	if (GetDraw() == false) { return; }

	UpdateInputSelect();	//アイコン選択処理
	UpdatePos();			//位置の更新処理
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayerIcon::Draw()
{
	CScene2D::Draw();
}

//=============================================================================
// 国の選択処理
//=============================================================================
void CPlayerIcon::UpdateInputSelect(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CGamePad * pGamePad = CManager::GetInputGamePad();				//ゲームパッドの取得
	CInputXPad * pXPad = CManager::GetXPad();
	m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する

	if (pXPad->GetStick(0, m_nNumPlayer) == true)
	{
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}

	if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, m_nNumPlayer) == true)
	{//上移動
		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nNumPlayer) == true)
	{//右移動
		m_nNumCuntry += 1;

		if (m_nNumCuntry <= WIDTH_DIVISION)
		{
			if ((m_nNumCuntry % WIDTH_DIVISION) == 0) { m_nNumCuntry = 0; }
		}
		else
		{
			if ((m_nNumCuntry % MAX_CUNTORY) == 0) { m_nNumCuntry = MAX_CUNTORY / 2; }
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, m_nNumPlayer) == true)
	{//下移動
		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nNumPlayer) == true)
	{//左移動
		m_nNumCuntry -= 1;

		if (m_nNumCuntry < 0)
		{
			m_nNumCuntry = WIDTH_DIVISION - 1;
		}
		else if (m_nNumCuntry == WIDTH_DIVISION - 1)
		{
			m_nNumCuntry = MAX_CUNTORY - 1;
		}
	}

	//決定ボタンを押したら
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_A, m_nNumPlayer) == true)
	{
		//国旗の生成
		m_nCountInput++;
		if (m_nCountInput < 2) { return; }

		if (m_pNationalFlag == NULL)
		{
			//テクスチャの割当て
			LPDIRECT3DTEXTURE9 pTexture = CManager::GetSelect()->GetCuntryTex(m_nNumCuntry);
			
			//国旗の生成
			m_pNationalFlag = CMoveUI::Create(D3DXVECTOR3(CUNTRY_POS.x + (ADD_X * m_nNumPlayer), CUNTRY_POS.y, 0.0f), CUNTRY_SIZE, pTexture);
		}
		else
		{
			CManager::GetSelect()->SetState(CSelect::STATE_CHANGE);
		}
	}
}

//=============================================================================
// 位置の設定
//=============================================================================
void CPlayerIcon::UpdatePos(void)
{
	if (m_nOldNumCuntry == m_nNumCuntry) { return; }

	//位置を取得する
	D3DXVECTOR3 CuntryPos = CSelect::GetNatonalFlag(m_nNumCuntry)->GetPos();
	CuntryPos.x += ADDPOS_X;

	D3DXVECTOR3 OtherPos = CScene2D::GetOtherPos();
	CScene2D::SetPos(CuntryPos, OtherPos);
}