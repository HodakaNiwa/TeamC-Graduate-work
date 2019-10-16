//=============================================================================
//
// ナンバーの処理 [number.cpp]
// Author :Yamashita
//
//=============================================================================
#include "number.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "score.h"


//============================================================================
//	マクロ定義
//============================================================================
#define MAX (20)
#define BG_TEXTURE_UV_U			(1.0f)							// テクスチャアニメーションU範囲
#define BG_TEXTURE_UV_V			(1.0f)	

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = {};
//=============================================================================
//	コンストラクタ
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
}

//=============================================================================
//デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CNumber::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//インスタンス
	CManager Manager;

	//デバイスを取得
	pDevice = Manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &m_pTexture);

	return S_OK;
}
//=============================================================================
// ナンバーの初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fWeight, float fHeight)
{
	LPDIRECT3DDEVICE9 pDevice;
	
	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	m_pos = pos;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	//頂点情報へのポインタ
	VERTEX_2D*pVtx;

	//バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWeight, m_pos.y - fHeight, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWeight, m_pos.y - fHeight, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWeight, m_pos.y + fHeight, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWeight, m_pos.y + fHeight, 0.0f) + m_pos;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//色の設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ナンバーの終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// ナンバーの更新処理
//=============================================================================
void CNumber::Update(void)
{
	
}

//=============================================================================
// ナンバーの描画処理
//=============================================================================
void CNumber::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//ナンバーの生成処理
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, float fWeight, float fHeight)
{

	CNumber *pNumber = NULL;

	if (pNumber == NULL)
	{
		//メモリを動的確保
		pNumber = new CNumber;

		if (pNumber != NULL)
		{
			// ポリゴンの初期化処理
			pNumber->Init(pos,fWeight,fHeight);
		}
		else
		{
			MessageBox(0, "NULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "NULLじゃないです", "警告", MB_OK);
	}

	return pNumber;
}

//=============================================================================
// ナンバーの開放処理
//=============================================================================
void CNumber::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ナンバーの設定処理
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	//頂点情報へのポインタ
	VERTEX_2D*pVtx;

	//バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2((nNumber % 10) * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((nNumber % 10) * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((nNumber % 10) * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((nNumber % 10) * 0.1f + 0.1f, 1.0f);
	
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}


//=============================================================================
// ナンバーの色の設定処理
//=============================================================================
void CNumber::SetColNumber(D3DXCOLOR col)
{
	//頂点情報へのポインタ
	VERTEX_2D*pVtx;

	//バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}