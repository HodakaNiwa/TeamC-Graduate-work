//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					エネミー処理 [enemy.cpp]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#include "enemy.h"
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
#include "AIController.h"
#include "territory.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//静的メンバ変数宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CSceneX *CEnemy::m_apSceneX = NULL;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED (4.0f)
#define MODEL_RENG (50)
#define MAX_BUNDCNT (120)
#define MAX_BULLET (50)

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEnemy::CEnemy()
{
	m_Angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CEnemy::~CEnemy()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CCharacter::Init(ModelTxt, MotionTxt);
	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);

	//	テリトリー情報の取得 / 確保
	m_nMax = 0;
	int nNum = 0;
	CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);
	CScene * pScene = pSceneTop;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			m_pTerritory = (CTerritory*)pScene;
			if (m_nMax == 0)
			{
				//	テリトリー最大数分確保
				m_nMax = m_pTerritory->GetMaxObj();
				m_TerritoryInfo = new CEnemy::TERRITORY_INFO[m_nMax];
			}
			//	各テリトリー情報の位置を記憶 / 次のテリトリー番号へ
			m_TerritoryInfo[nNum].pos = m_pTerritory->GetPos();
			nNum += 1;
		}
		pScene = pSceneNext;
	}
	
	//	各テリトリーのフラグを初期化
	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		m_TerritoryInfo[nCnt].bFlag = false;
	}

	//	ループ解除用
	m_bBreak = false;
	//	テリトリー通過数記録
	m_nTargetCnt = 0;

	//	エネミーとテリトリー間の距離を計算
	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		m_TerritoryInfo[nCnt].fDistance = CAIController::dist(m_TerritoryInfo[nCnt].pos, m_beforePos);
		m_TerritoryInfo[nCnt].fChange = CAIController::dist(m_TerritoryInfo[nCnt].pos, m_beforePos);
	}

	//	距離を短い順にソート
	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		for (int nCnt02 = 0; nCnt02 < m_nMax; nCnt02++)
		{
			if (m_TerritoryInfo[nCnt].fChange < m_TerritoryInfo[nCnt02].fChange)
			{
				float fChange = m_TerritoryInfo[nCnt].fChange;
				m_TerritoryInfo[nCnt].fChange = m_TerritoryInfo[nCnt02].fChange;
				m_TerritoryInfo[nCnt02].fChange = fChange;
			}
		}
	}
	//	一番短い距離を算出
	while (m_bBreak != true)
	{
		for (int nCnt = 0; nCnt < m_nMax; nCnt++)
		{
			for (int nInitTarget = 0; nInitTarget < m_nMax; nInitTarget++)
			{					//	一番短い距離 == 同じ距離間の拠点番号
				if (m_TerritoryInfo[nCnt].fChange == m_TerritoryInfo[nInitTarget].fDistance)
				{
					m_nTargetNum = nInitTarget;
					m_bBreak = true;
					break;
				}
			}
			break;
		}
	}
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CEnemy::Uninit(void)
{
	//	テリトリー情報の解放
	if (m_TerritoryInfo != NULL)
	{
		delete[] m_TerritoryInfo;
		m_TerritoryInfo = NULL;
	}

	CCharacter::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void  CEnemy::Update(void)
{
	CCharacter::Update();

	Move();	//	エネミー移動処理
}

//=============================================================================
// 描画処理
//=============================================================================
void  CEnemy::Draw(void)
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
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		//メモリを動的確保
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{//NULLチェック

		 //初期化処理
			pEnemy->Init(pos, ModelTxt, MotionTxt);
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
	return pEnemy;
}

//=============================================================================
//
//=============================================================================
void  CEnemy::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//　プレイヤーの移動処理
//=============================================================================
void  CEnemy::Move(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPos();
	m_posOld = pos;

	
	//!	---<<AI関連>---

	m_nAreaNum = m_pTerritory->RequestErea(pos);

	//	★角度計算
	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		m_TerritoryInfo[nCnt].fRadian = (float)atan2(m_TerritoryInfo[nCnt].pos.z - pos.z, m_TerritoryInfo[nCnt].pos.x - pos.x);
	}
	//	★次の座標の予測値を計算する
	D3DXVECTOR3 nextPos;
	nextPos.x = (float)cos(m_TerritoryInfo[m_nTargetNum].fRadian) * SPEED + pos.x;
	nextPos.z = (float)sin(m_TerritoryInfo[m_nTargetNum].fRadian) * SPEED + pos.z;

	//	★目的地範囲内に入った
	if (pos.x <= m_TerritoryInfo[m_nTargetNum].pos.x + 35.0f && pos.x >= m_TerritoryInfo[m_nTargetNum].pos.x - 35.0f &&
		pos.z <= m_TerritoryInfo[m_nTargetNum].pos.z + 35.0f && pos.z >= m_TerritoryInfo[m_nTargetNum].pos.z - 35.0f)
	{
		//	[[フラグを立てる]]
		m_TerritoryInfo[m_nTargetNum].bFlag = true;
		//	[[通過記録カウントアップ]]
		m_nTargetCnt += 1;
		//	[[再度距離を計算]]
		for (int nCnt = 0; nCnt < m_nMax; nCnt++)
		{
			m_TerritoryInfo[nCnt].fDistance = CAIController::dist(m_TerritoryInfo[nCnt].pos, pos);
			m_TerritoryInfo[nCnt].fChange = CAIController::dist(m_TerritoryInfo[nCnt].pos, pos);
		}
		//	[[短い距離順に変える]]
		for (int nCnt = 0; nCnt < m_nMax; nCnt++)
		{
			for (int nCnt2 = 0; nCnt2 < m_nMax; nCnt2++)
			{
				if (m_TerritoryInfo[nCnt].fChange < m_TerritoryInfo[nCnt2].fChange)
				{
					float fChange = m_TerritoryInfo[nCnt].fChange;
					m_TerritoryInfo[nCnt].fChange = m_TerritoryInfo[nCnt2].fChange;
					m_TerritoryInfo[nCnt2].fChange = fChange;
				}
			}
		}

		// [[現地点から近い距離を探す]]
		m_bBreak = false;
		int nNextNum = 1;//	次の近いターゲット拠点番号(0は現在自分がいる場所)

		if (m_nTargetCnt == m_nMax)	//	通過回数もフラグもリセット
		{
			m_nTargetCnt = 0;
			for (int nCnt = 0; nCnt < m_nMax; nCnt++)
			{
				m_TerritoryInfo[nCnt].bFlag = false;
			}
		}

		while (m_bBreak != true)
		{
			for (int nCnt = 0; nCnt < m_nMax; nCnt++)
			{
				if (m_TerritoryInfo[nCnt].bFlag == false)
				{
					if (m_TerritoryInfo[nCnt].fDistance == m_TerritoryInfo[nNextNum].fChange)
					{
						m_nTargetNum = nCnt;
						m_bBreak = true;
						break;
					}
				}
			}
			nNextNum += 1;	//	すでにフラグが立っている場合は次の近場へ
		}
	}

	//m_beforePos.x = pos.x;
	//m_beforePos.z = pos.z;

	if (pos.x > nextPos.x) { pos.x -= 4.0f; }
	else if (pos.x < nextPos.x) { pos.x += 4.0f; }
	if (pos.z > nextPos.z) { pos.z -= 4.0f; }
	else if (pos.z < nextPos.z) { pos.z += 4.0f; }

	// ★拠点までの角度 / 自身の軸調整
	float fDestAngle = atan2f(pos.x - m_TerritoryInfo[m_nTargetNum].pos.x, pos.z - m_TerritoryInfo[m_nTargetNum].pos.z);
	float fAngle = fDestAngle - m_rot.y;


	if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
	if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }

	if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }

	//	移動方向に角度調整
	m_rot.y += fAngle * 0.1f;

	//	位置・回転情報の反映
	CCharacter::SetPos(pos);
	CCharacter::SetRot(m_rot);

}


//=============================================================================
//　エネミーとテリトリーの当たり判定
//=============================================================================
void CEnemy::Collision(void)
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
bool CEnemy::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
bool CEnemy::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
