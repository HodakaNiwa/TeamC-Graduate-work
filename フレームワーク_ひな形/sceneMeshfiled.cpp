//=============================================================================
//
// メッシュフィールド処理 [sceneMeshfiled.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "sceneMeshfiled.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FILE_NAME00		("data/TEXT/MeshFiled.txt")
#define TEXTURE_NAME00	("data/TEXTURE/fild.png")
#define MAX_POS			(4)
#define SIZE_POLYGON	(50.0f)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CSceneMeshFiled::m_pTexture = NULL;

//=============================================================================
// 生成処理
//=============================================================================
CSceneMeshFiled * CSceneMeshFiled::Create(const D3DXVECTOR3 pos)
{
	//インスタンスの生成
	CSceneMeshFiled * pSceneMeshFiled;
	pSceneMeshFiled = new CSceneMeshFiled;

	//初期化処理
	pSceneMeshFiled->Init();

	//テキストを読み込む
	pSceneMeshFiled->LoadData();

	//テクスチャの読み込み
	pSceneMeshFiled->Load();

	//設定処理
	pSceneMeshFiled->Set(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return pSceneMeshFiled;
}

//=============================================================================
// テクスチャ設定
//=============================================================================
void CSceneMeshFiled::Load(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME00,
		&m_pTexture);
}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CSceneMeshFiled::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// データをロードする
//=============================================================================
void CSceneMeshFiled::LoadData(void)
{
	//変数宣言
	FILE *pFile;

	//テキストの読み込み
	pFile = fopen(FILE_NAME00, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &m_nWidth);		//横幅の読み込み
		fscanf(pFile, "%d", &m_nHight);		//縦幅の読み込み
		fscanf(pFile, "%d", &m_nPrimitiv);	//頂点の読み込み

		for (int nCnt = 0; nCnt < m_nPrimitiv; nCnt++)
		{
			fscanf(pFile, "%f", &m_LoadPos[nCnt].x);		//横幅の読み込み
			fscanf(pFile, "%f", &m_LoadPos[nCnt].y);		//横幅の読み込み
			fscanf(pFile, "%f", &m_LoadPos[nCnt].z);		//横幅の読み込み
		}

		//読み込みを終了する
		fclose(pFile);
	}
}

