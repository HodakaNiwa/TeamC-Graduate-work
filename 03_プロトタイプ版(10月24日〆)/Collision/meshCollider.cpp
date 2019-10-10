//*****************************************************************************
//
//     当たり判定用メッシュモデルの処理[meshCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
#include "manager.h"
#include "system.h"
#include "library.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CMeshCollider::CMeshCollider(TYPE type, CScene *pParent) : CCollider(type, pParent)
{
	// 各種値のクリア
	m_pVerPos = NULL;           // 頂点座標へのポインタ
	m_fWidth = 0.0f;            // 1ブロック分の幅
	m_fDepth = 0.0f;            // 1ブロック分の奥行
	m_nXBlock = 0;              // 横の分割数
	m_nZBlock = 0;              // 奥行の分割数
	m_nNumVertex = 0;           // 頂点数
	m_pVtxBuffParent = NULL;    // 親頂点バッファへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshCollider::~CMeshCollider()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshCollider *CMeshCollider::Create(float fWidth, float fDepth, int nXBlock, int nZBlock, int nNumVertex, LPDIRECT3DVERTEXBUFFER9 pVtxBuffParent, D3DXMATRIX *pMtxParent, bool bReturnFlag, CScene *pParent)
{
	CMeshCollider *pMeshCollider = NULL;   // 当たり判定用メッシュモデルクラス型のポインタ
	pMeshCollider = new CMeshCollider;
	if (pMeshCollider == NULL) { return NULL; }

	// 各種値の設定
	pMeshCollider->SetWidth(fWidth);                   // 1ブロック分の幅
	pMeshCollider->SetDepth(fDepth);                   // 1ブロック分の奥行
	pMeshCollider->SetXBlock(nXBlock);                 // 横の分割数
	pMeshCollider->SetZBlock(nZBlock);                 // 奥行の分割数
	pMeshCollider->SetVtxBuffParent(pVtxBuffParent);   // 親頂点バッファへのポインタ
	pMeshCollider->SetParentMtxWorld(pMtxParent);      // 親マトリックスへのポインタ
	pMeshCollider->SetReturnFlag(bReturnFlag);         // 当たった際に戻すかどうか
	if (FAILED(pMeshCollider->Init()))
	{
		pMeshCollider->Uninit();
		pMeshCollider = NULL;
		return NULL;
	}

	return pMeshCollider;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshCollider::Init(void)
{
	// 描画デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 作成に必要な頂点数,インデックス数,ポリゴン数を計算
	m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);

	// 頂点情報の作成
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMeshCollider::Uninit(void)
{
	// 頂点座標へのポインタを開放
	if (m_pVerPos != NULL)
	{
		delete[] m_pVerPos;
		m_pVerPos = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMeshCollider::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshCollider::Draw(void)
{

}

//=============================================================================
//    頂点情報を作成する処理
//=============================================================================
void CMeshCollider::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点座標用のメモリを確保
	if (m_pVerPos == NULL)
	{
		m_pVerPos = new D3DXVECTOR3[m_nNumVertex];
	}

	// ワールド空間上に頂点座標を設定
	SetWorldVertexPos();
}

//=============================================================================
//    ワールド空間上に頂点座標を設定する処理
//=============================================================================
void CMeshCollider::SetWorldVertexPos(void)
{
	if (m_pVerPos == NULL) { return; }

	// 頂点情報の設定
	VERTEX_3D *pVtxParent = NULL;
	D3DXMATRIX *m_pMtxParent = GetParentMtxWorld();

	// 親頂点バッファへのポインタからデータを取得
	if (m_pVtxBuffParent != NULL)
	{
		m_pVtxBuffParent->Lock(0, 0, (void**)&pVtxParent, 0);
	}

	D3DXVECTOR3 VerPos = INITIALIZE_VECTOR3;
	VerPos.x = -(m_fWidth * m_nXBlock) / 2;
	VerPos.z = (m_fDepth * m_nZBlock) / 2;
	for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
			// 親頂点バッファから情報を取得
			if (pVtxParent != NULL)
			{
				VerPos = pVtxParent[nCntH].pos;
			}

			// 親マトリックスと掛け合わせる
			if (m_pMtxParent != NULL)
			{
				D3DXVec3TransformCoord(&VerPos, &VerPos, m_pMtxParent);
			}

			// 頂点座標を設定
			m_pVerPos[nCntH + ((m_nXBlock + 1) * nCntV)] = VerPos;
		}

		// ポインタを進める
		if (pVtxParent != NULL)
		{
			pVtxParent += m_nXBlock + 1;
		}
	}

	// 頂点バッファをアンロックする
	if (m_pVtxBuffParent != NULL)
	{
		m_pVtxBuffParent->Unlock();
	}
}

//=============================================================================
//    ポリゴンの高さを取得する処理
//=============================================================================
float CMeshCollider::GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal, D3DXVECTOR3 *pVerPos)
{
	if (m_pVerPos == NULL) { return 0.0f; }

	float fHeight = 0.0f;               // 高さ(最終的にこの値を返す)
	float fRivision = 0.0f;             // 高さの補正値(法線を割り出すのに0.0の高さにそろえるため)
	int nVertexIdxStart = 0;            // 左上の頂点番号(ループするたびにずれていく)
	D3DXVECTOR3 VertexLeftWorldPos[3];  // 左側のポリゴンの頂点のワールド座標3つ分(頂点座標へのポインタから抜き出す)
	D3DXVECTOR3 VertexRightWorldPos[3]; // 右側のポリゴンの頂点のワールド座標3つ分(頂点座標へのポインタから抜き出す)
	D3DXVECTOR3 VectorVerToVer;         // 外積ベクトル1本目(頂点から頂点へのベクトル)
	D3DXVECTOR3 VectorVerToPos;         // 外積ベクトル2本目(頂点から引数の座標へのベクトル)
	D3DXVECTOR3 PolygonVerPos[3];       // ポリゴンの座標(判定用の法線を導くために必要)
	D3DXVECTOR3 PolygonNormal;          // ポリゴンの法線(判定用の法線なので見た目の法線とは異なる)
	D3DXVECTOR3 PolygonVector[2];       // ポリゴンの法線を割り出すためのベクトル
	D3DXVECTOR3 VecPosToPolygon;        // 引数の座標とポリゴンとのベクトル
	int nCntLeftPolygon = 0;            // 左側のポリゴンのベクトル判定を通った回数
	int nCntRightPolygon = 0;           // 右側のポリゴンのベクトル判定を通った回数

	// フィールド内にいるかどうか判定
	D3DXVECTOR3 FieldPos = GetPos();
	if (pos.x <= FieldPos.x - (m_fWidth * m_nXBlock) || pos.x >= FieldPos.x + (m_fWidth * m_nXBlock)
		&& pos.z <= FieldPos.z - (m_fDepth * m_nZBlock) || pos.z >= FieldPos.z + (m_fDepth * m_nZBlock))
	{
		if (pLand != NULL)
		{
			*pLand = false;
		}
		return 0.0f;
	}

	for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
	{// 奥行の分割数分繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
		{// 横の分割数分繰り返し
			// 左側のポリゴンの判定開始
			VertexLeftWorldPos[0] = m_pVerPos[nVertexIdxStart];
			VertexLeftWorldPos[1] = m_pVerPos[nVertexIdxStart + (m_nXBlock + 1) + 1];
			VertexLeftWorldPos[2] = m_pVerPos[nVertexIdxStart + (m_nXBlock + 1)];
			for (int nCntLeft = 0; nCntLeft < 3; nCntLeft++)
			{// ポリゴンの頂点数分繰り返し
			    // 外積のベクトルを作り出す
				VectorVerToVer = VertexLeftWorldPos[(nCntLeft + 1) % 3] - VertexLeftWorldPos[nCntLeft];
				VectorVerToPos = pos - VertexLeftWorldPos[nCntLeft];

				if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
				{// 引数の座標が頂点と頂点のベクトルの右側にいる
					nCntLeftPolygon++;  // 判定を通った回数を進める
				}
			}
			if (nCntLeftPolygon == 3)
			{// 外積の判定を全て通った
			    // 判定用の頂点座標を作る
				PolygonVerPos[0] = D3DXVECTOR3(VertexLeftWorldPos[2].x, VertexLeftWorldPos[2].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[2].z);
				PolygonVerPos[1] = D3DXVECTOR3(VertexLeftWorldPos[1].x, VertexLeftWorldPos[1].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[1].z);
				PolygonVerPos[2] = D3DXVECTOR3(VertexLeftWorldPos[0].x, VertexLeftWorldPos[0].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[0].z);

				// 外積ベクトルを作る
				PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
				PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

				// 外積ベクトルから法線を割り出す
				D3DXVec3Cross(&PolygonNormal, &PolygonVector[0], &PolygonVector[1]);
				D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

				// 判定の仕方が0に戻したうえで計算しているので補正をするために値を保持
				fRivision = VertexLeftWorldPos[2].y;

				// 引数の座標と頂点座標のベクトルを計算
				VecPosToPolygon = pos - PolygonVerPos[0];

				// 内積の概念を利用して高さを割り出す
				float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X成分の内積を計算
				float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z成分の内積を計算
				float Dot = -DotX - DotZ;                               // X成分とZ成分の内積同士を引く
				if (PolygonNormal.y != 0.0f)
				{
					fHeight = (Dot / PolygonNormal.y) + fRivision;      // 引いたものをY成分の法線で割って補正値を足してあげる
				}
				else
				{
					return 0.0f;
				}

				// 高さから角度を求める
				//float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
				//float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
				//float DotY = (PolygonNormal.y * fHeight);
				//float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
				//float fAngle = acosf(cosSita);

				if (pLand != NULL)
				{
					*pLand = true;
				}
				if (pPolyNormal != NULL)
				{
					// 判定用の頂点座標を作る
					PolygonVerPos[0] = D3DXVECTOR3(VertexLeftWorldPos[2].x, VertexLeftWorldPos[2].y, VertexLeftWorldPos[2].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexLeftWorldPos[1].x, VertexLeftWorldPos[1].y, VertexLeftWorldPos[1].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexLeftWorldPos[0].x, VertexLeftWorldPos[0].y, VertexLeftWorldPos[0].z);

					// 外積ベクトルを作る
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// 外積ベクトルから法線を割り出す
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[1], &PolygonVector[0]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);


					*pPolyNormal = PolygonNormal;
				}
				if (pVerPos != NULL)
				{
					*pVerPos = VertexLeftWorldPos[2];
				}
			}

			// 右側のポリゴンの判定開始
			VertexRightWorldPos[0] = m_pVerPos[nVertexIdxStart];
			VertexRightWorldPos[1] = m_pVerPos[nVertexIdxStart + 1];
			VertexRightWorldPos[2] = m_pVerPos[nVertexIdxStart + (m_nXBlock + 1) + 1];
			for (int nCntRight = 0; nCntRight < 3; nCntRight++)
			{// ポリゴンの頂点数分繰り返し
			 // 外積のベクトルを作り出す
				VectorVerToVer = VertexRightWorldPos[(nCntRight + 1) % 3] - VertexRightWorldPos[nCntRight];
				VectorVerToPos = pos - VertexRightWorldPos[nCntRight];

				if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
				{// 引数の座標が頂点と頂点のベクトルの右側にいる
					nCntRightPolygon++;  // 判定を通った回数を進める
				}
			}
			if (nCntRightPolygon == 3)
			{// 外積の判定を全て通った
			    // 判定用の頂点座標を作る
				PolygonVerPos[0] = D3DXVECTOR3(VertexRightWorldPos[1].x, VertexRightWorldPos[1].y - VertexRightWorldPos[1].y, VertexRightWorldPos[1].z);
				PolygonVerPos[1] = D3DXVECTOR3(VertexRightWorldPos[0].x, VertexRightWorldPos[0].y - VertexRightWorldPos[1].y, VertexRightWorldPos[0].z);
				PolygonVerPos[2] = D3DXVECTOR3(VertexRightWorldPos[2].x, VertexRightWorldPos[2].y - VertexRightWorldPos[1].y, VertexRightWorldPos[2].z);

				// 外積ベクトルを作る
				PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
				PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

				// 外積ベクトルから法線を割り出す
				D3DXVec3Cross(&PolygonNormal, &PolygonVector[1], &PolygonVector[0]);
				D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

				// 判定の仕方が0に戻したうえで計算しているので補正をするために値を保持
				fRivision = VertexRightWorldPos[1].y;

				// 引数の座標と頂点座標のベクトルを計算
				VecPosToPolygon = pos - PolygonVerPos[0];

				// 内積の概念を利用して高さを割り出す
				float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X成分の内積を計算
				float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z成分の内積を計算
				float Dot = -DotX - DotZ;                               // X成分とZ成分の内積同士を引く
				if (PolygonNormal.y != 0.0f)
				{
					fHeight = (Dot / PolygonNormal.y) + fRivision;          // 引いたものをY成分の法線で割って補正値を足してあげる
				}
				else
				{
					return 0.0f;
				}

				// 高さから角度を求める
				//float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
				//float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
				//float DotY = (PolygonNormal.y * fHeight);
				//float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
				//float fAngle = acosf(cosSita);

				if (pLand != NULL)
				{
					*pLand = true;
				}
				if (pPolyNormal != NULL)
				{
					// 判定用の頂点座標を作る
					PolygonVerPos[0] = D3DXVECTOR3(VertexRightWorldPos[1].x, VertexRightWorldPos[1].y, VertexRightWorldPos[1].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexRightWorldPos[0].x, VertexRightWorldPos[0].y, VertexRightWorldPos[0].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexRightWorldPos[2].x, VertexRightWorldPos[2].y, VertexRightWorldPos[2].z);

					// 外積ベクトルを作る
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// 外積ベクトルから法線を割り出す
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[0], &PolygonVector[1]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);


					*pPolyNormal = PolygonNormal;
				}
				if (pVerPos != NULL)
				{
					*pVerPos = VertexRightWorldPos[1];
				}
			}


			// 判定用の変数を初期化しておく
			if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
			{// ポリゴンに乗っていると判定されている
				break;
			}
			else
			{// まだポリゴンに乗っていないと判定されている
				nCntLeftPolygon = 0;
				nCntRightPolygon = 0;
				nVertexIdxStart++;
			}
		}
		if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
		{// ポリゴンに乗っていると判定されている
			break;
		}
		else
		{// まだポリゴンに乗っていないと判定されている
		 // 右端のブロックに判定するべきポリゴンは存在しないので番号をずらしておく
			nVertexIdxStart++;
		}
	}

	return fHeight;
}

