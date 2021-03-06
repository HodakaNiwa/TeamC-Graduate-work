//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//				エネミー処理 [enemy.cpp]
//		Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"
#include "library.h"
#include "collision.h"
#include "line.h"
#include "AIController.h"
#include "model.h"
#include "territory.h"
#include "mine.h"
#include "bullet.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define SPEED (1.0f)

//当たり判定・ダメージ関連
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_ENEMYMOVING   (12.0f)	
#define BLOW_ROBOTMOVING   (30.0f)	
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)

//デバック用
#define FLAG	(false)

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_nDamageCount = 0;
	m_nDamageCounter = 0;
	m_nLevel = 0;
	m_nDownCount = 0;
	m_nRandNum = 0;
	m_nCountTime = 0;
	m_nTargetNum = 0;
	m_nPassCnt = 0;
	m_nAreaNow = 0;
	m_nRandNum = 0;
	m_nLineNum = 2;
	m_nEnemyNo = -1;
	m_fBlowAngle = 0.0f;
	m_fBlowLength = 0.0f;
	m_fSpeed = 0.0f;
	m_rot = INITIALIZE_VECTOR3;
	m_posOld = INITIALIZE_VECTOR3;
	m_bSuperArmor = false;
	m_bTarget = false;
	m_EventCheck = false;
	m_bReset = false;
	m_bDebugStop = false;
	m_EveCheckDivi = false;
	m_nRandStop = false;
	m_bWalk = false;
	m_bSprintMotion = false;
	m_bStop = false;
	m_bFinish = false;
	m_bBreak = false;
	m_pModel = NULL;
	m_pMotion = NULL;
	m_pLoadEffect = NULL;
	m_pTerritory = NULL;
	m_state = STATE_NONE;

	//視点終点の記憶
	m_nTerrStart.bFlag = false;
	m_nTerrStart.fDisSort = 0.0f;
	m_nTerrStart.fDistance = 0.0f;
	m_nTerrStart.fRadian = 0.0f;
	m_nTerrStart.nAreaNum = 0;
	m_nTerrStart.pos = INITIALIZE_VECTOR3;

	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		m_AreaInfo[nCnt] = NULL;
		m_nAreaTerrNum[nCnt] = 0;
		m_tmp[nCnt] = NULL;
	}
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CEnemy::~CEnemy() { }
//=============================================================================
//生成処理
//=============================================================================
CEnemy *CEnemy::Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charatype)
{
	return NULL;	//←使用するなら消す
}

//=============================================================================
//初期化処理	＊各派生クラスの初期化処理を使っている
//=============================================================================
HRESULT CEnemy::Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nType)
{ return S_OK; }

HRESULT CEnemy::Init(void)
{ return S_OK; }

//=============================================================================
//終了処理
//=============================================================================
void  CEnemy::Uninit(void)
{
	//マージソートの解放
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		if (m_tmp[nCnt] != NULL)
		{
			delete[] m_tmp[nCnt];
			m_tmp[nCnt] = NULL;
		}
	}
	//各エリアのテリトリー情報の解放
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		if (m_AreaInfo[nCnt] != NULL)
		{
			delete[] m_AreaInfo[nCnt];
			m_AreaInfo[nCnt] = NULL;
		}
	}

	//個々のテリトリー情報の解放
	if (m_TerritoryInfo != NULL)
	{
		delete[] m_TerritoryInfo;
		m_TerritoryInfo = NULL;
	}
	
	CCharacter::Uninit();
}

