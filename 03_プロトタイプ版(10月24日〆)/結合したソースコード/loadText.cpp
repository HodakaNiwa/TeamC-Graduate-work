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

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//ゼロベクトル

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