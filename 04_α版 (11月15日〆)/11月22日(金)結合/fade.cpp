//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : 佐藤安純　SATOU ASUMI
//
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "scene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FADE_SPEED	(0.025f)		//フェードするスピード
#define FADE_SIZE	(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))
#define FADE_POS

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CFade::FADE CFade::m_fade = FADE_NONE;							// フェードの状態
D3DXCOLOR CFade::m_Coloer = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色の設定
CManager::MODE CFade::m_ModeNext = CManager::MODE_MAX;			//次のモード

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade() :CScene(MAX_LAYER - 1, OBJTYPE_FADE)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(void)
{
	//変数の初期化
	m_fade = FADE_IN;
	m_Coloer = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_ModeNext = CManager::GetMode();

	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)						//フェードインの状態処理
		{
			m_Coloer.a -= FADE_SPEED;				//画面を透明にしていく

			if (m_Coloer <= 0.0f)
			{
				m_Coloer.a = 0.0f;					//0.0fに再設定する
				m_fade = FADE_NONE;					//何もしていない状態にする
			}
		}
		else if (m_fade == FADE_OUT)				//フェードアウトの状態処理
		{
			m_Coloer.a += FADE_SPEED;				//画面を不透明にしていく

			if (m_Coloer.a >= 1.0f)
			{
				m_Coloer.a = 1.0f;					//1.0fに設定する
				m_fade = FADE_IN;					//フェードイン状態にする

				//モード設定
				CManager::SetMode(m_ModeNext);
			}
		}

		// 頂点バッファの情報を設定
		VERTEX_2D*pVtx;

		//頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Coloer.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Coloer.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Coloer.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, m_Coloer.a);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}

//=============================================================================
// フェード設定処理
//=============================================================================
void CFade::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
}

//=============================================================================
// がめん切り替え設定処理
//=============================================================================
void CFade::SetFade(CManager::MODE ModeNext)
{
	m_fade = FADE_OUT;								//フェードアウト状態にする
	m_ModeNext = ModeNext;
	m_Coloer = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//透明状態にする
}

//=============================================================================
// がめん切り替え設定処理
//=============================================================================
CFade::FADE CFade::GetFadeMode(void)
{
	return m_fade;
}