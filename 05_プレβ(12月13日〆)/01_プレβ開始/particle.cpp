//=============================================================================
//
// パーティクルビルボード処理 [particlebillboad.cpp]
// Author : 佐藤 安純
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "loadText.h"
#include "tutorial.h"
#include "sceneBillBorad.h"
#include "camera.h"

//==================================
// マクロ定義
//==================================
#define MAX_LIFE_GRAND		(30)
#define CHANGE_SIZE			(0.1f)
#define VECTOR_ZERO			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//ベクトルの初期化
#define FLAM				(8)
#define CHANGE_A			(0.06f)
#define CHANGE_A_STAR		(0.14f)
#define RAND_ANGLE			(314)
#define CICLE_MIN			(-200)
#define CICLE_MAX			(400)
#define LIGHT_MIN			(18.0f)
#define LIGHT_MAX			(32.0f)
#define SIZE_UP				(0.15f)
#define SPEW_MIN			(-100)
#define SPEW_MAX			(200)

//==================================
// 静的メンバ変数宣言
//==================================
LPDIRECT3DTEXTURE9 * CParticle::m_ppTexture = NULL;
int CParticle::m_nMaxTexture = 0;

//=============================================================================
// 生成処理
//=============================================================================
CParticle * CParticle::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nNumTex, DRAWTYPE DrawType, TYPE type, D3DXCOLOR col, int nLife, float fSpeed, int nMinHight, int nMaxHight, bool bZBuffu, int nPriority)
{
	//インスタンスの生成
	CParticle * pParticle = NULL;

	if (type == TYPE_NORMAL)
	{//通常
		pParticle = new CParticle(nPriority);
	}
	else if (type == TYPE_SPEW)
	{//噴水型
		pParticle = new CSpewParticle(nPriority);
	}
	else if (type == TYPE_GRANDSMOKE)
	{//その場で拡大
		pParticle = new CGrandSmokeParticle(nPriority);
	}
	else if (type == TYPE_LIGHT)
	{//拡大縮小
		pParticle = new CLightParticle(nPriority);
	}
	else if (type == TYPE_CICLE)
	{//円形型
		pParticle = new CCicleParticle(nPriority);
	}
	else if (type == TYPE_STAR)
	{//爆発型
		pParticle = new CStarParticle(nPriority);
	}

	if (pParticle != NULL)
	{
		pParticle->SetSpeed(fSpeed);						//スピードの設定
		pParticle->SetMinHight(nMinHight);					//高さの最小値
		pParticle->SetMaxHight(nMaxHight);					//高さの最大値

		pParticle->Init();					//初期化処理

		//設定処理
		pParticle->SetSize(D3DXVECTOR2(size.x, size.y));	//サイズの設定
		pParticle->SetPos(pos);				//位置の設定
		pParticle->SetDrawType(DrawType);	//描画タイプの設定
		pParticle->SetColor(col);			//色の設定
		pParticle->SetColorA(col.a);		//色のa値の設定
		pParticle->SetZbuffu(bZBuffu);		//Zバッファの設定
		pParticle->SetLife(nLife);			//ライフの設定

		if (m_ppTexture[nNumTex] != NULL)
		{
			pParticle->BindTexture(m_ppTexture[nNumTex]);		//テクスチャの割当て
		}
	}
	else
	{
		MessageBox(NULL, TEXT("pParticleがNULLです"), TEXT("CEffectTool::Create()"), MB_OK);
	}

	return pParticle;
}

//=========================================
// デフォルトコンストラクタ
//=========================================
CParticle::CParticle() : CSceneBillborad(3, CScene::OBJTYPE_PARTICLE)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//=========================================
// コンストラクタ
//=========================================
CParticle::CParticle(int nPriority) : CSceneBillborad(nPriority, CScene::OBJTYPE_PARTICLE)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR2(0.0f, 0.0f);
}

//=========================================
// デストラクタ
//=========================================
CParticle::~CParticle(){}

//=============================================================================
// テクスチャの読み込み
//=============================================================================
void CParticle::LoadTexture(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//テクスチャ情報の取得
	CLoadTextEffect * pLoadTextTexture = {};
	pLoadTextTexture = CGame::GetEffectLoad();	//パーティクルテクスチャのポインタを取得
	CLoadText::TEX_INFO TexInfo = pLoadTextTexture->GetTexInfo();	//テクスチャ情報の取得
	m_nMaxTexture = TexInfo.nMaxTex;								//テクスチャの最大数を取得
	

	//テクスチャをサイズ分ヒープ領域からメモリを動的確保する
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTexture];
	}

	// テクスチャの生成
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TexInfo.ppFileName[nCntTex],
			&m_ppTexture[nCntTex]);
	}
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CParticle::UnloadTexture(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < m_nMaxTexture; nCntTex++)
	{
		if (m_ppTexture[nCntTex] != NULL)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}
	}

	//テクスチャポインタの破棄
	if (m_ppTexture != NULL)
	{
		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}
}

//=========================================
// 初期化処理
//=========================================
HRESULT CParticle::Init(void)
{
	CSceneBillborad::Init();

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDeth = false;
	return S_OK;
}

