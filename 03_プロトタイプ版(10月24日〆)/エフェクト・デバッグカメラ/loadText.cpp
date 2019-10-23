//=============================================================================
//
// テキストの読み込み処理 [LoadText.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "loadText.h"
#include "manager.h"
#include "Territory.h"
#include "objbase.h"
#include "sceneOrbit.h"
#include "particle.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//ゼロベクトル
#define FILE_NAME_ORBIT ("data//TEXT//ORBIT//orbit.txt")


//=============================================================================
// 生成処理
//=============================================================================
CLoadText * CLoadText::Create(void)
{
	//インスタンスの生成
	CLoadText * pLoadText = NULL;

	pLoadText->Init();
	return pLoadText;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadText::CLoadText()
{
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadText::~CLoadText(){}

//=============================================================================
// 列の読み込み
//=============================================================================
char * CLoadText::ReadLine(FILE * pFile, char * pDest)
{
	//変数宣言
	char * pStrCur = NULL;

	while (1)
	{//行を飛ばす
		fgets(pDest, 256, pFile);					//一行分抜き出す
		pStrCur = pDest;							//先頭の文字列を保管する

		if (false == ((*pStrCur != '\0') && (*pStrCur != '\n') && (*pStrCur != '#'))) { continue; }

		while (1)
		{//列に有効な文字がないがチェックする
			if ((*pStrCur != '\n') && (*pStrCur != '\t') && (*pStrCur != ' ') && (*pStrCur != '#') && (*pStrCur != '\0'))
			{
				break;
			}

			pStrCur++;	//一文字ずつ進める
		}
		break;
	}

	return pStrCur;
}

//=============================================================================
// 先頭ポインタの文字を進める
//=============================================================================
char * CLoadText::AddChar(char * pStrCur, char * aStrCur, int nAddChar)
{
	pStrCur += nAddChar;						//先頭アドレスを文字数分進める
	strcpy(aStrCur, pStrCur);					//文字列を保管する

	return pStrCur;
}

//=============================================================================
// 文字列を抜き出す
//=============================================================================
int CLoadText::PopString(char * pStrCur, char * aStrCur)
{
	//変数宣言
	int nCounterWord = 0;	//進んだ文字をカウントする
	while (1)
	{
		aStrCur++;
		nCounterWord++;

		if ((*aStrCur == ' ') || (*aStrCur == '\t') || (*aStrCur == '#') || (*aStrCur == '\n'))
		{
			aStrCur[0] = '\0';
			nCounterWord++;
			break;
		}
	}

	return nCounterWord;
}

//=============================================================================
// ヌル文字の追加
//=============================================================================
void CLoadText::AddNull(char * pLine)
{
	//変数宣言
	char * cTop = &pLine[0];
	int nCounter = 0;

	while (1)
	{
		cTop++;
		nCounter++;

		if ((*cTop == '\n') || (*cTop == '\t') || (*cTop == ' ') || (*cTop == '#'))
		{
			pLine[nCounter] = '\0';	//無効文字を \0( ヌル文字 ) に置き換える
			break;
		}
	}
}

//*****************************************************************************
//
// オブジェクト情報読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLoadTextObject * CLoadTextObject::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextObject * pLoadTextObject = new CLoadTextObject;
	pLoadTextObject->SetLoadTextFileName(pFileName);			//読み込むテキストファイル名の保管
	pLoadTextObject->Init();

	return pLoadTextObject;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextObject::CLoadTextObject() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextObject::~CLoadTextObject() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextObject::Init(void)
{
	//変数の初期化
	m_pStrCur = NULL;					//文字列の先頭へのポインタ
	m_aLine[256] = {};						//文字列読み込み用（一行分）
	m_aStrCur[256] = {};						//文字列抜き出し用
	m_TexInfo.ppFileName = NULL;
	m_ObjInfo.nMaxModel = 0;
	int nTex = 0;
	int nModeltype = 0;
	int nType = 0;
	D3DXVECTOR3 Pos = VECTOR_ZERO;
	D3DXVECTOR3 Rot = VECTOR_ZERO;
	D3DXVECTOR3 Scale = VECTOR_ZERO;


	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//テキストの読み込み

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
			strcpy(m_aStrCur, m_pStrCur);

			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)		//テクスチャの最大数の取得
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//テクスチャファイル名を動的確保する
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				//文字数分メモリを動的確保する
				if (m_TexInfo.ppFileName != NULL)
				{
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
					{
						m_TexInfo.ppFileName[nCntTex] = new char[256];
					}
				}

				//テクスチャのファイル名を取得
				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("テキストファイルポインタがNULLです"), TEXT("CLoadTextObject::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);				//ファイル名の最後にNULLを追加する
				}

				//テクスチャの読み込み処理
				CObjBase::LoadTex(m_TexInfo);

			}
			else if (memcmp(m_aStrCur, "MAX_MODEL = ", 12) == 0)		//モデルファイルの最大数
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
				m_ObjInfo.nMaxModel = atoi(m_pStrCur);

				//モデルファイル名分メモリを動的確保する
				if (m_ObjInfo.ppModelFileName == NULL)
				{
					m_ObjInfo.ppModelFileName = new char *[m_ObjInfo.nMaxModel];
				}
				if (m_ObjInfo.ppModelFileName != NULL)
				{
					for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)
					{
						m_ObjInfo.ppModelFileName[nCntModel] = new char[256];
					}
				}

				//モデルのファイル名を保存する
				for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)
				{
					if (m_ObjInfo.ppModelFileName[nCntModel] == NULL)
					{
						MessageBox(NULL, TEXT("モデルファイルポインタがNULLです"), TEXT("CLoadTextObject::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "MODELNAME = ", 12) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
					strcpy(m_ObjInfo.ppModelFileName[nCntModel], m_pStrCur);
					AddNull(m_ObjInfo.ppModelFileName[nCntModel]);			//ファイル名の最後にNULLを追加する
				}

				//モデルの読み込み
				CObjBase::LoadModel(m_ObjInfo);
			}
			else if (memcmp(m_aStrCur, "SET", 3) == 0)
			{
				while (memcmp(m_aStrCur, "END_SET", 7) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "MODELTYPE = ", 12) == 0)			//モデルタイプの取得
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
						nModeltype = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 6);
						nTex = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "POS = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "ROT = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "SCALE = ", 8) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "TYPE = ", 7) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 7);
						nType = atoi(m_pStrCur);
					}
				}

				//オブジェクトを生成する
				CObjBase::Create(Pos, Rot, Scale, nTex, nModeltype, (CObjBase::TYPE)nType);
				CObjBase::SetAddMaxObj();
			}
		}
		fclose(pFile);	//ファイル読み込みを終わる
	}
	else
	{
		MessageBox(NULL, TEXT("pFileがNULLです"), TEXT("CLoadTextObject::Init()"), MB_OK);
	}

	//初期化する
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextObject::Uninit(void)
{
	//テクスチャ情報の破棄
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] == NULL) { continue; }

		delete[] m_TexInfo.ppFileName[nCntTex];
		m_TexInfo.ppFileName[nCntTex] = NULL;
	}

	//テクスチャのファイルポインタの破棄
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}

	//オブジェクト情報を破棄する
	for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)		//モデルファイル名の破棄
	{
		if (m_ObjInfo.ppModelFileName[nCntModel] == NULL) { continue; }

		delete[] m_ObjInfo.ppModelFileName[nCntModel];
		m_ObjInfo.ppModelFileName[nCntModel] = NULL;
	}
	if (m_ObjInfo.ppModelFileName != NULL)
	{
		delete[] m_ObjInfo.ppModelFileName;
		m_ObjInfo.ppModelFileName = NULL;
	}

	//モデル・テクスチャの破棄
	CObjBase::UnloadModel();
	CObjBase::UnloadTex();
}