//=============================================================================
//更新処理
//=============================================================================
void  CEnemy::Update(void)
{
	CGame *pGame = CManager::GetGame();			
	CEventCamera *pEveCam = pGame->GetEveCam();
	if (pEveCam == NULL)
	{
		CCharacter::Update();

		int nGameState = CGame::GetGameState();
		//---[[ゲームの開始]]---
		if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
		{
			AIBasicAction();//AI共通処理
		}
		//---[[ゲームの終了]]---
		else if (nGameState == CGame::GAMESTATE_END)
		{
			if (m_bCharaMotionState == false)
			{
				m_state = STATE_NONE;
				m_pMotion->SetNumMotion(m_state);
				m_bCharaMotionState = true;
			}
		}

		Program_Line();		//ライン関連
		Program_State();	//モーション関連

		SetCharaState(m_state);//現在のステータスを反映

		D3DXVECTOR3 pos = GetPos();
		if (pos.x >= 1400.0f || pos.x <= -1400.0f || pos.z >= 1600.0f || pos.z <= -1200.0f)
		{
			pos.x = 0.0f;
			pos.z = 0.0f;
			SetPos(pos);
			CDebugProc::Print("\nげろ\n");

		}
	}
	/*CDebugProc::Print("\nタイプ : %d", m_CharcterType);
	CDebugProc::Print("\nステート : %d", m_state);
	CDebugProc::Print("\n現在のモーション : %d", m_pMotion->GetNumMotion());
	CDebugProc::Print("\nぶっ飛びダメージ : %d", m_nDamageCount);
	CDebugProc::Print("\nダメージダウン : %d\n", m_nDownCount);*/
	CDebugProc::Print("\n現在の位置 : %.1f %.1f %.1f\n", GetPos().x, GetPos().y, GetPos().z);
	CDebugProc::Print("\nm_bFinish : %d\n", m_bFinish);
}

//=============================================================================
//描画処理
//=============================================================================
void  CEnemy::Draw(void)
{
	CGame *pGame = CManager::GetGame();			
	CEventCamera *pEveCam = pGame->GetEveCam();

	if (pEveCam == NULL)
	{
		CCharacter::Draw();
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);
		CManager::GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxWorld);
	}
}
//=============================================================================
//デフォルト関数
//=============================================================================
void  CEnemy::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) { }

//=============================================================================
//初期距離ソート処理
//=============================================================================
void CEnemy::InitSort(D3DXVECTOR3 pos)
{
	for (int nCntArea = 0; nCntArea < AREA_MAX; nCntArea++)
	{
		m_nAreaTerrNum[nCntArea] = 0;
		m_AreaInfo[nCntArea] = NULL;
		m_tmp[nCntArea] = NULL;
	}
	
	//++ ===[[テリトリー情報の取得 / 確保]]===  
	int nMax = 0;		//テリトリーの最大数が入る
	int nCnt01 = 0;		//個々の拠点を見るのに必要

	CScene* pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);
	CScene* pScene = pSceneTop;
	while (pScene != NULL)
	{
		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())//テリトリーかを判別
		{
			m_pTerritory = (CTerritory*)pScene;

			if (nMax == 0)
			{
				nMax = m_pTerritory->GetMaxObj();
				m_TerritoryInfo = new CEnemy::TERRITORY_INFO[nMax];//テリトリーの最大数分newする
			}

			//各テリトリーの情報を取得
			m_TerritoryInfo[nCnt01].pos = m_pTerritory->GetPos();			//位置情報
			m_TerritoryInfo[nCnt01].nAreaNum = m_pTerritory->GetErea();		//割り振られたエリア番号

			//各エリアのテリトリー数をカウントアップ
			m_nAreaTerrNum[m_TerritoryInfo[nCnt01].nAreaNum] += 1;

			nCnt01 += 1;//次のテリトリーを見る
		}

		pScene = pScene->GetpNext();
	}

	//エリアごとの拠点数newする
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		m_AreaInfo[nCnt] = new CEnemy::TERRITORY_INFO[m_nAreaTerrNum[nCnt]];
	}

	//エリアごとにテリトリーの情報を分ける
	for (int nCntA = 0; nCntA < AREA_MAX; nCntA++)
	{
		int nTerrCnt = 0;		//テリトリー番号
		int nAllTerrCnt = 0;	//全てのテリトリーを見るため

		while (nAllTerrCnt != nMax)//全てのテリトリーをエリア分けするまで続く
		{
			if (nCntA == m_TerritoryInfo[nAllTerrCnt].nAreaNum)
			{
				m_AreaInfo[nCntA][nTerrCnt].pos = m_TerritoryInfo[nAllTerrCnt].pos;
				nTerrCnt += 1;
			}
			nAllTerrCnt += 1;
		}
	}

	//各エリアのテリトリーのフラグを初期化
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nTerrCnt = 0; nTerrCnt < m_nAreaTerrNum[nAreaCnt]; nTerrCnt++)	//エリアごとのテリトリー数分回す
		{
			m_AreaInfo[nAreaCnt][nTerrCnt].bFlag = false;
		}
	}

	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)	//エリアごとのテリトリー数分回す
	{
		m_tmp[nCnt] = new float[m_nAreaTerrNum[nCnt]];
	}

	//ループ解除用
	m_bBreak = false;

	//テリトリー通過数記録 / 現在のライン数
	m_nPassCnt = 0;
	m_bFinish = false;

	//エネミーとテリトリー間の距離を計算
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nTerrCnt = 0; nTerrCnt < m_nAreaTerrNum[nAreaCnt]; nTerrCnt++)	//エリアごとのテリトリー数分回す
		{
			m_AreaInfo[nAreaCnt][nTerrCnt].fDistance = CAIController::dist(m_AreaInfo[nAreaCnt][nTerrCnt].pos, pos);
			m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort = CAIController::dist(m_AreaInfo[nAreaCnt][nTerrCnt].pos, pos);
		}
	}

	//距離を短い順にマージソート
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		MergeSort(m_AreaInfo[nAreaCnt], 0, m_nAreaTerrNum[nAreaCnt] - 1, nAreaCnt);
		
	}

	//ゲーム開始の際に、どこのエリアが一番近いかを決める(ソート処理)
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nAreaCnt02 = 0; nAreaCnt02 < AREA_MAX; nAreaCnt02++)
		{
			if (m_AreaInfo[nAreaCnt][0].fDisSort < m_AreaInfo[nAreaCnt02][0].fDisSort)
			{
				float fSort = m_AreaInfo[nAreaCnt][0].fDisSort;
				m_AreaInfo[nAreaCnt][0].fDisSort = m_AreaInfo[nAreaCnt02][0].fDisSort;
				m_AreaInfo[nAreaCnt02][0].fDisSort = fSort;
			}
		}
	}

	//一番近いエリア、テリトリーが決まる
	int nAreaCnt = 0;
	while (m_bBreak != true)
	{
		for (int nInitTarget = 0; nInitTarget < m_nAreaTerrNum[nAreaCnt]; nInitTarget++)//エリアごとのテリトリー数分回す
		{
			if (m_AreaInfo[0][0].fDisSort == m_AreaInfo[nAreaCnt][nInitTarget].fDistance)
			{
				m_nTargetNum = nInitTarget;
				m_nAreaNow = nAreaCnt;	//一番最初に所属するエリアが決まる
				m_bBreak = true;		//決まった瞬間ループを抜ける
				break;
			}
		}
		nAreaCnt += 1;
	}
}

