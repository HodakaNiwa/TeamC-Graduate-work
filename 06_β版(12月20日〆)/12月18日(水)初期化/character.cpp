//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					キャラクター管理 [character.cpp]
//			Author : Kobayashi_Sho-hei / 小林 将兵 & Hodaka Niwa
//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆

//	---<<#include>>---
#include "character.h"
#include "model.h"
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "line.h"
#include "library.h"
#include "territory.h"
#include "sound.h"
#include "UI.h"
#include "score.h"
#include "sceneTriangle3D.h"
#include "game.h"
#include "select.h"
#include "lightorbit.h"
#include "scoreGauge.h"
#include "robot.h"
#include "point.h" 

//	---<<マクロ定義>>---
#define CHARACTER_COLLISION_POS    (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define CHARACTER_COLLISION_HEIGHT (100.0f)
#define CHARACTER_COLLISION_RADIUS (20.0f)

//色
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//赤
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//青
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//緑
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//白
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//黒
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//黄
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//オレンジ
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//紫

#define FAR_POS			(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))

//#define LIGHTORBITDRAW

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//静的メンバ変数宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CCharacter::CHARCTERTYPE CCharacter::m_CharcterTypeResult[8] = {};		//リザルト用のキャラタイプ
int CCharacter::m_nCuntry[8] = {};										//リザルト用の国タイプ

//国の色の設定
D3DXCOLOR CCharacter::m_CountryColor[CCharacter::TYPE_MAX] =
{
	COLOR_RED,		//日本
	COLOR_BULE,		//ロシア
	COLOR_GREEN,	//アメリカ
	COLOR_YELLOW,	//ブラジル
	COLOR_ORENGE,	//イギリス
	COLOR_PAPULE,	//イタリア
	COLOR_BRACK,	//ニュージーランド
	COLOR_WHITE,	//南アフリカ
};

//==============================================
//					コンストラクタ
//==============================================
CCharacter::CCharacter(int nPriority, OBJTYPE objType) : CScene(nPriority,objType)
{
	m_pOldStartTerritory = NULL;
	m_pMotion = NULL;
	m_pMotionInfo = NULL;
	m_nCountGetTerritry = 0;
	m_nCntTimeCopyLine = 0;
	m_nLineTime = 0;
	m_nNumPlayerNo = -1;
	m_nType = -1;
	m_nButtonCounter = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nState = 0;
	m_nNumMotionMAX = 0;
	m_nNumKey = 0;
	m_nAttack = 0;
	m_nGetScorePoint = 0;
	m_nDestPlayer = 0;
	m_nPointCounter = 0;
	m_nCountMakeShape = 0;
	m_nCountGetTerritry = 0;
	m_nCountRobbtedTerritory = 0;
	m_fAttackSpeed = 0.0f;
	m_fRange = 0.0f;
	m_fSpeed = 0.0f;
	m_fInertia = 0.0f;
	m_fLineTime = 0.0f;
	m_bRecast = false;
	m_bBlockStartTerritory = false;
	m_bMakeShape = false;
	m_bCharaMotionState = false;
	m_bSprint = false;
	m_CharcterType = CHARCTERTYPE_MAX;
	m_Pos = INITIALIZE_VECTOR3;
	m_Rot = INITIALIZE_VECTOR3;
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//==============================================
//					デストラクタ
//==============================================
CCharacter::~CCharacter(){}

//=============================================================================
// モデルの破棄
//=============================================================================
void CCharacter::Unload(void)
{

}


//==============================================
//					初期化
////==============================================
HRESULT CCharacter::Init(int nCharaNum, char ModelTxt[40], char MotionTxt[40],int nType, int nCuntry)
{
	//初期化処理
	m_Pos = INITIALIZE_VECTOR3;
	m_Rot = INITIALIZE_VECTOR3;
	m_nCountTerritory = 0;
	m_ppUVTexName = NULL;
	m_bCharaMotionState = false;
	m_nDestPlayer = 0;
	m_nGetScorePoint = 0;
	m_nPointCounter = 0;
	m_nCountMakeShape = 0;			//図形を作った数
	m_nCountGetTerritry = 0;		//テリトリーの取得数
	m_nCountRobbtedTerritory = 0;	//テリトリーの奪われた数
	m_nLineTime = FIRST_LINETIME;
	m_nCntTimeCopyLine = 0;
	m_bSprint = false;

	m_cModelTxt[0] = &ModelTxt[0];
	m_cMotionTxt[0] = &MotionTxt[0];
	ModelSetLode();
	int nData = 0;

	m_ppModel = NULL;
	m_pMotion = NULL;

	if (m_ppModel == NULL)
	{
		m_ppModel = new CModel*[m_nPartsNum];		//	パーツ分確保する
	}
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		m_ppModel[nCnt] = NULL;
	}

	CSelect *pSelect = CManager::GetSelect();
	int nPlayer[4] = {};

	/*for (int nCntCountry = 0; nCntCountry < 4; nCntCountry++)
	{
	m_nCuntry[nCntCountry] = pSelect->GetPlayerCuntry(nCntCountry);
	}*/

	for (int nCntTerritory = 0; nCntTerritory < MAX_TERRITORY; nCntTerritory++)
	{//テリトリー・ラインの初期化
		m_apTerritory[nCntTerritory] = NULL;
		m_apLine[nCntTerritory] = NULL;
		m_apCopyLine[nCntTerritory] = NULL;
	}

	// 読み込むテクスチャの数を設定
	m_nLoadTexNum = 0;
	if (nType == CHARCTERTYPE_SPEED)
	{
		m_nLoadTexNum = 9;
	}
	if (nType == CHARCTERTYPE_POWER)
	{
		m_nLoadTexNum = 8;
	}
	if (nType == CHARCTERTYPE_TECHNIQUE)
	{
		m_nLoadTexNum = 10;
	}

	// テクスチャの読み込み
	if (nCuntry < 0) { nCuntry = 0; }
	TextureSetLode(nCuntry * m_nLoadTexNum, m_nLoadTexNum);

	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_ppModel[nCnt] == NULL)
		{
			//	モデルパーツの初期値を決める
			D3DXVECTOR3 Pos = D3DXVECTOR3(m_aOffset[nCnt].pos.x, m_aOffset[nCnt].pos.y, m_aOffset[nCnt].pos.z);
			char * Name = m_aPartsName[nCnt];

			m_ppModel[nCnt] = CModel::Create(D3DXVECTOR3(m_aOffset[nCnt].pos.x, m_aOffset[nCnt].pos.y, m_aOffset[nCnt].pos.z), m_aPartsName[nCnt], D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			//	モデルUVテクスチャを決める←追加(よしろう)
			if (nCharaNum != ROBOT_CHARNUM)
			{
				m_ppModel[nCnt]->BindTexture(m_ppTexture[m_nTexIdx[nCnt]]);
			}
			else
			{
				m_ppModel[nCnt]->BindTexture(NULL);
			}

			//	各モデルパーツの親を決める
			if (nCnt == 0)
			{
				m_ppModel[nCnt]->SetParent(NULL);
			}
			else
			{
				int nParent = m_nParent[nCnt];
				m_ppModel[nCnt]->SetParent(m_ppModel[m_nParent[nCnt]]);
			}
		}
	}
	MotionSetLode();
	m_pMotion = CMotion::Create();
	m_pMotion->SetMaxModel(m_nPartsNum);
	m_pMotion->SetModel(m_ppModel);
	m_pMotion->SetMotion(m_pMotionInfo);
	m_pMotion->SetMaxMotion(m_nNumMotionMAX);
	m_pMotion->Init();

	m_nNumPlayerNo = nCharaNum;

	// コライダーを作成する処理
	CreateCylinderCollider();


	return S_OK;
}

