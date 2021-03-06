//*****************************************************************************
//
//     スキルアイコンの処理[skilicon.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "skilicon.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "library.h"
#include "input.h"
#include "scene2D.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SKILICON_TEX_FILENAME_0 "data/TEXTURE/game/icon_speed_color.png"	// スピード型のスキルアイコン
#define SKILICON_TEX_FILENAME_1 "data/TEXTURE/game/icon_power_color.png"	// パワー型のスキルアイコン
#define SKILICON_TEX_FILENAME_2 "data/TEXTURE/game/icon_mine_color.png"		// 地雷型のスキルアイコン
#define SKILICON_TEX_FILENAME_3 "data/TEXTURE/game/icon_speed_bg.png"		// スピード型のスキルアイコン(灰色)
#define SKILICON_TEX_FILENAME_4 "data/TEXTURE/game/icon_power_bg.png"		// パワー型のスキルアイコン(灰色)
#define SKILICON_TEX_FILENAME_5 "data/TEXTURE/game/icon_mine_bg.png"		// 地雷型のスキルアイコン(灰色)

#define SKILICON_TEX_GROW_0		"data/TEXTURE/game/icon_speed_grow.png"		//スピード型の光
#define SKILICON_TEX_GROW_1		"data/TEXTURE/game/icon_power_grow.png"		//パワー型の光
#define SKILICON_TEX_GROW_2		"data/TEXTURE/game/icon_mine_grow.png"		//地雷型の光
#define SKILICON_TEX_GROW_3		"data/TEXTURE/game/RightEffect.jpg"			//光
#define CHANGE_COL_A			(0.025f)									//色の変更
#define RIGHT_SIZE				(25.0f)										//光のサイズ

// ポリゴンの値設定用
#define SKILICON_POLYGON_POS_0  (D3DXVECTOR3(75.0f,75.0f,0.0f))
#define SKILICON_POLYGON_POS_1  (D3DXVECTOR3(1205.0f,75.0f,0.0f))
#define SKILICON_POLYGON_POS_2  (D3DXVECTOR3(75.0f,645.0f,0.0f))
#define SKILICON_POLYGON_POS_3  (D3DXVECTOR3(1205.0f,645.0f,0.0f))

#define SKILICON_POLYGON_WIDTH  (120.0f)
#define SKILICON_POLYGON_HEIGHT (120.0f)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSkilicon::m_apTexture[SKILICON_TEXTURE_MAX] = {};
LPDIRECT3DTEXTURE9 CSkilicon::m_apTectureGrow[SKILLICON_GROW_TEX] = {};

//*****************************************************************************
//     CSkiliconの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSkilicon::CSkilicon()
{
	m_pIconMask = NULL;
	m_pIcon = NULL;
	m_pIconBG = NULL;
	m_pParentChara = NULL;
	m_pGrowIcon = NULL; 
	m_pRightEffect = NULL;
	m_pRingEffect = NULL;
	m_nIdx = 0;
	m_nRecastTime = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSkilicon::~CSkilicon()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSkilicon *CSkilicon::Create(CCharacter *pChara, int nIdx, int nRecastTime)
{
	// メモリを確保
	CSkilicon *pSkilicon = NULL;
	pSkilicon = new CSkilicon;
	if (pSkilicon == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pSkilicon->Init(pChara, nIdx, nRecastTime)))
	{
		pSkilicon->Uninit();
		delete pSkilicon;
		return NULL;
	}

	return pSkilicon;	// インスタンスのアドレスを返す
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CSkilicon::Load(void)
{
	char aFileName[SKILICON_TEXTURE_MAX][256]
	{
		SKILICON_TEX_FILENAME_0,
		SKILICON_TEX_FILENAME_1,
		SKILICON_TEX_FILENAME_2,
		SKILICON_TEX_FILENAME_3,
		SKILICON_TEX_FILENAME_4,
		SKILICON_TEX_FILENAME_5,
	};

	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return E_FAIL; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < SKILICON_TEXTURE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, aFileName[nCntTex], &m_apTexture[nCntTex]);
	}

	//光るテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SKILICON_TEX_GROW_0, &m_apTectureGrow[0]);
	D3DXCreateTextureFromFile(pDevice, SKILICON_TEX_GROW_1, &m_apTectureGrow[1]);
	D3DXCreateTextureFromFile(pDevice, SKILICON_TEX_GROW_2, &m_apTectureGrow[2]);
	D3DXCreateTextureFromFile(pDevice, SKILICON_TEX_GROW_3, &m_apTectureGrow[3]);

	return S_OK;
}