//=============================================================================
//　距離間ソート処理
//=============================================================================
void CEnemy::DisSort(D3DXVECTOR3 pos)
{
	//[[現地点から近い距離を探す]]
	m_bBreak = false;
	int nNextNum = 0;//次の近いターゲット拠点番号(0は現在自分がいる場所)

	//[[再度距離を計算]]
	for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
	{
		m_AreaInfo[m_nAreaNow][nCnt].fDistance = CAIController::dist(m_AreaInfo[m_nAreaNow][nCnt].pos, pos);
		m_AreaInfo[m_nAreaNow][nCnt].fDisSort = CAIController::dist(m_AreaInfo[m_nAreaNow][nCnt].pos, pos);
	}

	//[[短い距離順に変える]]
	MergeSort(m_AreaInfo[m_nAreaNow], 0, m_nAreaTerrNum[m_nAreaNow] - 1, m_nAreaNow);


	while (m_bBreak != true)
	{
		for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
		{
			if (m_AreaInfo[m_nAreaNow][nCnt].bFlag == false)
			{
				if (m_AreaInfo[m_nAreaNow][nCnt].fDistance == m_AreaInfo[m_nAreaNow][nNextNum].fDisSort)
				{
					float fDis = m_AreaInfo[m_nAreaNow][nCnt].fDistance;
					m_nTargetNum = nCnt;
					m_bBreak = true;
					//[[次の拠点があまりに遠い場合は始点に戻る]]
					if (m_nLineNum >= 2)
					{
						if (fDis >= 340.0f - (m_nLineNum*20.0f) && m_apTerritory[m_nLineNum] != NULL)
						{
							m_bFinish = true;	//始点に戻す
							m_nLineNum = 2;		//初期最低値に戻す
						}
					}
					break;
				}
			}
		}
		nNextNum += 1;	//すでにフラグが立っている場合は次の近場へ
	}
}