//*****************************************************************************
//
// テリトリー情報読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLoadTextTerritory * CLoadTextTerritory::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextTerritory * pLoadTextObject = new CLoadTextTerritory;
	pLoadTextObject->SetLoadTextFileName(pFileName);			//読み込むテキストファイル名の保管
	pLoadTextObject->Init();

	return pLoadTextObject;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextTerritory::CLoadTextTerritory() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextTerritory::~CLoadTextTerritory() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextTerritory::Init(void)
{
	//変数の初期化
	m_pStrCur = NULL;					//文字列の先頭へのポインタ
	m_aLine[256] = {};					//文字列読み込み用（一行分）
	m_aStrCur[256] = {};				//文字列抜き出し用
	m_TexInfo.ppFileName = NULL;
	m_ObjInfo.nMaxModel = 0;
	int nTex = 0;
	int nModeltype = 0;
	int nType = 0;
	int nPoint = 0;
	D3DXVECTOR3 Pos = VECTOR_ZERO;
	D3DXVECTOR3 Rot = VECTOR_ZERO;
	D3DXVECTOR3 Scale = VECTOR_ZERO;


	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//テキストの読み込み

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
			strcpy(m_aStrCur, m_pStrCur);

			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)		//テクスチャの最大数の取得
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//テクスチャファイル名を動的確保する
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				//文字数分メモリを動的確保する
				if (m_TexInfo.ppFileName != NULL)
				{
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
					{
						m_TexInfo.ppFileName[nCntTex] = new char[256];
					}
				}

				//テクスチャのファイル名を取得
				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("テキストファイルポインタがNULLです"), TEXT("CLoadTextTerritory::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);				//ファイル名の最後にNULLを追加する
				}

				//テクスチャの読み込み処理
				CTerritory::LoadTex(m_TexInfo);

			}
			else if (memcmp(m_aStrCur, "MAX_MODEL = ", 12) == 0)		//モデルファイルの最大数
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
				m_ObjInfo.nMaxModel = atoi(m_pStrCur);

				//モデルファイル名分メモリを動的確保する
				if (m_ObjInfo.ppModelFileName == NULL)
				{
					m_ObjInfo.ppModelFileName = new char *[m_ObjInfo.nMaxModel];
				}
				if (m_ObjInfo.ppModelFileName != NULL)
				{
					for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)
					{
						m_ObjInfo.ppModelFileName[nCntModel] = new char[256];
					}
				}

				//モデルのファイル名を保存する
				for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)
				{
					if (m_ObjInfo.ppModelFileName[nCntModel] == NULL)
					{
						MessageBox(NULL, TEXT("モデルファイルポインタがNULLです"), TEXT("CLoadTextTerritory::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "MODELNAME = ", 12) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
					strcpy(m_ObjInfo.ppModelFileName[nCntModel], m_pStrCur);
					AddNull(m_ObjInfo.ppModelFileName[nCntModel]);			//ファイル名の最後にNULLを追加する
				}

				//モデルの読み込み
				CTerritory::LoadModel(m_ObjInfo);
			}
			else if (memcmp(m_aStrCur, "SET", 3) == 0)
			{
				while (memcmp(m_aStrCur, "END_SET", 7) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "MODELTYPE = ", 12) == 0)			//モデルタイプの取得
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
						nModeltype = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 6);
						nTex = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "POS = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "ROT = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "SCALE = ", 8) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "POINT = ", 8) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 8);
						nPoint = atoi(m_pStrCur);
					}
				}

				//オブジェクトを生成する
				CTerritory::Create(Pos, Rot, Scale, nTex, nModeltype, nPoint);
				CTerritory::SetAddMaxObj();
			}
		}
		fclose(pFile);	//ファイル読み込みを終わる
	}
	else
	{
		MessageBox(NULL, TEXT("pFileがNULLです"), TEXT("CLoadTextTerritory::Init()"), MB_OK);
	}

	//初期化する
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextTerritory::Uninit(void)
{
	//テクスチャ情報の破棄
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] == NULL) { continue; }

		delete[] m_TexInfo.ppFileName[nCntTex];
		m_TexInfo.ppFileName[nCntTex] = NULL;
	}

	//テクスチャのファイルポインタの破棄
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}

	//オブジェクト情報を破棄する
	for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)		//モデルファイル名の破棄
	{
		if (m_ObjInfo.ppModelFileName[nCntModel] == NULL) { continue; }

		delete[] m_ObjInfo.ppModelFileName[nCntModel];
		m_ObjInfo.ppModelFileName[nCntModel] = NULL;
	}
	if (m_ObjInfo.ppModelFileName != NULL)
	{
		delete[] m_ObjInfo.ppModelFileName;
		m_ObjInfo.ppModelFileName = NULL;
	}

	//モデル・テクスチャの破棄
	CTerritory::UnloadModel();
	CTerritory::UnloadTex();
}

