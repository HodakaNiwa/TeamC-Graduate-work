//*****************************************************************************
//
//     当たり判定可視化用メッシュスフィアの処理[colDispSphere.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
#include "manager.h"
#include "library.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CColDispSphere::CColDispSphere(int nPriority, OBJTYPE objType) : CMeshSphere(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CColDispSphere::~CColDispSphere()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CColDispSphere *CColDispSphere::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	// メモリ確保
	CColDispSphere *pColDispSphere = NULL;
	pColDispSphere = new CColDispSphere(nPriority);
	if (pColDispSphere == NULL)
	{
		return NULL;
	}

	// 初期化処理
	if (FAILED(pColDispSphere->Init(pos, col, fRadius, nXBlock, nYBlock)))
	{
		pColDispSphere->Uninit();
		return NULL;
	}

	return pColDispSphere;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CColDispSphere::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock)
{
	// 変数のクリア
	ClearVariable();

	// 各種値の設定
	GetObject3D()->SetPos(pos);                 // 球の座標
	GetObject3D()->SetRot(INITIALIZE_VECTOR3);  // 球の向き
	SetCol(col);                                // 球の色
	SetRadius(fRadius);                         // 球の半径
	SetXBlock(nXBlock);                         // 横の分割数
	SetYBlock(nYBlock);                         // 縦の分割数

	// 共通の初期化処理
	if (FAILED(CMeshSphere::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CColDispSphere::Uninit(void)
{
	CMeshSphere::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CColDispSphere::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CColDispSphere::Draw(void)
{

}

//=============================================================================
//    可視化処理
//=============================================================================
void CColDispSphere::Disp(void)
{
	if (CCollider::GetDrawAll() == false) { return; }

	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return; }

	// ライティングを無効に
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 表面カリングに切り替え
	DWORD Culling;
	pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// 共通の描画処理
	CMeshSphere::Draw();

	// カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

	// ライティングの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//    頂点バッファに値を設定する
//=============================================================================
void CColDispSphere::SetVtxBuffValue(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == NULL) { return; }
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;                   // 角度
	float fRadius = 0;                  // 半径
	float fSphere = 0.0f;               // スフィア計算用(半径を縮める倍率)
	D3DXVECTOR3 pos;                    // 座標
	D3DXCOLOR col = GetCol();           // 色
	int nXBlock = GetXBlock();          // 横の分割数
	int nYBlock = GetYBlock();          // 横の分割数
	float fSphereRadius = GetRadius();  // 球の半径
	float Utex;                         // テクスチャU座標
	float Vtex;                         // テクスチャV座標

	fAngle = 0.0f;                            // 角度は0に
	fSphere = 0.0f;                           // 倍率を設定
	fRadius = cosf(fSphere) * fSphereRadius;  // 半径を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 座標を設定
	Utex = 0.0f;                              // テクスチャU座標を設定
	Vtex = 0.01f;                             // テクスチャV座標を設定

	// 上側の蓋部分の値を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, fSphereRadius, 0.0f);
	pVtx[0].col = col;
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.005f);
	pVtx++;
	fSphere -= D3DX_PI / (nYBlock + 2);      // 半径を縮める倍率を上げる
	fRadius = sinf(fSphere) * fSphereRadius; // 半径をサインカーブで広げていく

	for (int nCntV = 0; nCntV < nYBlock + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			pos.x = sinf(fAngle) * -fRadius;
			pos.y = cosf(fSphere) * fSphereRadius;
			pos.z = cosf(fAngle) * -fRadius;

			// 頂点座標
			pVtx[nCntH].pos = pos;

			// 頂点カラー
			pVtx[nCntH].col = col;

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);
			if (nCntV >= ((nYBlock + 1) / 2))
			{
				pVtx[nCntH].tex = D3DXVECTOR2(Utex, 1.005f);
			}

			fAngle += D3DX_PI / (nXBlock / 2);  // 角度を進める
			Utex += 1.0f / (nXBlock / 2);       // テクスチャU座標を進める

			if (fAngle >= D3DX_PI)
			{// 倍率が円周率を超えた
				fAngle -= D3DX_PI * 2.0f;
			}
		}
		fSphere -= D3DX_PI / (nYBlock + 2);         // 半径を縮める倍率を上げる
		fRadius = sinf(fSphere) * fSphereRadius;    // 半径をサインカーブで広げていく
		fAngle = 0.0f;                              // 角度は0に
		Utex = 0.0f;                                // テクスチャU座標を設定
		Vtex += 2.0f / (nYBlock + 2);               // テクスチャV座標を進める
		pVtx += nXBlock + 1;                        // 頂点の先頭の番号を進める
	}

	// 下側の蓋部分の値を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, -fSphereRadius, 0.0f);
	pVtx[0].col = col;
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.01f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファに法線を設定する
	SetVtxBuffNor();
}