//=============================================================================
//　共通処理
//=============================================================================
void  CEnemy::AIBasicAction(void)
{
	CGame * pGame = CManager::GetGame();
	D3DXVECTOR3 pos = CCharacter::GetPos();
	m_posOld = pos;				//ワンフレーム前の位置


	if (m_nAreaNow != CTerritory::RequestErea(GetPos()))
	{
		Interruption();
	}
	if (pGame->GetDivisionEventFlag() == true && m_EveCheckDivi == false)//分断エリアのイベントが発生している場合...
	{
		m_nPassCnt = 0;
		for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
		{
			m_AreaInfo[m_nAreaNow][nCnt].bFlag = false;

		}
		m_nAreaNow = CTerritory::RequestErea(GetPos());
	}

	if (m_bFinish == true)	//!	[[始点への移動処理 / その後、進む拠点を決める]]
	{
		//★角度計算
		m_nTerrStart.fRadian = (float)atan2(m_nTerrStart.pos.z - pos.z, m_nTerrStart.pos.x - pos.x);

		//★目的地範囲内に入った
		if (pos.x <= m_nTerrStart.pos.x + 25.0f && pos.x >= m_nTerrStart.pos.x - 25.0f &&
			pos.z <= m_nTerrStart.pos.z + 25.0f && pos.z >= m_nTerrStart.pos.z - 25.0f)
		{
			Event_Bonus();

			//[[ライン数の初期化 / 新たな始点・終点を決める]]
			DisSort(pos);//一番短い距離が始点となる
			m_bFinish = false;
			m_nTerrStart.pos = m_AreaInfo[m_nAreaNow][m_nTargetNum].pos;
			m_nLineNum = 2;		//初期最低値に戻す
			m_nRandStop = false;
		}

		Program_Motion();	//モーションアクション(キャラ同士のぶつかり等)
		Program_Move(pos, m_nTerrStart);	//移動処理
	}
	else if (m_bFinish == false) //! [[始点を除く拠点の計算]]
	{
		//★角度計算
		for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
		{
			m_AreaInfo[m_nAreaNow][nCnt].fRadian = (float)atan2(m_AreaInfo[m_nAreaNow][nCnt].pos.z - pos.z, m_AreaInfo[m_nAreaNow][nCnt].pos.x - pos.x);
		}

		//★目的地範囲内に入った
		if (pos.x <= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.x + 25.0f && pos.x >= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.x - 25.0f &&
			pos.z <= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.z + 25.0f && pos.z >= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.z - 25.0f)
		{

			if (pGame != NULL)
			{
				CCharacter *pChara = pGame->GetChara(m_nEnemyNo);
				if (pChara != NULL)
				{
					switch (pChara->GetCharcterType())
					{
					case CCharacter::CHARCTERTYPE_SPEED:

						if (m_state == STATE_ACTION) { LineConnect(((9 + m_nLevel) - (rand() % 4))); }	//アクション時
						else { LineConnect(((7 + m_nLevel) - (rand() % 3))); } //通常時
						break;

					case CCharacter::CHARCTERTYPE_POWER:
						LineConnect((12 - (rand() % 3)));
						break;

					case CCharacter::CHARCTERTYPE_TECHNIQUE:
						LineConnect((12 - (rand() % 3)));
						break;
					}
				}
			}

			//[[フラグを立てる]]
			m_AreaInfo[m_nAreaNow][m_nTargetNum].bFlag = true;
			//[[通過記録カウントアップ]]
			m_nPassCnt += 1;


			//イベント時
			if (pGame->GetDivisionEventFlag() == true)//分断エリアのイベントが発生している場合...
			{
				//発生ごとに更新一回のみ
				if (m_EveCheckDivi == false)
				{
					//InitSort(GetPos());
					Interruption();//引いているラインを一度リセット
				}
				m_EveCheckDivi = true;

				int nType = pGame->GetDivisionType();//分割方向(縦,横)
				switch (nType)
				{//分断の方向によって処理わけ
				case 0://縦
					Event_Division(nType, m_nAreaNow, m_nAreaNow + 2);
					break;
				case 1://横
					Event_Division(nType, m_nAreaNow, m_nAreaNow + 1);
					break;
				}
			}
			else
			{
				m_EveCheckDivi = false;

				//通常時
				if (m_nPassCnt == m_nAreaTerrNum[m_nAreaNow])	//通過回数もフラグもリセット
				{
					m_nPassCnt = 0;

					for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
					{
						m_AreaInfo[m_nAreaNow][nCnt].bFlag = false;

					}

					m_nAreaNow += 1;

					if (m_nAreaNow == AREA_MAX)
					{
						m_nAreaNow = 0;
					}

					for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
					{
						m_AreaInfo[m_nAreaNow][nCnt].fRadian = (float)atan2(m_AreaInfo[m_nAreaNow][nCnt].pos.z - pos.z, m_AreaInfo[m_nAreaNow][nCnt].pos.x - pos.x);
					}
				}
			}

			DisSort(pos);	//距離の再計算

		}

		Program_Motion();	//モーションアクション(キャラ同士のぶつかり等)
		Program_Move(pos, m_AreaInfo[m_nAreaNow][m_nTargetNum]);	//移動処理
	}
}

