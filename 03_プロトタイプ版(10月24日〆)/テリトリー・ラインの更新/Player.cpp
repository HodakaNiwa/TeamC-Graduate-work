
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
#include "Territory.h"
#include "library.h"
#include "line.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)
#define FIRST_LINETIME	(60 * 20)	//ラインの時間の初期数
#define MIN_TERRITORY	(3)			//テリトリーの最低限数
#define TERRITORY_TOP	(0)			//テリトリーの先頭情報
#define NOT_GETTIME		(60)		//テリトリーの取得できない時間
#define LINE_TIME		(90)		//ラインを消さない時間

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
CSceneX *CPlayer::m_apSceneX = NULL;

//国の色の設定
D3DXCOLOR CPlayer::m_CountryColor[CPlayer::TYPE_MAX] =
{
	COLOR_WHITE,	//日本
	COLOR_BULE,		//ロシア
	COLOR_RED,		//アメリカ
	COLOR_YELLOW,	//ブラジル
	COLOR_ORENGE,	//イギリス
	COLOR_PAPULE,	//イタリア
	COLOR_BRACK,	//ニュージーランド
	COLOR_GREEN,	//南アフリカ
};


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
CPlayer::~CPlayer(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, int nNumPlayer, char ModelTxt[40], char MotionTxt[40])
{
	m_nNumPlayer = nNumPlayer;
	CCharacter::Init(ModelTxt,MotionTxt);
	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;

	//コピーラインの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	ResetLine();	//ラインの初期化
	ResetList();	//テリトリーポインタの初期化
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

	//ラインのカウントダウン処理
	CountDownLineTime();

	//プレイヤーの移動処理
	PlayerMove();

	//時間の可算
	if (m_bBlockStartTerritory) 
	{ 
		m_nCountTime++;
		if ((m_nCountTime % NOT_GETTIME) == 0) { m_bBlockStartTerritory = false; }
	}

	//図形が完成した後の処理
	UpdateShapeComplete();
}

