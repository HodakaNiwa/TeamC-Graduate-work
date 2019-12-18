//=============================================================================
//
// オブジェクトベース処理 [objbase.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "objbase.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>

//=============================================================================
// マクロ定義
//=============================================================================
#define SaveTextName ("data/TEXT/Save.txt")		//保存するテキスト名
#define OBJ_PRIORITY (3)

//=============================================================================
// 前方宣言
//=============================================================================
LPDIRECT3DTEXTURE9 * CObjBase::m_ppTexture = NULL;
LPD3DXMESH * CObjBase::m_pMesh = NULL;
LPD3DXBUFFER * CObjBase::m_pBuffMat = NULL;
DWORD *	CObjBase::m_pNumMat = NULL;
D3DXVECTOR3 * CObjBase::m_pVtxMax = NULL;
D3DXVECTOR3 * CObjBase::m_pVtxMin = NULL;
int CObjBase::m_nMaxTex = 0;
int CObjBase::m_nMaxModel = 0;
int CObjBase::m_nCountObj = 0;
int CObjBase::m_nMaxObj = 0;

//=============================================================================
// 生成処理
//=============================================================================
CObjBase * CObjBase::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexNum, int ModelNum, TYPE type)
{
	//インスタンスの生成
	CObjBase * pObjBase = NULL;

	if (type == TYPE_NORMAL)
	{
		pObjBase = new CObjBase;
	}

	//初期化処理
	pObjBase->Init();
	pObjBase->SetPos(pos);						//位置の設定
	pObjBase->SetRot(rot);						//向きの設定
	pObjBase->SetvtxMax(m_pVtxMax[ModelNum]);	//最大値
	pObjBase->SetvtxMin(m_pVtxMin[ModelNum]);	//最小値
	pObjBase->SetScale(scale);					//大きさの設定
	pObjBase->SetTexNum(nTexNum);				//テクスチャ番号
	pObjBase->SetModelType(ModelNum);			//モデル番号
	pObjBase->SetType(type);					//タイプ

	//モデル・テクスチャの割り当て
	pObjBase->BindModel(m_pMesh[ModelNum], m_pBuffMat[ModelNum], m_pNumMat[ModelNum], m_ppTexture[nTexNum]);

	return pObjBase;
}

//=============================================================================
// モデルの破棄
//=============================================================================
void CObjBase::UnloadModel(void)
{
	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}

		delete[] m_pMesh;
		m_pMesh = NULL;
	}

	//バッファの破棄
	if (m_pBuffMat != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}

		delete[] m_pBuffMat;
		m_pBuffMat = NULL;
	}

	//マテリアルの破棄
	if (m_pNumMat != NULL)
	{
		delete m_pNumMat;
		m_pNumMat = NULL;
	}

	//最大値の破棄
	if (m_pVtxMax != NULL)
	{
		delete m_pVtxMax;
		m_pVtxMax = NULL;
	}

	//最小値の破棄
	if (m_pVtxMin != NULL)
	{
		delete m_pVtxMin;
		m_pVtxMin = NULL;
	}
}

//=============================================================================
// コンストラクタ
//=============================================================================
CObjBase::CObjBase() : CSceneX(3, OBJTYPE_OBJ)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
	m_nTex = 0;
	m_nModelType = 0;
	m_Type = TYPE_MAX;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CObjBase::CObjBase(int nPriority, OBJTYPE objtype) : CSceneX(nPriority, objtype)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
	m_nTex = 0;
	m_nModelType = 0;
	m_Type = TYPE_MAX;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjBase::~CObjBase(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObjBase::Init(void)
{
	//変数の初期化
	m_nTex = 0;
	m_nModelType = 0;
	m_Type = TYPE_MAX;

	CSceneX::Init();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObjBase::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObjBase::Update(void)
{
	CSceneX::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CObjBase::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CObjBase::LoadTex(CLoadTextObject::TEX_INFO TexInfo)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの最大数
	m_nMaxTex = TexInfo.nMaxTex;

	//テクスチャメモリを動的確保
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTex];
	}

	//テクスチャを生成する
	for (int nCntTex = 0; nCntTex < m_nMaxTex; nCntTex++)
	{
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, &TexInfo.ppFileName[nCntTex][0], &m_ppTexture[nCntTex]);
	}
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CObjBase::UnloadTex(void)
{
	if (m_ppTexture != NULL)
	{
		for (int nCntTex = 0; nCntTex < m_nMaxTex; nCntTex++)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}

		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}
}