//*****************************************************************************
//
// テクスチャのファイル名読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLoadTextTexture * CLoadTextTexture::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextTexture * pLoadTextObject = new CLoadTextTexture;

	pLoadTextObject->SetLoadTextFileName(pFileName);	//読み込むファイル名を保管する
	pLoadTextObject->Init();

	return pLoadTextObject;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextTexture::CLoadTextTexture() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextTexture::~CLoadTextTexture() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextTexture::Init(void)
{
	//変数の初期化
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
	m_TexInfo.ppFileName = NULL;
	m_pPolygonInfo = NULL;
	m_nCountPoly = 0;

	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//テキストの読み込み

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
			strcpy(m_aStrCur, m_pStrCur);

			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)
			{
				//テクスチャの最大数の取得
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//テクスチャファイル名を動的確保する
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				if (m_TexInfo.ppFileName != NULL)
				{
					//文字数分メモリを動的確保する
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++) { m_TexInfo.ppFileName[nCntTex] = new char[256]; }
				}

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("テキストファイルポインタがNULLです"), TEXT("CLoadTextTexture::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }		//テクスチャのファイル名を取得

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);			//ファイル名の最後にNULLを追加する
				}
			}
			else if (memcmp(m_aStrCur, "MAX_UI = ", 9) == 0)
			{
				//テクスチャの最大数の取得
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 9);
				m_nMaxPolygon = atoi(m_pStrCur);

				//メモリを動的確保する
				if (m_pPolygonInfo == NULL)
				{
					m_pPolygonInfo = new POLYGON_INFO[m_nMaxPolygon];
				}
			}
			else if (memcmp(m_aStrCur, "SET_UI", 6) == 0)
			{
				while (memcmp(m_aStrCur, "END_UI", 6) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);
					
					if (memcmp(m_aStrCur, "POS = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						m_pPolygonInfo[m_nCountPoly].pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						m_pPolygonInfo[m_nCountPoly].pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						m_pPolygonInfo[m_nCountPoly].pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "SIZE = ", 7) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						m_pPolygonInfo[m_nCountPoly].size.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						m_pPolygonInfo[m_nCountPoly].size.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						m_pPolygonInfo[m_nCountPoly].size.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 6);
						m_pPolygonInfo[m_nCountPoly].nTex = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "TYPE = ", 7) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 7);
						m_pPolygonInfo[m_nCountPoly].type = (CScene2D::TYPE)atoi(m_pStrCur);
						m_nCountPoly++;
					}
				}
				
			}
		}
		fclose(pFile);	//ファイル読み込みを終わる
	}
	else
	{
		MessageBox(NULL, TEXT("pFileがNULLです"), TEXT("CLoadTextTexture::Init()"), MB_OK);
	}

	//初期化する
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextTexture::Uninit(void)
{
	//テクスチャ情報の破棄
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] == NULL) { continue; }

		delete[] m_TexInfo.ppFileName[nCntTex];
		m_TexInfo.ppFileName[nCntTex] = NULL;
	}

	//テクスチャのファイルポインタの破棄
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}

	//ポリゴン情報を破棄
	if (m_pPolygonInfo != NULL)
	{
		delete[] m_pPolygonInfo;
		m_pPolygonInfo = NULL;
	}
}

