//*****************************************************************************
//
//     スコア変動の処理[scoreChange.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "scoreChange.h"
#include "number2D.h"
#include "manager.h"
#include "game.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SCORECHANGE_PRIORITY	(3)		// スコア変動の処理優先順位
#define SCORECHANGE_INTERVAL	(1.4f)	// スコア変動の数字の間隔
#define SCORECHANGE_ALPHA_CUT	(0.03f)	// スコア変動の透明度をどれくらい削るか
#define SCORECHANGE_POS_DOWN	(0.70f)	// スコア変動をどれくらい下に下げるか
#define SCORECHANGE_ALPHA_TIME	(70)	// スコア変動の透明度をどれくらいから削り始めるか
#define SCORECHANGE_SIZE_CUT	(0.7f)	// スコア変動のサイズをどれくらい小さくするか(人数によって変動)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CScoreChange::CScoreChange(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pNumber2D = NULL;
	m_nNumberDigit = 0;
	m_nScoreChange = 0;
	m_nChangeTimer = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScoreChange::~CScoreChange()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScoreChange *CScoreChange::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 size, int nScoreChange)
{
	// メモリを確保
	CScoreChange *pScoreChange = NULL;
	pScoreChange = new CScoreChange(SCORECHANGE_PRIORITY);
	if (pScoreChange == NULL) { return NULL; }

	// 値の設定
	pScoreChange->SetScoreChange(nScoreChange);

	// 初期化処理
	if (FAILED(pScoreChange->Init()))
	{
		pScoreChange->Uninit();
		return NULL;
	}

	// サイズの調整
	size = pScoreChange->SizeAdjust(size);

	// 位置の設定
	pos.x += (size.x * 0.75f) * pScoreChange->GetNumberDigit();

	// ポリゴンの設定
	pScoreChange->Set(pos, size);
	pScoreChange->SetColoer(col);
	pScoreChange->CreateNumber();

	return pScoreChange;	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScoreChange::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	// スコア変動の桁数を計算
	CalcScoreChangeDigit();

	return S_OK;
}

//=============================================================================
//    数字の桁数を計算する処理
//=============================================================================
void CScoreChange::CalcScoreChangeDigit(void)
{
	m_nNumberDigit = (int)log10f((float)m_nScoreChange) + 1;
	if (m_nNumberDigit <= 0) { m_nNumberDigit = 1; }
}

//=============================================================================
//    数字ポリゴンを生成する処理
//=============================================================================
void CScoreChange::CreateNumber(void)
{
	if (m_pNumber2D != NULL) { return; }

	// 桁数分メモリを確保
	m_pNumber2D = new CNumber2D*[m_nNumberDigit];
	if (m_pNumber2D == NULL) { return; }

	// 数字ポリゴンを桁数分生成
	D3DXVECTOR3 pos = GetPos();
	pos.x += (GetSize().x * SCORECHANGE_INTERVAL) * (m_nNumberDigit + 1);
	int nNumber = 0;
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		// ポインタを初期化
		m_pNumber2D[nCntDigit] = NULL;

		// 表示する数字の計算
		nNumber = m_nScoreChange % ((int)powf(10.0f, (float)nCntDigit) * 10) / (int)powf(10.0f, (float)nCntDigit);

		// 数字の場所をずらしておく
		pos.x -= GetSize().x * SCORECHANGE_INTERVAL;

		// 生成
		m_pNumber2D[nCntDigit] = CNumber2D::Create(pos, GetSize(), GetColor());
		if (m_pNumber2D[nCntDigit] == NULL) { continue; }
		m_pNumber2D[nCntDigit]->SetbUse(true);
		m_pNumber2D[nCntDigit]->SetNumber(nNumber);
	}
}

//=============================================================================
//    終了処理
//=============================================================================
void CScoreChange::Uninit(void)
{
	// 数字ポリゴンクラスの開放
	ReleaseNumber();

	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    数字ポリゴンを開放する処理
//=============================================================================
void CScoreChange::ReleaseNumber(void)
{
	if (m_pNumber2D == NULL) { return; }
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumber2D[nCntDigit] != NULL)
		{
			m_pNumber2D[nCntDigit]->Uninit();
			delete m_pNumber2D[nCntDigit];
			m_pNumber2D[nCntDigit] = NULL;
		}
	}
	delete[] m_pNumber2D;
	m_pNumber2D = NULL;
}

//=============================================================================
//    更新処理
//=============================================================================
void CScoreChange::Update(void)
{
	// 位置を下げる
	PosDown();

	// 透明度を上げる
	AlphaAdd();
}

//=============================================================================
//    位置を下げる処理
//=============================================================================
void CScoreChange::PosDown(void)
{
	// 位置の取得
	D3DXVECTOR3 pos = GetPos();

	// 下に下げる
	pos.y += SCORECHANGE_POS_DOWN;

	// 位置の設定
	SetPos(pos, GetOtherPos());
	PosDown_Number();
}
void CScoreChange::PosDown_Number(void)
{
	D3DXVECTOR3 pos;
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumber2D[nCntDigit] != NULL)
		{
			pos = m_pNumber2D[nCntDigit]->GetPos();
			pos.y += SCORECHANGE_POS_DOWN;
			m_pNumber2D[nCntDigit]->SetPos(pos);
		}
	}
}

//=============================================================================
//    透明度を上げる処理処理
//=============================================================================
void CScoreChange::AlphaAdd(void)
{
	m_nChangeTimer++;
	if (m_nChangeTimer <= SCORECHANGE_ALPHA_TIME) { return; }

	// 色の取得
	D3DXCOLOR col = GetColor();

	// 透明度を増加
	col.a -= SCORECHANGE_ALPHA_CUT;

	// 色の設定
	SetColoer(col);
	AlphaAdd_Number(col);

	// 死亡判定
	if (col.a <= 0.0f)
	{
		Uninit();
	}
}
void CScoreChange::AlphaAdd_Number(const D3DXCOLOR col)
{
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumber2D[nCntDigit] != NULL)
		{
			m_pNumber2D[nCntDigit]->SetColor(col);
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CScoreChange::Draw(void)
{
	// 数字の描画
	DrawNumber();

	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    数字を描画する処理
//=============================================================================
void CScoreChange::DrawNumber(void)
{
	if (m_pNumber2D == NULL) { return; }

	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumber2D[nCntDigit] != NULL)
		{
			m_pNumber2D[nCntDigit]->Draw();
		}
	}
}

//=============================================================================
//    サイズを調整する処理
//=============================================================================
D3DXVECTOR3 CScoreChange::SizeAdjust(D3DXVECTOR3 size)
{
	// ゲームクラスを取得
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return size; }

	// 2人以上参加していたらサイズを変更する
	if (pGame->GetNumPlay() >= 2)
	{
		size.x *= SCORECHANGE_SIZE_CUT;
		size.z *= SCORECHANGE_SIZE_CUT;
	}

	return size;
}