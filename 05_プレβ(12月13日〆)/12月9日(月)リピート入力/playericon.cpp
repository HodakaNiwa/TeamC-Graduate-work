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
#include "fade.h"
#include "playericon.h"
#include "RawMouse.h"

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
#define MOUSE_MOVE		(15.0f)

CPlayerIcon::STATE CPlayerIcon::m_state = CPlayerIcon::STATE_NORMAL;

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
	m_nNumCuntry = -1;
	m_nOldNumCuntry = 0;
	m_pNationalFlag = NULL;
	m_state = STATE_NORMAL;
	m_nNumInput = -1;
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

	if (m_state == STATE_NORMAL)
	{
		UpdateInputXpad();		//アイコン選択処理
		UpdateInputKeyboard();
		UpdateInputMouse();
		UpdatePos();			//位置の更新処理
	}
	else
	{
		UpdateMove();	//移動処理
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayerIcon::Draw()
{
	CScene2D::Draw();
}

//=============================================================================
// 国の選択処理(XPad)
//=============================================================================
void CPlayerIcon::UpdateInputXpad(void)
{
	if (m_nNumInput == -1 || m_InputType != CSelect::INPUTTYPE_GAMEPAD) { return; }		//コントローラーが登録されていないなら更新しない

	//サウンドの取得
	CSound *pSound = CManager::GetSound();
	CGamePad * pGamePad = CManager::GetInputGamePad();				//ゲームパッドの取得
	CInputXPad * pXPad = CManager::GetXPad();

	if (pXPad->GetStick(0, m_nNumInput) == true)
	{
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}

	if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, m_nNumInput) == true
		|| pXPad->GetRepeat(CInputXPad::XPADOTHER_STICK_L_UP, m_nNumInput) == true)
	{//上移動
	 //セレクト音
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nNumInput) == true
		|| pXPad->GetRepeat(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nNumInput) == true)
	{//右移動
	 //セレクト音
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);

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
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, m_nNumInput) == true
		|| pXPad->GetRepeat(CInputXPad::XPADOTHER_STICK_L_DOWN, m_nNumInput) == true)
	{//下移動
	 //セレクト音
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}
	}
	else if (pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nNumInput) == true
		|| pXPad->GetRepeat(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nNumInput) == true)
	{//左移動
	 //セレクト音
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
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
	if (pXPad->GetTrigger(XINPUT_GAMEPAD_B, m_nNumInput) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{//フェードアウトの時じゃなかったら音を鳴らす
		 //セレクト音
			pSound->PlaySound(CSound::SOUND_LABEL_SE007);
		}

		//国旗の生成
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
			m_state = STATE_MOVE;	//移動状態にする
		}
	}
}

//=============================================================================
// 国の選択処理(キーボード)
//=============================================================================
void CPlayerIcon::UpdateInputKeyboard(void)
{
	int nNumControl = CManager::GetSelect()->GetNumControler();
	if (nNumControl != m_nNumPlayer) { return; }

	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();	//キーボードの取得

	if (pKeyboard->GetKeyboardTrigger(DIK_W) == true
		|| pKeyboard->GetKeyboardRepeat(DIK_W) == true)
	{//上移動
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}

		//国旗の破棄
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_D) == true
		|| pKeyboard->GetKeyboardRepeat(DIK_D) == true)
	{//右移動
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する
		m_nNumCuntry += 1;

		if (m_nNumCuntry <= WIDTH_DIVISION)
		{
			if ((m_nNumCuntry % WIDTH_DIVISION) == 0) { m_nNumCuntry = 0; }
		}
		else
		{
			if ((m_nNumCuntry % MAX_CUNTORY) == 0) { m_nNumCuntry = MAX_CUNTORY / 2; }
		}

		//国旗の破棄
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_S) == true
		|| pKeyboard->GetKeyboardRepeat(DIK_S) == true)
	{//下移動
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}

		//国旗の破棄
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pKeyboard->GetKeyboardTrigger(DIK_A) == true
		|| pKeyboard->GetKeyboardRepeat(DIK_A) == true)
	{//左移動
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する
		m_nNumCuntry -= 1;

		if (m_nNumCuntry < 0)
		{
			m_nNumCuntry = WIDTH_DIVISION - 1;
		}
		else if (m_nNumCuntry == WIDTH_DIVISION - 1)
		{
			m_nNumCuntry = MAX_CUNTORY - 1;
		}

		//国旗の破棄
		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}

	//決定ボタンを押したら
	if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
	{
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
			m_state = STATE_MOVE;	//移動状態にする
		}
	}
}