//*****************************************************************************
//
// 奇跡の読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLoadTextOrbit * CLoadTextOrbit::Create(void)
{
	//インスタンスの生成
	CLoadTextOrbit * pLoadTextObject = new CLoadTextOrbit;
	pLoadTextObject->Init();

	return pLoadTextObject;
}


//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextOrbit::CLoadTextOrbit() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextOrbit::~CLoadTextOrbit() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextOrbit::Init(void)
{
	//変数宣言
	int nMaxOrbit;
	FILE *pFile;

	//変数の初期化
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
	m_pOrbit = NULL;

	pFile = fopen(FILE_NAME_ORBIT, "r");	//テキストの読み込み

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
			strcpy(m_aStrCur, m_pStrCur);

			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)		//テクスチャの最大数
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//ファイル名のポインタを動的確保する
				m_TexInfo.ppFileName = new char*[m_TexInfo.nMaxTex];	//行数分確保する

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					m_TexInfo.ppFileName[nCntTex] = new char[256];	//文字数分確保する
				}

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

					if (memcmp(m_aLine, "FILE_NAME = ", 12) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);
				}
			}
			else if (memcmp(m_aStrCur, "MAX_ORBIT = ", 12) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
				nMaxOrbit = atoi(m_pStrCur);

				//軌跡情報を動的確保する
				if (m_pOrbit == NULL)
				{
					m_pOrbit = new CSceneOrbit::ORBIT_INFO[nMaxOrbit];
				}
			}
			else if (memcmp(m_aStrCur, "ORBITSET", 8) == 0)
			{
				while (memcmp(m_aStrCur, "END_ORBITSET", 12) != 0)
				{
					int nIndex = 0;

					while (memcmp(m_aStrCur, "END_SET", 7) != 0)
					{
						m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
						strcpy(m_aStrCur, m_pStrCur);

						if (memcmp(m_aStrCur, "INDEX = ", 8) == 0)		//インデックスの読み込み
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							nIndex = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
							m_pOrbit[nIndex].nTex = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "POS[0] = ", 9) == 0)	//オフセット[0]読み込む
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 9);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[0].x = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[0].y = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[0].z = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "POS[1] = ", 9) == 0) //オフセット[1]読み込む
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 9);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[1].x = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[1].y = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[1].z = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "PRIMITIV = ", 11) == 0) //頂点数を読み込む
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pOrbit[nIndex].nPrimitiv = atoi(m_pStrCur);

							//ポリゴン数の設定
							m_pOrbit[nIndex].nPorigon = m_pOrbit[nIndex].nPrimitiv - 2;
						}
						else if (memcmp(m_aStrCur, "COLOR = ", 8) == 0)	//色情報を読み込む
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].color.r = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].color.g = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].color.b = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].color.a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "UV = ", 5) == 0)		//UV値の読み込み
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 5);
							m_pOrbit[nIndex].fUV = (float)atof(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "ANIM = ", 7) == 0)	//アニメーション値の読み込み
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pOrbit[nIndex].fAnim = (float)atof(m_pStrCur);
						}
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);
				}
			}
		}
		fclose(pFile);
	}
	else
	{
		MessageBox(NULL, TEXT("pFileがNULLです"), TEXT("LoadTextOrbit::Init()"), MB_OK);
	}

	//初期化する
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextOrbit::Uninit(void)
{
	//ポインタの破棄
	if (m_pOrbit != NULL)
	{
		delete[] m_pOrbit;
		m_pOrbit = NULL;
	}

	//テクスチャ情報の破棄
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] == NULL) { continue; }

		delete[] m_TexInfo.ppFileName[nCntTex];
		m_TexInfo.ppFileName[nCntTex] = NULL;
	}

	//テクスチャのファイルポインタの破棄
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}
}

