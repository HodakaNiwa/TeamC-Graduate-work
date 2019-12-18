//*****************************************************************************
//
//     光の軌跡の処理[lightorbit.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "lightorbit.h"
#include "manager.h"
#include "library.h"
#include "effect3D.h"
#include "character.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define LIGHTORBIT_MAX_ACCEL        (13.0f)		// 位置の加速値の最大値
#define LIGHTORBIT_MAX_ACCELTIME    (120)		// 最大の速さになるまでの時間
#define LIGHTORBIT_DEATH_LENGTH     (10.0f)		// 消す範囲
#define LIGHTORBIT_MAX_HEIGHT       (50.0f)		// 高さ

#define LIGHTORBIT_EFFECT_SPAWN     (1)			// エフェクトを出す間隔
#define LIGHTORBIT_EFFECT_WIDTH     (23.0f)		// エフェクトの幅
#define LIGHTORBIT_EFFECT_HEIGHT    (23.0f)		// エフェクトの高さ
#define LIGHTORBIT_EFFECT_LIFE      (90)		// エフェクトの寿命
#define LIGHTORBIT_EFFECT_CUTALPHA  (0.08f)		// エフェクトの透明度を削る量
#define LIGHTORBIT_EFFECT_CUTLENGTH (0.7f)		// エフェクトの大きさを削る量
#define LIGHTORBIT_EFFECT_PRIORITY  (7)			// エフェクトの処理優先順位

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CLightOrbit::CLightOrbit(int nPriority, OBJTYPE objType) : CScene(nPriority, objType){}

//=============================================================================
//    デストラクタ
//=============================================================================
CLightOrbit::~CLightOrbit(){}

//=============================================================================
//    生成処理
//=============================================================================
CLightOrbit *CLightOrbit::Create(D3DXVECTOR3 pos, D3DXVECTOR3 *pDestPos, int nDestPlayerIdx, int nPriority)
{
	// メモリの確保
	CLightOrbit *pLightOrbit = NULL;
	pLightOrbit = new CLightOrbit(nPriority);
	if (pLightOrbit == NULL) { return NULL; }

	// 変数の初期化
	pLightOrbit->ClearVariable();

	// 各種値の代入
	pLightOrbit->SetPos(pos);						// ポリゴンの座標
	pLightOrbit->SetDestPos(pDestPos);				// 目的の座標
	pLightOrbit->SetDestPlayerIdx(nDestPlayerIdx);	// 目的のプレイヤー番号

	// 初期化処理
	if (FAILED(pLightOrbit->Init()))
	{
		pLightOrbit->Uninit();
		return NULL;
	}

	return pLightOrbit;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLightOrbit::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLightOrbit::Uninit(void)
{
	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CLightOrbit::Update(void)
{
	// カウンター加算
	m_nCounter++;

	// 移動量計算処理
	bool bDeath = false;
	bDeath = CalcMove();

	// 位置を移動させる
	Movement();

	// エフェクト(3D)を生成する
	CreateEffect3D();

	// 死亡判定
	if (bDeath == true)
	{
		Uninit();
	}
}

//=============================================================================
//    移動量を計算する処理
//=============================================================================
bool CLightOrbit::CalcMove(void)
{
	bool bDeath = false;

	// 角度を出す
	float fAngle = CFunctionLib::CalcAngleToDest(m_Pos.x, m_Pos.z, m_pDestPos->x, m_pDestPos->z);

	// 目的地に移動させる
	m_Move.x = sinf(fAngle) * m_fAccel;
	m_Move.z = cosf(fAngle) * m_fAccel;

	// 距離を計算
	float fLength = CFunctionLib::Vec2Length(m_Pos.x, m_Pos.z, m_pDestPos->x, m_pDestPos->z);
	if (fLength <= LIGHTORBIT_DEATH_LENGTH) { bDeath = true; }

	return bDeath;
}

//=============================================================================
//    位置を移動させる処理
//=============================================================================
void CLightOrbit::Movement(void)
{
	// 位置を移動させる
	m_Pos += m_Move;
	m_Pos.y = LIGHTORBIT_MAX_HEIGHT;

	// 加速値の計算
	float fRivision = (float)m_nCounter / (float)LIGHTORBIT_MAX_ACCELTIME;
	if (fRivision >= 1.0f)
	{
		fRivision = 1.0f;
	}
	m_fAccel = sinf((D3DX_PI * 0.5f) * fRivision) * LIGHTORBIT_MAX_ACCEL;
}

//=============================================================================
//    エフェクト(3D)を生成する処理
//=============================================================================
void CLightOrbit::CreateEffect3D(void)
{
	if (m_nCounter % LIGHTORBIT_EFFECT_SPAWN != 0) { return; }
	D3DXCOLOR col = CCharacter::m_CountryColor[m_nDestPlayerIdx];
	CEffect3D::Create(m_Pos, col, LIGHTORBIT_EFFECT_WIDTH, LIGHTORBIT_EFFECT_HEIGHT,
		LIGHTORBIT_EFFECT_LIFE, LIGHTORBIT_EFFECT_CUTALPHA, LIGHTORBIT_EFFECT_CUTLENGTH,
		false, LIGHTORBIT_EFFECT_PRIORITY);
	switch (m_nDestPlayerIdx)
	{
	case 1:
		CEffect3D::Create(m_Pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), LIGHTORBIT_EFFECT_WIDTH * 0.6f, LIGHTORBIT_EFFECT_HEIGHT  * 0.6f,
			LIGHTORBIT_EFFECT_LIFE, LIGHTORBIT_EFFECT_CUTALPHA, LIGHTORBIT_EFFECT_CUTLENGTH,
			false, LIGHTORBIT_EFFECT_PRIORITY);
		break;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CLightOrbit::Draw(void)
{

}

//=============================================================================
//    変数の初期化処理
//=============================================================================
void CLightOrbit::ClearVariable(void)
{
	m_Pos = INITIALIZE_VECTOR3;
	m_Move = INITIALIZE_VECTOR3;
	m_pDestPos = NULL;
	m_nCounter = 0;
	m_nDestPlayerIdx = 0;
	m_fAccel = 0.0f;
}