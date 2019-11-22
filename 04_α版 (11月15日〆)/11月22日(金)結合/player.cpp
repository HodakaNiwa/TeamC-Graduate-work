
//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Kobayashi & Yamashita & Niwa
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
#include "tutorial.h"
#include "collision.h"
#include "model.h"
#include "Territory.h"
#include "library.h"
#include "line.h"
#include "loadEffect.h"
#include "inputmouce.h"
#include "RawMouse.h"
#include "UI.h"
#include "score.h"
#include "mine.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// コリジョン関係
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)

//色
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//赤
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//青
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//緑
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//白
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//黒
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//黄
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//オレンジ
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//紫

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//静的メンバ変数宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
{
	m_move = INITIALIZE_VECTOR3;
	m_Angle = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nDamageCount = 0;
	m_fBlowAngle = 0.0f;
	m_fBlowLength = 0.0f;
	m_rot = INITIALIZE_VECTOR3;
	m_posOld = INITIALIZE_VECTOR3;
	m_nDamageCounter = 0;
	m_PlayerState = PLAYERSTATE_NONE;
	m_bSuperArmor = false;
	m_bSprint = false;
	m_bSprintMotion = false;

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
HRESULT CPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType)
{
	//プレイヤーのナンバーを取得
	m_nNumPlayer = nNumPlayer;

	//変数の初期化
	m_bWalk = true;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
	CCharacter::SetPos(pos);

	//モデルの情報を取得
	m_pModel = CCharacter::GetModel();

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();

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
	//キャラクターの破棄
	CCharacter::Uninit();

	if (m_pLoadEffect != NULL)
	{//エフェクトの破棄
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void  CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();							//サウンドの取得

	//キャラクターの更新
	CCharacter::Update();

	//	ラインのカウントダウン処理
	CountDownLineTime();

	//	ライン引きの開始(カウントアップ)
	if (m_bBlockStartTerritory)
	{
		m_nCountTime++;
		if ((m_nCountTime % NOT_GETTIME) == 0) { m_bBlockStartTerritory = false; }
	}

	//図形が完成した後の処理
	UpdateShapeComplete();
	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		//プレイヤーの移動処理
		PlayerMove();
	}
	else if (nGameState == CGame::GAMESTATE_END)
	{//ゲーム終了時にモーションをニュートラル
		if (m_bCharaMotionState == false)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE018);
			m_pMotion->SetNumMotion(0);
			m_bCharaMotionState = true;
		}
	}
	
	//プレイヤーの状態
	switch (m_PlayerState)
	{
	case PLAYERSTATE_NONE:	//通常状態

		//歩ける用にする
		m_bWalk = true;
		m_bSprintMotion = true;

		break;

	case PLAYERSTATE_WALK:	//移動状態

		//m_bSprintMotion = true;

		break;

	case PLAYERSTATE_ACTION: //アクション状態
		//m_nDamageCounter = 0;

		//m_bWalk = true;

		break;

	case PLAYERSTATE_BLOWAWAY:	//吹っ飛ばされてる状態
		// ダメージカウント加算
		m_nDamageCount++;

		if (m_nDamageCount >= MAX_DAMAGECOUNT)
		{
			m_nDamageCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;
			//if (m_bSprintMotion == true)
			{
				m_PlayerState = PLAYERSTATE_NONE;
				m_pMotion->SetNumMotion(m_PlayerState);
			}
		
		}
		
		// 吹っ飛ばす
		m_fBlowLength -= BLOW_MOVING_CUT;
		if (m_fBlowLength >= 0.0f)
		{
			m_move.x -= sinf(m_fBlowAngle) * m_fBlowLength;
			m_move.z -= cosf(m_fBlowAngle) * m_fBlowLength;
		}
		
		break;

	case PLAYERSTATE_DAMAGE:
		// ダメージカウント加算
		m_nDamageCount++;
		if (m_nDamageCount == 60)
		{//60秒たったら起き上がる
			m_pMotion->SetNumMotion(5);
		}
		if (m_nDamageCount == 110)
		{//110秒で動けるようになる
			m_nDamageCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;
			m_nDamageCounter = 0;
			m_bSuperArmor = false;
			m_PlayerState = PLAYERSTATE_NONE;
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CPlayer::Draw(void)
{
	//キャラクターの描画
	CCharacter::Draw();

	if (m_pLoadEffect != NULL)
	{//エフェクトの描画
		m_pLoadEffect->Draw();
	}
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, TYPE type, CHARCTERTYPE charctertype)
{
	CPlayer *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//メモリを動的確保
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{//NULLチェック
			pPlayer->m_CharcterType = charctertype;
			pPlayer->Init(pos, ModelTxt,MotionTxt, nNumPlayer,type);
		}
		else
		{
			MessageBox(0, "プレイヤーがNULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "プレイヤーがNULLじゃないです", "警告", MB_OK);
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
	//各情報の取得
	CSound *pSound = CManager::GetSound();							//サウンドの取得
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	
	CManager *pManager = NULL;									
	CGame *pGame = pManager->GetGame();								//ゲームの取得
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//ゲームカメラの取得
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//向きの取得
	D3DXVECTOR3 pos = CCharacter::GetPos();							//位置の取得

																	//ステータスの取得

	float fSpeed = GetSpeed();				//速さ
	float fInertia = GetInertia();			//慣性

	
	//前回の位置を格納
	m_posOld = pos;

	if (pInputKeyboard->GetKeyboardPress(DIK_H) == true)
	{
		m_nControllerType = 0;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_J) == true)
	{
		m_nControllerType = 1;
	}

	//プレイヤーの移動処理（キーボード）
	if (pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//ポリゴンを左上に移動
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//ポリゴンを左下に移動
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.75f);
		}
		else
		{//ポリゴンを左に移動
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//ポリゴンを右上に移動
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_Angle.y = rot.y + (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//ポリゴンを右下に移動
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_Angle.y = rot.y + (D3DX_PI * 0.75f);
		}
		else
		{//ポリゴンを右に移動
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_Angle.y = rot.y + (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
	{
		m_move.x -= sinf(rot.y) * fSpeed;
		m_move.z -= cosf(rot.y) * fSpeed;
		m_Angle.y = rot.y;
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
	{
		m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_Angle.y = D3DX_PI + rot.y;
	}

	

	if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true)
	{
		//スピードUPの時はこっち
		m_pLoadEffect->OnTimeEffect();
	}

	if (m_nControllerType == 0)
	{
		PlayerMovePad(rot, pos);
	}
	else if (m_nControllerType == 1)
	{
		PlayerMoveMouse(rot, pos);

	}
	D3DXVECTOR3 DiffAngle = INITIALIZE_VECTOR3;

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
	
	//移動処理
	pos += m_move;

	//慣性
	m_move.x += (0.0f - m_move.x) * fInertia;
	m_move.z += (0.0f - m_move.z) * fInertia;

	//位置を設定
	CCharacter::SetPos(pos);

	//向きを設定
	CCharacter::SetRot(m_rot);

	//ボリュームの設定
	pSound->SetVolume(CSound::SOUND_LABEL_SE018, 50.0f);
}

//=============================================================================
//　プレイヤーのPADでの移動処理
//=============================================================================
void  CPlayer::PlayerMovePad(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos)
{
	CSound *pSound = CManager::GetSound();							//サウンドの取得
	CInputXPad * pXPad = CManager::GetXPad();						//Xinputの取得

	//ステータスの取得
	int   nAttack = GetAttack();			//攻撃力
	float fAttackSpeed = GetAttackSpeed();	//攻撃スピード
	float fRange = GetRange();				//攻撃範囲
	float fSpeed = GetSpeed();				//速さ
	float fInertia = GetInertia();			//慣性
	float fLineTime = GetLineTime();		//ラインを引ける時間


	//コントローラー（XInput）
	float fRot = pXPad->GetStickRot(0, m_nNumPlayer);				//スティックの取得
	if (pXPad->GetStick(0, m_nNumPlayer) == true)
	{
		//移動状態
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE)
		{
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//移動状態
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				m_PlayerState != PLAYERSTATE_ACTION)
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//移動状態
				m_PlayerState = PLAYERSTATE_ACTION;
			}
			
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				m_PlayerState != PLAYERSTATE_ACTION)
			{
				m_move.x -= sinf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + fRot;
			}
			
			else if(m_CharcterType == CCharacter::CHARCTERTYPE_SPEED)
			{
				m_move.x -= sinf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + fRot;
			}
			//移動エフェクト
			m_pLoadEffect->SetPos(D3DXVECTOR3(pos.x, pos.y + 20.0f, pos.z));
			//通常の移動はこっち
			m_pLoadEffect->Update();
		}
		if (m_bWalk == true)
		{//移動モーション
			if (m_PlayerState == PLAYERSTATE_WALK)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE018);//足音
				m_pMotion->SetNumMotion(PLAYERSTATE_WALK);
				m_bWalk = false;
			}
		}
		if (m_bSprintMotion == true)
		{//スプリントモーション
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState == PLAYERSTATE_ACTION)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE018);//足音
				m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);
				m_bSprintMotion = false;
			}
		}
	}
	else
	{
		if (m_PlayerState == PLAYERSTATE_WALK || (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState == PLAYERSTATE_ACTION) &&
			(m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f)
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE018);
			//ニュートラル状態にする
			m_PlayerState = PLAYERSTATE_NONE;
			m_pMotion->SetNumMotion(PLAYERSTATE_NONE);
			//m_bWalk = true;
		}
	}
}

