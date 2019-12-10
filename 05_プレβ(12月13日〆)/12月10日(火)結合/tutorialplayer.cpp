
//=============================================================================
//
//　チュートリアルプレイヤー処理 [tutorialplayer.cpp]
// Author : Kobayashi & Yamashita & Niwa
//
//=============================================================================
#include "tutorialplayer.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "model.h"
#include "Territory.h"
#include "library.h"
#include "line.h"
#include "loadEffect.h"
#include "UI.h"
#include "score.h"
#include "mine.h"
#include "debuglog.h"
#include "collision.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIALPLAYER_SPEED	(3.2f) //3.2

//リストの最大数
#define AIMPOS_LIST00	(4)
#define AIMPOS_LIST01	(7)
#define AIMPOS_LIST02	(3)
#define AIMPOS_LIST03	(1)
#define AIMPOS_LIST04	(1)
#define AIMPOS_LIST05	(2)
#define AIMPOS_LIST06	(1)
#define AIMPOS_LIST07	(1)

// コリジョン関係
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//静的メンバ変数宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CTutorialPlayer::CTutorialPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
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
	m_bSprint = false;
	m_bSprintMotion = false;
	m_bWalk = true;
	m_fInertia = 0.0f;
	m_nDownCount = 0;
	m_pLoadEffect = NULL;
	m_bMovePlayer = false;
	m_bMoveEnd = false;
	m_nNumPos = 0;
	m_nNumList = 0;
	m_fSpeed = TUTORIALPLAYER_SPEED;
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTutorialPlayer::~CTutorialPlayer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType)
{
	//プレイヤーのナンバーを取得
	m_nNumPlayer = nNumPlayer;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
	CCharacter::SetPos(pos);

	//モデルの情報を取得
	m_pModel = CCharacter::GetModel();

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();

	//目的地の設定
	SetAimPos();
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorialPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CTutorialPlayer::Uninit(void)
{
	//キャラクターの破棄
	CCharacter::Uninit();

	if (m_pLoadEffect != NULL)
	{//エフェクトの破棄
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	//リストの破棄
	for (int nCnt = 0; nCnt < MAX_LIST; nCnt++)
	{
		for (int nCntList = 0; nCntList < m_AimPosList[nCnt].nMaxList; nCntList++)
		{
			delete[] m_AimPosList[nCnt].List;
			m_AimPosList[nCnt].List = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void  CTutorialPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();							//サウンドの取得

	//キャラクターの更新
	CCharacter::Update();

	//プレイヤーの状態
	switch (m_PlayerState)
	{
	case PLAYERSTATE_DAMAGE:
		// ダメージカウント加算
		m_nDownCount++;

		if (m_nDownCount == 70)
		{
			m_pMotion->SetNumMotion(5);
		}
		if (m_nDownCount >= 120)
		{//110秒で動けるようになる
			m_bWalk = true;
			m_bSprintMotion = true;
			m_nDownCount = 0;
			m_PlayerState = PLAYERSTATE_NONE;
			m_nDamageCounter = 0;
		}
		break;
	}

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

	//移動処理
	UpdateMoveTutorial();

#if _DEBUG
	DebugMove();
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void  CTutorialPlayer::Draw(void)
{
	if (GetDeth() == true) { return; }

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
CTutorialPlayer *CTutorialPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, TYPE type, CHARCTERTYPE charctertype)
{
	CTutorialPlayer *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//メモリを動的確保
		pPlayer = new CTutorialPlayer;
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
void  CTutorialPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
//　吹っ飛び処理
//=============================================================================
void CTutorialPlayer::BlowAway(D3DXVECTOR3 AnotherPos)
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
// デバッグ用の移動処理
//=============================================================================
void CTutorialPlayer::DebugMove(void)
{
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	//位置の取得
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//移動処理
	if (pKeyboard->GetKeyboardPress(DIK_UP) == true)
	{
		pos.z += 1.5f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_DOWN) == true)
	{
		pos.z -= 1.5f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_LEFT) == true)
	{
		pos.x -= 1.5f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
	{
		pos.x += 1.5f;
	}
	
	//位置設定
	CCharacter::SetPos(pos);
}

//=============================================================================
// 目的地の位置設定
//=============================================================================
void CTutorialPlayer::SetAimPos(void)
{
	for (int nCnt = 0; nCnt < MAX_LIST; nCnt++)
	{
		m_AimPosList[nCnt].List = NULL;
		m_AimPosList[nCnt].nMaxList = 0;
	}

	//1つ目のリスト作成
	m_AimPosList[0].nMaxList = AIMPOS_LIST00;
	m_AimPosList[0].List = new D3DXVECTOR3[m_AimPosList[0].nMaxList];

	m_AimPosList[0].List[0] = D3DXVECTOR3(10.0f, 0.0f, 250.0f);
	m_AimPosList[0].List[1] = D3DXVECTOR3(-190.0f, 0.0f, 250.0f);
	m_AimPosList[0].List[2] = D3DXVECTOR3(-190.0f, 0.0f, 20.0f);
	m_AimPosList[0].List[3] = D3DXVECTOR3(20.0f, 0.0f, 20.0f);

	//２つ目のリストを作成
	m_AimPosList[1].nMaxList = AIMPOS_LIST01;
	m_AimPosList[1].List = new D3DXVECTOR3[m_AimPosList[1].nMaxList];

	m_AimPosList[1].List[0] = D3DXVECTOR3(-10.0f, 0.0f, -20.0f);
	m_AimPosList[1].List[1] = D3DXVECTOR3(-250.0f, 0.0f, -10.0f);
	m_AimPosList[1].List[2] = D3DXVECTOR3(-230.0f, 0.0f, 300.0f);
	m_AimPosList[1].List[3] = D3DXVECTOR3(150.0f, 0.0f, 300.0f);
	m_AimPosList[1].List[4] = D3DXVECTOR3(160.0f, 0.0f, 150.0f);
	m_AimPosList[1].List[5] = D3DXVECTOR3(140.0f, 0.0f, -10.0f);
	m_AimPosList[1].List[6] = D3DXVECTOR3(-60.0f, 0.0f, -20.0f);

	//3つ目のリストを作成
	m_AimPosList[2].nMaxList = AIMPOS_LIST02;
	m_AimPosList[2].List = new D3DXVECTOR3[m_AimPosList[2].nMaxList];

	m_AimPosList[2].List[0] = D3DXVECTOR3(-200.0f, 0.0f, 150.0f);
	m_AimPosList[2].List[1] = D3DXVECTOR3(-300.0f, 0.0f, 300.0f);
	m_AimPosList[2].List[2] = D3DXVECTOR3(0.0f, 0.0f, 380.0f);

	//4つ目のリストを作成
	m_AimPosList[3].nMaxList = AIMPOS_LIST03;
	m_AimPosList[3].List = new D3DXVECTOR3[m_AimPosList[3].nMaxList];

	m_AimPosList[3].List[0] = D3DXVECTOR3(250.0f, 0.0f, 0.0f);

	//5つ目のリストを作成
	m_AimPosList[4].nMaxList = AIMPOS_LIST04;
	m_AimPosList[4].List = new D3DXVECTOR3[m_AimPosList[4].nMaxList];

	m_AimPosList[4].List[0] = D3DXVECTOR3(-300.0f, 0.0f, 300.0f);

	//6つ目のリストを作成
	m_AimPosList[5].nMaxList = AIMPOS_LIST05;
	m_AimPosList[5].List = new D3DXVECTOR3[m_AimPosList[5].nMaxList];

	m_AimPosList[5].List[0] = D3DXVECTOR3(250.0f, 0.0f, 0.0f);
	m_AimPosList[5].List[1] = D3DXVECTOR3(-600.0f, 0.0f, 150.0f);

	//7つ目のリストを作成
	m_AimPosList[6].nMaxList = AIMPOS_LIST06;
	m_AimPosList[6].List = new D3DXVECTOR3[m_AimPosList[6].nMaxList];

	m_AimPosList[6].List[0] = D3DXVECTOR3(0.0f, 0.0f, 180.0f);

	//8つ目のリストを作成
	m_AimPosList[7].nMaxList = AIMPOS_LIST07;
	m_AimPosList[7].List = new D3DXVECTOR3[m_AimPosList[7].nMaxList];

	m_AimPosList[7].List[0] = D3DXVECTOR3(-600.0f, 0.0f, 150.0f);
}

//=============================================================================
// チュートリアル用の移動処理
//=============================================================================
void CTutorialPlayer::UpdateMoveTutorial(void)
{
	if (!m_bMovePlayer) { return; }
	if (m_pMotion == NULL) { return; }

	int nNumMotion = m_pMotion->GetNumMotion();
	if (nNumMotion == PLAYERSTATE_DAMAGE || nNumMotion == PLAYERSTATE_MAX) { return; }
	
	//歩く状態にする
	SetMotion(PLAYERSTATE_WALK);

	//変数宣言
	bool bBigerX = false;
	bool bBigerZ = false;
	bool bSetPotisionX = false;
	bool bSetPotisionZ = false;

	if(m_AimPosList[m_nNumList].List == NULL){return;}

	//位置の取得
	D3DXVECTOR3 Pos = CCharacter::GetPos();
	if (Pos.x > m_AimPosList[m_nNumList].List[m_nNumPos].x) { bBigerX = true; }
	if (Pos.z > m_AimPosList[m_nNumList].List[m_nNumPos].z) { bBigerZ = true; }

	//目的値までの角度を求める
	float fAngle = CFunctionLib::CalcAngleToDest(Pos.x, Pos.z, m_AimPosList[m_nNumList].List[m_nNumPos].x, m_AimPosList[m_nNumList].List[m_nNumPos].z);

	//移動処理
	Pos.x += sinf(fAngle) * m_fSpeed;
	Pos.z += cosf(fAngle) * m_fSpeed;

	//値の修正
	if (bBigerX)
	{//X値が目的地より大きい
		if (Pos.x <= m_AimPosList[m_nNumList].List[m_nNumPos].x)
		{ 
			Pos.x = m_AimPosList[m_nNumList].List[m_nNumPos].x;
			bSetPotisionX = true;
		}
	}
	else
	{//X値が目的地より小さい
		if (Pos.x >= m_AimPosList[m_nNumList].List[m_nNumPos].x) 
		{ 
			Pos.x = m_AimPosList[m_nNumList].List[m_nNumPos].x; 
			bSetPotisionX = true;
		}
	}

	if (bBigerZ)
	{//Y値が目的地より大きい
		if (Pos.z <= m_AimPosList[m_nNumList].List[m_nNumPos].z) 
		{ 
			Pos.z = m_AimPosList[m_nNumList].List[m_nNumPos].z; 
			bSetPotisionZ = true;
		}
	}
	else
	{//Y値が目的地より小さい
		if (Pos.z >= m_AimPosList[m_nNumList].List[m_nNumPos].z)
		{ 
			Pos.z = m_AimPosList[m_nNumList].List[m_nNumPos].z; 
			bSetPotisionZ = true;
		}
	}

	//角度修正
	if (fAngle == 0.0f)
	{
		fAngle = D3DX_PI;
	}
	else if (fAngle == D3DX_PI)
	{
		fAngle = 0.0f;
	}
	else
	{
		fAngle += D3DX_PI;
	}

	//位置設定
	CCharacter::SetRot(D3DXVECTOR3(0.0f, fAngle, 0.0f));
	CCharacter::SetPos(Pos);

	//フラグ判定
	if (bSetPotisionX && bSetPotisionZ)
	{
		if (m_nNumPos != m_AimPosList[m_nNumList].nMaxList - 1)
		{//リストの値より小さかったら
			m_nNumPos++;
		}
		else
		{//目的地までたどり着いたら
			m_nNumPos = 0;
			m_bMovePlayer = false;
			m_bMoveEnd = true;
			SetMotion(PLAYERSTATE_NONE);	//ニュートラルモーションにする
		}
	}
}

//=============================================================================
// モーション設定
//=============================================================================
void CTutorialPlayer::SetMotion(PLAYERSTATE PlayerState)
{
	if (m_pMotion != NULL)
	{
		if (m_pMotion->GetNumMotion() != PlayerState)
		{
			m_pMotion->SetNumMotion(PlayerState);
			m_PlayerState = PlayerState;
		}
	}
}

//=============================================================================
//　コライダーとの判定
//=============================================================================
void CTutorialPlayer::Collision(void)
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
			CollisionCollider(pCol, pos, posOld, m_move, D3DXVECTOR3(15.0f, 15.0f, 15.0f));
			pCol = pColNext;
		}
	}
	CCharacter::SetPos(pos);
}

//=============================================================================
//　コライダーとの当たり判定処理
//=============================================================================
bool CTutorialPlayer::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
		CScene *pParent = pCollider->GetParent();

		if (CollisionPlayerAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, ColRange) == true
			&& pParent->GetObjType() != OBJTYPE_ROBOT)
		{
		}
	}
	return false;
}