//*****************************************************************************
//
// エフェクト読み込みクラス
//
//*****************************************************************************

//=============================================================================
// 生成処理
//=============================================================================
CLoadTextEffect * CLoadTextEffect::Create(char * pFileName)
{
	//インスタンスの生成
	CLoadTextEffect * pLoadTextEffect = new CLoadTextEffect;

	pLoadTextEffect->SetLoadTextFileName(pFileName);	//読み込むファイル名を保管する
	pLoadTextEffect->Init();

	return pLoadTextEffect;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadTextEffect::CLoadTextEffect() {}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadTextEffect::~CLoadTextEffect() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLoadTextEffect::Init(void)
{
	//変数宣言
	int	 nCountParticle = 0;				//カウンター
	int	 nCountEffect = 0;					//エフェクトカウンター

											//変数の初期化
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
	m_TexInfo.ppFileName = NULL;
	m_nMaxEffect = 0;
	m_pEffect = NULL;

	//ファイル宣言
	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//テキストの読み込み

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
			strcpy(m_aStrCur, m_pStrCur);

			//----------------------------------
			//テクスチャの最大数・情報の読み込み
			//----------------------------------
			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//テクスチャファイル名を動的確保する
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				if (m_TexInfo.ppFileName != NULL)
				{
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
					{//文字数分メモリを動的確保する
						m_TexInfo.ppFileName[nCntTex] = new char[256];
					}
				}

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("テクスチャファイルポインタがNULLです"), TEXT("CLoadTextEffect::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }

					//テクスチャのファイル名を取得
					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);			//ファイル名の最後にNULLを追加する
				}

				m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
				strcpy(m_aStrCur, m_pStrCur);
			}

			//----------------------------------
			//エフェクトの最大数・情報の初期化
			//----------------------------------
			if (memcmp(m_aStrCur, "MAX_EFFECT = ", 13) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
				m_nMaxEffect = atoi(m_pStrCur);

				//メモリの動的確保
				if (m_pEffect == NULL) { m_pEffect = new CEffectTool::Effect[m_nMaxEffect]; }	//エフェクトの個数分メモリを確保する

																								//変数の初期化
				for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
				{
					if (m_pEffect != NULL)
					{
						m_pEffect[nCntEffect].EffectInfo = new CEffectTool::Effect_Info[MAX_EFFECT];
					}

					if (m_pEffect[nCntEffect].EffectInfo != NULL)
					{
						for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++)
						{
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bCreateEffect = false;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bLoop = false;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bUseGravity = true;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bZbuffur = true;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].DrawType = CParticle::DRAWTYPE_ADD;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fChangeCol_a = 0.15f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fGravity = 0.01f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fSpeed = 2.0f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nInterbalTime = 10;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nCountTime = 0;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nLife = 50;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMaxHight = 3;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMinHight = 2;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nNumCreate = 1;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nTexNum = 0;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].size = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].Type = CParticle::TYPE_SPEW;
						}
					}
				}
			}

			//-------------------------------------------
			//エフェクトの設定
			//-------------------------------------------
			if (memcmp(m_aStrCur, "SETEFFECT", 9) == 0)
			{
				while (memcmp(m_aLine, "END_EFFECT", 10) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "SETPARTICLE", 11) != 0) { continue; }
					while (memcmp(m_aStrCur, "END_PARTICLE", 12) != 0)
					{
						if (memcmp(m_aStrCur, "LOOP = ", 7) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bLoop = (atoi(m_pStrCur) != 0);	//ループ状態の設定

							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bCreateEffect = true;			//生成状態の設定
						}
						else if (memcmp(m_aStrCur, "SIZE = ", 7) == 0)						//サイズの設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.x = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.y = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.z = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)						//テクスチャ番号の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nTexNum = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "DRAWTYPE = ", 11) == 0)					//描画タイプの設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].DrawType = (CParticle::DRAWTYPE)atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "TYPE = ", 7) == 0)						//タイプの設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Type = (CParticle::TYPE)atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "COLOR = ", 8) == 0)						//色の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.r = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.g = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.b = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "ZBUFFER = ", 10) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bZbuffur = (atoi(m_pStrCur) != 0);	//Zバッファの設定
						}
						else if (memcmp(m_aStrCur, "LIFE = ", 7) == 0)								//表示時間の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nLife = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "CHANGECOLOR = ", 14) == 0)						//透明度の変化量
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 14);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fChangeCol_a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "USEGRAVITY = ", 13) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bUseGravity = (atoi(m_pStrCur) != 0);	//重力の状態設定
						}
						else if (memcmp(m_aStrCur, "TIME = ", 7) == 0)									//生成時間の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nInterbalTime = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "NUMCREATE = ", 12) == 0)								//生成量の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nNumCreate = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "SPEED = ", 8) == 0)									//スピードの設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fSpeed = (float)atof(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "MINHIGHT = ", 11) == 0)								//高さの最小値の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nMinHight = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "MAXHIGHT = ", 11) == 0)								//高さの最大値の設置
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nMaxHight = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "GRAVITY = ", 10) == 0)								//重力の設定
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fGravity = (float)atof(m_pStrCur);
						}

						m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//無効文字を飛ばす
						strcpy(m_aStrCur, m_pStrCur);
					}

					//カウンターを進める
					nCountParticle++;
				}

				nCountEffect++;			//エフェクトカウンターを進める
				nCountParticle = 0;		//パーティクルカウンターの初期化
			}
		}
		fclose(pFile);	//ファイル読み込みを終わる
	}
	else
	{
		MessageBox(NULL, TEXT("pFileがNULLです"), TEXT("CLoadTextObject::Init()"), MB_OK);
	}

	//初期化する
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLoadTextEffect::Uninit(void)
{
	//エフェクトの情報破棄
	for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
	{
		if (m_pEffect[nCntEffect].EffectInfo != NULL)
		{
			delete[] m_pEffect[nCntEffect].EffectInfo;
			m_pEffect[nCntEffect].EffectInfo = NULL;
		}
	}
	if (m_pEffect != NULL)
	{
		delete[] m_pEffect;
		m_pEffect = NULL;
	}

	//テクスチャ情報の破棄
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] != NULL)
		{
			delete[] m_TexInfo.ppFileName[nCntTex];
			m_TexInfo.ppFileName[nCntTex] = NULL;
		}
	}

	//テクスチャのファイルポインタの破棄
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}
}