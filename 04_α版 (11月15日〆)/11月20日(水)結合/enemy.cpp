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
#include "library.h"
#include "territory.h"
#include "line.h"
#include "mine.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//静的メンバ変数宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CSceneX *CEnemy::m_apSceneX = NULL;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED (1.0f)
#define MODEL_RENG (50)
#define MAX_BUNDCNT (120)
#define MAX_BULLET (50)

//国の色の設定
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//赤
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//青
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//緑
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//白
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//黒
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//黄
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//オレンジ
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//紫

//	当たり判定・ダメージ関連
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)


//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_move = INITIALIZE_VECTOR3;
	m_Angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nDamageCount = 0;
	m_fBlowAngle = 0.0f;
	m_fBlowLength = 0.0f;
	m_rot = INITIALIZE_VECTOR3;
	m_posOld = INITIALIZE_VECTOR3;
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
HRESULT CEnemy::Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CCharacter::Init(nNum, ModelTxt, MotionTxt, m_CharcterType);
	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);

	m_nLineNum = 2;	//	最低限のラインを引いたら始点に戻る(拠点を2つ繋いだら始点に戻る、始点に戻ってきたらラインは3つになりポイント加算の条件を満たせる)
	InitSort(pos);	//	ゲーム開始時の近場のエリア・テリトリーを見つける

					//ライン変数の初期化
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;
	m_bCheckS = false;

	//コピーラインの初期化
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	ResetLine();	//ラインの初期化

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
	//	各エリアのテリトリー情報の解放
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		if (m_AreaInfo[nCnt] != NULL)
		{
			delete[] m_AreaInfo[nCnt];
			m_AreaInfo[nCnt] = NULL;
		}
	}

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

	//ゲームの状態を取得
	int nGameState = CGame::GetGameState();
	float aaa = m_fSpeed;
	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		Move();	//	エネミー移動処理
	}

	//	ラインのカウントダウン処理
	CountDownLineTime();

	//	ライン引きの開始(カウントアップ)
	if (m_bBlockStartTerritory)
	{
		m_nCountTime++;
		if ((m_nCountTime % NOT_GETTIME) == 0) { m_bBlockStartTerritory = false; }
	}

	//	図形が完成した後の処理
	UpdateShapeComplete();

	//	現在の状態
	switch (m_state)
	{
	case STATE_NONE://	通常
					//	歩行可能
		m_bWalk = true;
		m_bSprintMotion = true;
		break;

	case STATE_WALK://	移動状態
					//m_bSprintMotion = true;

		break;

	case STATE_ACTION: //アクション状態
					   //m_bWalk = true;

		break;

	case STATE_BLOWAWAY:	//吹っ飛ばされてる状態
							// ダメージカウント加算
		m_nDamageCount++;

		if (m_nDamageCount >= MAX_DAMAGECOUNT)
		{
			m_nDamageCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;

			//if (m_bSprintMotion == true)
			{
				m_state = STATE_NONE;
				m_pMotion->SetNumMotion(m_state);
			}

		}

		// 吹っ飛ばす
		m_fBlowLength -= BLOW_MOVING_CUT;
		if (m_fBlowLength >= 0.0f)
		{
			D3DXVECTOR3 pos = CCharacter::GetPos();
			pos.x -= sinf(m_fBlowAngle) * m_fBlowLength;
			pos.z -= cosf(m_fBlowAngle) * m_fBlowLength;
			CCharacter::SetPos(pos);
		}

		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CEnemy::Draw(void)
{
	CCharacter::Draw();
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	CManager::GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CEnemy *CEnemy::Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],CHARCTERTYPE charatype)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			pEnemy->m_CharcterType = charatype;
			pEnemy->SetType(type);
			pEnemy->Init(nNum, pos, ModelTxt, MotionTxt);
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
//	初期距離ソート処理
//=============================================================================
void CEnemy::InitSort(D3DXVECTOR3 pos)
{
	for (int nCntArea = 0; nCntArea < AREA_MAX; nCntArea++)
	{
		m_nAreaTerrNum[nCntArea] = 0;
	}

	for (int nCntTe = 0; nCntTe < 10; nCntTe++)
	{
		//m_apTerritory[nCntTe] = NULL;
	}
	//	テリトリー情報の取得 / 確保
	m_nMax = 0;
	int nCnt01 = 0;
	CScene* pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);
	CScene* pScene = pSceneTop;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			m_pTerritory = (CTerritory*)pScene;

			if (m_nMax == 0)//	テリトリーの最大数分newする
			{
				m_nMax = m_pTerritory->GetMaxObj();
				m_TerritoryInfo = new CEnemy::TERRITORY_INFO[m_nMax];
			}

			//	各テリトリーの情報を取得
			m_TerritoryInfo[nCnt01].pos = m_pTerritory->GetPos();			//	位置情報
			m_TerritoryInfo[nCnt01].nAreaNum = m_pTerritory->GetErea();		//	割り振られたエリア番号

																			//	各エリア事のテリトリーの数をカウントアップ[エリアの番号]
			m_nAreaTerrNum[m_TerritoryInfo[nCnt01].nAreaNum] += 1;

			nCnt01 += 1;
		}
		pScene = pSceneNext;
	}

	//	エリアごとの情報が欲しいのでエリアの数分newする
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		m_AreaInfo[nCnt] = new CEnemy::TERRITORY_INFO[m_nAreaTerrNum[nCnt]];
	}

	//	エリアごとにテリトリーの情報を振り分ける
	for (int nCntA = 0; nCntA < AREA_MAX; nCntA++)
	{
		int nTerrCnt = 0;		//	テリトリー番号
		int nAllTerrCnt = 0;	//	全てのテリトリーを見るため

		while (nAllTerrCnt != m_nMax)// 全てのテリトリーをエリア分けするまで続く
		{
			if (nCntA == m_TerritoryInfo[nAllTerrCnt].nAreaNum)
			{
				m_AreaInfo[nCntA][nTerrCnt].pos = m_TerritoryInfo[nAllTerrCnt].pos;
				nTerrCnt += 1;
			}
			nAllTerrCnt += 1;
		}
	}

	//	各エリアのテリトリーのフラグを初期化
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nTerrCnt = 0; nTerrCnt < m_nAreaTerrNum[nAreaCnt]; nTerrCnt++)	//	エリアごとのテリトリー数分回す
		{
			m_AreaInfo[nAreaCnt][nTerrCnt].bFlag = false;
		}
	}

	//	ループ解除用
	m_bBreak = false;

	//	テリトリー通過数記録 / 現在のライン数
	m_nTargetCnt = 0;
	m_bFinish = false;

	//	エネミーとテリトリー間の距離を計算
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nTerrCnt = 0; nTerrCnt < m_nAreaTerrNum[nAreaCnt]; nTerrCnt++)	//	エリアごとのテリトリー数分回す
		{
			m_AreaInfo[nAreaCnt][nTerrCnt].fDistance = CAIController::dist(m_AreaInfo[nAreaCnt][nTerrCnt].pos, pos);
			m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort = CAIController::dist(m_AreaInfo[nAreaCnt][nTerrCnt].pos, pos);
		}
	}

	//	距離を短い順にソート
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		//	ここからソート処理
		for (int nTerrCnt = 0; nTerrCnt < m_nAreaTerrNum[nAreaCnt]; nTerrCnt++)	//	エリアごとのテリトリー数分回す
		{
			for (int nTerrCnt02 = 0; nTerrCnt02 < m_nAreaTerrNum[nAreaCnt]; nTerrCnt02++)
			{
				if (m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort < m_AreaInfo[nAreaCnt][nTerrCnt02].fDisSort)
				{
					float fDisSort = m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort;
					m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort = m_AreaInfo[nAreaCnt][nTerrCnt02].fDisSort;
					m_AreaInfo[nAreaCnt][nTerrCnt02].fDisSort = fDisSort;
				}
			}
		}
	}

	//	ゲーム開始の際に、どこのエリアが一番近いかを決める(ソート処理)
	float fSort = 0.0f;
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nAreaCnt02 = 0; nAreaCnt02 < AREA_MAX; nAreaCnt02++)
		{
			if (m_AreaInfo[nAreaCnt][0].fDisSort < m_AreaInfo[nAreaCnt02][0].fDisSort)
			{
				fSort = m_AreaInfo[nAreaCnt][0].fDisSort;
				m_AreaInfo[nAreaCnt][0].fDisSort = m_AreaInfo[nAreaCnt02][0].fDisSort;
				m_AreaInfo[nAreaCnt02][0].fDisSort = fSort;
			}
		}
	}

	//	一番近いエリア、テリトリーが決まる
	int nAreaCnt = 0;
	while (m_bBreak != true)
	{
		for (int nInitTarget = 0; nInitTarget < m_nAreaTerrNum[nAreaCnt]; nInitTarget++)//	エリアごとのテリトリー数分回す
		{
			if (m_AreaInfo[0][0].fDisSort == m_AreaInfo[nAreaCnt][nInitTarget].fDistance)
			{
				m_nTargetNum = nInitTarget;
				m_nAreaNow = nAreaCnt;	//	一番最初に所属するエリアが決まる
				m_bBreak = true;		//	決まった瞬間ループを抜ける
				break;
			}
		}
		nAreaCnt += 1;
	}
}
//=============================================================================
//　プレイヤーの移動処理
//=============================================================================
void  CEnemy::Move(void)
{

	D3DXVECTOR3 pos = CCharacter::GetPos();
	m_posOld = pos;				//	ワンフレーム前の位置
	float fSpeed = GetSpeed();	//	速さ

	if (m_bFinish == true)	//!	[[始点への移動処理 / その後、進む拠点を決める]]
	{
		//	★角度計算
		m_nTerrStart.fRadian = (float)atan2(m_nTerrStart.pos.z - pos.z, m_nTerrStart.pos.x - pos.x);

		//	★目的地範囲内に入った
		if (pos.x <= m_nTerrStart.pos.x + 25.0f && pos.x >= m_nTerrStart.pos.x - 25.0f &&
			pos.z <= m_nTerrStart.pos.z + 25.0f && pos.z >= m_nTerrStart.pos.z - 25.0f)
		{
			//	距離の再計算
			DisSort(pos);

			//	ライン数の初期化 / 新たな始点・終点を決める
			m_bFinish = false;
			m_nTerrStart.pos = m_AreaInfo[m_nAreaNow][m_nTargetNum].pos;
		}

		MotionAction();	//	モーションアクション(キャラ同士のぶつかり等)

						//	移動加算処理
		if (m_state == STATE_WALK || m_state == STATE_ACTION)
		{
			pos.x += (float)cos(m_nTerrStart.fRadian) * (fSpeed * m_fSpeed);
			pos.z += (float)sin(m_nTerrStart.fRadian) * (fSpeed * m_fSpeed);
		}
		// ★拠点までの角度 / 自身の軸調整
		float fDestAngle = atan2f(pos.x - m_nTerrStart.pos.x, pos.z - m_nTerrStart.pos.z);
		float fAngle = fDestAngle - m_rot.y;

		if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
		if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }

		//	移動方向に角度調整
		m_rot.y += fAngle * 0.1f;

		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }

		//	位置・回転情報の反映
		CCharacter::SetPos(pos);
		CCharacter::SetRot(m_rot);
	}
	else //! [[始点を除く拠点の計算]]
	{
		//	★角度計算
		for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
		{
			m_AreaInfo[m_nAreaNow][nCnt].fRadian = (float)atan2(m_AreaInfo[m_nAreaNow][nCnt].pos.z - pos.z, m_AreaInfo[m_nAreaNow][nCnt].pos.x - pos.x);
		}

		//	★目的地範囲内に入った
		if (pos.x <= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.x + 25.0f && pos.x >= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.x - 25.0f &&
			pos.z <= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.z + 25.0f && pos.z >= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.z - 25.0f)
		{
			//	[[最新の始点・終点に更新]]
			if (m_apTerritory[0] != NULL)
			{
				m_nTerrStart.pos = m_apTerritory[0]->GetPos();
			}
	
			//	[[ラインを?本引いたら始点に戻る]]
			if (m_apTerritory[m_nLineNum] != NULL)
			{
				if (m_nLineTime <= (12 - m_nCreateTime) * 100)	// 制限時間が迫ってきたら
				{
					m_nCreateTime = (rand() % 4);
					m_nLineNum = 2;
					m_bFinish = true;	//	始点に戻す
				}
				else//	ラインを伸ばす時間に余裕があれば
				{
					m_nLineNum += 1;
				}

			}
			//	[[フラグを立てる]]
			m_AreaInfo[m_nAreaNow][m_nTargetNum].bFlag = true;
			//	[[通過記録カウントアップ]]
			m_nTargetCnt += 1;

			if (m_nTargetCnt == m_nAreaTerrNum[m_nAreaNow])	//	通過回数もフラグもリセット
			{
				m_nTargetCnt = 0;

				for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
				{
					m_AreaInfo[m_nAreaNow][nCnt].bFlag = false;

				}
				//	このif文を何回か通ったら次のエリアに移動させよう
				//int 〇 = 0;
				//〇++;
				//if (〇 == 3)
				//{

				//}
				m_nAreaNow += 1;
				if (m_nAreaNow == AREA_MAX)
				{
					m_nAreaNow = 0;
				}
			}

			DisSort(pos);	//	距離の再計算
		}

		MotionAction();	//	モーションアクション(キャラ同士のぶつかり等)

						//	移動加算処理
		if (m_state == STATE_WALK || m_state == STATE_ACTION)
		{
			pos.x += (float)cos(m_AreaInfo[m_nAreaNow][m_nTargetNum].fRadian) * (fSpeed * m_fSpeed);
			pos.z += (float)sin(m_AreaInfo[m_nAreaNow][m_nTargetNum].fRadian) * (fSpeed * m_fSpeed);
		}
		// ★拠点までの角度 / 自身の軸調整
		float fDestAngle = atan2f(pos.x - m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.x, pos.z - m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.z);
		float fAngle = fDestAngle - m_rot.y;

		if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
		if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }

		//	移動方向に角度調整
		m_rot.y += fAngle * 0.1f;

		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }

		//	位置・回転情報の反映
		CCharacter::SetPos(pos);
		CCharacter::SetRot(m_rot);
	}
}

