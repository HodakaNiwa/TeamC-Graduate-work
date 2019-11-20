//=============================================================================
//
// キャラ選択の処理 [charselect.cpp]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#include "charSelect.h"
#include "moveUI.h"
#include "charMultRendering.h"
#include "Player.h"
#include "manager.h"
#include "input.h"
#include "gamepad.h"
#include "RawMouse.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEX_TYPENAME	("data/TEXTURE/Select/TypeName.png")
#define TEX_ENTER		("data/TEXTURE/Select/CharEnter.png")
#define MOUSE_MOVE		(15.0f)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CSelectCharacter::m_pTexture[SELECTCHAR_TEX] = {};

//=============================================================================
// 生成処理
//=============================================================================
CSelectCharacter * CSelectCharacter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCuntry, int nNumPlayer, int nNumControler)
{
	CSelectCharacter *pSelectChar = new CSelectCharacter;
	pSelectChar->Init(pos, size, nCuntry, nNumPlayer, nNumControler);

	return pSelectChar;
}

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CSelectCharacter::CSelectCharacter() {}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CSelectCharacter::~CSelectCharacter() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSelectCharacter::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, int nCuntry, int nNumPlayer, int nNumControler)
{
	//変数の初期化
	m_pMoveUI = NULL;
	m_pTypeName = NULL;
	m_pStatus = NULL;
	m_pStatusTex = NULL;				//ステータスのテクスチャ
	m_pEnter = NULL;					//エンターキーを押したかどうか
	m_nCuntry = nCuntry;				//国番号
	m_nType = 0;						//タイプ番号
	m_nNumPlayer = nNumPlayer;			//プレイヤー番号
	m_nNumControler = nNumControler;	//コントローラー番号
	m_state = STATE_SPEED;				//現在の状態
	m_nCountTime = 0;					//時間の加算
	m_nOldType = 0;						//前回の時間
	m_bEnter = false;					//キャラを決定したかどうか

	//マルチレンダリング用テクスチャの生成
	LoadMultRendering();

	//キャラ表示
	if (m_pMoveUI == NULL) { m_pMoveUI = CMoveUI::Create(pos, size, m_pCharMultRender[m_nType]->GetTexture()); }

	//タイプ名の表示
	if (m_pTypeName == NULL)
	{
		m_pTypeName = CMoveUI::Create(D3DXVECTOR3(170.0f + (nNumPlayer * 315.0f), 530.0f, 0.0f), D3DXVECTOR3(10.0f, 1.0f, 0.0f), m_pTexture[0]);
		m_pTypeName->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
	}

	//ステータス表の生成
	if (m_pStatus == NULL)
	{
		m_pStatus = CMoveUI::Create(D3DXVECTOR3(170.0f + (nNumPlayer * 315.0f), 628.0f, 0.0f), D3DXVECTOR3(90.0f, 13.0f, 0.0f), m_pStatusTex);
		m_pStatus->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
	}
	
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CSelectCharacter::Uninit(void)
{
	//メモリの初期化
	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{
		m_pCharMultRender[nCnt] = NULL;
	}

	m_pMoveUI = NULL;
	m_pTypeName = NULL;
	m_pStatus = NULL;
	m_pEnter = NULL;

	//ステータステクスチャの破棄
	if (m_pStatusTex != NULL)
	{
		m_pStatusTex->Release();
		m_pStatusTex = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void  CSelectCharacter::Update(void)
{
	//前回のタイプを保存
	m_nOldType = m_nType;

	//入力処理
	UpdateInput();

	//エンターキーの生成 / 破棄
	if (m_bEnter)
	{// 生成
		D3DXVECTOR3 Size = m_pMoveUI->GetSize();
		Size.y += 5.0f;
		if (m_pEnter == NULL) { m_pEnter = CMoveUI::Create(m_pMoveUI->GetPos(), Size, m_pTexture[1]); }
	}
	else
	{//	破棄
		if (m_pEnter != NULL)
		{
			m_pEnter->Uninit();
			m_pEnter = NULL;
		}
	}

	switch (m_state)
	{
	case STATE_SPEED:
		m_nType = 0;

		if ((m_pMoveUI != NULL) && (m_pTypeName != NULL) && (m_pStatus != NULL))
		{//テクスチャとUVの設定
			m_pMoveUI->BindTexture(m_pCharMultRender[m_nType]->GetTexture()); 
			m_pTypeName->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
			m_pStatus->SetTexUV(0.0f, 1.0f, 0.0f, 0.25f);
		}
		break;

	case STATE_PAWER:
		m_nType = 1;

		if ((m_pMoveUI != NULL) && (m_pTypeName != NULL) && (m_pStatus != NULL))
		{//テクスチャとUVの設定
			m_pMoveUI->BindTexture(m_pCharMultRender[m_nType]->GetTexture());
			m_pTypeName->SetTexUV(0.0f, 1.0f, 0.25f, 0.5f);
			m_pStatus->SetTexUV(0.0f, 1.0f, 0.25f, 0.5f);
		}
		break;

	case STATE_MINE:
		m_nType = 2;

		if ((m_pMoveUI != NULL) && (m_pTypeName != NULL) && (m_pStatus != NULL))
		{//テクスチャとUVの設定
			m_pMoveUI->BindTexture(m_pCharMultRender[m_nType]->GetTexture());
			m_pTypeName->SetTexUV(0.0f, 1.0f, 0.5f, 0.75f);
			m_pStatus->SetTexUV(0.0f, 1.0f, 0.5f, 0.75f);
		}
		break;

	case STATE_RANDUM:	//ランダムの時
		m_nCountTime++;

		if ((m_nCountTime % 5) == 0)
		{
			m_nType += 1;
			m_nType = m_nType % MAX_TYPE;
		}

		//テクスチャを割り当てる
		if ((m_pMoveUI != NULL) && (m_pTypeName != NULL) && (m_pStatus != NULL))
		{
			m_pMoveUI->BindTexture(m_pCharMultRender[m_nType]->GetTexture());
			m_pTypeName->SetTexUV(0.0f, 1.0f, 0.75f, 1.0f);
			m_pStatus->SetTexUV(0.0f, 1.0f, 0.75f, 1.0f);
		}
	}
}

//=============================================================================
// Xパッドの入力処理
//=============================================================================
void CSelectCharacter::UpdateInput(void)
{
	//入力デバイスの取得
	CInputXPad * pXPad = CManager::GetXPad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CRawMouse * pRawMouse = CManager::GetRawMouse();

	if (m_InputType == CSelect::INPUTTYPE_GAMEPAD)
	{
		if ((pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, m_nNumControler) == true) || (pKeyboard->GetKeyboardTrigger(DIK_RIGHT) == true))
		{//右移動
			MoveRight();
		}
		else if ((pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, m_nNumControler) == true) || (pKeyboard->GetKeyboardTrigger(DIK_LEFT) == true))
		{//左移動
			MoveLeft();
		}

		//決定処理
		if ((pXPad->GetTrigger(XINPUT_GAMEPAD_A, m_nNumControler) == true) || (pKeyboard->GetKeyboardTrigger(DIK_SPACE)) == true)
		{
			PressEnter();
		}
	}
	else
	{
		//マウスの角度を取得
		int MouseX = pRawMouse->GetMouse(m_nNumControler).lLastX;
		int MouseY = pRawMouse->GetMouse(m_nNumControler).lLastY;

		if ((MouseX > MOUSE_MOVE) || (pKeyboard->GetKeyboardTrigger(DIK_RIGHT) == true))
		{//右移動
			MoveRight();
		}
		else if ((MouseX < -MOUSE_MOVE) || (pKeyboard->GetKeyboardTrigger(DIK_LEFT) == true))
		{//左移動
			MoveLeft();
		}

		//決定処理
		if ((pRawMouse->GetTrigger(m_nNumControler, CRawMouse::RIMS_BUTTON_LEFT) == true) || (pKeyboard->GetKeyboardTrigger(DIK_SPACE)) == true)
		{
			PressEnter();
		}
	}
}

//=============================================================================
// 拡大処理
//=============================================================================
void CSelectCharacter::UpdateScaleUp(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move)
{
	if (m_pMoveUI != NULL) { m_pMoveUI->UpdateScaleUp(AimSize, Move); }
	if (m_pTypeName != NULL) { m_pTypeName->UpdateScaleUp(D3DXVECTOR3(70.0f, 30.0f, 0.0f), Move); }
	if(m_pStatus != NULL){ m_pStatus->UpdateScaleUp(D3DXVECTOR3(150.0f, 75.0f, 0.0f), Move); }
}

//=============================================================================
// 縮小処理
//=============================================================================
void CSelectCharacter::UpdateScaleDown(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move)
{
	if (m_pMoveUI != NULL) { m_pMoveUI->UpdateScaleDown(AimSize, Move); }
	if(m_pTypeName != NULL){ m_pTypeName->UpdateScaleDown(D3DXVECTOR3(10.0f, 1.0f, 0.0f), Move); }
	if (m_pStatus != NULL) { m_pStatus->UpdateScaleDown(D3DXVECTOR3(25.0f, 25.0f, 0.0f), Move); }
}

//=============================================================================
// 左移動
//=============================================================================
void CSelectCharacter::MoveLeft(void)
{
	int nState = m_state - 1;

	if (nState < 0) { nState = (int)STATE_RANDUM; }
	if (m_state == STATE_MAX) { nState = (int)STATE_MINE; }

	m_state = (STATE)nState;
	m_bEnter = false;
}

//=============================================================================
// 右移動
//=============================================================================
void CSelectCharacter::MoveRight(void)
{
	int nState = m_state + 1;

	if ((nState % (int)STATE_MAX) == 0) { nState = 0; }
	if (m_state == STATE_MAX) { nState = 0; }

	m_state = (STATE)nState;
	m_bEnter = false;
}

//=============================================================================
// 決定処理
//=============================================================================
void CSelectCharacter::PressEnter(void)
{
	if (m_state == STATE_RANDUM)
	{//ランダムを止める
		m_state = (STATE)m_nType;
	}
	else
	{
		m_bEnter = true;	//決定状態にする
	}
}

//=============================================================================
// UIの消去
//=============================================================================
void CSelectCharacter::UninitUI(void)
{
	//キャラクターのマルチレンダリングを消す
	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{
		if (m_pCharMultRender[nCnt] != NULL)
		{
			m_pCharMultRender[nCnt]->Uninit();
			m_pCharMultRender[nCnt] = NULL;
		}
	}

	//ポリゴンを消す
	if (m_pMoveUI != NULL)
	{
		m_pMoveUI->Uninit();
		m_pMoveUI = NULL;
	}

	//タイプの名前の破棄
	if (m_pTypeName != NULL)
	{
		m_pTypeName->Uninit();
		m_pTypeName = NULL;
	}

	//ステータスの破棄
	if (m_pStatus != NULL)
	{
		m_pStatus->Uninit();
		m_pStatus = NULL;
	}

	//決定テクスチャの破棄
	if (m_pEnter != NULL)
	{
		m_pEnter->Uninit();
		m_pEnter = NULL;
	}
}

//=============================================================================
// マルチレンダリング用テクスチャの読み込み
//=============================================================================
void CSelectCharacter::LoadMultRendering(void)
{
	//変数の初期化
	for (int nCnt = 0; nCnt < MAX_TYPE; nCnt++)
	{
		m_pCharMultRender[nCnt] = NULL;
	}

	switch (m_nCuntry)
	{
	case CPlayer::TYPE_JAPAN:	//日本
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_RUSSIA:	//ロシア
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_AMERICA:	//アメリカ
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_BRAZIL:	//ブラジル
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_ENGLAND:	//イギリス
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_ITALY:	//イタリア
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_NEWZEALAND:	//ニュージーランド
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;

	case CPlayer::TYPE_SOUTHAFRICA:	//南アフリカ
		m_pCharMultRender[0] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\SPEEDPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\SPEEDPLAYER_MOTION.txt");
		m_pCharMultRender[1] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\POWERPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\POWERPLAYER_MOTION.txt");
		m_pCharMultRender[2] = CCharMultRender::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\TEXT\\ModelLoad\\TEQPLAYER_LOAD.txt", "data\\TEXT\\MotionLoad\\TEQPLAYER_MOTION.txt");
		break;
	}
}


//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void CSelectCharacter::LoadTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//国旗のテクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_TYPENAME, &m_pTexture[0]);	//タイプ名
	D3DXCreateTextureFromFile(pDevice, TEX_ENTER, &m_pTexture[1]);	//キャラ決定
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CSelectCharacter::UnloadTex(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < SELECTCHAR_TEX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}