//=============================================================================
//    テクスチャの開放処理
//=============================================================================
void CSkilicon::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < SKILICON_TEXTURE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < SKILLICON_GROW_TEX; nCnt++)
	{
		if (m_apTectureGrow[nCnt] != NULL)
		{
			m_apTectureGrow[nCnt]->Release();
			m_apTectureGrow[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSkilicon::Init(CCharacter *pChara, int nIdx, int nRecastTime)
{
	// 親元のキャラクターへのポインタを保存する
	m_pParentChara = pChara;

	// 自身の番号を保存
	m_nIdx = nIdx;

	// リキャストにかかる時間を保存
	m_nRecastTime = nRecastTime;

	// 背景を作成
	CreateIconBG();

	// マスク用のポリゴンを作成
	CreateIconMask();

	//アイコンの光を生成
	CreateIconGrow();

	//光を生成
	CreateIconRight();

	// スキルアイコンを作成
	CreateIcon();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSkilicon::Uninit(void)
{
	// 背景を開放
	ReleaseIconBG();

	// マスク用のポリゴンを開放
	ReleaseIconMask();

	// スキルアイコンを開放
	ReleaseIcon();

	// アイコンの光を開放
	ReleaseIconGrow();

	//光を開放
	ReleaseIconRight();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSkilicon::Update(void)
{
	// アイコンのリキャスト処理
	RecastIcon();

	//アイコンの光を更新する 安
	if (m_pGrowIcon != NULL) { m_pGrowIcon->Update(); }
	if (m_pRightEffect != NULL) { m_pRightEffect->Update(); }
}

//=============================================================================
//    アイコン用のマスクを元に戻す処理
//=============================================================================
void CSkilicon::RevivalIconMask(void)
{
	if (m_pIcon == NULL || m_pIconMask == NULL) { return; }
	m_pIconMask->SetPos(m_pIcon->GetPos());
	m_pIconMask->SetScale(SKILICON_POLYGON_WIDTH, SKILICON_POLYGON_HEIGHT);
	m_pIconMask->SetVtxBuffPos();
}

//=============================================================================
//    アイコン用のマスクを元に戻す処理(地雷型用)
//=============================================================================
void CSkilicon::RevivalIconMask_teq(void)
{
	if (m_pIcon == NULL || m_pIconMask == NULL) { return; }
	D3DXVECTOR3 IconPos = m_pIcon->GetPos();
	float fHeight = m_pIconMask->GetHeight();
	float fCutValue = (SKILICON_POLYGON_HEIGHT * 0.2f);
	fHeight += fCutValue;
	float fAddValue = fHeight * 0.5f;
	if (fHeight >= SKILICON_POLYGON_HEIGHT)
	{
		fHeight = SKILICON_POLYGON_HEIGHT;
		fAddValue = SKILICON_POLYGON_HEIGHT * 0.5f;
	}
	m_pIconMask->SetPos(IconPos + D3DXVECTOR3(0.0f, (-SKILICON_POLYGON_HEIGHT * 0.5f) + fAddValue, 0.0f));
	m_pIconMask->SetScale(SKILICON_POLYGON_WIDTH, fHeight);
	m_pIconMask->SetVtxBuffPos();
}

//=============================================================================
//    描画処理
//=============================================================================
void CSkilicon::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// スキルアイコンの背景を描画
	DrawIconBG();

	// ステンシルバッファをクリアする
	ClearStencil(pDevice);

	// スキルアイコンマスク用のポリゴンをステンシルバッファに描画
	DrawIconMask(pDevice);

	// スキルアイコンを描画(その時マスクをかける)
	DrawIcon(pDevice);

	//アイコンの光を描画
	DrawIconGrow(pDevice);

	//光を描画
	DrawIconRight(pDevice);

	// ステンシルバッファをクリアする
	ClearStencil(pDevice);
}

//=============================================================================
//    スキルアイコン背景を開放する処理
//=============================================================================
void CSkilicon::CreateIconBG(void)
{
	if (m_pIconBG != NULL || m_pParentChara == NULL) { return; }
	D3DXVECTOR3 pos[MAX_PLAYERNUM]
	{
		SKILICON_POLYGON_POS_0,
		SKILICON_POLYGON_POS_1,
		SKILICON_POLYGON_POS_2,
		SKILICON_POLYGON_POS_3,
	};

	m_pIconBG = CIcon2D::Create(pos[m_nIdx], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) ,
		SKILICON_POLYGON_WIDTH, SKILICON_POLYGON_HEIGHT);
	if (m_pIconBG == NULL) { return; }
	m_pIconBG->BindTexture(m_apTexture[m_pParentChara->GetCharcterType() + (SKILICON_TEXTURE_MAX / 2)]);
}

//=============================================================================
//    マスク用のポリゴンを開放する処理
//=============================================================================
void CSkilicon::CreateIconMask(void)
{
	if (m_pIconMask != NULL || m_pParentChara == NULL) { return; }
	D3DXVECTOR3 pos[MAX_PLAYERNUM]
	{
		SKILICON_POLYGON_POS_0,
		SKILICON_POLYGON_POS_1,
		SKILICON_POLYGON_POS_2,
		SKILICON_POLYGON_POS_3,
	};

	m_pIconMask = CIcon2D::Create(pos[m_nIdx], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		0.0f, 0.0f);
	if (m_pIconMask == NULL) { return; }
	//m_pIconMask->BindTexture(m_apTexture[2 + (SKILICON_TEXTURE_MAX / 2)]);
}

//=============================================================================
//    スキルアイコンを開放する処理
//=============================================================================
void CSkilicon::CreateIcon(void)
{
	if (m_pIcon != NULL || m_pParentChara == NULL) { return; }
	D3DXVECTOR3 pos[MAX_PLAYERNUM]
	{
		SKILICON_POLYGON_POS_0,
		SKILICON_POLYGON_POS_1,
		SKILICON_POLYGON_POS_2,
		SKILICON_POLYGON_POS_3,
	};

	m_pIcon = CIcon2D::Create(pos[m_nIdx], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
		SKILICON_POLYGON_WIDTH, SKILICON_POLYGON_HEIGHT);
	if (m_pIcon == NULL) { return; }
	m_pIcon->BindTexture(m_apTexture[m_pParentChara->GetCharcterType()]);
}

//=============================================================================
//    アイコンの光生成
//=============================================================================
void CSkilicon::CreateIconGrow(void)
{
	if (m_pGrowIcon != NULL) { return; }

	D3DXVECTOR3 pos[MAX_PLAYERNUM]
	{//位置を列挙
		SKILICON_POLYGON_POS_0,
		SKILICON_POLYGON_POS_1,
		SKILICON_POLYGON_POS_2,
		SKILICON_POLYGON_POS_3,
	};

	m_pGrowIcon = CIcon2D::Create(pos[m_nIdx], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SKILICON_POLYGON_WIDTH + 25.0f, SKILICON_POLYGON_HEIGHT + 25.0f);
	m_pGrowIcon->BindTexture(m_apTectureGrow[m_pParentChara->GetCharcterType()]);
	m_pGrowIcon->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pGrowIcon->SetUpdateFlag(false);
}

//=============================================================================
//    光の生成
//=============================================================================
void CSkilicon::CreateIconRight(void)
{
	if (m_pRightEffect != NULL) { return; }

	D3DXVECTOR3 pos[MAX_PLAYERNUM]
	{//位置を列挙
		SKILICON_POLYGON_POS_0,
		SKILICON_POLYGON_POS_1,
		SKILICON_POLYGON_POS_2,
		SKILICON_POLYGON_POS_3,
	};

	int nType = m_pParentChara->GetCharcterType();
	m_pRightEffect = CIcon2D::Create(pos[m_nIdx], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), RIGHT_SIZE, RIGHT_SIZE);
	m_pRightEffect->BindTexture(m_apTectureGrow[3]);
	m_pRightEffect->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pRightEffect->SetVtxBuffCol();
}

//=============================================================================
//    スキルアイコン背景を開放する処理
//=============================================================================
void CSkilicon::ReleaseIconBG(void)
{
	if (m_pIconBG != NULL)
	{
		m_pIconBG->Uninit();
		delete m_pIconBG;
		m_pIconBG = NULL;
	}
}

//=============================================================================
//    マスク用のポリゴンを開放する処理
//=============================================================================
void CSkilicon::ReleaseIconMask(void)
{
	if (m_pIconMask != NULL)
	{
		m_pIconMask->Uninit();
		delete m_pIconMask;
		m_pIconMask = NULL;
	}
}

//=============================================================================
//    スキルアイコンを開放する処理
//=============================================================================
void CSkilicon::ReleaseIcon(void)
{
	if (m_pIcon != NULL)
	{
		m_pIcon->Uninit();
		delete m_pIcon;
		m_pIcon = NULL;
	}
}

//=============================================================================
//    アイコンの光を開放する処理
//=============================================================================
void CSkilicon::ReleaseIconGrow(void)
{
	if (m_pGrowIcon != NULL)
	{
		m_pGrowIcon->Uninit();
		delete m_pGrowIcon;
		m_pGrowIcon = NULL;
	}
}

//=============================================================================
//    光を開放する処理
//=============================================================================
void CSkilicon::ReleaseIconRight(void)
{
	if (m_pRightEffect != NULL)
	{
		m_pRightEffect->Uninit();
		delete m_pRightEffect;
		m_pRightEffect = NULL;
	}
}

//=============================================================================
//    アイコンをリキャストする処理
//=============================================================================
void CSkilicon::RecastIcon(void)
{
	if (m_pIconMask == NULL) { return; }
	float fHeight = m_pIconMask->GetHeight();
	float fCutValue = SKILICON_POLYGON_HEIGHT / m_nRecastTime;
	fHeight -= fCutValue;
	fCutValue /= 2.0f;
	if (fHeight <= 0.0f)
	{
		fHeight = 0.0f;
		fCutValue = 0.0f;

		//アイコンの光を有効にする
		if (m_pGrowIcon != NULL)
		{
			if (m_pGrowIcon->GetUpdateFlag() == false)
			{
				CSound * pSound = CManager::GetSound();
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE033);
				m_pGrowIcon->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_pGrowIcon->SetVtxBuffCol();
				m_pGrowIcon->SetUpdateFlag(true);
			}
		}
		if (m_pRightEffect != NULL)
		{//安
			if (m_pRightEffect->GetEffectFlag() == false && m_pRightEffect->GetOneTime() == false)
			{
				m_pRightEffect->SetEffectFlag(true);
				m_pRightEffect->SetLife(25);
				m_pRightEffect->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_pRightEffect->SetVtxBuffCol();
			}
		}
	}
	else
	{
		//アイコンの光を無効にする
		if (m_pGrowIcon != NULL)
		{
			if (m_pGrowIcon->GetUpdateFlag() == true)
			{
				m_pGrowIcon->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				m_pGrowIcon->SetVtxBuffCol();
				m_pGrowIcon->SetUpdateFlag(false);
			}
		}
		if (m_pRightEffect != NULL)
		{
			m_pRightEffect->SetOneTime(false);
		}
	}
	m_pIconMask->SetScale(m_pIconMask->GetWidth(), fHeight);
	m_pIconMask->SetPos(m_pIconMask->GetPos() + D3DXVECTOR3(0.0f, -fCutValue, 0.0f));
	m_pIconMask->SetVtxBuffPos();
}

//=============================================================================
//    スキルアイコン用背景を描画する処理
//=============================================================================
void CSkilicon::DrawIconBG(void)
{
	if (m_pIconBG != NULL)
	{
		m_pIconBG->Draw();
	}
}

//=============================================================================
//   アイコンの光の描画処理
//=============================================================================
void CSkilicon::DrawIconGrow(const LPDIRECT3DDEVICE9 pDevice)
{

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	if (m_pGrowIcon != NULL) { m_pGrowIcon->Draw(); }

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//   光の描画処理
//=============================================================================
void CSkilicon::DrawIconRight(const LPDIRECT3DDEVICE9 pDevice)
{
	 //αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	if (m_pRightEffect != NULL) { m_pRightEffect->Draw(); }

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//    マスク用のポリゴンをステンシルバッファに描画する処理
//=============================================================================
void CSkilicon::CSkilicon::DrawIconMask(const LPDIRECT3DDEVICE9 pDevice)
{
	if (m_pIconMask == NULL) { return; }

	// ステンシルバッファを使用する設定に
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// 色を描画しない設定に
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x00000000);

	// ステンシルテストの方法を設定(必ず合格)
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x00);

	// ステンシルマスク値の設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// ステンシルテストによる値の変動を設定
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	// マスクポリゴンの描画
	m_pIconMask->Draw();

	// ステンシルバッファの設定を戻す
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// 描画する色の設定を戻す
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

//=============================================================================
//    スキルアイコンを描画する処理
//=============================================================================
void CSkilicon::DrawIcon(const LPDIRECT3DDEVICE9 pDevice)
{
	if (m_pIcon == NULL) { return; }

	// ステンシルバッファを使用する設定に
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// ステンシルテストの方法を設定(参照値より大きいものが合格)
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x00);

	// ステンシルマスク値の設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

	// テストによる値の変動を設定
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	// アイコンを描画する
	m_pIcon->Draw();

	// ステンシルバッファの設定を戻す
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//=============================================================================
//    ステンシルバッファをクリアする処理
//=============================================================================
void CSkilicon::ClearStencil(const LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
}


//*****************************************************************************
//     CIcon2Dの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CIcon2D::CIcon2D()
{
	// 各種値のクリア
	m_pTexture = NULL;                         // テクスチャへのポインタ
	m_pVtxBuff = NULL;                         // 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ポリゴンの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // ポリゴンの色
	m_fRot = 0.0f;                             // ポリゴンの向き
	m_fWidth = 0.0f;                           // ポリゴンの幅
	m_fHeight = 0.0f;                          // ポリゴン高さ
	m_fAngle = 0.0f;                           // ポリゴンを出す角度
	m_fLength = 0.0f;                          // ポリゴンの長さ
	m_fTexU = 0.0f;                            // 左上テクスチャU座標
	m_fTexV = 0.0f;                            // 左上テクスチャV座標
	m_fTexWidth = 0.0f;                        // テクスチャ座標の横幅
	m_fTexHeight = 0.0f;                       // テクスチャ座標の縦幅
	m_bUpdate = false;				
	m_bEffect = false;				
	m_bOneTime = false;				//一度だけやったかどうか
	m_fChangeCol = CHANGE_COL_A;	
	m_nLife = 0;					
}

//=============================================================================
//    デストラクタ
//=============================================================================
CIcon2D::~CIcon2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CIcon2D *CIcon2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot)
{
	// メモリを確保
	CIcon2D *pIcon2D = NULL;
	pIcon2D = new CIcon2D;
	if (pIcon2D == NULL) { return NULL; }

	// 各種値の設定
	pIcon2D->SetPos(pos);              // ポリゴンの座標
	pIcon2D->SetRot(fRot);             // ポリゴンの向き
	pIcon2D->SetCol(col);              // ポリゴンの色
	pIcon2D->SetWidth(fWidth);         // ポリゴンの幅
	pIcon2D->SetHeight(fHeight);       // ポリゴンの高さ
	pIcon2D->SetTexU(fTexU);           // ポリゴンの左上テクスチャU座標
	pIcon2D->SetTexV(fTexV);           // ポリゴンの左上テクスチャV座標
	pIcon2D->SetTexWidth(fTexWidth);   // ポリゴンのテクスチャ座標の横幅
	pIcon2D->SetTexHeight(fTexHeight); // ポリゴンのテクスチャ座標の縦幅

	// 初期化処理
	if (FAILED(pIcon2D->Init()))
	{
		pIcon2D->Uninit();
		delete pIcon2D;
		return NULL;
	}

	return pIcon2D;		// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CIcon2D::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return E_FAIL; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// 頂点バッファの作成
	MakeVertex(pDevice);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CIcon2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CIcon2D::Update(void)
{
	UpdateGrow();	//アイコンの光の更新
	UpdateRight();	//光の更新処理
}

//=============================================================================
//    描画処理
//=============================================================================
void CIcon2D::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//    アイコンの光の更新処理
//=============================================================================
void CIcon2D::UpdateGrow(void)
{//安
	if (!m_bUpdate) { return; }

	//透明度の変更
	m_Col.a += m_fChangeCol;

	if (m_Col.a > 1.0f)
	{
		m_Col.a = 1.0f;
		m_fChangeCol *= -1;
	}
	else if (m_Col.a < 0.0f)
	{
		m_Col.a = 0.0f;
		m_fChangeCol *= -1;
	}

	//色をバッファに設定する
	SetVtxBuffCol();
}

//=============================================================================
//    光の更新処理
//=============================================================================
void CIcon2D::UpdateRight(void)
{
	if (!m_bEffect) { return; }

	if (m_nLife > 0)
	{
		//拡大する
		m_fWidth += 35.0f;
		m_fHeight += 35.0f;

		//スケールの更新
		SetScale(m_fWidth, m_fHeight);
		SetVtxBuffPos();
	}
	else
	{//α値を減らす
		m_Col.a -= 0.1f;

		//α値のチェック
		if (m_Col.a < 0.0f)
		{
			m_Col.a = 0.0f;
			m_bEffect = false;
			m_bOneTime = true;
			m_fWidth = RIGHT_SIZE;
			m_fHeight = RIGHT_SIZE;

			//スケールの更新
			SetScale(m_fWidth, m_fHeight);
			SetVtxBuffPos();
		}

		//色を設定する
		SetVtxBuffCol();
	}
	m_nLife--;
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CIcon2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファへ値を設定する処理
	SetVtxBuffValue();
}

//=============================================================================
//    ポリゴンの大きさを設定する処理
//=============================================================================
void CIcon2D::SetScale(float fWidth, float fHeight)
{
	// オフセットの長さと角度を計算
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fLength /= 2.0f;
	m_fAngle = atan2f(m_fWidth, -m_fHeight);
}

//=============================================================================
//    頂点バッファへ値を設定する処理
//=============================================================================
void CIcon2D::SetVtxBuffValue(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 大きさを計算
	SetScale(m_fWidth, m_fHeight);

	// 頂点の回転を考慮して座標を計算
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
	}

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ頂点座標を設定する処理
//=============================================================================
void CIcon2D::SetVtxBuffPos(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の回転を考慮して座標を計算
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへ頂点カラーを設定する処理
//=============================================================================
void CIcon2D::SetVtxBuffCol(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファへテクスチャ座標を設定する処理
//=============================================================================
void CIcon2D::SetVtxBuffTex(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_fTexU, m_fTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fTexU, m_fTexV + m_fTexHeight);
	pVtx[3].tex = D3DXVECTOR2(m_fTexU + m_fTexWidth, m_fTexV + m_fTexHeight);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}