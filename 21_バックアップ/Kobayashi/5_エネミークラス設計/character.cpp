//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					キャラクター管理 [character.cpp]
//			Author : Kobayashi_Sho-hei / 小林 将兵
//	◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆

//	---<<#include>>---
#include "character.h"
#include "model.h"
#include "motion.h"
#include "manager.h"
#include "renderer.h"

//	---<<静的メンバ変数>>---
LPDIRECT3DTEXTURE9 CCharacter::m_pTexture = NULL;
//char *CCharacter::m_pUVTexName = NULL;
//int CCharacter::m_nPartsNum = 0;							//	パーツ数
//char **CCharacter::m_aPartsName = {};						//	パーツ名
//int *CCharacter::m_aIndex= {};								//	インデックス
//int *CCharacter::m_nParent = {};							//	親番号
//CCharacter::MODEL_OFFSET *CCharacter::m_aOffset = {};		//	パーツのオフセット
//CMotion::MOTION_INFO *CCharacter::m_pMotionInfo = {};

//int CCharacter::m_nNumMotionMAX = 0;
//CMotion *CCharacter::m_pMotion = NULL;
//==============================================
//					コンストラクタ
//==============================================
CCharacter::CCharacter()
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
HRESULT CCharacter::Init(char ModelTxt[40], char MotionTxt[40])
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
		m_ppModel[nCnt]->BindTexture(m_pTexture);
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

	return S_OK;
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
				delete m_pMotionInfo[nCnt].m_pKeyInfo[nCnt02].aKey;
				m_pMotionInfo[nCnt].m_pKeyInfo[nCnt02].aKey = NULL;
			}
		}
	}
	//	キーフレーム情報の解放(モーションの種類分)
	for (int nCnt = 0; nCnt < m_nNumMotionMAX; nCnt++)
	{		if (m_pMotionInfo[nCnt].m_pKeyInfo != NULL)
		{
			delete m_pMotionInfo[nCnt].m_pKeyInfo;
			m_pMotionInfo[nCnt].m_pKeyInfo = NULL;
		}
	}
	//	モーション情報の解放
	if (m_pMotionInfo != NULL)
	{
		delete m_pMotionInfo;
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
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_aOffset != NULL)
		{
			delete m_aOffset;
			m_aOffset = NULL;
		}
	}
	//	ペアレント情報
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_nParent != NULL)
		{
			delete m_nParent;
			m_nParent = NULL;
		}
	}
	//	インデックス情報
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_aIndex != NULL)
		{
			delete m_aIndex;
			m_aIndex = NULL;
		}
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

	if (m_pUVTexName != NULL)
	{
		delete[] m_pUVTexName;
		m_pUVTexName = NULL;
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
							m_pUVTexName = new char[256];	//	文字列の確保
							strcpy(&m_pUVTexName[0], pStrCur);
						}

						//デバイス情報の取得
						LPDIRECT3DDEVICE9 pDevice;
						pDevice = CManager::GetRenderer()->GetDevice();
						D3DXCreateTextureFromFile(pDevice, m_pUVTexName, &m_pTexture);
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
		}

		
		while (1)
		{
			if (m_pMotionInfo == NULL)
			{
				m_pMotionInfo = new CMotion::MOTION_INFO[m_nNumMotionMAX];
			}
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
					if (m_pMotionInfo[nMotionNum].m_pKeyInfo != NULL)
					{
						m_pMotionInfo[nMotionNum].m_pKeyInfo = new CMotion::KEY_INFO[m_pMotionInfo[nMotionNum].nNumKey];	//	キーの数分確保
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
////					生成
////==============================================
//CCharacter* CCharacter::Create()
//{
//	CCharacter * pCharacter = NULL;
//	if (pCharacter == NULL)
//	{
//		pCharacter = new CCharacter;
//		pCharacter->Init("data\\TEXT\\PLAYER_LODE.txt", "data\\TEXT\\PLAYER_MOTION.txt");
//	}
//
//	return pCharacter;
//}
////=============================================================================
////
////=============================================================================
//void  CCharacter::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
//{
//
//}