//=============================================================================
//    メッシュに貫通しているか判定する処理
//=============================================================================
bool CMeshCollider::CheckPenetration(D3DXVECTOR3 pos, int *pXBlock, int *pYBlock, int *pLeftVerIndex)
{
	bool bPene = false;

	return bPene;
}

//=============================================================================
//    頂点座標へのポインタを設定する処理
//=============================================================================
void CMeshCollider::SetVerPos(D3DXVECTOR3 *pVerPos)
{
	m_pVerPos = pVerPos;
}

//=============================================================================
//    1ブロック分の幅を設定する処理
//=============================================================================
void CMeshCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    1ブロック分の奥行を設定する処理
//=============================================================================
void CMeshCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    横の分割数を設定する処理
//=============================================================================
void CMeshCollider::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    奥行の分割数を設定する処理
//=============================================================================
void CMeshCollider::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;
}

//=============================================================================
//    親頂点バッファへのポインタを設定する処理
//=============================================================================
void CMeshCollider::SetVtxBuffParent(const LPDIRECT3DVERTEXBUFFER9 pVtxBuffParent)
{
	m_pVtxBuffParent = pVtxBuffParent;
}

//=============================================================================
//    当たった際に戻すかどうか設定する処理
//=============================================================================
void CMeshCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    頂点座標へのポインタを取得する処理
//=============================================================================
D3DXVECTOR3 *CMeshCollider::GetVerPos(void)
{
	return m_pVerPos;
}

//=============================================================================
//    1ブロック分の幅を取得する処理
//=============================================================================
float CMeshCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1ブロック分の奥行を取得する処理
//=============================================================================
float CMeshCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    横の分割数を取得する処理
//=============================================================================
int CMeshCollider::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数を取得する処理
//=============================================================================
int CMeshCollider::GetZBlock(void)
{
	return m_nZBlock;
}

//=============================================================================
//    頂点数を取得する処理
//=============================================================================
int CMeshCollider::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    親頂点バッファへのポインタを取得する処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshCollider::GetVtxBuffParent(void)
{
	return m_pVtxBuffParent;
}

//=============================================================================
//    当たった際に戻すかどうか取得する処理
//=============================================================================
bool CMeshCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}