//==============================================
//			円筒を作成する処理
//==============================================
void CCharacter::CreateCylinderCollider(void)
{
	// コライダー管轄クラスを生成
	CColliderManager *pColManager = CColliderManager::Create();
	if (pColManager == NULL) { return; }

	// 円筒を作成
	CCylinderCollider *pCylinder = CCylinderCollider::Create(CHARACTER_COLLISION_POS, D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		CHARACTER_COLLISION_RADIUS, CHARACTER_COLLISION_HEIGHT, false);
	if (pCylinder == NULL) { return; }

	// 親を設定
	pCylinder->SetParent(this);
	pCylinder->SetParentMtxWorld(&m_mtxWorld);

	// コライダー管轄クラスのポインタを設定
	pColManager->SetCollider(pCylinder, 0);
	SetCOlliderManager(pColManager);
}


//==============================================
//					終了
//==============================================
void CCharacter::Uninit()
{
	//!+ ---<<モーション情報の解放>>---
		//キーフレームの位置・回転情報の解放(モーションの種類分 / キー分)
	for (int nCnt = 0; nCnt < m_nNumMotionMAX; nCnt++)
	{
		for (int nCnt02 = 0; nCnt02 < m_pMotionInfo[nCnt].nNumKey; nCnt02++)
		{
			if (m_pMotionInfo[nCnt].m_pKeyInfo[nCnt02].aKey != NULL)
			{
				delete[] m_pMotionInfo[nCnt].m_pKeyInfo[nCnt02].aKey;
				m_pMotionInfo[nCnt].m_pKeyInfo[nCnt02].aKey = NULL;
			}
		}
	}
	//	キーフレーム情報の解放(モーションの種類分)
	for (int nCnt = 0; nCnt < m_nNumMotionMAX; nCnt++)
	{
		if (m_pMotionInfo[nCnt].m_pKeyInfo != NULL)
		{
			delete[] m_pMotionInfo[nCnt].m_pKeyInfo;
			m_pMotionInfo[nCnt].m_pKeyInfo = NULL;
		}
	}
	//	モーション情報の解放
	if (m_pMotionInfo != NULL)
	{
		delete[] m_pMotionInfo;
		m_pMotionInfo = NULL;
	}
	//	解放
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//!+ ---<<モデル情報の解放>>---
	//	オフセット
	if (m_aOffset != NULL)
	{
		delete[] m_aOffset;
		m_aOffset = NULL;
	}

	//	ペアレント情報
	if (m_nParent != NULL)
	{
		delete[] m_nParent;
		m_nParent = NULL;
	}

	//	インデックス情報
	if (m_aIndex != NULL)
	{
		delete[] m_aIndex;
		m_aIndex = NULL;
	}

	// 使用するテクスチャの番号
	if (m_nTexIdx != NULL)
	{
		delete[] m_nTexIdx;
		m_nTexIdx = NULL;
	}

	//	ファイル名情報(ダブルポインタを扱っているので)
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_aPartsName[nCnt] != NULL)
		{
			delete[] m_aPartsName[nCnt];
			m_aPartsName[nCnt] = NULL;
		}
	}
	if (m_aPartsName != NULL)
	{
		delete[] m_aPartsName;
		m_aPartsName = NULL;
	}

	// テクスチャのファイル名情報
	for (int nCnt = 0; nCnt < m_nTexNum; nCnt++)
	{
		if (m_ppUVTexName[nCnt] != NULL)
		{
			delete[] m_ppUVTexName[nCnt];
			m_ppUVTexName[nCnt] = NULL;
		}
	}
	if (m_ppUVTexName != NULL)
	{
		delete[] m_ppUVTexName;
		m_ppUVTexName = NULL;
	}

	//	モデルパーツ情報(ダブルポインタを扱っているので)
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_ppModel[nCnt] != NULL)
		{
			m_ppModel[nCnt]->Uninit();
			delete m_ppModel[nCnt];
			m_ppModel[nCnt] = NULL;
		}
	}
	if (m_ppModel != NULL)
	{
		delete[] m_ppModel;
		m_ppModel = NULL;
	}

	// テクスチャの開放
	if (m_ppTexture != NULL)
	{
		for (int nCntTex = 0; nCntTex < m_nLoadTexNum; nCntTex++)
		{
			if (m_ppTexture[nCntTex] != NULL)
			{
				m_ppTexture[nCntTex]->Release();
				m_ppTexture[nCntTex] = NULL;
			}
		}
		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//==============================================
//					更新
//==============================================
void CCharacter::Update()
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得

	if (m_pMotion != NULL)
	{
		m_pMotion->Update();
	}

	// スコア変動の表示
	if (CManager::GetMode() != CManager::MODE_GAME || m_nGetScorePoint == 0) { return; }
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }
	CUI *pUI = pGame->GetUI();
	if (pUI == NULL) { return; }
	pUI->CreateScoreChange(m_nNumPlayerNo, m_nGetScorePoint);
	m_nGetScorePoint = 0;