//=============================================================================
//　モデルの読み込み
//=============================================================================
void CObjBase::LoadModel(CLoadTextObject::OBJ_INFO ObjInfo)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//モデルの最大数
	m_nMaxModel = ObjInfo.nMaxModel;

	//モデルの動的確保
	if ((m_pMesh == NULL) && (m_pBuffMat == NULL) && (m_pNumMat == NULL))
	{
		m_pMesh = new LPD3DXMESH[m_nMaxModel];
		m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
		m_pNumMat = new DWORD[m_nMaxModel];
		m_pVtxMax = new D3DXVECTOR3[m_nMaxModel];
		m_pVtxMin = new D3DXVECTOR3[m_nMaxModel];
	}

	//モデルの生成
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(&ObjInfo.ppModelFileName[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nCntModel],
			NULL,
			&m_pNumMat[nCntModel],
			&m_pMesh[nCntModel]);

		//変数宣言
		int		nNumVtx = 0;	//頂点数
		DWORD	sizeFVF = 0;	//頂点フォーマットのサイズ
		BYTE *	pVtxBuff = 0;	//頂点バッファへのポインタ

		//頂点数を取得
		nNumVtx = m_pMesh[nCntModel]->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nCntModel]->GetFVF());

		m_pVtxMax[nCntModel] = D3DXVECTOR3(-100000.0f, 0.0f, -100000.0f);	//頂点の最大値
		m_pVtxMin[nCntModel] = D3DXVECTOR3(100000.0f, 0.0f, 100000.0f);		//頂点の最小値

																			//頂点バッファをロック
		m_pMesh[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

														//最大値を比較
			if (m_pVtxMax[nCntModel].x < vtx.x)
			{
				m_pVtxMax[nCntModel].x = vtx.x;
			}
			if (m_pVtxMax[nCntModel].y < vtx.y)
			{
				m_pVtxMax[nCntModel].y = vtx.y;
			}
			if (m_pVtxMax[nCntModel].z < vtx.z)
			{
				m_pVtxMax[nCntModel].z = vtx.z;
			}

			//最小値を比較
			if (m_pVtxMin[nCntModel].x > vtx.x)
			{
				m_pVtxMin[nCntModel].x = vtx.x;
			}
			if (m_pVtxMin[nCntModel].y > vtx.y)
			{
				m_pVtxMin[nCntModel].y = vtx.y;
			}
			if (m_pVtxMin[nCntModel].z > vtx.z)
			{
				m_pVtxMin[nCntModel].z = vtx.z;
			}
			//サイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		//頂点バッファをアンロック
		m_pMesh[nCntModel]->UnlockVertexBuffer();
	}
}

//=============================================================================
//　オブジェクト検索
//=============================================================================
CObjBase * CObjBase::RequestPointer(int nCountObj)
{
	//敵のオブジェクトポインタの取得
	CScene * pSceneTop = CScene::GetTop(OBJ_PRIORITY);	 //オブジェクトポインタの先頭を取得			
	CScene * pScene = pSceneTop;						 //トップの位置を保存する
	CObjBase * m_pObjBase = NULL;
	int nNumObject = 0;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();					//次のオブジェクトのポインタを保存する

		if (CScene::OBJTYPE_OBJ == pScene->GetObjType())
		{
			if (nNumObject == nCountObj)
			{//番号が一致したらポインタを保存
				m_pObjBase = (CObjBase*)pScene;
				break;
			}

			nNumObject++;	//番号の加算
		}

		pScene = pSceneNext;		//ポインタを進める
	}

	return m_pObjBase;
}

//=============================================================================
// テキスト保存処理
//=============================================================================
void CObjBase::Save(HWND hWnd)
{
	//変数宣言
	FILE * pFile = NULL;

	//テクストの作成
	pFile = fopen(SaveTextName, "w");

	//書き出し処理
	if (pFile != NULL)
	{
		//敵のオブジェクトポインタの取得
		CScene * pSceneTop = CScene::GetTop(OBJ_PRIORITY);	 //オブジェクトポインタの先頭を取得			
		CScene * pScene = pSceneTop;						 //トップの位置を保存する

		while (pScene != NULL)
		{
			CScene * pSceneNext = pScene->GetpNext();					//次のオブジェクトのポインタを保存する

			if (CScene::OBJTYPE_OBJ == pScene->GetObjType())
			{
				if (pScene->GetDeth() == false)
				{
					CObjBase * pObject = (CObjBase*)pScene;						//敵クラスにキャストする
					int ModelType = pObject->GetModelType();
					int nTexNum = pObject->GetTex();
					D3DXVECTOR3 Pos = pObject->GetPos();
					D3DXVECTOR3 Rot = pObject->GetRot();
					D3DXVECTOR3 Scale = pObject->GetScale();
					TYPE type = pObject->GetType();

					fprintf(pFile, "SET\n");
					fprintf(pFile, "\tMODELTYPE = %d\n", ModelType);							//モデルのタイプ
					fprintf(pFile, "\tTEX = %d\n", nTexNum);									//テクスチャ番号
					fprintf(pFile, "\tPOS = %.1f %.1f %.1f\n", Pos.x, Pos.y, Pos.z);			//位置
					fprintf(pFile, "\tROT = %.1f %.1f %.1f\n", Rot.x, Rot.y, Rot.z);			//向き
					fprintf(pFile, "\tSCALE = %.1f %.1f %.1f\n", Scale.x, Scale.y, Scale.z);		//スケール
					fprintf(pFile, "\tTYPE = %d\n", (int)type);									//更新のタイプ
					fprintf(pFile, "END_SET\n");
					fprintf(pFile, "\n");
				}
			}

			pScene = pSceneNext;		//ポインタを進める
		}

		//書き込みを終了する
		fclose(pFile);

		//メッセージボックスの表示
		MessageBox(hWnd, "セーブが完了しました", "処理完了通知", MB_OK);
	}
}