//=============================================================================
//　距離間ソート処理
//=============================================================================
void CEnemy::DisSort(D3DXVECTOR3 pos)
{
	// [[現地点から近い距離を探す]]
	m_bBreak = false;
	int nNextNum = 0;//	次の近いターゲット拠点番号(0は現在自分がいる場所)

					 //	[[再度距離を計算]]
	for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
	{
		m_AreaInfo[m_nAreaNow][nCnt].fDistance = CAIController::dist(m_AreaInfo[m_nAreaNow][nCnt].pos, pos);
		m_AreaInfo[m_nAreaNow][nCnt].fDisSort = CAIController::dist(m_AreaInfo[m_nAreaNow][nCnt].pos, pos);

	}

	//	[[短い距離順に変える]]
	for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nAreaTerrNum[m_nAreaNow]; nCnt2++)
		{
			if (m_AreaInfo[m_nAreaNow][nCnt].fDisSort < m_AreaInfo[m_nAreaNow][nCnt2].fDisSort)
			{
				float fDisSort = m_AreaInfo[m_nAreaNow][nCnt].fDisSort;
				m_AreaInfo[m_nAreaNow][nCnt].fDisSort = m_AreaInfo[m_nAreaNow][nCnt2].fDisSort;
				m_AreaInfo[m_nAreaNow][nCnt2].fDisSort = fDisSort;
			}
		}
	}

	while (m_bBreak != true)
	{
		for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
		{
			if (m_AreaInfo[m_nAreaNow][nCnt].bFlag == false)
			{
				if (m_AreaInfo[m_nAreaNow][nCnt].fDistance == m_AreaInfo[m_nAreaNow][nNextNum].fDisSort)
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

//=============================================================================
//　吹っ飛び処理
//=============================================================================
void CEnemy::BlowAway(D3DXVECTOR3 AnotherPos)
{
	// 座標を取得
	D3DXVECTOR3 pos = GetPos();

	// 吹っ飛ぶ向きを計算
	m_fBlowAngle = CFunctionLib::CalcAngleToDest(pos.x, pos.z, AnotherPos.x, AnotherPos.z);

	// 吹っ飛ぶ距離を設定
	m_fBlowLength = BLOW_MOVING;

	// 吹っ飛び状態にする
	m_state = STATE_BLOWAWAY;
	m_pMotion->SetNumMotion(STATE_BLOWAWAY);

	// 向きを変える
	float fRot = m_fBlowAngle + D3DX_PI;
	fRot = CFunctionLib::CheckRotationOverPiX(fRot);
	m_rot.y = fRot;
	m_Angle = D3DXVECTOR3(0.0f, fRot, 0.0f);
}

//=============================================================================
//　モーションアクション(キャラ同士ぶつかった際)
//=============================================================================
void CEnemy::MotionAction(void)
{
	CSound *pSound = CManager::GetSound();	//	サウンドの取得

											//移動状態
	if (m_state != STATE_BLOWAWAY)
	{
		if (m_state != STATE_ACTION && m_bSprint != true)
		{
			//移動状態
			m_state = STATE_WALK;
		}
		if (m_bSprint == true)
		{
			//移動状態
			m_state = STATE_ACTION;
		}
	}
	if (m_bWalk == true)
	{//移動モーション
		if (m_state == STATE_WALK)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);//足音
			m_pMotion->SetNumMotion(STATE_WALK);
			m_bWalk = false;
		}
	}
	if (m_bSprintMotion == true)
	{//スプリントモーション
		if (m_state == STATE_ACTION)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);//足音
			m_pMotion->SetNumMotion(STATE_ACTION);
			m_bSprintMotion = false;
		}
	}
}

