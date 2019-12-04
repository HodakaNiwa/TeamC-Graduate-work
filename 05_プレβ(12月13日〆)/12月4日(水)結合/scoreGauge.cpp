//*****************************************************************************
//
//     スコアゲージの処理[skilicon.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "scoreGauge.h"
#include "skilicon.h"
#include "character.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// ゲージ背景用
#define SCOREGAUGE_BG_POS           (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 640.0f,0.0f))
#define SCOREGAUGE_BG_COL           (D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))
#define SCOREGAUGE_BG_WIDTH         (1100.0f)
#define SCOREGAUGE_BG_HEIGHT        (70.0f)
#define SCOREGAUGE_BG_FRAMESIZE_X   (10.0f)
#define SCOREGAUGE_BG_FRAMESIZE_Y   (10.0f)

// ゲージ用
#define SCOREGAUGE_RING_PRIORITY    (3)

// プレイヤー番号用
#define SCOREGAUGE_PLAYERIDX_POS    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 680.0f,0.0f))
#define SCOREGAUGE_PLAYERIDX_WIDTH  (60.0f)
#define SCOREGAUGE_PLAYERIDX_HEIGHT (40.0f)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CScoreGauge::CScoreGauge()
{
	m_nNumPlayer = 0;
	m_pGaugeBg = NULL;
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		m_pGauge2D[nCnt] = NULL;
		m_pPlayerIdx[nCnt] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScoreGauge::~CScoreGauge()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScoreGauge *CScoreGauge::Create(const int nNumPlayer)
{
	// メモリを確保
	CScoreGauge *pScoreGauge = NULL;
	pScoreGauge = new CScoreGauge;
	if (pScoreGauge == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pScoreGauge->Init(nNumPlayer)))
	{
		pScoreGauge->Uninit();
		delete pScoreGauge;
		return NULL;
	}

	return pScoreGauge;	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScoreGauge::Init(const int nNumPlayer)
{
	// 参加しているプレイヤーの人数を保存
	m_nNumPlayer = nNumPlayer;
	m_fGaugePosY = SCOREGAUGE_BG_POS.y;
	m_fGaugeHeight = SCOREGAUGE_BG_HEIGHT;
	if (m_nNumPlayer >= 3)
	{// 3人以上ならばゲージを出す高さを変える
		m_fGaugePosY = SCREEN_HEIGHT * 0.5f;
		m_fGaugeHeight *= 0.5f;
	}

	// ゲージ背景の生成
	CreateGaugeBg();

	// ゲージの生成
	CreateGauge2D();

	// プレイヤー番号の生成
	CreatePlayerIdx();

	// 1ポイントごとのゲージ量を計算する
	CalcGaugeOnceFromRing();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CScoreGauge::Uninit(void)
{
	// ゲージ背景の開放
	ReleaseGaugeBg();

	// ゲージの開放
	ReleaseGauge2D();

	// プレイヤー番号の開放
	ReleasePlayerIdx();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScoreGauge::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CScoreGauge::Draw(void)
{
	// ゲージ背景の描画
	DrawGaugeBg();

	// ゲージの描画
	DrawGauge2D();

	// プレイヤー番号の描画
	DrawPlayerIdx();
}

//=============================================================================
//    ゲージに加える処理
//=============================================================================
void CScoreGauge::AddGauge(const int nIdx, const int nPoint)
{
	if (m_pGauge2D[nIdx] == NULL) { return; }
	float fWidth = m_pGauge2D[nIdx]->GetWidth();
	D3DXVECTOR3 pos = m_pGauge2D[nIdx]->GetPos();
	fWidth += m_fGaugeOnce * nPoint;
	float fCutValue = (m_fGaugeOnce * nPoint) * 0.5f;
	if (fWidth <= 0.0f)
	{
		fWidth = 0.0f;
		fCutValue = 0.0f;
	}
	pos.x += fCutValue;
	m_pGauge2D[nIdx]->SetPos(pos);
	m_pPlayerIdx[nIdx]->SetPos(pos);
	m_pGauge2D[nIdx]->SetScale(fWidth, m_pGauge2D[nIdx]->GetHeight());
	m_pGauge2D[nIdx]->SetVtxBuffPos();
	if (m_pPlayerIdx[nIdx] != NULL)
	{
		m_pPlayerIdx[nIdx]->SetPos(pos + D3DXVECTOR3(0.0f, (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f), 0.0f));
		m_pPlayerIdx[nIdx]->SetVtxBuffPos();
	}

	// 自分以降のゲージの場所をずらす
	D3DXVECTOR3 GaugePos = pos;
	fWidth = fWidth * 0.5f;
	for (int nCnt = nIdx + 1; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] == NULL) { continue; }
		GaugePos.x += fWidth;
		GaugePos.x += m_pGauge2D[nCnt]->GetWidth() * 0.5f;
		m_pGauge2D[nCnt]->SetPos(GaugePos);
		m_pGauge2D[nCnt]->SetVtxBuffPos();
		if (m_pPlayerIdx[nCnt] != NULL)
		{
			GaugePos.y += (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f);
			m_pPlayerIdx[nCnt]->SetPos(GaugePos);
			m_pPlayerIdx[nCnt]->SetVtxBuffPos();
		}
		fWidth = m_pGauge2D[nCnt]->GetWidth() * 0.5f;
		GaugePos = m_pGauge2D[nCnt]->GetPos();
	}
}

//=============================================================================
//    ゲージを削る処理
//=============================================================================
void CScoreGauge::CutGauge(const int nIdx, const int nPoint)
{
	if (m_pGauge2D[nIdx] == NULL) { return; }
	float fWidth = m_pGauge2D[nIdx]->GetWidth();
	D3DXVECTOR3 pos = m_pGauge2D[nIdx]->GetPos();
	fWidth -= m_fGaugeOnce * nPoint;
	float fCutValue = (m_fGaugeOnce * nPoint) * 0.5f;
	if (fWidth <= 0.0f)
	{
		fWidth = 0.0f;
		fCutValue = 0.0f;
	}
	pos.x -= fCutValue;
	if (nIdx == 0 && fWidth <= 0.0f)
	{
		pos.x = SCOREGAUGE_BG_POS.x;
		pos.x -= SCOREGAUGE_BG_WIDTH * 0.5f;
	}
	m_pGauge2D[nIdx]->SetPos(pos);
	m_pPlayerIdx[nIdx]->SetPos(pos);
	m_pGauge2D[nIdx]->SetScale(fWidth, m_pGauge2D[nIdx]->GetHeight());
	m_pGauge2D[nIdx]->SetVtxBuffPos();
	if (m_pPlayerIdx[nIdx] != NULL)
	{
		m_pPlayerIdx[nIdx]->SetPos(pos + D3DXVECTOR3(0.0f, (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f), 0.0f));
		m_pPlayerIdx[nIdx]->SetVtxBuffPos();
	}

	// 自分以降のゲージの場所をずらす
	D3DXVECTOR3 GaugePos = pos;
	fWidth = fWidth * 0.5f;
	for (int nCnt = nIdx + 1; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] == NULL) { continue; }
		GaugePos.x += fWidth;
		GaugePos.x += m_pGauge2D[nCnt]->GetWidth() * 0.5f;
		m_pGauge2D[nCnt]->SetPos(GaugePos);
		m_pGauge2D[nCnt]->SetVtxBuffPos();
		if (m_pPlayerIdx[nCnt] != NULL)
		{
			GaugePos.y += (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f);
			m_pPlayerIdx[nCnt]->SetPos(GaugePos);
			m_pPlayerIdx[nCnt]->SetVtxBuffPos();
		}
		fWidth = m_pGauge2D[nCnt]->GetWidth() * 0.5f;
		GaugePos = m_pGauge2D[nCnt]->GetPos();
	}
}

