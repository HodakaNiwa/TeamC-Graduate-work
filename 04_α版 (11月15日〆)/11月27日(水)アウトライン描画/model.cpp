//=============================================================================
//
// モデル専用の処理 [model.cpp]
// Author : 有馬武志
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_OUTLINESHADER_FILENAME "data/outline.hlsl"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
COutlineShader *CModel::m_pOutlineShader = NULL;		// アウトラインシェーダークラスへのポインタ
#ifdef _DEBUG
int CModel::m_nCreateNum = 0;
#endif

//=============================================================================
// 生成処理
//=============================================================================
CModel * CModel::Create(const D3DXVECTOR3 pos, char FileName[40], D3DXVECTOR3 Scale)
{
	//インスタンスの生成
	CModel * pModel = NULL;

	if (pModel == NULL)
	{

		pModel = new CModel;
		if (pModel != NULL)
		{
			//初期化処理
			pModel->Init(FileName);

			//設定処理
			pModel->SetPos(pos);
			pModel->SetFirstPos(pos);
			pModel->SetScale(Scale);

		}
	}

	return pModel;
}

//=============================================================================
// シェーダーを読み込む処理
//=============================================================================
HRESULT CModel::ShaderLoad(void)
{
	if (m_pOutlineShader != NULL) { return E_FAIL; }
	m_pOutlineShader = COutlineShader::Create(MODEL_OUTLINESHADER_FILENAME);

	return S_OK;
}

//=============================================================================
// シェーダーを開放する処理
//=============================================================================
void CModel::ShaderUnLoad(void)
{
	if (m_pOutlineShader != NULL)
	{
		m_pOutlineShader->Uninit();
		delete m_pOutlineShader;
		m_pOutlineShader = NULL;
	}
}

//===============================================================================
//　コンストラクタ
//===============================================================================
CModel::CModel()
{
	m_pMesh = NULL;								//メッシュ情報へのポインタ
	m_pBuffMat = NULL;							//マテリアルの情報へのポインタ
	m_nNumMat = 0;								//マテリアルの情報数
	m_VtxMin, m_VtxMax;							//モデルの最小値・最大値
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_FirstPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//初期位置
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//拡大縮小率
	m_pTextures = NULL;
	m_pParent = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);			// ワールドマトリックスの初期化
	m_Move = INITIALIZE_VECTOR3;				// 移動量
}
//===============================================================================
//　デストラクタ
//===============================================================================
CModel::~CModel(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(char FileName[40])
{
#ifdef _DEBUG
	m_nCreateNum++;
#endif

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CreateXFile(FileName);		//Xファイルの生成

	int nNumVtx;				//頂点数
	DWORD sizeFVF;				//頂点フォーマットのサイズ
	BYTE *pVtxBuff;				//頂点バッファへのポインタ

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//位置
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向き達する
	m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);			//最大値
	m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);			//最小値

	//頂点数を取得
	nNumVtx = m_pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

		//最大値
		if (vtx.x > m_VtxMax.x)
		{
			m_VtxMax.x = vtx.x;
		}
		if (vtx.y > m_VtxMax.y)
		{
			m_VtxMax.y = vtx.y;
		}
		if (vtx.z > m_VtxMax.z)
		{
			m_VtxMax.z = vtx.z;
		}
		//最小値
		if (vtx.x < m_VtxMin.x)
		{
			m_VtxMin.x = vtx.x;
		}
		if (vtx.y < m_VtxMin.y)
		{
			m_VtxMin.y = vtx.y;
		}
		if (vtx.z < m_VtxMin.z)
		{
			m_VtxMin.z = vtx.z;
		}

		//サイズ文のポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	m_Rot.y = 0.0f;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
#ifdef _DEBUG
	m_nCreateNum--;
#endif

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

	//テクスチャの破棄
	if (m_pTextures != NULL)
	{
		//m_pTextures->Release();
		m_pTextures = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void) {}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(float fAlpha)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans,mtxParent, mtxScale;	//計算
	D3DMATERIAL9 matDef;								//現在のマテリアルを保存
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//拡大縮小行列の作成
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{//親の場合
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//親ではない場合
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//親のマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//マテリアルの設定
		pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャをNULLにする
		pDevice->SetTexture(0, m_pTextures);

		//オブジェクト(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
//　モデルのアウトラインを描画する処理
//=============================================================================
void CModel::DrawOutline(void)
{
	if (m_pOutlineShader == NULL) { return; }

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent, mtxScale;	//計算

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//拡大縮小行列の作成
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{//親の場合
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//親ではない場合
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//親のマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	m_pOutlineShader->SetMtxWorld(m_mtxWorld);

	// プロジェクションマトリックスとビューマトリックスの設定
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_pOutlineShader->SetMtxProjection(mtxProj);
	m_pOutlineShader->SetMtxView(mtxView);

	// シェーダーに値をセット
	m_pOutlineShader->SetParamToEffect();

	// 表面カリングに設定
	DWORD CullMode;
	pDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// 描画開始
	m_pOutlineShader->Begin();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// レンダリングパス開始
		m_pOutlineShader->BeginPass(0);

		// オブジェクト(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);

		// レンダリングパス終了
		m_pOutlineShader->EndPass();
	}

	// 描画終了
	m_pOutlineShader->End();

	// カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, CullMode);
}

//=============================================================================
//　マテリアルカラーの設定
//=============================================================================
void CModel::SetColor(D3DXCOLOR col)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// 色を付ける
	pMat->MatD3D.Diffuse = col;

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//　Xファイルの読み込み
//=============================================================================
void CModel::CreateXFile(char FileName[40])
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(&FileName[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);
}