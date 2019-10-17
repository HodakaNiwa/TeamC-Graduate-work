
//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Kobayashi & Yamashita
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "title.h"
#include "player.h"
#include "tutorial.h"
#include "collision.h"
#include "model.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//静的メンバ変数宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CSceneX *CPlayer::m_apSceneX = NULL;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED (2.0f)
#define MODEL_RENG (50)
#define MAX_BUNDCNT (120)
#define MAX_BULLET (50)

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPlayer::CPlayer()
{
	m_Angle = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CPlayer::~CPlayer()
{

}



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, NUMPLAYER nNumPlayer, char ModelTxt[40], char MotionTxt[40])
{
	m_NumPlayer = nNumPlayer;
	CCharacter::Init(ModelTxt,MotionTxt);
	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);
	m_pTerritoryTop = NULL;		//リストの先頭ポインタ
	m_pTerritoryCur = NULL;		//リストの最後のポインタ

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CPlayer::Uninit(void)
{
	
	CCharacter::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CPlayer::Update(void)
{
	CCharacter::Update();

	//プレイヤーの移動処理
	PlayerMove();
}

//=============================================================================
// 描画処理
//=============================================================================
void  CPlayer::Draw(void)
{
	//ローテション
	//位置

	CCharacter::Draw();
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	CManager::GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, NUMPLAYER nNumPlayer, char ModelTxt[40], char MotionTxt[40], TYPE type)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//メモリを動的確保
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{//NULLチェック

			//初期化処理
			pPlayer->Init(pos,nNumPlayer,ModelTxt,MotionTxt);
			pPlayer->SetType(type);
		}
		else
		{
			MessageBox(0, "territoryがNULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "territoryがNULLじゃないです", "警告", MB_OK);
	}
	return pPlayer;
}

//=============================================================================
//
//=============================================================================
void  CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//　プレイヤーの移動処理
//=============================================================================
void  CPlayer::PlayerMove(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CGamePad * pGamePad = CManager::GetInputGamePad();				//ゲームパッドの取得
	CInputXPad * pXPad = CManager::GetXPad();
	float fRot = pXPad->GetStickRot(0, m_NumPlayer);


	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);
	D3DXVECTOR3 rot = pGameCamera->GetRot();
	

	D3DXVECTOR3 pos = CCharacter::GetPos();
	m_posOld = pos;

	//プレイヤーの移動処理（キーボード）
	if (pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//ポリゴンを左上に移動
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * SPEED;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * SPEED;
			m_Angle.y = rot.y - (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//ポリゴンを左下に移動
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * SPEED;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * SPEED;
			m_Angle.y = rot.y - (D3DX_PI * 0.75f);
		}
		else
		{//ポリゴンを左に移動
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * SPEED;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * SPEED;
			m_Angle.y = rot.y - (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//ポリゴンを右上に移動
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * SPEED;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * SPEED;
			m_Angle.y = rot.y + (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//ポリゴンを右下に移動
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * SPEED;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * SPEED;
			m_Angle.y = rot.y + (D3DX_PI * 0.75f);
		}
		else
		{//ポリゴンを右に移動
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * SPEED;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * SPEED;
			m_Angle.y = rot.y + (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
	{
		m_move.x -= sinf(rot.y) * SPEED;
		m_move.z -= cosf(rot.y) * SPEED;
		m_Angle.y = rot.y;
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
	{
		m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * SPEED;
		m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * SPEED;
		m_Angle.y = D3DX_PI + rot.y;
	}

		//コントローラー（XInput）
		if (pXPad->GetStick(0, m_NumPlayer) == true)
		{
			m_move.x -= sinf(fRot + rot.y) * SPEED;
			m_move.z -= cosf(fRot + rot.y) * SPEED;
			m_Angle.y = rot.y + fRot;
		}
	

		D3DXVECTOR3 DiffAngle;


		// プレイヤーの角度を修正
		DiffAngle.y = m_Angle.y - m_rot.y;   //差分を計算

		if (DiffAngle.y > D3DX_PI)
		{//D3DX_PIを超えた
			DiffAngle.y -= D3DX_PI * 2.0f;
		}
		if (DiffAngle.y < -D3DX_PI)
		{//-D3DX_PIを超えた
			DiffAngle.y += D3DX_PI * 2.0f;
		}

		//差分を倍率で直す
		m_rot.y += DiffAngle.y * 0.1f;

		if (m_rot.y > D3DX_PI)
		{//D3DX_PIを超えた
			m_rot.y -= D3DX_PI * 2.0f;
		}
		if (m_rot.y < -D3DX_PI)
		{//-D3DX_PIを超えた
			m_rot.y += D3DX_PI * 2.0f;
		}

		pos += m_move;

		//慣性
		m_move.x += (0.0f - m_move.x) * 1.0f;
		m_move.z += (0.0f - m_move.z) * 1.0f;
	
		CCharacter::SetPos(pos);
		CCharacter::SetRot(m_rot);
}


//=============================================================================
//　プレイヤーとテリトリーの当たり判定
//=============================================================================
void CPlayer::Collision(void)
{
	//各種値の取得
	D3DXVECTOR3 pos = CCharacter::GetPos();
	D3DXVECTOR3 posOld = m_posOld;
	D3DXVECTOR3 Move = INITIALIZE_VECTOR3;
	D3DXVECTOR3 ColRange;
	//判定開始
	CCollider *pCol = NULL;
	CCollider *pColNext = NULL;

	for (int nCntType = 0; nCntType < CCollider::TYPE_MAX; nCntType++)
	{
		pCol = CCollider::GetTop(nCntType);

		while (pCol != NULL)
		{
			pColNext = pCol->GetNext();
			CollisionCollider(pCol, pos, posOld, m_move, D3DXVECTOR3(15.0f,15.0f,15.0f));
			pCol = pColNext;
		}
	}
	CCharacter::SetPos(pos);
}

//=============================================================================
//　コライダーとの当たり判定処理
//=============================================================================
bool CPlayer::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	if (pCollider->GetType() == CCollider::TYPE_BOX)
	{
		if (CollisionBoxCollider((CBoxCollider*)pCollider, pos, posOld, Move, ColRange) == true)
		{
			int nData = 0;
		}
	}
	return false;
}

//=============================================================================
//　ボックスコライダーとの当たり判定処理
//=============================================================================
bool CPlayer::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;

	if (pBoxCollider->Collision(&pos, &posOld, &Move, ColRange, this, &bLand) == true)
	{
		if (bLand == true)
		{
			
		}
		return true;
	}
	return false;
}