//=============================================================================
//    ゲージ背景の生成処理
//=============================================================================
void CScoreGauge::CreateGaugeBg(void)
{
	D3DXVECTOR3 GaugePos = SCOREGAUGE_BG_POS;
	GaugePos.y = m_fGaugePosY;
	if (m_pGaugeBg != NULL) { return; }
	m_pGaugeBg = CIcon2D::Create(GaugePos, SCOREGAUGE_BG_COL,
		SCOREGAUGE_BG_WIDTH + SCOREGAUGE_BG_FRAMESIZE_X, m_fGaugeHeight + SCOREGAUGE_BG_FRAMESIZE_Y);
}

//=============================================================================
//    ゲージの生成処理
//=============================================================================
void CScoreGauge::CreateGauge2D(void)
{
	D3DXVECTOR3 GaugePos = SCOREGAUGE_BG_POS;
	GaugePos.x -= SCOREGAUGE_BG_WIDTH * 0.5f;
	GaugePos.y = m_fGaugePosY;
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] != NULL) { continue; }
		m_pGauge2D[nCnt] = CIcon2D::Create(GaugePos, CCharacter::m_CountryColor[nCnt],
			0.0f, m_fGaugeHeight);
	}
}

//=============================================================================
//    プレイヤー番号の生成処理
//=============================================================================
void CScoreGauge::CreatePlayerIdx(void)
{
	D3DXVECTOR3 PlayerIdxPos = INITIALIZE_VECTOR3;
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pPlayerIdx[nCnt] != NULL || m_pGauge2D[nCnt] == NULL) { continue; }
		PlayerIdxPos = m_pGauge2D[nCnt]->GetPos();
		PlayerIdxPos.y += (SCOREGAUGE_PLAYERIDX_HEIGHT * 0.5f) + (m_fGaugeHeight * 0.5f);
		m_pPlayerIdx[nCnt] = CIcon2D::Create(PlayerIdxPos, CCharacter::m_CountryColor[nCnt],
			SCOREGAUGE_PLAYERIDX_WIDTH, SCOREGAUGE_PLAYERIDX_HEIGHT);
	}
}

