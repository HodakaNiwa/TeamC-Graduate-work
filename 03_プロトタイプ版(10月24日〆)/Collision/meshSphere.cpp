//*****************************************************************************
//
//     メッシュスフィアの処理[meshSphere.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "3DMesh.h"
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
CMeshSphere::CMeshSphere(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshSphere::~CMeshSphere()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	// メモリ確保
	CMeshSphere *pMeshSphere = NULL;
	pMeshSphere = new CMeshSphere(nPriority);
	if (pMeshSphere == NULL)
	{
		return NULL;
	}

	// 各種値の代入
	pMeshSphere->SetPos(pos);         // メッシュスフィアの座標
	pMeshSphere->SetRot(rot);         // メッシュスフィアの向き
	pMeshSphere->SetCol(col);         // メッシュスフィアの色
	pMeshSphere->SetRadius(fRadius);  // メッシュスフィアの半径
	pMeshSphere->SetXBlock(nXBlock);  // 横の分割数
	pMeshSphere->SetYBlock(nYBlock);  // 縦の分割数

	// 初期化処理
	if (FAILED(pMeshSphere->Init()))
	{
		pMeshSphere->Uninit();
		return NULL;
	}

	return pMeshSphere;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshSphere::Init(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 作成に必要な頂点数,インデックス数,ポリゴン数を計算
	m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);
	m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);
	m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);

	// 上側と下側の蓋部分を縮退ポリゴンの分を考えてそれぞれ加算
	m_nNumVertex += 2;
	m_nNumIndex += ((m_nXBlock + 1) * 2 + 4) * 2;
	m_nNumPolygon += (((m_nXBlock * 2) + 4)) * 2;
	if (m_nXBlock % 2 == 0)
	{
		m_nNumPolygon += 4;
	}

	// 頂点情報の作成
	MakeVertex(pDevice);

	// インデックス情報の作成
	MakeIndex(pDevice);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMeshSphere::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMeshSphere::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshSphere::Draw(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return; }

	// トランスフォーム情報を計算しデバイスに設定
	CalcNotScaleTransform();
	SetTransformToDevice(pDevice);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデバイスのデータストリームにバインド
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
}

//=============================================================================
//    トランスフォーム情報(大きさを含めない)を計算する
//=============================================================================
void CMeshSphere::CalcNotScaleTransform(void)
{
	D3DXMATRIX mtxRot; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転行列を作成(D3DXMatrixRotationYawPitchRoll参照)
	float fSinPitch = sinf(m_Rot.x);
	float fCosPitch = cosf(m_Rot.x);
	float fSinYaw = sinf(m_Rot.y);
	float fCosYaw = cosf(m_Rot.y);
	float fSinRoll = sinf(m_Rot.z);
	float fCosRoll = cosf(m_Rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// 回転を反映する
	m_MtxWorld._11 = mtxRot._11;
	m_MtxWorld._12 = mtxRot._12;
	m_MtxWorld._13 = mtxRot._13;
	m_MtxWorld._21 = mtxRot._21;
	m_MtxWorld._22 = mtxRot._22;
	m_MtxWorld._23 = mtxRot._23;
	m_MtxWorld._31 = mtxRot._31;
	m_MtxWorld._32 = mtxRot._32;
	m_MtxWorld._33 = mtxRot._33;

	// オフセット位置を反映
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;
}

//=============================================================================
//    トランスフォーム情報をデバイスに設定する
//=============================================================================
void CMeshSphere::SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CMeshSphere::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファに値を設定する
	SetVtxBuffValue();
}

//=============================================================================
//    インデックスバッファ生成処理
//=============================================================================
void CMeshSphere::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 1;  // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 縮退ポリゴンを考慮した上側の蓋部分のインデックス情報を設定
	for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
	{// 横の分割数+１繰り返し
	    // 上下の二つ分インデックス番号を設定
		pIdx[0] = nCntIdx;     // 下側
		pIdx[1] = 0;           // 上側

		pIdx += 2;  // 2つ分進める
		if (nCntIdxX == m_nXBlock)
		{// 横の分割数分設定が終わった
			pIdx[0] = 0; // 上側
			pIdx[1] = 1; // 次の下側

			pIdx += 2; // 2つ分進める
		}
	}
	// 縮退ポリゴンでもう1枚隠ぺい
	pIdx[0] = 1;             // 上側
	pIdx[1] = m_nXBlock + 2; // 次の下側
	pIdx += 2; // 2つ分進める

	nCntIdx = 1;
	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		 // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + m_nXBlock + 1; // 下側
			pIdx[1] = nCntIdx;                 // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < m_nYBlock && nCntIdxX == m_nXBlock)
			{// 1 , 下側が縦の分割数の最下層より小さい
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                 // 上側
				pIdx[1] = nCntIdx + m_nXBlock + 2; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// 下側の蓋部分のインデックス情報を設定
	for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
	{// 横の分割数+１繰り返し
	    // 上下の二つ分インデックス番号を設定
		pIdx[0] = m_nNumVertex - 1; // 下側
		pIdx[1] = nCntIdx;          // 上側

		pIdx += 2;  // 2つ分進める
		if (nCntIdxX == m_nXBlock)
		{// 横の分割数分設定が終わった
			pIdx[1] = m_nNumVertex - 1 - m_nXBlock - 1;    // 上側
			pIdx[0] = m_nNumVertex - 1;                    // 次の下側
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    頂点バッファに値を設定する
//=============================================================================
void CMeshSphere::SetVtxBuffValue(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;     // 角度
	float fRadius = 0;    // 半径
	float fSphere = 0.0f; // スフィア計算用(半径を縮める倍率)
	D3DXVECTOR3 pos;      // 座標
	float Utex;           // テクスチャU座標
	float Vtex;           // テクスチャV座標

	fAngle = 0.0f;                         // 角度は0に
	fSphere = 0.0f;                        // 倍率を設定
	fRadius = cosf(fSphere) * m_fRadius;   // 半径を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 座標を設定
	Utex = 0.0f;                           // テクスチャU座標を設定
	Vtex = 0.01f;                          // テクスチャV座標を設定

	// 上側の蓋部分の値を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_fRadius, 0.0f);
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.005f);
	pVtx++;
	fSphere -= D3DX_PI / (m_nYBlock + 2);    // 半径を縮める倍率を上げる
	fRadius = sinf(fSphere) * m_fRadius;     // 半径をサインカーブで広げていく

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			pos.x = sinf(fAngle) * fRadius;
			pos.y = cosf(fSphere) * m_fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// 頂点座標
			pVtx[nCntH].pos = pos;

			// 頂点カラー
			pVtx[nCntH].col = m_Col;

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);
			if (nCntV >= ((m_nYBlock + 1) / 2))
			{
				pVtx[nCntH].tex = D3DXVECTOR2(Utex, 1.005f);
			}

			fAngle += D3DX_PI / (m_nXBlock / 2);  // 角度を進める
			Utex += 1.0f / (m_nXBlock / 2);       // テクスチャU座標を進める

			if (fAngle >= D3DX_PI)
			{// 倍率が円周率を超えた
				fAngle -= D3DX_PI * 2.0f;
			}
		}
		fSphere -= D3DX_PI / (m_nYBlock + 2);    // 半径を縮める倍率を上げる
		fRadius = sinf(fSphere) * m_fRadius;     // 半径をサインカーブで広げていく
		fAngle = 0.0f;                           // 角度は0に
		Utex = 0.0f;                             // テクスチャU座標を設定
		Vtex += 2.0f / (m_nYBlock + 2);          // テクスチャV座標を進める
		pVtx += m_nXBlock + 1;                   // 頂点の先頭の番号を進める
	}

	// 下側の蓋部分の値を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, -m_fRadius, 0.0f);
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.01f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 頂点バッファに法線を設定する
	SetVtxBuffNor();
}