//=============================================================================
//　プレイヤーのMOUSEでの移動処理
//=============================================================================
void  CPlayer::PlayerMoveMouse(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos)
{
	CSound *pSound = CManager::GetSound();							//サウンドの取得

	//RawMouseの取得
	CRawMouse *pRawMouse = CManager::GetRawMouse();					

	//マウスの角度を取得
	int MouseX = pRawMouse->GetMouse(m_nNumPlayer).lLastX;
	int MouseY = pRawMouse->GetMouse(m_nNumPlayer).lLastY;

	// 接続されているマウスの数を取得
	int nMouseNum = pRawMouse->GetMouseNum();

	//マウス用
	//float Mouse = atan2f((float)MouseX, (float)MouseY * -1.0f);

	//トラックボール用
	float Mouse = atan2f((float)MouseX * -1.0f, (float)MouseY * -1.0f);

	//ステータスの取得
	int   nAttack = GetAttack();			//攻撃力
	float fAttackSpeed = GetAttackSpeed();	//攻撃スピード
	float fRange = GetRange();				//攻撃範囲
	float fSpeed = GetSpeed();				//速さ
	float fInertia = GetInertia();			//慣性
	float fLineTime = GetLineTime();		//ラインを引ける時間

	if (MouseX != 0 || MouseY != 0)
	{//マウス

		if (m_PlayerState != PLAYERSTATE_BLOWAWAY || m_PlayerState != PLAYERSTATE_DAMAGE)
		{
			if (m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//移動状態
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_bSprint == true)
			{
				//移動状態
				m_PlayerState = PLAYERSTATE_ACTION;
			}

			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				m_PlayerState != PLAYERSTATE_ACTION)
			{//パワー型もしくは地雷型がアクション中じゃなかったら
				m_move.x -= sinf((Mouse + CameraRot.y)) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(Mouse + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + Mouse;
			}
			else
			{
				m_move.x -= sinf((Mouse + CameraRot.y)) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(Mouse + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + Mouse;
			}

			//移動エフェクト
			m_pLoadEffect->SetPos(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z));
			//通常の移動はこっち
			m_pLoadEffect->Update();

			//スピードUPの時はこっち
			//m_pLoadEffect->OnTimeEffect();
		}
		if (m_bWalk == true)
		{//移動モーション
			if (m_PlayerState == PLAYERSTATE_WALK)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE018);//足音
				m_pMotion->SetNumMotion(PLAYERSTATE_WALK);
				m_bWalk = false;
			}
		}
		if (m_bSprintMotion == true)
		{//スプリントモーション
			if (m_PlayerState == PLAYERSTATE_ACTION)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE018);//足音
				m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);
				m_bSprintMotion = false;
			}
		}
	}
	else
	{
		if ((m_PlayerState == PLAYERSTATE_WALK || m_PlayerState == PLAYERSTATE_ACTION) && (m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f)
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE018);
			//ニュートラル状態にする
			m_PlayerState = PLAYERSTATE_NONE;
			m_pMotion->SetNumMotion(PLAYERSTATE_NONE);
		}
	}
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
		
		}
	}
	else if (pCollider->GetType() == CCollider::TYPE_CYLINDER)
	{
		if (CollisionCylinderyCollider((CCylinderCollider*)pCollider, pos, posOld, Move, ColRange) == true)
		{
		}
	}
	else if (pCollider->GetType() == CCollider::TYPE_SPHERE_PLAYERATTACK)
	{
		if (CollisionPlayerAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, ColRange) == true)
		{
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

//=============================================================================
//　シリンダーコライダーとの当たり判定処理
//=============================================================================
bool CPlayer::CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;
	if (pCylinderCollider->Collision(&pos, &posOld, &Move, CYLINDER_COLRADIUS, CYLINDER_COLHEIGHT, this, &bLand) == true)
	{
		if (bLand == true)
		{

		}

		CScene *pParent = pCylinderCollider->GetParent();
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER)
		{// 親が敵かプレイヤーだった場合自分を吹っ飛ばす
			CCharacter *pCharacter = (CCharacter*)pParent;
			if (pCharacter == NULL) { return true; }
			D3DXVECTOR3 AnotherPos = pCharacter->GetPos();

			if (m_bSuperArmor != true)
			{//スーパーアーマ状態じゃないなら
				BlowAway(AnotherPos);
			}
		}
		else if (pParent->GetObjType() == OBJTYPE_MINE)
		{// 地雷だったら
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this && pMine->GetType() == CMine::TYPE_NONE)
			{// 自分以外が出した地雷なら起動
				pMine->SetType(CMine::TYPE_STARTUP);
			}
		}

		return true;
	}

	return false;
}