//=============================================================================
//    1ポイントごとのゲージ量を計算する処理
//=============================================================================
void CScoreGauge::CalcGaugeOnceFromRing(void)
{
	// 生成されているリングの数を求める
	int nNumRing = 0;
	CScene *pScene = CScene::GetTop(SCOREGAUGE_RING_PRIORITY);
	CScene *pSceneNext = NULL;
	while (pScene != NULL)
	{
		pSceneNext = pScene->GetpNext();
		if (pScene->GetObjType() == CScene::OBJTYPE_RING)
		{// リングだったら
			nNumRing++;
		}
		pScene = pSceneNext;
	}

	// 生成されているリングの数からゲージの幅を計算する
	if (nNumRing <= 0) { return; }
	m_fGaugeOnce = SCOREGAUGE_BG_WIDTH / nNumRing;
}

//=============================================================================
//    ゲージ背景の開放処理
//=============================================================================
void CScoreGauge::ReleaseGaugeBg(void)
{
	if (m_pGaugeBg != NULL)
	{
		m_pGaugeBg->Uninit();
		delete m_pGaugeBg;
		m_pGaugeBg = NULL;
	}
}

//=============================================================================
//    ゲージの開放処理
//=============================================================================
void CScoreGauge::ReleaseGauge2D(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] != NULL)
		{
			m_pGauge2D[nCnt]->Uninit();
			delete m_pGauge2D[nCnt];
			m_pGauge2D[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    プレイヤー番号の開放処理
//=============================================================================
void CScoreGauge::ReleasePlayerIdx(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pPlayerIdx[nCnt] != NULL)
		{
			m_pPlayerIdx[nCnt]->Uninit();
			delete m_pPlayerIdx[nCnt];
			m_pPlayerIdx[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    ゲージ背景の描画処理
//=============================================================================
void CScoreGauge::DrawGaugeBg(void)
{
	if (m_pGaugeBg != NULL)
	{
		m_pGaugeBg->Draw();
	}
}

//=============================================================================
//   ゲージの描画処理
//=============================================================================
void CScoreGauge::DrawGauge2D(void)
{
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER * 2; nCnt++)
	{
		if (m_pGauge2D[nCnt] != NULL)
		{
			m_pGauge2D[nCnt]->Draw();
		}
	}
}

//=============================================================================
//    プレイヤー番号の描画処理
//=============================================================================
void CScoreGauge::DrawPlayerIdx(void)
{
	for (int nCnt = (MAX_NUMPLAYER * 2) - 1; nCnt >= 0; nCnt--)
	{
		if (m_pPlayerIdx[nCnt] != NULL)
		{
			m_pPlayerIdx[nCnt]->Draw();
		}
	}
}