//=============================================================================
//　移動・位置変更
//=============================================================================
void CEnemy::Program_Move(D3DXVECTOR3 pos,TERRITORY_INFO territory)
{
#if 1
	if (m_bTarget == false)
	{
		float fSpeed = GetSpeed();	//速さ

		//移動加算処理
		if (m_bDebugStop == FLAG)
		{
			if (m_state == STATE_WALK || m_state == STATE_ACTION)
			{
				//CDebugProc::Print("\nfSpeed : %.1f  fSpeed : %.1f\n", fSpeed, m_fSpeed);

				pos.x += (float)cos(territory.fRadian) * (fSpeed * m_fSpeed);
				pos.z += (float)sin(territory.fRadian) * (fSpeed * m_fSpeed);
			}
		}
		//★拠点までの角度 / 自身の軸調整
		float fDestAngle = atan2f(pos.x - territory.pos.x, pos.z - territory.pos.z);
		float fAngle = fDestAngle - m_rot.y;

		if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
		if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }

		//移動方向に角度調整
		m_rot.y += fAngle * 0.1f;

		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }

		//位置・回転情報の反映
		CCharacter::SetPos(pos);
		CCharacter::SetRot(m_rot);
		//CDebugProc::Print("\n角度 : %.1f", territory.fRadian);
		/*CDebugProc::Print("\n角度A : %.1f", m_nTerrStart.fRadian);
		for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
		{
			CDebugProc::Print("角度B : %.1f", m_AreaInfo[m_nAreaNow][nCnt].fRadian);

		}*/

	}
#endif
}
//=============================================================================
//ライン関数まとめ
//=============================================================================
void  CEnemy::Program_Line(void)
{
	//ラインのカウントダウン処理
	CountDownLineTime();

	//ライン引きの開始(カウントアップ)
	if (m_bBlockStartTerritory)
	{
		m_nCountTime++;
		if ((m_nCountTime % NOT_GETTIME) == 0) { m_bBlockStartTerritory = false; }
	}

	//図形が完成した後の処理
	UpdateShapeComplete();
}