//=============================================================================
//    頂点バッファに頂点座標を設定する
//=============================================================================
void CMeshSphere::SetVtxBuffPos(void)
{

}

//=============================================================================
//    頂点バッファに法線を設定する
//=============================================================================
void CMeshSphere::SetVtxBuffNor(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 Pos = GetPos();
	D3DXVECTOR3 VerPos;
	D3DXVECTOR3 VerNormal;
	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++, pVtx++)
	{// 頂点の数だけ繰り返し
	    // 頂点座標を取得
		VerPos = pVtx[0].pos;

		// 法線ベクトルを設定
		CFunctionLib::Vec3Direction(&VerNormal, &Pos, &VerPos);
		D3DXVec3Normalize(&VerNormal, &VerNormal);
		pVtx[0].nor = VerNormal;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファに頂点カラーを設定する
//=============================================================================
void CMeshSphere::SetVtxBuffCol(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
		 // 頂点カラー
			pVtx[nCntH].col = m_Col;
		}
		// ポインタを進める
		pVtx += m_nXBlock + 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファにテクスチャ座標を設定する
//=============================================================================
void CMeshSphere::SetVtxBuffTex(void)
{

}

//=============================================================================
//    変数をクリアする
//=============================================================================
void CMeshSphere::ClearVariable(void)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;                      // 頂点バッファへのポインタ
	m_pIdxBuff = NULL;                      // インデックスバッファへのポインタ
	m_pTexture = NULL;	                    // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // メッシュスフィアの座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // メッシュスフィアの向き
	D3DXMatrixIdentity(&m_MtxWorld);        // メッシュスフィアのワールドマトリックス
	m_Col = INITIALIZE_VECTOR4;             // メッシュスフィアの色
	m_fRadius = 0.0f;                       // メッシュスフィアの半径
	m_nXBlock = 0;                          // 横の分割数
	m_nYBlock = 0;                          // 奥行の分割数
	m_nNumVertex = 0;                       // 頂点数
	m_nNumIndex = 0;                        // インデックス数
	m_nNumPolygon = 0;                      // ポリゴン数
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CMeshSphere::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMeshSphere::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    インデックスバッファ設定処理
//=============================================================================
void CMeshSphere::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    メッシュスフィアの座標設定処理
//=============================================================================
void CMeshSphere::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    メッシュスフィアの向き設定処理
//=============================================================================
void CMeshSphere::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    メッシュスフィアのワールドマトリックス設定処理
//=============================================================================
void CMeshSphere::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    メッシュスフィアの色設定処理
//=============================================================================
void CMeshSphere::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    メッシュスフィアの半径設定処理
//=============================================================================
void CMeshSphere::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CMeshSphere::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    奥行の分割数設定処理
//=============================================================================
void CMeshSphere::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshSphere::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    インデックスバッファ取得処理
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshSphere::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshSphere::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    メッシュスフィアの座標取得処理
//=============================================================================
D3DXVECTOR3 CMeshSphere::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    メッシュスフィアの向き取得処理
//=============================================================================
D3DXVECTOR3 CMeshSphere::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    メッシュスフィアのワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CMeshSphere::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    メッシュスフィアの色取得処理
//=============================================================================
D3DXCOLOR CMeshSphere::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    メッシュスフィアの半径取得処理
//=============================================================================
float CMeshSphere::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMeshSphere::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CMeshSphere::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMeshSphere::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    インデックス数取得処理
//=============================================================================
int CMeshSphere::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    ポリゴン数取得処理
//=============================================================================
int CMeshSphere::GetNumPolygon(void)
{
	return m_nNumPolygon;
}