#ifdef LIGHTORBITDRAW
	// 光の軌跡を表示する
	CreateLightOrbit(pGame);
#endif

	// 画面エフェクトを生成
	pUI->CreateScreenEffect(m_nNumPlayerNo, m_nDestPlayer);
}

//==============================================
//					描画
//==============================================
void CCharacter::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent, mtxScale;	//計算

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// アウトラインの描画
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_ppModel[nCnt] != NULL)
		{
			m_ppModel[nCnt]->DrawOutline();
		}
	}

	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_ppModel[nCnt] != NULL)
		{
			m_ppModel[nCnt]->Draw(1.0f);
		}
	}
}

//==============================================
//					モデルの読み込み
//==============================================
#if 1
void CCharacter::ModelSetLode(void)
{
	FILE* pFile;
	char* pStrCur;		//	先頭文字列のポインタ
	char aLine[256];	//	文字列の読み込み
	char aStr[256];		//	文字列の抜き出し
	int nDelay;			//	読み込む文字位置をずらす
	int nCnt = 0;
	int nCntLoadTex = 0;
	m_ppUVTexName = NULL;
	m_ppTexture = NULL;
	m_aPartsName = NULL;
	m_aIndex = NULL;
	m_nParent = NULL;
	m_nTexIdx = NULL;
	m_aOffset = NULL;

	pFile = fopen(m_cModelTxt[0], "r");	//	読み込むテキスト名

	if (pFile != NULL)
	{
		while (1)
		{
			pStrCur = CCharacter::ReadLine(pFile, &aLine[0]);
			strcpy(&aLine[0], pStrCur);

			if (memcmp(pStrCur, "MODEL_LOAD", strlen("MODEL_LOAD")) == 0)
			{
				while (1)
				{
					fgets(&aLine[0], 256, pFile);
					pStrCur = CCharacter::GetLIneTop(&aLine[0]);
					if (memcmp(pStrCur, "NUM_UV = ", strlen("NUM_UV = ")) == 0)	// ---<<使用するUVテクスチャの数の読み込み>>---
					{
						// 文字列を進める
						pStrCur += strlen("NUM_UV = ");
						strcpy(aStr, pStrCur);

						// 使用するUVテクスチャの数を読み込む
						m_nTexNum = atoi(aStr);

						// 読み込むUVテクスチャの数だけ動的確保
						m_ppUVTexName = new char*[m_nTexNum];
					}

					if (memcmp(pStrCur, "UV_NAME ", strlen("UV_NAME ")) == 0)	// ---<<UVテクスチャの読み込み>>---
					{
						pStrCur += strlen("UV_NAME ");
						strcpy(aStr, pStrCur);
						while (1)
						{
							if (pStrCur[strlen(pStrCur) - 1] != 'g')
							{
								pStrCur[strlen(pStrCur) - 1] = '\0';//改行文字をNULL文字に変換(参考サイト::http://hisashi.me/blog/?p=714)
							}
							else
							{
								break;
							}
						}
						m_ppUVTexName[nCntLoadTex] = new char[256];	//	文字列の確保
						strcpy(m_ppUVTexName[nCntLoadTex], pStrCur);
						nCntLoadTex++;
					}

					if (memcmp(pStrCur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)	// ---<<パーツ数の読み込み>>---
					{
						pStrCur += strlen("NUM_MODEL = ");
						strcpy(aStr, pStrCur);
						m_nPartsNum = atoi(aStr);
						m_aPartsName = new char*[m_nPartsNum];
						for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
						{
							m_aPartsName[nCnt] = new char[256];
						}
						m_aIndex = new int[m_nPartsNum];
						m_nParent = new int[m_nPartsNum];
						m_nTexIdx = new int[m_nPartsNum];
						m_aOffset = new MODEL_OFFSET[m_nPartsNum];

						for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
						{
							m_aIndex[nCnt] = 0;
							m_nParent[nCnt] = 0;
							m_nTexIdx[nCnt] = 0;
							m_aOffset[nCnt].pos = INITIALIZE_VECTOR3;
							m_aOffset[nCnt].rot = INITIALIZE_VECTOR3;
						}
					}

					if (memcmp(pStrCur, "MODEL_NAME ", strlen("MODEL_NAME ")) == 0)	// ---<<各パーツの名前を取得>>---
					{
						pStrCur += strlen("MODEL_NAME ");

						strcpy(aStr, pStrCur);

						while (1)
						{
							if (pStrCur[strlen(pStrCur) - 1] != 'x') { pStrCur[strlen(pStrCur) - 1] = '\0'; }

							else { break; }

							strcpy(&m_aPartsName[nCnt][0], pStrCur);
						}

						nCnt += 1;
					}

					//読み込みの終わり
					if (memcmp(pStrCur, "END_MODEL_LOAD", strlen("END_MODEL_LOAD")) == 0) { break; }
				}
			}

			nCnt = 0;	//	ここで一回0にもどす

			if (memcmp(pStrCur, "PARAMETERSET", strlen("PARAMETERSET")) == 0)
			{
				while (1)
				{
					fgets(&aLine[0], 256, pFile);
					pStrCur = GetLIneTop(&aLine[0]);

					if (memcmp(pStrCur, "STATUSSET", strlen("STATUSSET")) == 0)
					{
						while (1)
						{
							fgets(&aLine[0], 256, pFile);
							pStrCur = GetLIneTop(&aLine[0]);
							if (memcmp(pStrCur, "ATTACK = ", strlen("ATTACK = ")) == 0)
							{//攻撃力を決める
								pStrCur += strlen("ATTACK = ");
								strcpy(aStr, pStrCur);
								m_nAttack = atoi(aStr);
							}
							//	---<<攻撃速度を決める>>---
							if (memcmp(pStrCur, "ATTACKSPEED = ", strlen("ATTACKSPEED = ")) == 0)
							{
								pStrCur += strlen("ATTACKSPEED = ");
								strcpy(aStr, pStrCur);
								m_fAttackSpeed = (float)atoi(aStr);
							}
							//	---<<射程>>---
							if (memcmp(pStrCur, "RANGE =", strlen("RANGE =")) == 0)
							{
								pStrCur += strlen("RANGE =");
								strcpy(aStr, pStrCur);
								m_fRange = (float)atof(aStr);

								//for (int nCntWord = 0; nCntWord < 3; nCntWord++)
								//{
								//	nDelay = PopString(pStrCur, aStr);
								//	pStrCur += nDelay;	//	読み取った文字分ずらす
								//	strcpy(aStr, pStrCur);

								//	if (nCntWord == 0) { m_aOffset[nCnt].pos.x = (float)atof(aStr); }
								//	else if (nCntWord == 1) { m_aOffset[nCnt].pos.y = (float)atof(aStr); }
								//	else { m_aOffset[nCnt].pos.z = (float)atof(aStr); }
								//}
							}
							//	---<<移動量>>---
							if (memcmp(pStrCur, "SPEED =", strlen("SPEED =")) == 0)
							{
								pStrCur += strlen("SPEED =");
								strcpy(aStr, pStrCur);
								m_fSpeed = (float)atof(aStr);

								//for (int nCntWord = 0; nCntWord < 3; nCntWord++)
								//{
								//	nDelay = PopString(pStrCur, aStr);
								//	pStrCur += nDelay;	//	読み取った文字分ずらす
								//	strcpy(aStr, pStrCur);

								//	if (nCntWord == 0) { m_aOffset[nCnt].rot.x = (float)atof(aStr); }
								//	else if (nCntWord == 1) { m_aOffset[nCnt].rot.y = (float)atof(aStr); }
								//	else { m_aOffset[nCnt].rot.z = (float)atof(aStr); }
								//}
							}
							//	---<<慣性>>---
							if (memcmp(pStrCur, "INERTIA =", strlen("INERTIA =")) == 0)
							{
								pStrCur += strlen("INERTIA =");
								strcpy(aStr, pStrCur);
								m_fInertia = (float)atof(aStr);
							}
							//	---<<線を引ける時間>>---
							if (memcmp(pStrCur, "LINETIME =", strlen("LINETIME =")) == 0)
							{
								pStrCur += strlen("LINETIME =");
								strcpy(aStr, pStrCur);
								m_fLineTime = (float)atof(aStr);
							}
							//	読み込み終了
							if (memcmp(pStrCur, "END_STATUSSET", strlen("END_STATUSSET")) == 0) { break; }
						}
					}
					//	読み込み終了
					if (memcmp(pStrCur, "END_PARAMETERSET", strlen("END_PARAMETERSET")) == 0) { break; }
				}
			}

			if (memcmp(pStrCur, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
			{
				while (1)
				{
					fgets(&aLine[0], 256, pFile);
					pStrCur = GetLIneTop(&aLine[0]);

					if (memcmp(pStrCur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							fgets(&aLine[0], 256, pFile);
							pStrCur = GetLIneTop(&aLine[0]);
							if (memcmp(pStrCur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								pStrCur += strlen("INDEX = ");
								strcpy(aStr, pStrCur);
								m_aIndex[nCnt] = atoi(aStr);
							}
							if (memcmp(pStrCur, "TEX_INDEX = ", strlen("TEX_INDEX = ")) == 0)
							{
								pStrCur += strlen("TEX_INDEX = ");
								strcpy(aStr, pStrCur);
								m_nTexIdx[nCnt] = atoi(aStr);
							}
							//	---<<各パーツの親を決める>>---
							if (memcmp(pStrCur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								pStrCur += strlen("PARENT = ");
								strcpy(aStr, pStrCur);
								m_nParent[nCnt] = atoi(aStr);
							}
							//	---<<各パーツの位置>>---
							if (memcmp(pStrCur, "POS =", strlen("POS =")) == 0)
							{
								pStrCur += strlen("POS =");
								strcpy(aStr, pStrCur);

								for (int nCntWord = 0; nCntWord < 3; nCntWord++)
								{
									nDelay = PopString(pStrCur, aStr);
									pStrCur += nDelay;	//	読み取った文字分ずらす
									strcpy(aStr, pStrCur);

									if (nCntWord == 0) { m_aOffset[nCnt].pos.x = (float)atof(aStr); }
									else if (nCntWord == 1) { m_aOffset[nCnt].pos.y = (float)atof(aStr); }
									else { m_aOffset[nCnt].pos.z = (float)atof(aStr); }
								}
							}
							//	---<<各パーツの向き>>---
							if (memcmp(pStrCur, "ROT =", strlen("ROT =")) == 0)
							{
								pStrCur += strlen("ROT =");
								strcpy(aStr, pStrCur);

								for (int nCntWord = 0; nCntWord < 3; nCntWord++)
								{
									nDelay = PopString(pStrCur, aStr);
									pStrCur += nDelay;	//	読み取った文字分ずらす
									strcpy(aStr, pStrCur);

									if (nCntWord == 0) { m_aOffset[nCnt].rot.x = (float)atof(aStr); }
									else if (nCntWord == 1) { m_aOffset[nCnt].rot.y = (float)atof(aStr); }
									else { m_aOffset[nCnt].rot.z = (float)atof(aStr); }
								}
							}
							//	読み込み終了
							if (memcmp(pStrCur, "END_PARTSSET", strlen("END_PARTSSET")) == 0) { nCnt++; break; }
						}
					}
					//	読み込み終了
					if (memcmp(pStrCur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0) { break; }
				}
			}
			//	読み込み終了
			if (memcmp(pStrCur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0) { break; }
		}
	}
	fclose(pFile);
}
#endif

//==============================================
//			モーションの読み込み
//==============================================
#if 1
void CCharacter::MotionSetLode(void)
{

	FILE *pFile;
	char * pStrCur;//先頭文字列のポインタ
	char aLine[256];//文字列の読み込み
	char aStr[256];//文字列の抜き出し
	int nWord;
	int nMotionNum = 0;	//	モーションの種類分カウントする
	m_pMotionInfo = NULL;


	pFile = fopen(m_cMotionTxt[0], "r");	//	---<<テキスト読み込み>>---

	if (pFile != NULL)
	{
		pStrCur = ReadLine(pFile, &aLine[0]);
		strcpy(&aLine[0], pStrCur);
		if (memcmp(pStrCur, "MOTIONNUM = ", strlen("MOTIONNUM = ")) == 0)	//	---<<モーション最大数の取得>>---
		{
			pStrCur += strlen("MOTIONNUM = ");
			strcpy(aStr, pStrCur);
			m_nNumMotionMAX = atoi(aStr);
			if (m_pMotionInfo == NULL)
			{
				m_pMotionInfo = new CMotion::MOTION_INFO[m_nNumMotionMAX];
				ClearMotionInfo();
			}
		}

		while (1)
		{
			fgets(&aLine[0], 256, pFile);
			pStrCur = GetLIneTop(&aLine[0]);

			if (memcmp(pStrCur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{

				fgets(&aLine[0], 256, pFile);
				pStrCur = GetLIneTop(&aLine[0]);
				if (memcmp(pStrCur, "LOOP = ", strlen("LOOP = ")) == 0)	//	---<<ループするかしないか>>---
				{
					pStrCur += strlen("LOOP = ");
					strcpy(aStr, pStrCur);
					bool bLoop;
					if (atoi(aStr) == 1) { bLoop = true; }
					else { bLoop = false; }
					m_pMotionInfo[nMotionNum].m_bLoop = bLoop;
				}

				fgets(&aLine[0], 256, pFile);
				pStrCur = GetLIneTop(&aLine[0]);
				if (memcmp(pStrCur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)	//	---<<キーフレームの数>>---
				{
					pStrCur += strlen("NUM_KEY = ");
					strcpy(aStr, pStrCur);
					m_nNumKey = atoi(aStr);
					m_pMotionInfo[nMotionNum].nNumKey = atoi(aStr);
					if (m_pMotionInfo[nMotionNum].m_pKeyInfo == NULL)
					{
						m_pMotionInfo[nMotionNum].m_pKeyInfo = new CMotion::KEY_INFO[m_pMotionInfo[nMotionNum].nNumKey];	//	キーの数分確保
						ClearMotionInfo_KeyInfo(nMotionNum);
					}
				}

				int nKeyNum = 0;	//	キー数分カウントする
				int nParts = 0;		//	パーツ分カウントする
				while (1)
				{
					fgets(&aLine[0], 256, pFile);
					pStrCur = GetLIneTop(&aLine[0]);

					if (memcmp(pStrCur, "KEYSET", strlen("KEYSET")) == 0)
					{
						if (m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey == NULL)
						{
							m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey = new CMotion::KEY[m_nPartsNum];	//	モデルのパーツ分確保
							ClearMotionInfo_KeyInfo_Key(nMotionNum, nKeyNum);
						}

						while (1)
						{
							fgets(&aLine[0], 256, pFile);
							pStrCur = GetLIneTop(&aLine[0]);
							if (memcmp(pStrCur, "FRAME = ", strlen("FRAME = ")) == 0)	//	---<<フレーム数の取得>>---
							{
								pStrCur += strlen("FRAME = ");
								strcpy(aStr, pStrCur);
								m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].nFrame = atoi(aStr);	//	モーションの数分回す
							}


							if (memcmp(pStrCur, "KEY", strlen("KEY")) == 0)
							{
								while (1)
								{
									if (memcmp(pStrCur, "POS =", strlen("POS =")) == 0)	//	---<<位置情報>>---
									{
										pStrCur += strlen("POS =");
										strcpy(aStr, pStrCur);

										for (int nCntWord = 0; nCntWord < 3; nCntWord++)	//	XYZ分回す
										{
											nWord = PopString(pStrCur, aStr);
											pStrCur += nWord;	//	同じ行でも読み込む位置を変える
											strcpy(aStr, pStrCur);

											if (nCntWord == 0) { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fPosX = (float)atof(aStr); }
											else if (nCntWord == 1) { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fPosY = (float)atof(aStr); }
											else { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fPosZ = (float)atof(aStr); }
										}
									}

									if (memcmp(pStrCur, "ROT =", strlen("ROT =")) == 0)	//	---<<回転軸情報>>---
									{
										pStrCur += strlen("ROT =");
										strcpy(aStr, pStrCur);

										for (int nCntWord = 0; nCntWord < 3; nCntWord++)	//	XYZ分回す
										{
											nWord = PopString(pStrCur, aStr);
											pStrCur += nWord;	//	同じ行でも読み込む位置を変える
											strcpy(aStr, pStrCur);

											if (nCntWord == 0) { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fRotX = (float)atof(aStr); }
											else if (nCntWord == 1) { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fRotY = (float)atof(aStr); }
											else { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fRotZ = (float)atof(aStr); }
										}
									}

									//	1パーツのキー情報取得終わり
									if (memcmp(pStrCur, "END_KEY", strlen("END_KEY")) == 0)
									{
										nParts += 1;
										break;
									}

									fgets(&aLine[0], 256, pFile);
									pStrCur = GetLIneTop(&aLine[0]);
								}
							}

							if (memcmp(pStrCur, "END_KEYSET", strlen("END_KEYSET")) == 0)	//	1パーツのキーフレーム情報取得終わり
							{
								nParts = 0;
								nKeyNum += 1;
								break;
							}
						}
					}
					//	ひと段落終わり
					if (memcmp(pStrCur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0) { nMotionNum += 1; break; }
				}
			}
			//	テキストの終了
			if (memcmp(pStrCur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0&& nMotionNum == m_nNumMotionMAX) { break; }
		}
	}
	fclose(pFile);
}
#endif

//==============================================
//					テクスチャの読み込み
//==============================================
void CCharacter::TextureSetLode(int nStartIdx, int nNumTex)
{

	//デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// メモリを確保
	m_ppTexture = new LPDIRECT3DTEXTURE9[nNumTex];
	if (m_ppTexture == NULL) { return; }

	// テクスチャの読み込み
	int nLoadTex = 0;
	for (int nCntTex = nStartIdx; nCntTex < nStartIdx + nNumTex; nCntTex++, nLoadTex++)
	{
		// メモリを初期化
		m_ppTexture[nLoadTex] = NULL;

		// ファイルから読み込み
		D3DXCreateTextureFromFile(pDevice, m_ppUVTexName[nCntTex], &m_ppTexture[nLoadTex]);
	}
}

#if 1
char *CCharacter::ReadLine(FILE *pFile, char *pDst)
{
	fgets(pDst, 256, pFile);

	while (1)
	{
		if (memcmp(pDst, "#", strlen("#")) == 0) { fgets(pDst, 256, pFile); }
		else if (memcmp(pDst, " ", strlen(" ")) == 0){ pDst += 1; }
		else if (memcmp(pDst, "\t", strlen("\t")) == 0) { pDst += 1; }
		else if (memcmp(pDst, "\n", strlen("\n")) == 0) { fgets(pDst, 256, pFile); }
		else { break; }
	}
	return pDst;
}

char *CCharacter::GetLIneTop(char *pSrc)
{
	while (1)
	{
		if (memcmp(pSrc, " ", strlen(" ")) == 0) { pSrc += 1; }
		else if (memcmp(pSrc, "\t", strlen("\t")) == 0) { pSrc += 1; }
		else { break; }
	}

	return pSrc;
}

int CCharacter::PopString(char *pSrc, char *pDst)
{
	int nCnt = 0;

	while (1)
	{
		nCnt += 1;
		if (memcmp(pSrc, " ", strlen(" ")) == 0) { pSrc = "\0"; break; }
		else { pSrc += 1; }
	}

	strcpy(pDst, pSrc);

	return nCnt;
}
#endif

//==============================================
//			モーション情報をクリアする処理
//==============================================
void CCharacter::ClearMotionInfo(void)
{
	if (m_pMotionInfo == NULL) { return; }
	for (int nCntMotion = 0; nCntMotion < m_nNumMotionMAX; nCntMotion++)
	{
		m_pMotionInfo[nCntMotion].m_bLoop = false;
		m_pMotionInfo[nCntMotion].nNumKey = 0;
		m_pMotionInfo[nCntMotion].m_pKeyInfo = NULL;
	}
}

//==============================================
//			キー情報をクリアする処理
//==============================================
void CCharacter::ClearMotionInfo_KeyInfo(int nMotionIdx)
{
	if (m_pMotionInfo == NULL) { return; }
	if (m_pMotionInfo[nMotionIdx].m_pKeyInfo == NULL) { return; }
	for (int nCntKeyInfo = 0; nCntKeyInfo < m_pMotionInfo[nMotionIdx].nNumKey; nCntKeyInfo++)
	{
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nCntKeyInfo].aKey = NULL;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nCntKeyInfo].nFrame = 0;
	}
}

//==============================================
//			キーをクリアする処理
//==============================================
void CCharacter::ClearMotionInfo_KeyInfo_Key(int nMotionIdx, int nKeyInfoIdx)
{
	if (m_pMotionInfo == NULL) { return; }
	if (m_pMotionInfo[nMotionIdx].m_pKeyInfo == NULL) { return; }
	if (m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey == NULL) { return; }
	for (int nCntKey = 0; nCntKey < m_nPartsNum; nCntKey++)
	{
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fPosX = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fPosY = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fPosZ = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fRotX = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fRotY = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fRotZ = 0.0f;
	}
}

//=============================================================================
// テリトリーの取得更新処理
//=============================================================================
void CCharacter::CountDownLineTime(void)
{
	
	if (m_apTerritory[TERRITORY_TOP] != NULL)
	{
		//線を描く時間を減らす
		m_nLineTime--;

		if (m_nLineTime < 0)
		{
			m_nLineTime = FIRST_LINETIME;								//描画できる時間をリセットする

			//獲得ポイントを表示	
			if (m_apTerritory[TERRITORY_TOP]->GetPlayerNumber() != m_nNumPlayerNo)
			{
				int nGetPoint = m_apTerritory[TERRITORY_TOP]->GetPoint();

				// ゲームクラスを取得
				CGame *pGame = CManager::GetGame();
				if (pGame != NULL)
				{
					if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((m_apTerritory[TERRITORY_TOP]->GetErea() + 2) % 4))
					{
						nGetPoint += 1;
					}
				}

				CPoint::Create(nGetPoint, m_apTerritory[TERRITORY_TOP]->GetPos(), m_CountryColor[m_nNumPlayerNo]);
			}

			GetTerritory(m_apTerritory[TERRITORY_TOP]);					//起点のみ点数を入れる
			ResetList();												//リストを初期化する
			UninitLine();												//ラインの破棄
		}
	}
}

//=============================================================================
// テリトリーの取得を中断する処理
//=============================================================================
void CCharacter::Interruption(void)
{
	//任意にテリトリー取得を中断する
	m_nLineTime = FIRST_LINETIME;					//描画できる時間をリセットする
	ResetList();									//リストを初期化する
	UninitLine();									//ラインの破棄
}


//=============================================================================
//　図形が完成した後の処理
//=============================================================================
void CCharacter::UpdateShapeComplete(void)
{
	if (!m_bMakeShape) { return; }

	//時間の可算
	m_nCntTimeCopyLine++;

	if (m_nCntTimeCopyLine > LINE_TIME) { UninitCopyLine(); }
}

//=============================================================================
// テリトリーをリストに追加
//=============================================================================
void CCharacter::AddTerritoryList(CTerritory * pTerritory)
{
	//サウンドの取得
	CSound *pSound = CManager::GetSound();
	int nMode = CManager::GetMode();

	//図形が完成しているかどうかの判定
	if (m_nCountTerritory >= MIN_TERRITORY)
	{
		if (true == ChackSamePos(pTerritory, TERRITORY_TOP))
		{

			int nGetPoint = 0;																					//総合ポイント
			ShapeComplete(pTerritory, &nGetPoint);																//図形が完成したときの処理
			ChackInShape(&nGetPoint);																			//図形内にテリトリーあるか判定
			CreateEndLine();																					//最後の線の生成
			CPoint::Create(nGetPoint, m_apTerritory[TERRITORY_TOP]->GetPos(), m_CountryColor[m_nNumPlayerNo]);	//獲得ポイントの表示
			ResetList();																						//リストの初期化
			ResetLine();																						//ラインの初期化
			m_nCountMakeShape++;																				//図形の完成した数加算

			if (nMode != CManager::MODE_TUTORIAL)
			{
				if (m_nPointCounter >= 9)
				{//テリトリーを9個以上取得したら歓声
					pSound->PlaySound(CSound::SOUND_LABEL_SE027);
					pSound->SetVolume(CSound::SOUND_LABEL_SE027, 3.0f);
				}
			}

			//初期化する
			m_nPointCounter = 0;
			return;
		}
	}

	//リストの追加処理
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apTerritory[nCnt] == NULL)
		{
			if (m_bBlockStartTerritory)
			{//取得後、同じ起点を取らないようにする
				D3DXVECTOR3 Pos = pTerritory->GetPos();
				D3DXVECTOR3 OldStartPos = INITIALIZE_VECTOR3;
				if (m_pOldStartTerritory != NULL) { OldStartPos = m_pOldStartTerritory->GetPos(); }

				if ((nCnt != TERRITORY_TOP) && (Pos != OldStartPos))
				{
					m_apTerritory[nCnt] = pTerritory;
					m_nCountTerritory++;
				}
			}
			else
			{//通常
				m_apTerritory[nCnt] = pTerritory;
				m_nCountTerritory++;
			}

			//ラインを生成する
			if ((nCnt != 0))
			{
				if (m_apLine[nCnt - 1] == NULL)
				{
					if ((m_apTerritory[nCnt - 1] != NULL) && (m_apTerritory[nCnt] != NULL))
					{
						m_apLine[nCnt - 1] = CLine::Create(m_apTerritory[nCnt - 1]->GetPos(), m_apTerritory[nCnt]->GetPos(), m_nNumPlayerNo, nCnt - 1);
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
bool CCharacter::ChackSamePos(CTerritory * pTerritory, int nNumTerritory)
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
void CCharacter::ShapeComplete(CTerritory * pTerritory, int * nGetPoint)
{
	//図形が完成
	m_bBlockStartTerritory = true;
	m_pOldStartTerritory = pTerritory;
	m_bMakeShape = true;
	m_nLineTime = FIRST_LINETIME;		//描画できる時間をリセットする

	//テリトリーの取得
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL)
		{
			//総合ポイントに加算
			if (m_apTerritory[nCnt]->GetPlayerNumber() != m_nNumPlayerNo)
			{
				*nGetPoint += m_apTerritory[nCnt]->GetPoint();

				// ゲームクラスを取得
				CGame *pGame = CManager::GetGame();
				if (pGame != NULL)
				{
					if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((pTerritory->GetErea() + 2) % 4))
					{
						*nGetPoint += 1;
					}
				}
			}

			m_nPointCounter++;
			GetTerritory(m_apTerritory[nCnt]);
		}
	}
}

//=============================================================================
// 図形内にテリトリーがあるかどうか
//=============================================================================
void CCharacter::ChackInShape(int * nGetPoint)
{
	//変数宣言
	D3DXVECTOR3 * pListPos = NULL;
	TRAIANGLE * pTraiangle = NULL;
	int * pnFarNumTerritory = NULL;
	float * pfLength = NULL;

	pListPos = new D3DXVECTOR3[m_nCountTerritory];
	pTraiangle = new TRAIANGLE[m_nCountTerritory - 2];	//三角形の数分メモリを確保する
	pnFarNumTerritory = new int[m_nCountTerritory - 1];		//遠い順
	pfLength = new float[m_nCountTerritory - 1];			//距離
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

			if (pTerritory->GetPlayerNumber() != m_nNumPlayerNo)
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
						//総合ポイントに加算　安
						if (pTerritory->GetPlayerNumber() != m_nNumPlayerNo)
						{
							*nGetPoint += pTerritory->GetPoint(); //安

							// ゲームクラスを取得
							CGame *pGame = CManager::GetGame();
							if (pGame != NULL)
							{
								if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((pTerritory->GetErea() + 2) % 4))
								{
									*nGetPoint += 1;
								}
							}
						}

						m_nPointCounter++;
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

				if (pTerritory->GetPlayerNumber() != m_nNumPlayerNo)
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
							//総合ポイントに加算　安
							if (pTerritory->GetPlayerNumber() != m_nNumPlayerNo)
							{
								*nGetPoint += pTerritory->GetPoint(); //安

								// ゲームクラスを取得
								CGame *pGame = CManager::GetGame();
								if (pGame != NULL)
								{
									if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((pTerritory->GetErea() + 2) % 4))
									{
										*nGetPoint += 1;
									}
								}
							}

							m_nPointCounter++;
							GetTerritory(pTerritory); //安
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

}

//=============================================================================
// 三角形の構造体の初期化
//=============================================================================
void CCharacter::InitTraiangle(TRAIANGLE * pTraiangle, int nMaxTraiangle)
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
// テリトリーの取得処理
//=============================================================================
void CCharacter::GetTerritory(CTerritory * pTerritory)
{
	CSound *pSound = CManager::GetSound();

	int nOldNumPlayer = pTerritory->GetPlayerNumber();
	pTerritory->SetColor(m_CountryColor[m_nNumPlayerNo]);	//色の変更
	pTerritory->SetPlayerNumber(m_nNumPlayerNo);			//プレイヤー番号の保存
	pTerritory->SetState(CTerritory::STATE_GET);			//取得状態にする

	//前回のプレイヤーの減点処理
	if (nOldNumPlayer != -1)
	{
		m_nCountRobbtedTerritory++;	//テリトリーの奪われた数を加算
		CutTerritoryPoint(pTerritory, nOldNumPlayer);
	}
	//領地獲得音
	pSound->PlaySound(CSound::SOUND_LABEL_SE017);
	//点数の追加処理
	AddTerritoryPoint(pTerritory, nOldNumPlayer);
	//エフェクトの生成
	pTerritory->CreateGetEffect();
	//テリトリーの取得数の加算
	m_nCountGetTerritry++;
}

//=============================================================================
// テリトリーのリストを初期化
//=============================================================================
void CCharacter::ResetList(void)
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
// ラインの初期化
//=============================================================================
void CCharacter::ResetLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apLine[nCnt] = NULL;
	}
}

//=============================================================================
// ラインの破棄
//=============================================================================
void CCharacter::UninitLine(void)
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
void CCharacter::CreateEndLine(void)
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
				m_apLine[nCnt] = CLine::Create(m_apTerritory[nCountTerritory]->GetPos(), m_apTerritory[TERRITORY_TOP]->GetPos(), m_nNumPlayerNo, nCnt);
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
void CCharacter::UninitCopyLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apCopyLine[nCnt] != NULL)
		{
			bool bDeth = m_apCopyLine[nCnt]->ColChange();

			if (bDeth)
			{//死亡フラグが有効だったら
				m_apCopyLine[nCnt]->Uninit();
				m_apCopyLine[nCnt] = NULL;
				m_nCntTimeCopyLine = 0;
				m_bMakeShape = false;
			}
		}
	}
}

//=============================================================================
// ラインの切断処理
//=============================================================================
void CCharacter::CutLine(int nID)
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
	//ライン切断音
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_SE029);
	pSound->SetVolume(CSound::SOUND_LABEL_SE029, 30.0f);
}

