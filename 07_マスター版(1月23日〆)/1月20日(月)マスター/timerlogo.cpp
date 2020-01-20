//=============================================================================
//
// タイマーロゴの処理 [timerlogo.cpp]
// Author : 山下敦史 & 佐藤安純
//
//=============================================================================
#include "timerlogo.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "logo.h"
#include "bottun.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CTimerLogo::m_pTexture[TIMERLOGO_MAX] = {};

//=============================================================================
// マクロ定義
//=============================================================================
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	//ベクトルの初期化

//=============================================================================
// 生成処理
//=============================================================================
CTimerLogo * CTimerLogo::Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size,TIMERLOGO timerlogo)
{
	//インスタンスの生成
	CTimerLogo * pTimerLogo = NULL;

	if (pTimerLogo == NULL)
	{
		pTimerLogo = new CTimerLogo;

		if (pTimerLogo != NULL)
		{
			pTimerLogo->m_Size = size;
			pTimerLogo->m_FirstSize = size * 2.0f;	//安
			pTimerLogo->m_pos = pos;
			pTimerLogo->m_TimerLogo = timerlogo;
			pTimerLogo->BindTexture(m_pTexture[timerlogo]);
			pTimerLogo->Init();
			pTimerLogo->SetColoer(INITIALIZE_VECTOR4);
		}
	}
	return pTimerLogo;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CTimerLogo::CTimerLogo(int nPriority, CScene::OBJTYPE type)
{
	m_pVtxBuff = NULL;				// 頂点バッファへのポインタ
	m_pos = VECTOR_ZERO;			// ポリゴンの位置
	m_Size = VECTOR_ZERO;			// ポリゴンのサイズ
	m_FirstSize = VECTOR_ZERO;		//最初のサイズ　安
	m_Color = INITIALIZE_VECTOR4;
	m_bDeth = false;				//死亡フラグ　安
	m_TimerLogo = TIMERLOGO_MAX;
}

//=============================================================================
// コンストラクタ
//=============================================================================
CTimerLogo::CTimerLogo()
{
	m_pVtxBuff = NULL;				// 頂点バッファへのポインタ
	m_pos = VECTOR_ZERO;			// ポリゴンの位置
	m_Size = VECTOR_ZERO;			// ポリゴンのサイズ
	m_FirstSize = VECTOR_ZERO;		//最初のサイズ　安
	m_Color = INITIALIZE_VECTOR4;
	m_bDeth = false;				//死亡フラグ　安
	m_TimerLogo = TIMERLOGO_MAX;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTimerLogo::~CTimerLogo() {}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
HRESULT CTimerLogo::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//インスタンス（マネージャー）
	CManager Manager;

	//デバイスを取得
	pDevice = Manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\game_start000.png", &m_pTexture[0]);			//スタートの画像
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\time_announce000.png", &m_pTexture[1]);		//2分の画像
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\time_announce001.png", &m_pTexture[2]);		//1分の画像
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\time_announce002.png", &m_pTexture[3]);		//30秒の画像
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\game\\game_finish000.png", &m_pTexture[4]);			//フィニッシュの画像

	return S_OK;
}
//=============================================================================
// テクスチャの開放処理
//=============================================================================
void CTimerLogo::Unload(void)
{
	for (int nCnt = 0; nCnt < TIMERLOGO_MAX; nCnt++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTimerLogo::Init(void)
{
	CScene2D::Init();

	//初期値
	m_Color = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);
	SetColoer(m_Color);

	if (m_TimerLogo == TIMERLOGO_2MIN || m_TimerLogo == TIMERLOGO_1MIN || m_TimerLogo == TIMERLOGO_30SEC)
	{
		Set(m_pos, m_FirstSize);
	}
	else
	{
		Set(m_pos, m_Size);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTimerLogo::Uninit()
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTimerLogo::Update()
{//安
	if (m_TimerLogo == TIMERLOGO_2MIN || m_TimerLogo == TIMERLOGO_1MIN || m_TimerLogo == TIMERLOGO_30SEC)
	{//FINテクスチャじゃなかったら

		//変数宣言
		bool m_SetSizeX = false;
		bool m_SetSizeY = false;

		//縮小する
		m_FirstSize.x -= 2.8f;
		m_FirstSize.y -= 2.5f;

		//値の修正
		if (m_FirstSize.x < m_Size.x)
		{
			m_FirstSize.x = m_Size.x;
			m_SetSizeX = true;
		}
		if (m_FirstSize.y < m_Size.y)
		{
			m_FirstSize.y = m_Size.y;
			m_SetSizeY = true;
		}

		//値を設定する
		Set(m_pos, m_FirstSize);

		if (m_SetSizeX && m_SetSizeY)
		{
			m_Color.a -= 0.01f;

			if (m_Color.a < 0.0f) { m_bDeth = true; }
		}

		SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Color.a));
	}
	else if (m_TimerLogo == TIMERLOGO_START)
	{//色の設定
		
		m_Color.a -= 0.01f;
		SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Color.a));
		if (m_Color.a < 0.0f) { m_bDeth = true; }
	}
	else if(m_TimerLogo == TIMERLOGO_FIN)
	{//色の設定
		SetColoer(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	//死亡フラグが有効だったら
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CTimerLogo::Draw()
{
	CScene2D::Draw();
}