//=============================================================================
//ラインを繋ぐ手順
//=============================================================================
void CEnemy::LineConnect(int nRand)
{
	if (m_nRandStop == false)
	{
		m_nRandNum = nRand;
	}
	m_nRandStop = true;

	//[[最新の始点・終点に更新]]
	if (m_apTerritory[0] != NULL)
	{
		m_nTerrStart.pos = m_apTerritory[0]->GetPos();
	}

	//[[ラインを(m_nLineNum)本引いたら始点に戻る]]
	if (m_apTerritory[m_nLineNum] != NULL)
	{
		if (m_nLineTime <= m_nRandNum * 100 && m_bFinish == false ||	//制限時間が迫ってきたら
			m_apTerritory[TERRITORY_MAX - 1] != NULL)					//最大ライン分引いていたら
		{
			m_bFinish = true;	//始点に戻す
		}
		else//ラインを伸ばす時間に余裕があれば
		{
			m_nLineNum += 1;
		}

	}
}
//=============================================================================
//モーション処理まとめ
//=============================================================================
void CEnemy::Program_State(void)
{
#if 1
	int nData = 0;

	switch (m_state)
	{
	case STATE_NONE:		//[[通常]]

		m_bWalk = true;
		m_bSprintMotion = true;

		break;

	case STATE_WALK:		//[[移動状態]]
		nData = 0;

		break;

	case STATE_ACTION:		//[[アクション状態]]

		break;

	case STATE_BLOWAWAY:	//[[吹っ飛ばされてる状態]]
		m_nDamageCount++;

		if (m_nDamageCount >= MAX_DAMAGECOUNT)
		{
			m_nDamageCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;
			m_state = STATE_NONE;
		}

		//吹っ飛ばす
		m_fBlowLength -= BLOW_MOVING_CUT;
		if (m_fBlowLength >= 0.0f)
		{
			D3DXVECTOR3 pos = CCharacter::GetPos();
			pos.x -= sinf(m_fBlowAngle) * m_fBlowLength;
			pos.z -= cosf(m_fBlowAngle) * m_fBlowLength;
			CCharacter::SetPos(pos);
		}

		break;

	case STATE_DAMAGE:		//[[ダメージ状態]]

		m_nDownCount++;
		if (m_nDownCount == 70)
		{//70秒たったら起き上がる
			m_pMotion->SetNumMotion(5);
		}
		if (m_nDownCount == 110)
		{//110秒で動けるようになる
			m_nDownCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;
			m_nDamageCounter = 0;
			m_bSuperArmor = false;
			m_state = STATE_NONE;
		}
		break;
	case STATE_ROBOTDAMAGE:		// ←追加(よしろう)
								//吹っ飛ばす
		m_fBlowLength -= BLOW_MOVING_CUT;
		if (m_fBlowLength >= 0.0f)
		{
			D3DXVECTOR3 pos = CCharacter::GetPos();
			pos.x -= sinf(m_fBlowAngle) * m_fBlowLength;
			pos.z -= cosf(m_fBlowAngle) * m_fBlowLength;
			CCharacter::SetPos(pos);
		}

		m_nDamageCount++;
		if (m_nDamageCount == 70)
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
			m_state = STATE_NONE;
		}

		break;
	}
#endif
}

//=============================================================================
//　吹っ飛び処理
//=============================================================================
void CEnemy::BlowAway(D3DXVECTOR3 AnotherPos,int Num)
{
	//座標を取得
	D3DXVECTOR3 pos = GetPos();

	//吹っ飛ぶ向きを計算
	m_fBlowAngle = CFunctionLib::CalcAngleToDest(pos.x, pos.z, AnotherPos.x, AnotherPos.z);

	//吹っ飛ぶ距離を設定
	if (Num == 1)
	{// ロボットの攻撃のとき
		m_fBlowLength = BLOW_ROBOTMOVING;
	}
	else
	{// その他
		m_fBlowLength = BLOW_ENEMYMOVING;
	}

	//吹っ飛び状態にする
	if (Num != 1)
	{
		m_state = STATE_BLOWAWAY;
		m_pMotion->SetNumMotion(STATE_BLOWAWAY);
	}

	//向きを変える
	float fRot = m_fBlowAngle + D3DX_PI;
	fRot = CFunctionLib::CheckRotationOverPiX(fRot);
	m_rot.y = fRot;
}

//=============================================================================
//　モーションアクション(キャラ同士ぶつかった際)
//=============================================================================
void CEnemy::Program_Motion(void)
{
	//移動状態
	if (m_state != STATE_BLOWAWAY && m_state != STATE_DAMAGE && m_state != STATE_ROBOTDAMAGE)
	{
		if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_state != STATE_ACTION)
		{
			//移動状態
			m_state = STATE_WALK;
		}
		else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE)
			&& (m_state != STATE_ACTION && m_state != STATE_BLOWAWAY && m_state != STATE_DAMAGE))
		{
			m_state = STATE_WALK;
		}
	}
	if (m_bWalk == true && m_state == STATE_WALK)
	{
		m_pMotion->SetNumMotion(m_state);
		m_bWalk = false;
	}

	if (m_bSprintMotion == true)
	{//スプリントモーション
		if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_state == STATE_ACTION)
		{
			m_pMotion->SetNumMotion(STATE_ACTION);
			m_bSprintMotion = false;
		}
	}
}

