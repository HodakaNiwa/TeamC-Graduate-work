//=============================================================================
//
// Xファイル処理 [sceneX.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "game.h"
#include "camera.h"

//=============================================================================
// 生成処理
//=============================================================================
CSceneX * CSceneX::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, char FileName[80], LPDIRECT3DTEXTURE9 pTexture, TYPE type)
{
	//インスタンスの生成
	CSceneX * pSceneX = NULL;

	//初期化処理
	pSceneX->Init();

	pSceneX->Load(FileName, pTexture);						//ファイル読み込み
	pSceneX->Set(pos, D3DXVECTOR3(100.0f, 0.0f, 100.0f));	//設定処理
	pSceneX->SetRot(rot);									//向きの設定

	return pSceneX;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneX::CSceneX() : CScene(3, OBJTYPE_OBJE)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_nNumMat = 0;
	m_ppTexture = NULL;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CSceneX::CSceneX(int nPriority, OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_nNumMat = 0;
	m_vtxMax = D3DXVECTOR3(0, 0, 0);
	m_vtxMin = D3DXVECTOR3(0, 0, 0);
}

//=============================================================================
// ファイルの読み込み
//=============================================================================
void CSceneX::Load(char FileName[80], LPDIRECT3DTEXTURE9 pTexture)
{
	//変数宣言
	D3DXMATERIAL	*pMat;				//マテリアルデータへのポインタ

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(FileName,
					  D3DXMESH_SYSTEMMEM,
					  pDevice,
					  NULL,
					  &m_pBuffMat,
					  NULL,
					  &m_nNumMat,
					  &m_pMesh);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//テクスチャのメモリを確保する
	if (pTexture == NULL)
	{
		m_ppTexture = NULL;
	}
	else
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		//テクスチャの生成
		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			m_ppTexture[nCntMat] = NULL;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				m_ppTexture[nCntMat] = pTexture;
			}
		}
	}

	//変数宣言
	int		nNumVtx = 0;	//頂点数
	DWORD	sizeFVF = 0;	//頂点フォーマットのサイズ
	BYTE *	pVtxBuff = 0;	//頂点バッファへのポインタ

	//頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	m_vtxMax = D3DXVECTOR3(-100000.0f, 0.0f, -100000.0f);	//頂点の最大値
	m_vtxMin = D3DXVECTOR3(100000.0f, 0.0f, 100000.0f);		//頂点の最小値

	//頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

													//最大値を比較
		if (m_vtxMax.x < vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (m_vtxMax.y < vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (m_vtxMax.z < vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}

		//最小値を比較
		if (m_vtxMin.x > vtx.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (m_vtxMin.y > vtx.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (m_vtxMin.z > vtx.z)
		{
			m_vtxMin.z = vtx.z;
		}
		//サイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneX::Init(void)
{
	m_pos = D3DXVECTOR3(0, 0, 0);	//位置
	m_rot = D3DXVECTOR3(0, 0, 0);	//向き
	m_Mat_a = 1.0f;					//マテリアルのα値
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	// メッシュの開放
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// マテリアルの開放
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//テクスチャの開放
	if (m_ppTexture != NULL)
	{
		for (int nCntTex = 0; nCntTex < (int)m_nNumMat; nCntTex++)
		{
			if (m_ppTexture[nCntTex] != NULL)
			{
				m_ppTexture[nCntTex] = NULL;
			}
		}

		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneX::Update(void){}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	//変数宣言
	D3DMATERIAL9	matDef;				//現在のマテリアル保存用
	D3DXMATERIAL	*pMat;				//マテリアルデータへのポインタ

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans, mtxShadow;			// 計算用マトリックス

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

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// テクスチャデータへのポインタを取得
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse.a = m_Mat_a;		//マテリアルのα値の設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// マテリアルの設定

		if (m_ppTexture != NULL)
		{
			if (m_ppTexture[nCntMat] != NULL)
			{
				pDevice->SetTexture(0, m_ppTexture[nCntMat]);	//テクスチャの設定
			}
			else
			{
				pDevice->SetTexture(0, NULL);	//テクスチャの設定
			}
		}
		else
		{
			pDevice->SetTexture(0, NULL);	//テクスチャの設定
		}
		m_pMesh->DrawSubset(nCntMat);					// モデル(パーツ)の描画
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 設定処理
//=============================================================================
void CSceneX::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
}