//=============================================================================
// 描画処理
//=============================================================================
void  CPlayer::Draw(void)
{
	//ローテション
	//位置

	CCharacter::Draw();
	//D3DXMATRIX mtxWorld;
	//D3DXMatrixIdentity(&mtxWorld);
	//CManager::GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, int nNumPlayer, char ModelTxt[40], char MotionTxt[40],TYPE type)
{
	CPlayer *pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//メモリを動的確保
		pPlayer = new CPlayer;

		if (pPlayer != NULL)
		{//NULLチェック

			//初期化処理
			pPlayer->SetType(type);
			pPlayer->Init(pos,nNumPlayer,ModelTxt,MotionTxt);

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
void  CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
//　プレイヤーの移動処理
//=============================================================================
void  CPlayer::PlayerMove(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CGamePad * pGamePad = CManager::GetInputGamePad();				//ゲームパッドの取得
	CInputXPad * pXPad = CManager::GetXPad();
	float fRot = pXPad->GetStickRot(0, m_nNumPlayer);


	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);

	//向きと位置の取得
	D3DXVECTOR3 rot = pGameCamera->GetRot();
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//ステータスの取得
	int   nAttack      = GetAttack();
	float fAttackSpeed = GetAttackSpeed();
	float fRange       = GetRange();
	float fSpeed       = GetSpeed();
	float fInertia     = GetInertia();
	float fLineTime    = GetLineTime();

	//前回の位置を格納
	m_posOld = pos;

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
		m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_Angle.y = D3DX_PI + rot.y;
	}

		//コントローラー（XInput）
		if (pXPad->GetStick(0, m_nNumPlayer) == true)
		{
			m_move.x -= sinf(fRot + rot.y) * fSpeed;
			m_move.z -= cosf(fRot + rot.y) * fSpeed;
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
		m_move.x += (0.0f - m_move.x) * fInertia;
		m_move.z += (0.0f - m_move.z) * fInertia;
	
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

//=============================================================================
// テリトリーの取得更新処理
//=============================================================================
void CPlayer::CountDownLineTime(void)
{
	if (m_apTerritory[TERRITORY_TOP] != NULL)
	{
		//線を描く時間を減らす
		m_nLineTime--;

		if (m_nLineTime < 0)
		{
			m_nLineTime = m_nMaxLineTime;					//描画できる時間をリセットする
			GetTerritory(m_apTerritory[TERRITORY_TOP]);		//起点のみ点数を入れる
			ResetList();									//リストを初期化する
			UninitOrtbitLine();								//奇跡の破棄
			UninitLine();										//ラインの破棄
		}
	}
}

//=============================================================================
//　図形が完成した後の処理
//=============================================================================
void CPlayer::UpdateShapeComplete(void)
{
	if (!m_bMakeShape) { return; }

	//時間の可算
	m_nCntTimeCopyLine++;

	if ((m_nCntTimeCopyLine % LINE_TIME) == 0)
	{
		m_bMakeShape = false;
		UninitCopyLine();
	}
}

//=============================================================================
// テリトリーをリストに追加
//=============================================================================
void CPlayer::AddTerritoryList(CTerritory * pTerritory)
{	
	//図形が完成しているかどうかの判定
	if (m_nCountTerritory >= MIN_TERRITORY)
	{
		if (true == ChackSamePos(pTerritory, TERRITORY_TOP))
		{
			ShapeComplete(pTerritory);	//図形が完成したときの処理
			ChackInShape();				//図形内にテリトリーあるか判定
			CreateEndLine();			//最後の線の生成
			ResetList();				//リストの初期化
			UninitOrtbitLine();			//奇跡の破棄
			ResetLine();				//ラインの初期化
			return;
		}
	}

	//リストの追加処理
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apTerritory[nCnt] == NULL)
		{
			UninitOrtbitLine();			//奇跡の破棄

			if (m_bBlockStartTerritory)
			{//取得後、同じ起点を取らないようにする
				D3DXVECTOR3 Pos = pTerritory->GetPos();
				D3DXVECTOR3 OldStartPos = m_pOldStartTerritory->GetPos();

				if ((nCnt != TERRITORY_TOP) && (Pos != OldStartPos))
				{
					m_apTerritory[nCnt] = pTerritory;
					m_nCountTerritory++;
					CreateOrbitLine();			//奇跡の生成
				}
			}
			else
			{//通常
				m_apTerritory[nCnt] = pTerritory;
				m_nCountTerritory++;
				CreateOrbitLine();				//奇跡の生成
			}

			//ラインを生成する
			if ((nCnt != 0)) 
			{ 
				if (m_apLine[nCnt - 1] == NULL) 
				{ 
					m_apLine[nCnt - 1] = CLine::Create(m_apTerritory[nCnt - 1]->GetPos(), m_apTerritory[nCnt]->GetPos(), m_nNumPlayer, nCnt - 1);
				}
			}
			break;
		}
		else if (m_apTerritory[nCnt] != NULL)
		{//同じテリトリーの情報ではないかどうか
			if (true == ChackSamePos(pTerritory, nCnt)) { break; }
		}
	}
}

//=============================================================================
// テリトリーで図形が出来ているかどうか
//=============================================================================
bool CPlayer::ChackSamePos(CTerritory * pTerritory, int nNumTerritory)
{
	//変数宣言
	bool bMakeShape = false;

	D3DXVECTOR3 TopPos = m_apTerritory[nNumTerritory]->GetPos();
	D3DXVECTOR3 CurPos = pTerritory->GetPos();

	if (TopPos == CurPos) { bMakeShape = true; }

	return bMakeShape;
}

//=============================================================================
// テリトリーが完成したときの処理
//=============================================================================
void CPlayer::ShapeComplete(CTerritory * pTerritory)
{
	//図形が完成
	m_bBlockStartTerritory = true;
	m_pOldStartTerritory = pTerritory;
	m_bMakeShape = true;
	m_nLineTime = m_nMaxLineTime;		//描画できる時間をリセットする

	//テリトリーの取得
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL) { GetTerritory(m_apTerritory[nCnt]); }
	}
}

//=============================================================================
// 図形内にテリトリーがあるかどうか
//=============================================================================
void CPlayer::ChackInShape(void)
{
	//変数宣言
	D3DXVECTOR3 * pListPos = new D3DXVECTOR3[m_nCountTerritory];
	D3DXVECTOR3 * pVecA = new D3DXVECTOR3[m_nCountTerritory];

	//位置を取得
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL)
		{
			pListPos[nCnt] = m_apTerritory[nCnt]->GetPos();
		}
	}

	//VecAを求める
	for (int nCntVec = 0; nCntVec < m_nCountTerritory; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % m_nCountTerritory) == 0) { nCnt = 0; }

		pVecA[nCntVec] = pListPos[nCnt] - pListPos[nCntVec];
	}

	//テリトリーを取得する
	CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY); //オブジェクトポインタの先頭を取得			
	CScene * pScene = pSceneTop;							 //トップの位置を保存する
	CTerritory * m_pTerritory = NULL;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();	//次のオブジェクトのポインタを保存する

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			CTerritory * pTerritory = (CTerritory *)pScene;

			if (pTerritory->GetPlayerNumber() != m_nNumPlayer)
			{
				D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();
				int nCountEnter = 0;

				for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
				{
					if (m_apTerritory[nCnt] != NULL)
					{
						D3DXVECTOR3 VecB = TerritoryPos - pListPos[nCnt];

						//外積を求める
						float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pVecA[nCnt].x, pVecA[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

						//ベクトルより大きいかどうか
						if (fCross >= 0) { nCountEnter++; }
					}
				}

				//範囲内ならテリトリーの取得
				if (nCountEnter == m_nCountTerritory) { GetTerritory(pTerritory); }
			}
		}

		pScene = pSceneNext;	//ポインタを進める
	}

	//メモリの破棄
	if (pListPos != NULL)
	{
		delete[] pListPos;
		pListPos = NULL;
	}
	if (pVecA != NULL)
	{
		delete[] pVecA;
		pVecA = NULL;
	}
}