//=============================================================================
//イベント
//=============================================================================
void CEnemy::Event_Bonus(void)
{
	CGame * pGame = CManager::GetGame();//現在のゲームの状態を取得

	if (pGame->GetAreaBonusEventFlag() == true && CCharacter::GetCountGetTerritory() <= 30)//ボーナスエリアのイベントが発生している場合...
	{
		//[[一度すべてリセット、ボーナスエリアを目指す]]
		int ntmpData = m_nAreaNow;
		if (m_EventCheck == false)
		{
			m_nAreaNow = (pGame->GetAreaBonusIdx() + 2) % 4;


			if (ntmpData != m_nAreaNow) //ボーナスエリア外
			{
				m_nPassCnt = 0;

				for (int nCnt = 0; nCnt < m_nAreaTerrNum[ntmpData]; nCnt++)
				{
					m_AreaInfo[ntmpData][nCnt].bFlag = false;

				}
				m_bReset = true;
			}
		}
		m_EventCheck = true; //ここで初めてフラグを立てる(無駄にリセットさせないため)

		
	}
	else
	{
		m_EventCheck = false;

		if (m_bReset == true) //イベント終わり次第,各エリアに散らばせる
		{
			int m_nBeArea = m_nAreaNow;
			while (m_nBeArea == m_nAreaNow)
			{
				m_nAreaNow = rand() % 4;
			}
			m_nPassCnt = 0;
			for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nBeArea]; nCnt++)
			{
				m_AreaInfo[m_nBeArea][nCnt].bFlag = false;

			}
			for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
			{
				m_AreaInfo[m_nAreaNow][nCnt].bFlag = false;

			}
			m_bReset = false;
		}

	}

}

void CEnemy::Event_Division(int nType, int nArea01, int nArea02)
{
	//縦
	if (nType == 0)
	{
		if (nArea02 > AREA_MAX - 1)
		{
			nArea02 = nArea02 - AREA_MAX;
		}
	}
	//横
	else
	{
		if ((nArea02 + 2) % 2 == 0)
		{
			nArea02 = nArea02 - 2;
		}
	}
	if (m_nAreaNow == nArea01 || m_nAreaNow == nArea02)
	{
		if (m_nPassCnt == m_nAreaTerrNum[m_nAreaNow])	//通過回数もフラグもリセット
		{
			m_nPassCnt = 0;

			for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
			{
				m_AreaInfo[m_nAreaNow][nCnt].bFlag = false;

			}

			if (m_nAreaNow == nArea01)
			{
				m_nAreaNow = nArea02;
			}
			else
			{
				m_nAreaNow = nArea01;
			}
		}
	}

	
}
//=============================================================================
//判定関連
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
			CollisionCollider(pCol, pos, posOld, INITIALIZE_VECTOR3, D3DXVECTOR3(15.0f, 15.0f, 15.0f));
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
		//★パワー型AI用の判定
		if (m_CharcterType == CCharacter::CHARCTERTYPE_POWER)
		{
			CGame * pGame = CManager::GetGame();
			if (pGame != NULL) 
			{ 
				CCharacter *pChara = pGame->GetChara(m_nEnemyNo); 
				if (pChara != NULL)
				{
					CTypePower *pPower = (CTypePower*)pChara;
					if (pPower->CollisionSkillTiming((CCylinderCollider*)pCollider, pos, posOld, Move, ColRange) == true) {}
				}
			}
			
		}
	}
	else if (pCollider->GetType() == CCollider::TYPE_SPHERE_PLAYERATTACK)
	{
		CScene *pParent = pCollider->GetParent();

		if (CollisionPlayerAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, ColRange) == true 
			&& pParent->GetObjType() != OBJTYPE_ROBOT)
		{
		}
		if (CollisionRobotAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, ColRange) == true 
			&& pParent->GetObjType() == OBJTYPE_ROBOT)
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
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ROBOT)
		{//親が敵かプレイヤーだった場合自分を吹っ飛ばす
			CCharacter *pCharacter = (CCharacter*)pParent;
			if (pCharacter == NULL) { return true; }
			D3DXVECTOR3 AnotherPos = pCharacter->GetPos();
			if (m_nDamageCount >= 1)
			{
				m_nDamageCount = 0;
			}
			if (m_bSuperArmor != true)
			{//スーパーアーマ状態じゃないなら
				BlowAway(AnotherPos, 0);
			}
		}
		else if (pParent->GetObjType() == OBJTYPE_MINE)
		{//地雷だったら
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this && pMine->GetType() == CMine::TYPE_NONE)
			{//自分以外が出した地雷なら起動
				pMine->SetType(CMine::TYPE_STARTUP);
			}
		}
		else if (pParent->GetObjType() == OBJTYPE_ROBOTBULLET)	// ←追加(よしろう)
		{// ミサイルだったら
			CBullet *pBullet = (CBullet*)pParent;
			if (pBullet->GetType() == CBullet::TYPE_MOVE)
			{// 爆発
				pBullet->SetType(CBullet::TYPE_EXPLOSION);
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
	{//自分以外の攻撃球が当たったら
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_MINE)
		{//地雷だったら
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this)
			{//自分以外が出した地雷ならダメージ
				m_state = STATE_DAMAGE;
				if (m_nDamageCounter == 0)
				{
					m_bSuperArmor = true;
					m_pMotion->SetNumMotion(4);
					m_nDamageCounter = 1;
					if (m_nDownCount >= 1)
					{
						m_nDownCount = 0;
					}
				}
			}
		}
		if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_ROBOTBULLET)
		{
			//当たってる間はダメージ状態
			m_state = STATE_DAMAGE;

			if (m_nDamageCounter == 0)
			{
				m_bSuperArmor = true;
				m_pMotion->SetNumMotion(4);
				m_nDamageCounter = 1;
				if (m_nDownCount >= 1)
				{
					m_nDownCount = 0;
				}
			}
		}
		return true;
	}

	return false;
}