//=============================================================================
//! 三角形を作る処理
//=============================================================================
void CCharacter::MakeTraiangle(int nCnt, int * nCountTraiangle, int * pnFarNumTerritory, D3DXVECTOR3 * pListPos, TRAIANGLE * pTraiangle)
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

		//三角形ポリゴンを生成する
		MakePolygon(pTraiangle[*nCountTraiangle]);

		//ポインタ内の数値を加算
		*nCountTraiangle += 1;
	}
}

//=============================================================================
// 三角形のポリゴン生成
//=============================================================================
void CCharacter::MakePolygon(TRAIANGLE pTraiangle)
{
	CSceneTriangle3D * pSceneTriangle = CSceneTriangle3D::Create(pTraiangle.pos[0], pTraiangle.pos[1], pTraiangle.pos[2]);

	CGame * pGame = CManager::GetGame();

	if (pGame == NULL) { return; }
	int nNumPlayer = m_nNumPlayerNo;
	pSceneTriangle->SetColor(m_CountryColor[nNumPlayer]);
}

//=============================================================================
// テリトリーの得点を削除する処理
//=============================================================================
void CCharacter::CutTerritoryPoint(CTerritory * pTerritory, int nOldPlayer)
{
	// 得点を取得
	if (pTerritory == NULL) { return; }
	int nPoint = pTerritory->GetPoint();

	// ゲームクラスを取得
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL || CManager::GetMode() != CManager::MODE_GAME) { return; }

	// UIクラスを取得
	CUI *pUI = pGame->GetUI();
	if (pUI == NULL) { return; }

	// ゲームスコアクラスを取得
	CScoreGame *pScoreGame = pUI->GetScoreGame(nOldPlayer);
	if (pScoreGame == NULL) { return; }

	// スコアを減点
	pScoreGame->AddScore(-nPoint);

	// ゲージを変動させる
	CScoreGauge *pScoreGauge = pUI->GetScoreGauge();
	if (pScoreGauge == NULL) { return; }
	pScoreGauge->CutGauge(nOldPlayer, nPoint);
	if (m_nNumPlayerNo == nOldPlayer) { return; }

	// スコアの変動を設定
	CCharacter *pChara = pGame->GetChara(nOldPlayer);
	if (pChara == NULL) { return; }
	pChara->SetGetScorePoint(pChara->GetGetScorePoint() + nPoint);
	pChara->SetDestPlayer(m_nNumPlayerNo);
}