//=========================================
// 終了処理
//=========================================
void CParticle::Uninit(void)
{
	CSceneBillborad::Uninit();
}

//=========================================
// 更新処理
//=========================================
void CParticle::Update(void)
{
	//変数宣言
	CGame * pGame = CManager::GetGame();
	D3DXVECTOR3 Pos = CSceneBillborad::GetPos();
	bool bDraw[MAX_NUMPLAYER] = {};
	bool bDrawChack = true;

	//カメラごとの描画判定
	if (pGame == NULL) { return; }
	for (int nCnt = 0; nCnt < MAX_NUMPLAYER; nCnt++)
	{
		//NULLチェック
		CGameCamera * pGameCamera = pGame->GetGameCamera(nCnt);
		if (pGameCamera == NULL) { continue; }

		bDraw[nCnt] = pGame->GetGameCamera(nCnt)->ReflectObject(Pos);

		if (bDraw[nCnt])
		{
			bDrawChack = false;	//描画フラグ
			SetDraw(true);
			break;
		}
	}

	if (!bDrawChack) { return; }
	SetDraw(false);		//描画しない状態にする
}

//=========================================
// 描画処理
//=========================================
void CParticle::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ライティングをしない設定に
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (m_DrawType == DRAWTYPE_ADD)
	{
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	if (m_bZBuffu)
	{
		//デフォルトの場合FALSEになっているからTRUEにする
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}

	//ビルボードの描画
	CSceneBillborad::Draw();

	// ライティングの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

	if (m_DrawType == DRAWTYPE_ADD)
	{
		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	if (m_bZBuffu)
	{
		//デフォルトの場合FALSEになっているからTRUEにする
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
}

//=========================================
// 色の設定
//=========================================
void CParticle::SetColor(D3DXCOLOR col)
{
	m_col = col;

	CSceneBillborad::SetColor(m_col);
}

//=========================================
// サイズの再設定
//=========================================
void CParticle::RemakeSize(D3DXVECTOR2 size)
{
	m_size = size;

	CSceneBillborad::SetSize(D3DXVECTOR3(m_size.x, 0.0f, m_size.y));
}

//=============================================================================
// 位置設定
//=============================================================================
void CParticle::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	CSceneBillborad::SetPos(pos);
}

//=============================================================================
// サイズ設定
//=============================================================================
void CParticle::SetSize(D3DXVECTOR2 size)
{
	m_size = size;

	CSceneBillborad::SetSize(D3DXVECTOR3(size.x, size.y, 0.0f));
}

//=============================================================================
// 回転の修正処理
//=============================================================================
void CParticle::SetAngle(float * pAngle)
{
	//角度の修正
	if (*pAngle < -D3DX_PI)
	{
		*pAngle += D3DX_PI * 2.0f;
	}
	if (*pAngle > D3DX_PI)
	{
		*pAngle -= D3DX_PI * 2.0f;
	}
}

//=========================================
// UV座標の設定
//=========================================
void CParticle::SetUV(D3DXVECTOR2 MinUV, D3DXVECTOR2 MaxUV)
{
	CSceneBillborad::SetUV(MinUV, MaxUV);
}

//*****************************************************************************
//
// 星型パーティクルクラス
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CSpewParticle::CSpewParticle(int nPriority) : CParticle(nPriority) {}

//=============================================================================
// デストラクタ
//=============================================================================
CSpewParticle::~CSpewParticle() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSpewParticle::Init(void)
{
	CParticle::Init();

	//変数の初期化
	m_nCountFlam = 0;		//フレームカウンター
	m_nLife = 30;			//表示時間
	m_fChangeCol_a = 0.0f;	//透明度の変化量;
	m_bGravity = true;		//重力のON・OFF
	m_fGravity = 0.0f;		//重力

	float fPosX = (float)(SPEW_MIN + rand() % SPEW_MAX);
	float fPosZ = (float)(SPEW_MIN + rand() % SPEW_MAX);

	//目的の角度を求める
	float fAngle = atan2f(fPosX, fPosZ);

	//移動量を求める
	m_move.x = sinf(fAngle) * m_fSpeed;
	m_move.z = cosf(fAngle) * m_fSpeed;

	//高さ設定
	float fHight = (float)(m_nMinHight + rand() % m_nMaxHight);
	m_move.y = fHight;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSpewParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSpewParticle::Update(void)
{
	//重力を足す
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}

	//時間の加算
	m_nCountFlam++;

	if (m_nCountFlam > FLAM)	//一定のフレーム数が経ったら
	{
		m_nLife--;						//表示時間の減算

		if (m_nLife < 0)
		{
			m_colA -= m_fChangeCol_a;	//透明度を下げる

			if (m_colA < 0.0f)
			{
				m_bDeth = true;
			}
			else
			{
				D3DXCOLOR col = CParticle::GetColor();
				CParticle::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
			}
		}
	}

	D3DXVECTOR3 pos = CParticle::GetPos();
	pos += m_move;
	CParticle::SetPos(pos);

	CParticle::Update();

	//死亡状態だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CSpewParticle::Draw(void)
{
	CParticle::Draw();
}

//*****************************************************************************
//
// 土煙パーティクルクラス
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CGrandSmokeParticle::CGrandSmokeParticle(int nPriority) : CParticle(nPriority) {}

//=============================================================================
// デストラクタ
//=============================================================================
CGrandSmokeParticle::~CGrandSmokeParticle() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGrandSmokeParticle::Init(void)
{
	CParticle::Init();

	m_nCountFlam = 0;		//フレームカウンター
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGrandSmokeParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGrandSmokeParticle::Update(void)
{
	//時間の加算
	m_nCountFlam++;

	m_nLife--;				//表示時間の減算

	if (m_nLife < 0)
	{
		m_colA -= CHANGE_A;	//透明度を下げる

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticle::GetColor();
			CParticle::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	//サイズの更新
	D3DXVECTOR2 size = CParticle::GetSize();
	size.x += SIZE_UP;
	size.y += SIZE_UP;

	CParticle::SetSize(size);
	CParticle::Update();

	//死亡状態だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CGrandSmokeParticle::Draw(void)
{
	CParticle::Draw();
}

//*****************************************************************************
//
// ライトパーティクル
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CLightParticle::CLightParticle(int nPriority) : CParticle(nPriority) {}

//=============================================================================
// デストラクタ
//=============================================================================
CLightParticle::~CLightParticle() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLightParticle::Init(void)
{
	CParticle::Init();
	m_fChangeSize = CHANGE_SIZE;
	m_nLife = MAX_LIFE_GRAND;	//表示時間
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLightParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CLightParticle::Update(void)
{
	//サイズの保存
	D3DXVECTOR2 size = CParticle::GetSize();

	//パーティクルのサイズを変更する
	if (size.x > LIGHT_MAX || size.x < LIGHT_MIN)
	{
		m_fChangeSize *= -1;	//符号の変更
	}

	//サイズの変更
	size.x += m_fChangeSize;
	size.y += m_fChangeSize;

	//サイズの設定
	SetSize(size);

	CParticle::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CLightParticle::Draw(void)
{
	CParticle::Draw();
}

//*****************************************************************************
//
// 円形パーティクルクラス
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CCicleParticle::CCicleParticle(int nPriority) : CParticle(nPriority) {}

//=============================================================================
// デストラクタ
//=============================================================================
CCicleParticle::~CCicleParticle() {}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCicleParticle::Init(void)
{
	CParticle::Init();

	m_nCountFlam = 0;						//フレームカウンター
	m_nLife = 30;							//表示時間
	m_fChangeCol_a = 0.0f;					//透明度の変化量;
	m_bGravity = true;						//重力のON・OFF
	m_fGravity = 0.0f;						//重力

	float fPosX = (float)(CICLE_MIN + rand() % CICLE_MAX);
	float fPosY = (float)(CICLE_MIN + rand() % CICLE_MAX);

	//目的の角度を求める
	float fAngle = atan2f(fPosX, fPosY);

	//移動量を求める
	m_move.x = sinf(fAngle) * m_fSpeed;
	m_move.z = cosf(fAngle) * m_fSpeed;
	m_move.y = cosf(fAngle) * m_fSpeed;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCicleParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCicleParticle::Update(void)
{
	//重力を足す
	if (m_bGravity == true)
	{
		m_move.y -= m_fGravity;
	}

	m_nLife--;						//表示時間の減算

	if (m_nLife < 0)
	{
		m_colA -= m_fChangeCol_a;	//透明度を下げる

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticle::GetColor();
			CParticle::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}
	D3DXVECTOR3 pos = CParticle::GetPos();
	pos += m_move;
	CParticle::SetPos(pos);

	CParticle::Update();

	//死亡状態だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CCicleParticle::Draw(void)
{
	CParticle::Draw();
}


//*****************************************************************************
//
// 星パーティクルクラス
//
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CStarParticle::CStarParticle(int nPriority) : CParticle(nPriority)
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CStarParticle::~CStarParticle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CStarParticle::Init(void)
{
	CParticle::Init();

	m_nCountFlam = 0;			//フレームカウンター
								//向き設定
	//m_rot.x = (float)(rand() % RAND_ANGLE);
	//m_rot.z = (float)(rand() % RAND_ANGLE);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CStarParticle::Uninit(void)
{
	CParticle::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CStarParticle::Update(void)
{
	//時間の加算
	m_nCountFlam++;

	m_nLife--;				//表示時間の減算

	if (m_nLife < 0)
	{
		m_colA -= CHANGE_A_STAR;	//透明度を下げる

		if (m_colA < 0.0f)
		{
			m_bDeth = true;
		}
		else
		{
			D3DXCOLOR col = CParticle::GetColor();
			CParticle::SetColor(D3DXCOLOR(col.r, col.g, col.b, m_colA));
		}
	}

	CParticle::Update();

	//死亡状態だったら破棄
	if (m_bDeth) { Uninit(); }
}

//=============================================================================
// 描画処理
//=============================================================================
void CStarParticle::Draw(void)
{
	CParticle::Draw();
}