//=============================================================================
// 国の選択処理(Mouse)
//=============================================================================
void CPlayerIcon::UpdateInputMouse(void)
{
	if (m_nNumInput == -1 || m_InputType != CSelect::INPUTTYPE_MOUSE) { return; }	//コントローラーが登録されていないなら更新しない

	//サウンドの取得
	CSound *pSound = CManager::GetSound();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_SLIDE_UP) == true)
	{//上移動
	 //セレクト音
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_SLIDE_RIGHT) == true)
	{//右移動
	 //セレクト音
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する

		m_nNumCuntry += 1;

		if (m_nNumCuntry <= WIDTH_DIVISION)
		{
			if ((m_nNumCuntry % WIDTH_DIVISION) == 0) { m_nNumCuntry = 0; }
		}
		else
		{
			if ((m_nNumCuntry % MAX_CUNTORY) == 0) { m_nNumCuntry = MAX_CUNTORY / 2; }
		}

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_SLIDE_DOWN) == true)
	{//下移動
	 //セレクト音
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する

		if (m_nNumCuntry < WIDTH_DIVISION)
		{
			m_nNumCuntry += WIDTH_DIVISION;
		}
		else
		{
			m_nNumCuntry -= WIDTH_DIVISION;
		}

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}
	else if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_SLIDE_LEFT) == true)
	{//左移動
	 //セレクト音
		pSound->PlaySound(CSound::SOUND_LABEL_SE009);
		m_nOldNumCuntry = m_nNumCuntry;									//前回の情報を保存する
		m_nNumCuntry -= 1;

		if (m_nNumCuntry < 0)
		{
			m_nNumCuntry = WIDTH_DIVISION - 1;
		}
		else if (m_nNumCuntry == WIDTH_DIVISION - 1)
		{
			m_nNumCuntry = MAX_CUNTORY - 1;
		}

		if (m_pNationalFlag != NULL)
		{
			m_pNationalFlag->Uninit();
			m_pNationalFlag = NULL;
		}
	}

	//決定ボタンを押したら
	if (pRawMouse->GetTrigger(m_nNumInput, CRawMouse::RIMS_BUTTON_RIGHT) == true)
	{
		if (CFade::FADE_OUT != CFade::GetFadeMode())
		{//フェードアウトの時じゃなかったら音を鳴らす
		 //セレクト音
			pSound->PlaySound(CSound::SOUND_LABEL_SE007);
		}

		//国旗の生成
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
			m_state = STATE_MOVE;	//移動状態にする
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

//=============================================================================
// 移動処理
//=============================================================================
void CPlayerIcon::UpdateMove(void)
{
	//位置の取得
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//移動量の加算
	if (Pos.x < (SCREEN_WIDTH / 2.0f))
	{
		Pos.x -= MOVE_RIGHT_LEFT;
	}
	else
	{
		Pos.x += MOVE_RIGHT_LEFT;
	}

	//位置設定
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());

	//国旗の移動
	if (m_pNationalFlag == NULL) { return; }
	if (m_nNumPlayer < (NUMPLAYER / 2))
	{
		m_pNationalFlag->UpdateMoveLeft(MOVE_RIGHT_LEFT);
	}
	else
	{
		m_pNationalFlag->UpdateMoveRight(MOVE_RIGHT_LEFT);
	}
}

//=============================================================================
// 左に移動する
//=============================================================================
void CPlayerIcon::UpdateMoveLeft(float fMove)
{
	//位置の取得
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//移動処理
	Pos.x -= fMove;

	//位置設定
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}

//=============================================================================
// 右に移動する
//=============================================================================
void CPlayerIcon::UpdateMoveRight(float fMove)
{
	//位置の取得
	D3DXVECTOR3 Pos = CScene2D::GetPos();

	//移動処理
	Pos.x += fMove;

	//位置設定
	CScene2D::SetPos(Pos, CScene2D::GetOtherPos());
}