//=============================================================================
//　ボックスコライダーとの当たり判定処理
//=============================================================================
bool CTutorialPlayer::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
bool CTutorialPlayer::CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;
	if (pCylinderCollider->Collision(&pos, &posOld, &Move, CYLINDER_COLRADIUS, CYLINDER_COLHEIGHT, this, &bLand) == true)
	{
		if (bLand == true)
		{

		}

		CScene *pParent = pCylinderCollider->GetParent();
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ROBOT)
		{// 親が敵かプレイヤーだった場合自分を吹っ飛ばす
			CCharacter *pCharacter = (CCharacter*)pParent;
			if (pCharacter == NULL) { return true; }
			D3DXVECTOR3 AnotherPos = pCharacter->GetPos();
			m_nDamageCount = 0;
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
bool CTutorialPlayer::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
{
	//サウンドの取得
	CSound *pSound = CManager::GetSound();

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
					pSound->PlaySound(CSound::SOUND_LABEL_SE025);	//地雷音
					pSound->PlaySound(CSound::SOUND_LABEL_SE019);	//ダウン音
																	/*pSound->SetVolume(CSound::SOUND_LABEL_SE025, 20.0f);
																	pSound->SetVolume(CSound::SOUND_LABEL_SE019, 20.0f);*/
					m_pMotion->SetNumMotion(4);
					m_nDamageCounter = 1;
				}
			}
		}
		if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ENEMY)
		{

			//当たってる間はダメージ状態
			m_PlayerState = PLAYERSTATE_DAMAGE;
			if (m_nDamageCounter == 0)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE023);	//衝撃波音
				pSound->PlaySound(CSound::SOUND_LABEL_SE019);	//ダウン音
																/*pSound->SetVolume(CSound::SOUND_LABEL_SE023, 20.0f);
																pSound->SetVolume(CSound::SOUND_LABEL_SE019, 20.0f);*/
				m_pMotion->SetNumMotion(4);
				m_nDamageCounter = 1;
			}

		}

		return true;
	}

	return false;
}


//=============================================================================
// ラインの生成処理
//=============================================================================
void CTutorialPlayer::CreateOrbitLine(void){}

//=============================================================================
// ラインの破棄
//=============================================================================
void CTutorialPlayer::UninitOrtbitLine(void){}