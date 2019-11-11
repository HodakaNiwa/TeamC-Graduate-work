
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
#include "loadEffect.h"
#include "inputmouce.h"
#include "RawMouse.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)
#define FAR_POS			(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))


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
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer)
{
	//プレイヤーのナンバーを取得
	m_nNumPlayer = nNumPlayer;

	//初期化
	m_bWalk = true;

	//キャラクターの初期化
	CCharacter::Init(ModelTxt, MotionTxt);
	CCharacter::SetPos(pos);

	//モデルの情報を取得
	m_pModel = CCharacter::GetModel();

	//ライン変数の初期化
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();

	//コピーラインの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	//エフェクトの初期化
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, pos, 8);
	}

	ResetLine();	//ラインの初期化
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

	if (m_pLoadEffect != NULL)
	{
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
	CCharacter::Update();

	//ラインのカウントダウン処理
	CountDownLineTime();

	int nGameState = CManager::GetGameState();

	if (nGameState != 1 && nGameState != 3)
	{
		//プレイヤーの移動処理
		PlayerMove();

		switch (m_PlayerState)
		{
		case PLAYERSTATE_NONE:

			//歩ける用にする
			m_bWalk = true;
			break;

		case PLAYERSTATE_WALK:
			
			break;
		}
	}

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
	CCharacter::Draw();

	if (m_pLoadEffect != NULL)
	{
		m_pLoadEffect->Draw();
	}
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, TYPE type, PLAYERTYPE playertype)
{
	CPlayer *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//メモリを動的確保
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{//NULLチェック
			pPlayer->SetType(type);
			pPlayer->m_PlayerType = playertype;
			pPlayer->Init(pos, ModelTxt,MotionTxt, nNumPlayer);
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
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得
	CGamePad * pGamePad = CManager::GetInputGamePad();				//ゲームパッドの取得
	CInputXPad * pXPad = CManager::GetXPad();
	float fRot = pXPad->GetStickRot(0, m_nNumPlayer);
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CRawMouse *pRawMouse = CManager::GetRawMouse();

	int MouseX = pRawMouse->GetMouse(0).lLastX;
	int MouseY = pRawMouse->GetMouse(0).lLastY;
	int MouseZ = pInputMouse->GetIZ();

	//マウス用
	float Mouse = atan2f((float)MouseX, (float)MouseY * -1.0f);
	
	//トラックボール用
	//float Mouse = atan2f((float)MouseX * -1.0f, (float)MouseY * -1.0f);


	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);

	//向きと位置の取得
	D3DXVECTOR3 rot = pGameCamera->GetRot();
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//ステータスの取得
	int   nAttack = GetAttack();
	float fAttackSpeed = GetAttackSpeed();
	float fRange = GetRange();
	float fSpeed = GetSpeed();
	float fInertia = GetInertia();
	float fLineTime = GetLineTime();

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
		//移動状態
		m_PlayerState = PLAYERSTATE_WALK;
		if (m_bWalk == true)
		{//移動モーション
			m_pMotion->SetNumMotion(1);
			m_bWalk = false;
		}
		m_move.x -= sinf(fRot + rot.y) * fSpeed;
		m_move.z -= cosf(fRot + rot.y) * fSpeed;
		m_Angle.y = rot.y + fRot;
	}
	else if (MouseX != 0 || MouseY != 0)
	{//マウス
		// 移動状態
		m_PlayerState = PLAYERSTATE_WALK;
		if (m_bWalk == true)
		{//移動モーション
			m_pMotion->SetNumMotion(1);
			m_bWalk = false;
		}
		m_move.x -= sinf((Mouse + rot.y)) * fSpeed;
		m_move.z -= cosf(Mouse + rot.y) * fSpeed;
		m_Angle.y = rot.y + Mouse;
	}
	else
	{
		if (m_PlayerState == PLAYERSTATE_WALK && (m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f))
		{
			//ニュートラル状態にする
			m_PlayerState = PLAYERSTATE_NONE;
			m_pMotion->SetNumMotion(0);

		}
	}


	//pInputMouse->SetNumMouse(m_nNumPlayer);
	

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
				D3DXVECTOR3 OldStartPos = INITIALIZE_VECTOR3;
				if (m_pOldStartTerritory != NULL) { OldStartPos = m_pOldStartTerritory->GetPos(); }

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
					if ((m_apTerritory[nCnt - 1] != NULL) && (m_apTerritory[nCnt] != NULL))
					{
						m_apLine[nCnt - 1] = CLine::Create(m_apTerritory[nCnt - 1]->GetPos(), m_apTerritory[nCnt]->GetPos(), m_nNumPlayer, nCnt - 1);
					}
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
	TRAIANGLE * pTraiangle = new TRAIANGLE[m_nCountTerritory - 2];	//三角形の数分メモリを確保する
	int * pnFarNumTerritory = new int[m_nCountTerritory - 1];		//遠い順
	float * pfLength = new float[m_nCountTerritory - 1];			//距離
	bool bEnter = false;

	InitTraiangle(pTraiangle, m_nCountTerritory - 2);	//三角形の構造体の初期化

	for (int nCnt = 0; nCnt < m_nCountTerritory - 1; nCnt++)
	{
		pnFarNumTerritory[nCnt] = 0;
		pfLength[nCnt] = 0.0f;
	}

	//テリトリーの座標を取得
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL) { pListPos[nCnt] = m_apTerritory[nCnt]->GetPos(); }
	}

	//--------------------------------
	// 出来た図形から三角形を生成する
	//--------------------------------
	//始点から遠い順にテリトリーのIDをソートする
	for (int nCntTerritory = 1; nCntTerritory < m_nCountTerritory; nCntTerritory++)
	{
		float fLength = CFunctionLib::Vec2Length(pListPos[TERRITORY_TOP].x, pListPos[TERRITORY_TOP].z, pListPos[nCntTerritory].x, pListPos[nCntTerritory].z);
		int NowTerritory = nCntTerritory;

		//距離を比較・ソートする
		for (int nCnt = 0; nCnt < m_nCountTerritory - 1; nCnt++)
		{
			//数値の入れ替え
			if (fLength > pfLength[nCnt])
			{
				//距離の入れ替え
				float fCopy = pfLength[nCnt];
				pfLength[nCnt] = fLength;
				fLength = fCopy;

				//座標番号の入れ替え
				int nCopy = pnFarNumTerritory[nCnt];
				pnFarNumTerritory[nCnt] = NowTerritory;	
				NowTerritory = nCopy;
			}
		}
	}

	//最も遠い頂点の両隣の頂点からなる三角形を作る
	int nCountTraiangle = 0;
	while (nCountTraiangle != m_nCountTerritory - 2)
	{//三角形を作る
		MakeTraiangle(0, &nCountTraiangle, pnFarNumTerritory, pListPos, pTraiangle);	
	}

	//----------------------------------------------
	// 生成した三角形内にテリトリーがあるかどうか
	//----------------------------------------------
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

				//反時計回りの判定
				for (int nCntTraiangle = 0; nCntTraiangle < m_nCountTerritory - 2; nCntTraiangle++)
				{
					//図形内にテリトリーがあるかどうか
					int nCountEnter = 0;
					for (int nCntVertex = 0; nCntVertex < TRIANGLE_VERTEX; nCntVertex++)
					{
						D3DXVECTOR3 VecB = TerritoryPos - pTraiangle[nCntTraiangle].pos[nCntVertex];

						//外積を求める
						D3DXVECTOR3 VecA = pTraiangle[nCntTraiangle].VecA[nCntVertex];
						float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(VecA.x, VecA.z), &D3DXVECTOR2(VecB.x, VecB.z));

						//ベクトルより大きいかどうか
						if (fCross >= 0)
						{
							nCountEnter++;
						}
						else
						{
							break;
						}
					}

					//範囲内ならテリトリーの取得
					if (nCountEnter == TRIANGLE_VERTEX)
					{
						GetTerritory(pTerritory);
						bEnter = true;
						break;
					}
				}
			}
		}

		pScene = pSceneNext;	//ポインタを進める
	}

	//--------------------------
	// 時計回りの判定
	//--------------------------
	if (!bEnter)
	{
		//時計回り用のVecAを求める
		for (int nCntTraiangle = 0; nCntTraiangle < m_nCountTerritory - 2; nCntTraiangle++)
		{
			for (int nCntVec = TRIANGLE_VERTEX - 1; nCntVec >= 0; nCntVec--)
			{
				int nCnt = nCntVec - 1;
				if (nCntVec == 0) { nCnt = TRIANGLE_VERTEX - 1; }
				pTraiangle[nCntTraiangle].VecA[nCntVec] = pTraiangle[nCntTraiangle].pos[nCnt] - pTraiangle[nCntTraiangle].pos[nCntVec];
			}
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

					for (int nCntTraiangle = 0; nCntTraiangle < m_nCountTerritory - 2; nCntTraiangle++)
					{
						//図形内にテリトリーがあるかどうか
						int nCountEnter = 0;
						for (int nCntVertex = TRIANGLE_VERTEX - 1; nCntVertex >= 0; nCntVertex--)
						{
							D3DXVECTOR3 VecB = TerritoryPos - pTraiangle[nCntTraiangle].pos[nCntVertex];

							//外積を求める
							D3DXVECTOR3 VecA = pTraiangle[nCntTraiangle].VecA[nCntVertex];
							float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(VecA.x, VecA.z), &D3DXVECTOR2(VecB.x, VecB.z));

							//ベクトルより大きいかどうか
							if (fCross >= 0)
							{
								nCountEnter++;
							}
							else
							{
								break;
							}
						}

						//範囲内ならテリトリーの取得
						if (nCountEnter == TRIANGLE_VERTEX)
						{
							GetTerritory(pTerritory);
							break;
						}
					}
				}
			}

			pScene = pSceneNext;	//ポインタを進める
		}
	}

	//----------------------------------------------
	// メモリの破棄
	//----------------------------------------------
	if (pListPos != NULL)
	{//位置のリスト
		delete[] pListPos;
		pListPos = NULL;
	}

	if (pTraiangle != NULL)
	{//三角形の情報
		delete[] pTraiangle;
		pTraiangle = NULL;
	}

	if (pnFarNumTerritory != NULL)
	{//始点から遠い座標順リスト
		delete[] pnFarNumTerritory;
		pnFarNumTerritory = NULL;
	}

	if (pfLength != NULL)
	{//始点との距離
		delete[] pfLength;
		pfLength = NULL;
	}