//=============================================================================
//　攻撃球との当たり判定処理
//=============================================================================
bool CPlayer::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
{
	m_nDamageCounter;
	if (pShere->Collision(&pos, CYLINDER_COLRADIUS, this) == true && pShere->GetParent() != this)
	{// 自分以外の攻撃球が当たったら
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_MINE)
		{// 地雷だったら
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this)
			{// 自分以外が出した地雷ならダメージ

				//当たってる間はダメージ状態
				m_PlayerState = PLAYERSTATE_DAMAGE;

				if (m_nDamageCounter == 0)
				{
					m_bSuperArmor = true;
					m_pMotion->SetNumMotion(4);
					m_nDamageCounter = 1;
				}
			}
		}
		if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ENEMY)
		{
			//if (m_bCollision == true)
			{
				m_pMotion->SetNumMotion(4);
			}

		}
		if (pParent->GetObjType() == OBJTYPE_PLAYER)
		{
			//m_pMotion->SetNumMotion(3);
		}
		return true;
	}

	return false;
}

//=============================================================================
//　吹っ飛び処理
//=============================================================================
void CPlayer::BlowAway(D3DXVECTOR3 AnotherPos)
{
	// 座標を取得
	D3DXVECTOR3 pos = GetPos();

	// 吹っ飛ぶ向きを計算
	m_fBlowAngle = CFunctionLib::CalcAngleToDest(pos.x, pos.z, AnotherPos.x, AnotherPos.z);

	// 吹っ飛ぶ距離を設定
	m_fBlowLength = BLOW_MOVING;

	// 吹っ飛び状態にする
	m_PlayerState = PLAYERSTATE_BLOWAWAY;
	m_pMotion->SetNumMotion(PLAYERSTATE_BLOWAWAY);

	// 向きを変える
	float fRot = m_fBlowAngle + D3DX_PI;
	fRot = CFunctionLib::CheckRotationOverPiX(fRot);
	m_rot.y = fRot;
	m_Angle = D3DXVECTOR3(0.0f, fRot, 0.0f);
}

//=============================================================================
// ラインの生成処理
//=============================================================================
void CPlayer::CreateOrbitLine(void)
{
	if (m_pOrbitLine == NULL)
	{
		m_pOrbitLine = CSceneOrbit::Create(CSceneOrbit::TYPE_PLAYER, CCharacter::GetPos());
		m_pOrbitLine->SetMtxParent(&m_pModel[10]->GetMtxWorld());
		m_pOrbitLine->SetMtxParentEnd(&m_pModel[1]->GetMtxWorld());
	}
}

//=============================================================================
// ラインの破棄
//=============================================================================
void CPlayer::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}