//=============================================================================
// テリトリーの取得処理
//=============================================================================
void CPlayer::GetTerritory(CTerritory * pTerritory)
{
	int nOldNumPlayer = pTerritory->GetPlayerNumber();
	pTerritory->SetColor(m_CountryColor[(int)m_type]);	//色の変更
	pTerritory->SetPlayerNumber(m_nNumPlayer);			//プレイヤー番号の保存

	//前回のプレイヤーの減点処理
	if (nOldNumPlayer != -1)
	{

	}

	//点数の追加処理
}

//=============================================================================
// テリトリーのリストを初期化
//=============================================================================
void CPlayer::ResetList(void)
{
	//テリトリーポインタの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apTerritory[nCnt] = NULL;
	}

	//カウンターの初期化
	m_nCountTerritory = 0;
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

//=============================================================================
// ラインの初期化
//=============================================================================
void CPlayer::ResetLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apLine[nCnt] = NULL;
	}
}

//=============================================================================
// ラインの破棄
//=============================================================================
void CPlayer::UninitLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apLine[nCnt]->Uninit();
			m_apLine[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ラインの最後の線の生成処理
//=============================================================================
void CPlayer::CreateEndLine(void)
{
	//変数宣言
	int nCountTerritory = 0;

	//テリトリーのリストの最後尾を求める
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apTerritory[nCnt] == NULL)
		{
			nCountTerritory = nCnt - 1;
			break;
		}
	}

	//最後のラインの生成
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] == NULL)
		{
			m_apLine[nCnt] = CLine::Create(m_apTerritory[nCountTerritory]->GetPos(), m_apTerritory[TERRITORY_TOP]->GetPos(), m_nNumPlayer, nCnt);
			break;
		}
	}

	//生成したラインをコピーする
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apCopyLine[nCnt] = m_apLine[nCnt];
		}
	}
}

//=============================================================================
// コピーラインの破棄
//=============================================================================
void CPlayer::UninitCopyLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apCopyLine[nCnt] != NULL)
		{
			m_apCopyLine[nCnt]->Uninit();
			m_apCopyLine[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ラインの切断処理
//=============================================================================
void CPlayer::CutLine(int nID)
{
	//ラインの破棄
	for (int nCnt = nID; nCnt >= 0; nCnt--)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apLine[nCnt]->Uninit();
			m_apLine[nCnt] = NULL;
		}
	}

	//数字を入れ替える
	int nLine = MAX_TERRITORY - nID - 1;

	for (int nCnt = 0; nCnt < nLine; nCnt++)
	{ 
		int nMoveID = nCnt + nID + 1;

		if ((m_apLine[nCnt] == NULL) && (m_apLine[nMoveID] != NULL))
		{
			m_apLine[nCnt] = m_apLine[nMoveID];	//IDをずらす
			m_apLine[nCnt]->SetID(nCnt);
			m_apLine[nMoveID] = NULL;
		}
	}

	//切断より後ろのテリトリーを破棄する
	for (int nCnt = nID; nCnt >= 0; nCnt--)
	{
		m_apTerritory[nCnt] = NULL;
	}

	//テリトリーの始点を移動させる
	int nNumTerritory = MAX_TERRITORY - (nID + 1);
	for (int nCnt = 0; nCnt < nNumTerritory; nCnt++)
	{
		if ((m_apTerritory[nCnt] == NULL)/* && (m_apTerritory[nCnt + nID] != NULL)*/)
		{
			m_apTerritory[nCnt] = m_apTerritory[nCnt + (nID + 1)];
			m_apTerritory[nCnt + (nID + 1)] = NULL;
		}
	}
}