//=============================================================================
// 光の軌跡を作る処理
//=============================================================================
void CCharacter::CreateLightOrbit(CGame *pGame)
{
	// キャラクタークラスを取得
	CCharacter *pChara = pGame->GetChara(m_nDestPlayer);
	if (pChara == NULL) { return; }

	// 光の軌跡を生成
	CLightOrbit::Create(m_Pos, pChara->GetPosPointer(), m_nDestPlayer);
}

//=============================================================================
// テリトリーの得点を追加する処理
//=============================================================================
void CCharacter::AddTerritoryPoint(CTerritory * pTerritory,int nOldPlayer)
{
	// 得点を取得
	if (pTerritory == NULL) { return; }
	int nPoint = pTerritory->GetPoint();

	// ゲームクラスを取得
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL || CManager::GetMode() != CManager::MODE_GAME) { return; }

	// UIクラスを取得
	CUI *pUI = pGame->GetUI();
	if (pUI == NULL) { return; }

	// ゲームスコアクラスを取得
	CScoreGame *pScoreGame = pUI->GetScoreGame(m_nNumPlayerNo);
	if (pScoreGame == NULL) { return; }

	// スコアを加算
	pScoreGame->AddScore(nPoint);

	// ボーナスイベント中ならさらに加算
	if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((pTerritory->GetErea() + 2) % 4)
		&& m_nNumPlayerNo != nOldPlayer)
	{// イベント中 && 番号が一致している && 自分が保有していなかったテリトリーだった
		pScoreGame->AddScore(1);
	}

	// 順位のソート
	pUI->SortRankFromScore();

	// ゲージを変動させる
	CScoreGauge *pScoreGauge = pUI->GetScoreGauge();
	if (pScoreGauge == NULL) { return; }
	pScoreGauge->AddGauge(m_nNumPlayerNo, nPoint);
}