//=============================================================================
// データを書き出す
//=============================================================================
void CSceneMeshFiled::SaveData(void)
{
	//変数宣言
	FILE *pFile = NULL;

	//テキストの作成
	pFile = fopen(FILE_NAME00, "w");

	if (pFile != NULL)
	{//txtへの書き込み

		VERTEX_3D * pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		fprintf(pFile, "%d %d %d\n ", m_nHight, m_nWidth, m_nPrimitiv);

		for (int nCntPrimitv = 0; nCntPrimitv < m_nPrimitiv; nCntPrimitv++)
		{//頂点座標の書き込み
			fprintf(pFile, "\n%.3f %.3f %.3f \n ", (double)pVtx[0].pos.x, (double)pVtx[0].pos.y, (double)pVtx[0].pos.z);

			//該当の位置まで進める
			pVtx++;
		}

		//書き込みを終了する
		fclose(pFile);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneMeshFiled::CSceneMeshFiled() : CScene(2, OBJTYPE_FLOR)
{
	m_pVtxBuff = NULL;				//頂点バッファ
	m_pIdxBuff = NULL;				//インデックスバッファ
	m_pos = D3DXVECTOR3(0, 0, 0);	//中心座標
	m_rot = D3DXVECTOR3(0, 0, 0);	//向き
	m_nHight = 0;					//楯列
	m_nWidth = 0;					//横列
	m_nIndex = 0;					//インデックス数
	m_nPolygon = 0;					//ポリゴン数
	m_nPrimitiv = 0;				//頂点数
	m_nSelectPrimitiv = 0;			//現在選択されている頂点
	m_bUpdatePrimitiv = false;		//頂点情報が更新されているかどうか
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneMeshFiled::~CSceneMeshFiled()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneMeshFiled::Init(void)
{
	//変数の初期化
	m_pVtxBuff = NULL;				//頂点バッファ
	m_pIdxBuff = NULL;				//インデックスバッファ
	m_pos = D3DXVECTOR3(0, 0, 0);	//中心座標
	m_rot = D3DXVECTOR3(0, 0, 0);	//向き
	m_nHight = 0;					//楯列
	m_nWidth = 0;					//横列
	m_nIndex = 0;					//インデックス数
	m_nPolygon = 0;					//ポリゴン数
	m_nPrimitiv = 0;				//頂点数
	m_LoadPos[1200] = {};			//初期化する

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneMeshFiled::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//テクスチャの破棄
	Unload();

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneMeshFiled::Update(void)
{
	//変数宣言
	int nCntVertex = 0;

	//デバック表示の取得
	CDebugProc * pDebugLeft = CRenderer::GetDebugLeft();

	//頂点の高さ変更処理
	//UpdatePrimitiv();

	//デバック表示の取得
	CDebugProc * pDebugRight = CRenderer::GetDebugRight();

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneMeshFiled::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
		
	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
								  0, 0,
								  m_nPrimitiv,
								  0,
								  m_nPolygon);
}

//=============================================================================
// 設定処理
//=============================================================================
void CSceneMeshFiled::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//変数宣言
	float fWidthSize = 0;		//横幅のサイズ
	float fHightSize = 0;		//縦幅のサイズ
	D3DXVECTOR3 Vec0[2] = {};	//ベクトル0
	D3DXVECTOR3 Vec1[2] = {};	//ベクトル1
	int nCntPos = 0;				//読み込んだ座標配列を進める

	//ポリゴン用の法線
	D3DXVECTOR3 nor[8000] = {};	//ポリゴンの法線ベクトル
	int nCntNor = 0;			//法線の配列を進める

	//変数の代入
	m_pos = pos;

	//頂点数, ポリゴン数, インデックス数の計算
	int LoadPrimitiv = m_nPrimitiv;
	m_nPrimitiv = (m_nWidth + 1) * (m_nHight + 1);					//頂点数
	m_nPolygon	= (m_nWidth * m_nHight) * 2 + (4 * (m_nHight - 1));	//ポリゴン数
	m_nIndex	= m_nPolygon + 2;									//インデックス数

	//中心からの半分の長さを求める
	fWidthSize = (SIZE_POLYGON * m_nWidth) * 0.5f;	//横の半分の長さ
	fHightSize = (SIZE_POLYGON * m_nHight) * 0.5f;	//縦の半分の長さ

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nPrimitiv,	//確保する頂点バッファの数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD)* m_nIndex,		//確保するインデックスバッファの数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,										//16ビット(2バイト)のデータを確保
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHight = 0; nCntHight <= m_nHight; nCntHight++)
	{//縦の頂点
		for (int nCntWidth = 0; nCntWidth <= m_nWidth; nCntWidth++)
		{//横の頂点

		 //頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-fWidthSize + (SIZE_POLYGON * nCntWidth), 0.0f, fHightSize - (SIZE_POLYGON * nCntHight));

			if (LoadPrimitiv <= nCntPos)
			{
				pVtx[0].pos.y = 0.0f;
			}
			else
			{
				pVtx[0].pos.y = m_LoadPos[nCntPos].y;
			}

			//法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//色の設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャの設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (nCntWidth * 1.0f), 0.0f + (nCntHight * 1.0f));

			//該当の位置まで進める
			pVtx += 1;
			nCntPos++;		//配列のカウンターを進める
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//法線を設定する
	SetNor();

	WORD *pIdx;		//インデックスデータへのポインタ

	//インデックスバッファをロックしインデックスバッファへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;			//インデックスのカウンター

	//インデックスの設定
	for (int nCntHight = 0; nCntHight < m_nHight; nCntHight++)
	{//縦の頂点
		for (int nCntWidth = 0; nCntWidth <= m_nWidth; nCntWidth++, nCntIdx++)
		{//横の頂点
			pIdx[0] = (m_nWidth + 1) + nCntIdx;	//横の頂点数 + グループの数
			pIdx[1] = nCntIdx;

			//該当の位置まで進める
			pIdx += 2;

			if (nCntWidth == m_nWidth && nCntHight < m_nHight - 1)
			{//X軸の折り返し
				pIdx[0] = nCntIdx;									//グループの数
				pIdx[1] = (m_nWidth + 1) + nCntIdx + 1;	//横の頂点数 + グループの数 + 1

				//該当の位置まで進める
				pIdx += 2;
			}
		}
	}

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
}


//=============================================================================
// 地面の高さ取得
//=============================================================================
float CSceneMeshFiled::GetHight(D3DXVECTOR3 pos)
{
	//デバック表示の取得
	CDebugProc * pDebugLeft = CRenderer::GetDebugLeft();

	//変数宣言
	D3DXVECTOR3 VecA = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 VecB = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 VecC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Vec0 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Vec1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float		fTouch = 0.0f;
	float		fHight = 0.0f;		//高さを保存する

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//どのブロックに乗っているかの判定
	int nBlockX = (int)((pos.x + (m_nWidth * SIZE_POLYGON / 2)) / ((m_nWidth * SIZE_POLYGON) / m_nWidth));
	int nBlockZ = (int)((pos.z - (m_nWidth * SIZE_POLYGON / 2)) / ((m_nHight * SIZE_POLYGON) / m_nHight) * -1);

	//各頂点番号を求める
	int nLU = nBlockX + nBlockZ * (m_nWidth + 1);
	int nRU = nLU + 1;
	int nLD = nLU + (m_nWidth + 1);
	int nRD = nLD + 1;

	//対角線のベクトルを求める
	VecA = pVtx[nLU].pos - pVtx[nRD].pos;

	//頂点と指定位置のベクトルを求める
	VecB = pos - pVtx[nLU].pos;

	//外積を求める
	fTouch = (VecA.z * VecB.x) - (VecA.x * VecB.z);

	if (fTouch <= 0)
	{
		//VecAを求める
		VecA = pos - pVtx[nLD].pos;	//頂点とプレイヤー座標の差分から求める

		//高さを保存する
		fHight = pVtx[nLD].pos.y;

		//法線の設定
		Vec0 = pVtx[nLD].pos - pVtx[nLU].pos;	//Vec1を求める
		Vec1 = pVtx[nLD].pos - pVtx[nRD].pos;	//Vec0を求める
	}
	else
	{
		//VecAを求める
		VecA = pos - pVtx[nRU].pos;	//頂点とプレイヤー座標の差分から求める

		//高さを保存する
		fHight = pVtx[nRU].pos.y;

		//法線の設定
		Vec0 = pVtx[nRU].pos - pVtx[nLU].pos;	//Vec1を求める
		Vec1 = pVtx[nRU].pos - pVtx[nRD].pos;	//Vec0を求める
	}

	D3DXVec3Cross(&nor, &Vec0, &Vec1);	//法線の設定

	 //正規化する
	D3DXVec3Normalize(&nor, &nor);

	//内積を求める
	VecC.y = ((VecA.x * nor.x) + (VecA.z * nor.z)) / -nor.y;

	//高さをもとに戻す
	VecC.y += fHight;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return VecC.y;
}

#ifdef _DEBUG
//=============================================================================
// 頂点の更新処理
//=============================================================================
void CSceneMeshFiled::UpdatePrimitiv(void)
{
	//編集状態か取得する
	if (CRenderer::GetDrawDebugRight() == true)
	{
		CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得

		//頂点の選択処理
		if (pInputKeyboard->GetKeyboardTrigger(DIK_T) == true)
		{
			m_nSelectPrimitiv = (m_nSelectPrimitiv + (m_nPrimitiv - 1)) % m_nPrimitiv;
		}
		else if(pInputKeyboard->GetKeyboardTrigger(DIK_G) == true)
		{
			m_nSelectPrimitiv = (m_nSelectPrimitiv + 1) % m_nPrimitiv;
		}
		else if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
		{//頂点情報を保存する
			SaveData();
			SetNor();
			m_bUpdatePrimitiv = false;	//最新状態にする
		}

		//高さの更新処理
		SetPosY(m_nSelectPrimitiv);
	}
}

//=============================================================================
// 頂点の高さ変更
//=============================================================================
void CSceneMeshFiled::SetPosY(int nVertex)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//キーボードの取得

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pInputKeyboard->GetKeyboardPress(DIK_Y) == true)
	{
		pVtx[nVertex].pos.y += 1.0f;
		m_bUpdatePrimitiv = true;	//更新必要状態にする
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_H) == true)
	{
		pVtx[nVertex].pos.y -= 1.0f;
		m_bUpdatePrimitiv = true;	//更新必要状態にする
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
#endif

//=============================================================================
// 法線を再設定する
//=============================================================================
void CSceneMeshFiled::SetNor(void)
{
	//変数宣言
	D3DXVECTOR3 Vec0[2] = {};	//ベクトル0
	D3DXVECTOR3 Vec1[2] = {};	//ベクトル1
	D3DXVECTOR3 nor[8000] = {};	//ポリゴンの法線ベクトル
	int nCntNor = 0;			//法線の配列を進める

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHight = 0; nCntHight < m_nHight; nCntHight++)
	{//縦の頂点
		for (int nCntWidth = 0; nCntWidth < m_nWidth; nCntWidth++)
		{//横の頂点

			//法線の設定
			Vec0[0] = pVtx[0].pos - pVtx[0 + m_nWidth + 1].pos;		//Vec1を求める
			Vec1[0] = pVtx[0].pos - pVtx[0 + m_nWidth + 2].pos;		//Vec0を求める
			Vec0[1] = pVtx[0].pos - pVtx[0 + m_nWidth + 2].pos;		
			Vec1[1] = pVtx[0].pos - pVtx[1].pos;					

			D3DXVec3Cross(&nor[nCntNor], &Vec0[0], &Vec1[0]);		//法線の設定
			D3DXVec3Cross(&nor[nCntNor + 1], &Vec0[1], &Vec1[1]);	

			//正規化する
			D3DXVec3Normalize(&nor[nCntNor], &nor[nCntNor]);
			D3DXVec3Normalize(&nor[nCntNor + 1], &nor[nCntNor + 1]);

			if (nCntWidth != m_nWidth - 1)
			{
				pVtx++;
			}
			else if (nCntWidth == m_nWidth - 1)
			{
				pVtx += 2;
			}

			nCntNor += 2;	//計算用変数の配列を進める
		}
	}

	nCntNor = 0;	//カウンターを初期化する

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHight = 0; nCntHight <= m_nHight; nCntHight++)
	{//縦の頂点
		for (int nCntWidth = 0; nCntWidth <= m_nWidth; nCntWidth++)
		{//横の頂点
			if (nCntHight == 0)
			{//最初の段
				if (nCntWidth == 0)
				{//左端
					pVtx[0].nor = (nor[nCntNor] + nor[nCntNor + 1]) * 0.5f;
				}
				else if (nCntWidth == m_nWidth)
				{//右端
					pVtx[0].nor = nor[nCntNor + 1];
				}
				else
				{//真ん中
					pVtx[0].nor = (nor[nCntNor - 1] + nor[nCntNor] + nor[nCntNor + 1]) / 3.0f;
				}
			}
			else if (nCntHight == m_nHight)
			{//最後の段
				if (nCntWidth == 0)
				{//左端
					pVtx[0].nor = nor[nCntNor];
				}
				else if (nCntWidth == m_nWidth)
				{//右端
					pVtx[0].nor = (nor[nCntNor + 1] + nor[nCntNor]) * 0.5f;
				}
				else
				{//真ん中
					pVtx[0].nor = (nor[nCntNor - 2] + nor[nCntNor - 1] + nor[nCntNor]) / 3.0f;
				}
			}
			else
			{//真ん中の段
				if (nCntWidth == 0)
				{//左端
					pVtx[0].nor = (nor[nCntNor + 1] + nor[nCntNor - m_nWidth * 2] + nor[nCntNor]) / 3.0f;
				}
				else if (nCntWidth == m_nWidth)
				{//右端
					pVtx[0].nor = (nor[nCntNor + 1] + nor[nCntNor - (m_nWidth * 2)] + nor[nCntNor - (m_nWidth * 2 + 1)]) / 3.0f;
				}
				else
				{//真ん中
					pVtx[0].nor = (nor[nCntNor - 1] + nor[nCntNor] + nor[nCntNor + 1] + nor[nCntNor - (m_nWidth * 2 - 2)] + nor[nCntNor - (m_nWidth * 2 - 1)] + nor[nCntNor - m_nWidth * 2]) / 6.0f;
				}
			}

			//配列のカウンターを進める
			if (nCntWidth != m_nWidth - 1)
			{
				nCntNor += 2;
			}

			if (nCntWidth == m_nWidth && nCntHight == m_nHight - 1)
			{
				nCntNor -= 2 + (m_nWidth * 2 - 2);
			}

			//ポインタを該当の位置まで進める
			pVtx++;
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

#ifdef _DEBUG
//=============================================================================
// 頂点の高さを設定する
//=============================================================================
void CSceneMeshFiled::SetHight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点が半径の範囲内にあるかどうか
	for (int nCntPrimitiv = 0; nCntPrimitiv < m_nPrimitiv; nCntPrimitiv++)
	{
		//プレイヤーの距離と対象の頂点までの距離
		float fPosX = pos.x - pVtx[0].pos.x;
		float fPosZ = pos.z - pVtx[0].pos.z;

		float fLength = sqrtf((fPosX * fPosX) + (fPosZ * fPosZ));

		//半径の範囲内にあるかどうか
		if (fLength < fRange)
		{
			float fAngle = (fLength / fRange);
			float fHight = cosf(D3DX_PI * -0.5f * fAngle) * fValue;	//高さを求める

			pVtx[0].pos.y += fHight;
		} 

		//該当の位置まで進める
		pVtx++;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//法線の設定
	SetNor();
}
#endif