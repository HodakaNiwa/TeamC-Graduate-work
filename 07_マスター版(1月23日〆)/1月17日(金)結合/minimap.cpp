//*****************************************************************************
//
//     ミニマップの処理[minimap.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "minimap.h"
#include "manager.h"
#include "modebase.h"
#include "player.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define VECTOR_ZERO		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// ベクトルの初期化
#define MINAMAP_PRIORITY (7)							// ミニマップの処理優先順位

// 背景用マクロ
#define MINIMAP_BG_POS      (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define MINIMAP_BG_POS_ONCE (D3DXVECTOR3(1220.0f, 60.0f, 0.0f))
#define MINIMAP_BG_SIZE     (D3DXVECTOR3(50.0f,50.0f,0.0f))
#define MINIMAP_BG_COL      (D3DXCOLOR(0.4f,0.4f,0.4f,1.0f))

// プレイヤーアイコン用マクロ
#define MINIMAP_PLAYERICON_TEXIDX     (0)
#define MINIMAP_PLAYERICON_SIZE       (D3DXVECTOR3(5.0f,5.0f,0.0f))
#define MINIMAP_PLAYERICON_LIMIT_X    (30.0f)
#define MINIMAP_PLAYERICON_LIMIT_Y    (30.0f)
#define MINIMAP_PLAYERICON_RIVISION_X (2.0f)
#define MINIMAP_PLAYERICON_RIVISION_Y (5.0f)

//=============================================================================
//    生成処理
//=============================================================================
CMiniMap * CMiniMap::Create(void)
{
	CMiniMap *pMiniMap = NULL;
	if (pMiniMap != NULL) { return NULL; }

	// メモリを確保
	pMiniMap = new CMiniMap;
	if (pMiniMap == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pMiniMap->Init()))
	{
		pMiniMap->Uninit();
		delete pMiniMap;
		return NULL;
	}

	return pMiniMap;	// インスタンスのアドレスを返す
}

//=============================================================================
//    コンストラクタ
//=============================================================================
CMiniMap::CMiniMap()
{
	m_pBG = NULL;
	for (int nCnt = 0; nCnt < MINIMAP_MAX_PLAYERNUM; nCnt++)
	{
		m_pPlayerIcon[nCnt] = NULL;
		m_pCharacter[nCnt] = NULL;
	}

	m_BGPos = INITIALIZE_VECTOR3;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMiniMap::~CMiniMap() {}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMiniMap::Init(void)
{
	// 背景の位置を初期化
	m_BGPos = MINIMAP_BG_POS;

	// 背景の作成
	CreateBG();

	// プレイヤーアイコンの作成
	CreatePlayerIcon();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMiniMap::Uninit()
{
	// 背景の開放
	ReleaseBG();

	// プレイヤーアイコンの開放
	ReleasePlayerIcon();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMiniMap::Update()
{
	// プレイヤーアイコンの位置を移動させる
	SettingPlayerIconPos();
}

//=============================================================================
//    描画処理
//=============================================================================
void CMiniMap::Draw()
{

}

//=============================================================================
//    プレイヤーアイコンの位置を設定する処理
//=============================================================================
void CMiniMap::SettingPlayerIconPos(void)
{
	D3DXVECTOR3 IconPos;
	for (int nCnt = 0; nCnt < MINIMAP_MAX_PLAYERNUM; nCnt++)
	{
		// アイコンが生成されていなければ処理せず次のループへ
		if (m_pPlayerIcon[nCnt] == NULL) { continue; }

		// ポインタが設定されていなければ処理せず次のループへ
		if (m_pCharacter[nCnt] == NULL) { continue; }

		// 位置をミニマップのサイズに合わせる
		IconPos.x = m_pCharacter[nCnt]->GetPos().x / MINIMAP_PLAYERICON_LIMIT_X + m_BGPos.x + MINIMAP_PLAYERICON_RIVISION_X;
		IconPos.y = -m_pCharacter[nCnt]->GetPos().z / MINIMAP_PLAYERICON_LIMIT_Y + m_BGPos.y + MINIMAP_PLAYERICON_RIVISION_Y;
		IconPos.z = 0.0f;

		// 位置の設定
		m_pPlayerIcon[nCnt]->SetPos(IconPos, MINIMAP_PLAYERICON_SIZE);
	}
}

//=============================================================================
//    背景の位置を設定する処理
//=============================================================================
void CMiniMap::SettingBGPos(void)
{
	m_BGPos = MINIMAP_BG_POS_ONCE;
	if (m_pBG == NULL) { return; }
	m_pBG->SetPos(m_BGPos, MINIMAP_BG_SIZE);
}

//=============================================================================
//    背景を作成する処理
//=============================================================================
void CMiniMap::CreateBG(void)
{
	if (m_pBG != NULL) { return; }
	m_pBG = CScene2D::Create(m_BGPos, MINIMAP_BG_SIZE, CScene2D::TYPE_LOGO);

	if (m_pBG != NULL)
	{
		m_pBG->BindTexture(NULL);
		m_pBG->SetColoer(MINIMAP_BG_COL);
	}
}

//=============================================================================
//    プレイヤーアイコンを作成する処理
//=============================================================================
void CMiniMap::CreatePlayerIcon(void)
{
	for (int nCnt = 0; nCnt < MINIMAP_MAX_PLAYERNUM; nCnt++)
	{
		// アイコンが生成されていなければ処理せず次のループへ
		if (m_pPlayerIcon[nCnt] != NULL) { continue; }

		// アイコンの作成
		m_pPlayerIcon[nCnt] = CScene2D::Create(VECTOR_ZERO, MINIMAP_PLAYERICON_SIZE, CScene2D::TYPE_LOGO);
		if (m_pPlayerIcon[nCnt] == NULL) { continue; }

		// 値の設定
		m_pPlayerIcon[nCnt]->BindTexture(CManager::GetModeBase()->GetTexture()[MINIMAP_PLAYERICON_TEXIDX]);
		m_pPlayerIcon[nCnt]->SetColoer(CPlayer::m_CountryColor[nCnt]);
	}
}

//=============================================================================
//    背景を削除する処理
//=============================================================================
void CMiniMap::ReleaseBG(void)
{
	if (m_pBG != NULL)
	{
		m_pBG->Uninit();
		m_pBG = NULL;
	}
}

//=============================================================================
//    プレイヤーアイコンを削除する処理
//=============================================================================
void CMiniMap::ReleasePlayerIcon(void)
{
	for (int nCnt = 0; nCnt < MINIMAP_MAX_PLAYERNUM; nCnt++)
	{
		if (m_pPlayerIcon[nCnt] != NULL)
		{
			m_pPlayerIcon[nCnt]->Uninit();
			m_pPlayerIcon[nCnt] = NULL;
		}
	}
}