#if 0
	//------------------------------
	// 半時計回りの判定
	//------------------------------
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
						if (fCross >= 0)
						{
							nCountEnter++;
						}
						else
						{
							break;
						}
					}
				}

				//範囲内ならテリトリーの取得
				if (nCountEnter == m_nCountTerritory)
				{
					GetTerritory(pTerritory);
					bEnter = true;
				}
			}
		}

		pScene = pSceneNext;	//ポインタを進める
	}

	if (bEnter)
	{
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
		return;
	}

	//------------------------------
	// 時計回りの判定
	//------------------------------

	//VecAを求める
	for (int nCntVec = m_nCountTerritory - 1; nCntVec >= 0; nCntVec--)
	{
		int nCnt = nCntVec - 1;
		if (nCntVec == 0) { nCnt = m_nCountTerritory - 1; }
		pVecA[nCntVec] = pListPos[nCnt] - pListPos[nCntVec];
	}

	//テリトリーを取得する
	pSceneTop = CScene::GetTop(TERRITORY_PRIORITY); //オブジェクトポインタの先頭を取得			
	pScene = pSceneTop;							 //トップの位置を保存する
	m_pTerritory = NULL;

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

				for (int nCnt = m_nCountTerritory - 1; nCnt >= 0; nCnt--)
				{
					if (m_apTerritory[nCnt] != NULL)
					{
						D3DXVECTOR3 VecB = TerritoryPos - pListPos[nCnt];

						//外積を求める
						float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pVecA[nCnt].x, pVecA[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

						//ベクトルより大きいかどうか
						if (fCross >= 0)
						{
							nCountEnter++;
						}
						else
						{
							break;
						}
					}
				}

				//範囲内ならテリトリーの取得
				if (nCountEnter == m_nCountTerritory)
				{ 
					GetTerritory(pTerritory); 
					bEnter = true;
				}
			}
		}

		pScene = pSceneNext;	//ポインタを進める
	}

	//メモリの破棄
	if (bEnter)
	{
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
#endif
}