//=============================================================================
// ラインの生成 / 破棄
//=============================================================================
#if 1
void CEnemy::CreateOrbitLine(void)
{
	if (m_pOrbitLine == NULL)
	{
		m_pOrbitLine = CSceneOrbit::Create(CSceneOrbit::TYPE_PLAYER, CCharacter::GetPos());
		m_pOrbitLine->SetMtxParent(&m_pModel[5]->GetMtxWorld());
		m_pOrbitLine->SetMtxParentEnd(&m_pModel[1]->GetMtxWorld());
	}
}

void CEnemy::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}
#endif
//=============================================================================
// 判定関連
//=============================================================================
#if 1
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

//=============================================================================
//　シリンダーコライダーとの当たり判定処理
//=============================================================================
bool CEnemy::CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
			BlowAway(AnotherPos);
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
bool CEnemy::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
{
	if (pShere->Collision(&pos, CYLINDER_COLRADIUS, this) == true && pShere->GetParent() != this)
	{// 自分以外の攻撃球が当たったら
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_MINE)
		{// 地雷だったら
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this)
			{// 自分以外が出した地雷ならダメージ

			}
		}
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER)
		{
			//if (m_bCollision == true)
			{
				m_pMotion->SetNumMotion(4);
			}

		}
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER)
		{
			//m_pMotion->SetNumMotion(3);
		}
		return true;
	}

	return false;
}
#endif