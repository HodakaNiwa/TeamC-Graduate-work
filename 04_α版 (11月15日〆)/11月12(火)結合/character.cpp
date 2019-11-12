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
#include "sceneOrbit.h"
#include "library.h"
#include "territory.h"
#include "sound.h"
#include "UI.h"
#include "score.h"

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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//静的メンバ変数宣言
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//国の色の設定
D3DXCOLOR CCharacter::m_CountryColor[CCharacter::TYPE_MAX] =
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

//==============================================
//					コンストラクタ
//==============================================
CCharacter::CCharacter(int nPriority, OBJTYPE objType) : CScene(nPriority,objType)
{

}


//==============================================
//					デストラクタ
//==============================================
CCharacter::~CCharacter()
{

}

//=============================================================================
// モデルの破棄
//=============================================================================
void CCharacter::Unload(void)
{

}


//==============================================
//					初期化
////==============================================
HRESULT CCharacter::Init(int nCharaNum, char ModelTxt[40], char MotionTxt[40])
{
	m_cModelTxt[0] = &ModelTxt[0];
	m_cMotionTxt[0] = &MotionTxt[0];
	ModelSetLode();

	m_ppModel = new CModel*[m_nPartsNum];		//	パーツ分確保する

	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		//	モデルパーツの初期値を決める
		m_ppModel[nCnt] = CModel::Create(D3DXVECTOR3(m_aOffset[nCnt].pos.x, m_aOffset[nCnt].pos.y, m_aOffset[nCnt].pos.z), m_aPartsName[nCnt], D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		//	モデルパーツを決める
		m_ppModel[nCnt]->BindModel(m_ppModel[nCnt]->GetMesh(), m_ppModel[nCnt]->GetBuffMat(), m_ppModel[nCnt]->GetNumMat());
		//	モデルUVテクスチャを決める
		m_ppModel[nCnt]->BindTexture(m_ppTexture[m_nTexIdx[nCnt]]);
		//	各モデルパーツの親を決める
		if (nCnt == 0)
		{
			m_ppModel[nCnt]->SetParent(NULL);
		}
		else
		{
			m_ppModel[nCnt]->SetParent(m_ppModel[m_nParent[nCnt]]);
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
	SetCOlliderManager(pColManager);
}


//==============================================
//					終了
//==============================================
void CCharacter::Uninit()
{
	//!+ ---<<モーション情報の解放>>---
	//	キーフレームの位置・回転情報の解放(モーションの種類分 / キー分)
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
		for (int nCntTex = 0; nCntTex < m_nTexNum; nCntTex++)
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

	//拡大縮小行列の作成
	//D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		m_ppModel[nCnt]->Draw(1.0f);
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
						m_ppTexture = new LPDIRECT3DTEXTURE9[m_nTexNum];
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
							m_ppUVTexName[nCntLoadTex] = new char[256];	//	文字列の確保
							strcpy(m_ppUVTexName[nCntLoadTex], pStrCur);
						}

						//デバイス情報の取得
						LPDIRECT3DDEVICE9 pDevice;
						pDevice = CManager::GetRenderer()->GetDevice();
						D3DXCreateTextureFromFile(pDevice, m_ppUVTexName[nCntLoadTex], &m_ppTexture[nCntLoadTex]);
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
						m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey = new CMotion::KEY[m_nPartsNum];	//	モデルのパーツ分確保
						ClearMotionInfo_KeyInfo_Key(nMotionNum, nKeyNum);

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
void CCharacter::UpdateShapeComplete(void)
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
void CCharacter::AddTerritoryList(CTerritory * pTerritory)
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
void CCharacter::ShapeComplete(CTerritory * pTerritory)
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
void CCharacter::ChackInShape(void)
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

															//前回のプレイヤーの減点処理
	if (nOldNumPlayer != -1)
	{
		CutTerritoryPoint(pTerritory, nOldNumPlayer);
	}
	//領地獲得音
	pSound->PlaySound(CSound::SOUND_LABEL_SE017);
	//点数の追加処理
	AddTerritoryPoint(pTerritory);
	//エフェクトの生成
	pTerritory->CreateGetEffect();
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
			m_apCopyLine[nCnt]->Uninit();
			m_apCopyLine[nCnt] = NULL;
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

		//ポインタ内の数値を加算
		*nCountTraiangle += 1;
	}
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
}



//=============================================================================
// テリトリーの得点を追加する処理
//=============================================================================
void CCharacter::AddTerritoryPoint(CTerritory * pTerritory)
{
	//// 得点を取得
	//if (pTerritory == NULL) { return; }
	//int nPoint = pTerritory->GetPoint();

	//// ゲームクラスを取得
	//CGame *pGame = CManager::GetGame();
	//if (pGame == NULL || CManager::GetMode() != CManager::MODE_GAME) { return; }

	//// UIクラスを取得
	//CUI *pUI = pGame->GetUI();
	//if (pUI == NULL) { return; }

	//// ゲームスコアクラスを取得
	//CScoreGame *pScoreGame = pUI->GetScoreGame(m_nNumPlayer);
	//if (pScoreGame == NULL) { return; }

	//// スコアを加算
	//pScoreGame->AddScore(nPoint);
}