//=============================================================================
// 三角形の構造体の初期化
//=============================================================================
void CPlayer::InitTraiangle(TRAIANGLE * pTraiangle, int nMaxTraiangle)
{
	for (int nCnt = 0; nCnt < nMaxTraiangle; nCnt++)
	{
		for (int nCntVertex = 0; nCntVertex < TRIANGLE_VERTEX; nCntVertex++)
		{
			pTraiangle[nCnt].pos[nCntVertex] = INITIALIZE_VECTOR3;
			pTraiangle[nCnt].VecA[nCntVertex] = INITIALIZE_VECTOR3;
		}
	}
}

//=============================================================================
// 三角形を作る処理
//=============================================================================
void CPlayer::MakeTraiangle(int nCnt, int * nCountTraiangle, int * pnFarNumTerritory, D3DXVECTOR3 * pListPos, TRAIANGLE * pTraiangle)
{
	//始点から遠い頂点とその両隣の座標番号を保存
	int nVertex = pnFarNumTerritory[nCnt];
	int nVertex2 = pnFarNumTerritory[nCnt] - 1;
	int nVertex3 = pnFarNumTerritory[nCnt] + 1;

	//値の修正
	if (nVertex == 0) { nVertex2 = m_nCountTerritory - 1; }
	if (nVertex == m_nCountTerritory - (1 + *nCountTraiangle)) { nVertex3 = 0; }

	//座標の保存
	pTraiangle[*nCountTraiangle].pos[0] = pListPos[nVertex2];	
	pTraiangle[*nCountTraiangle].pos[1] = pListPos[nVertex];	
	pTraiangle[*nCountTraiangle].pos[2] = pListPos[nVertex3];	

	//VecAの作成
	for (int nCntVec = 0; nCntVec < TRIANGLE_VERTEX; nCntVec++)
	{
		int nCntVex2 = nCntVec + 1;
		if ((nCntVex2 % TRIANGLE_VERTEX) == 0) { nCntVex2 = 0; }

		pTraiangle[*nCountTraiangle].VecA[nCntVec] = pTraiangle[*nCountTraiangle].pos[nCntVex2] - pTraiangle[*nCountTraiangle].pos[nCntVec];
	}

	//三角形の中にリストの座標があるかどうか判定する
	bool bNoEnter = false;
	for (int nCntList = 0; nCntList < m_nCountTerritory - *nCountTraiangle; nCntList++)
	{
		if (pListPos[nCntList] == FAR_POS) { break; }
		int nCountEnter = 0;
		for (int nCntVec = 0; nCntVec < TRIANGLE_VERTEX; nCntVec++)
		{
			D3DXVECTOR3 VecB = pListPos[nCntList] - pTraiangle[*nCountTraiangle].VecA[nCntVec];

			//外積を求める
			float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pTraiangle[*nCountTraiangle].VecA[nCntVec].x, pTraiangle[*nCountTraiangle].VecA[nCntVec].z), &D3DXVECTOR2(VecB.x, VecB.z));

			//ベクトルより大きいかどうか
			if (fCross >= 0) 
			{ 
				nCountEnter++; 
			}
			else 
			{ 
				break;
			}
		}

		if (nCountEnter == TRIANGLE_VERTEX)
		{//三角形内に頂点があるときは隣に頂点をずらしもう一度判定をする
			MakeTraiangle(nVertex3, nCountTraiangle, pnFarNumTerritory, pListPos, pTraiangle);	//三角形を作る
			bNoEnter = false;
		}
		else
		{
			bNoEnter = true;
		}
	}

	//使用した座標をリストから除外する
	if (bNoEnter)
	{
		//位置情報のリストを詰める
		for (int nCntPos = nVertex; nCntPos < m_nCountTerritory - 1; nCntPos++)
		{
			D3DXVECTOR3 Pos;
			int nCntSide = nCntPos + 1;

			//リストの入れ替え
			if (nCntSide != m_nCountTerritory) 
			{ 
				Pos = pListPos[nCntSide]; 
				pListPos[nCntSide] = FAR_POS;
			}

			pListPos[nCntPos] = Pos;
		}
		if (nVertex == m_nCountTerritory - 1) { pListPos[nVertex] = FAR_POS; }

		//最も遠い頂点座標リストを詰める
		for (int nCntPos = nCnt; nCntPos < m_nCountTerritory - (2 + *nCountTraiangle); nCntPos++)
		{
			int nCntSide = nCntPos + 1;
			int nSub;
			if (nCntSide != m_nCountTerritory - 1) 
			{ 
				nSub = pnFarNumTerritory[nCntSide];
				if (nSub > nVertex) { nSub -= 1; }		//値の修正

				pnFarNumTerritory[nCntSide] = pnFarNumTerritory[0];
			}

			pnFarNumTerritory[nCntPos] = nSub;
		}

		//使用しないリストに0番目の番号を入れる
		int FirstVale = m_nCountTerritory - (2 + *nCountTraiangle);
		for (int nCntPos = FirstVale; nCntPos <= m_nCountTerritory - 2; nCntPos++)
		{
			pnFarNumTerritory[nCntPos] = pnFarNumTerritory[0];
		}

		//ポインタ内の数値を加算
		*nCountTraiangle += 1;
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

	//エフェクトの生成
	pTerritory->CreateGetEffect();
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
			if ((m_apTerritory[nCountTerritory] != NULL) && (m_apTerritory[TERRITORY_TOP] != NULL))
			{
				m_apLine[nCnt] = CLine::Create(m_apTerritory[nCountTerritory]->GetPos(), m_apTerritory[TERRITORY_TOP]->GetPos(), m_nNumPlayer, nCnt);
			}
			break;

		}
	}

	//生成したラインをコピーする
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apCopyLine[nCnt] = m_apLine[nCnt];
			m_apCopyLine[nCnt]->SetCompleteShape(true);
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
		m_nCountTerritory--;
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