void CEnemy::MergeSort(TERRITORY_INFO* pArray, int start, int end, int AreaNum)
{
	// 作業用のデータ
	int middle, i, j, k;
	if (start >= end) {
		return;
	}
	// startとendの中間地点を分割点とする
	middle = (start + end) / 2;
	// 前半部分を再帰的に整列
	MergeSort(pArray, start, middle, AreaNum);
	// 後半部分を再帰的に整列
	MergeSort(pArray, middle + 1, end, AreaNum);
	k = 0;
	// 仮想領域のデータをマージしながらコピーする。
	for (i = start; i <= middle; i++) {
		m_tmp[AreaNum][k] = pArray[i].fDisSort;
		k++;
	}
	for (j = end; j >= middle + 1; j--) {
		m_tmp[AreaNum][k] = pArray[j].fDisSort;
		k++;
	}
	// 末端からデータを取得して、マージしていく。
	i = 0;
	j = end - start;
	for (k = start; k <= end; k++) {
		if (m_tmp[AreaNum][i] <= m_tmp[AreaNum][j]) {
			pArray[k].fDisSort = m_tmp[AreaNum][i];
			i++;
		}
		else {
			pArray[k].fDisSort = m_tmp[AreaNum][j];
			j--;
		}
	}

}

//=============================================================================
//　攻撃球との当たり判定処理(ロボット)
//=============================================================================
bool CEnemy::CollisionRobotAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
{
	if (pShere->Collision(&pos, 100.0f, this) == true && pShere->GetParent() != this)
	{//自分以外の攻撃球が当たったら
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_ROBOT)
		{
			//当たってる間はダメージ状態
			m_state = STATE_ROBOTDAMAGE;		// ←追加(よしろう)

			if (m_nDamageCounter == 0)
			{
				m_bSuperArmor = true;
				m_pMotion->SetNumMotion(4);
				m_nDamageCounter = 1;
				if (m_nDownCount >= 1)
				{
					m_nDownCount = 0;
				}
				// ←新規追加(よしろう)
				CCharacter *pCharacter = (CCharacter*)pParent;
				if (pCharacter == NULL) { return true; }
				D3DXVECTOR3 AnotherPos = pCharacter->GetPos();
				BlowAway(AnotherPos, 1);
			}
		}
		return true;
	}

	return false;
}



#endif