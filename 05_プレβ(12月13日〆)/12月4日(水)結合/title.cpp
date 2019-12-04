//=============================================================================
//
// タイトルの処理 [title.cpp]
// Author : 丹羽保貴
//
//=============================================================================
#include "title.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "ranking.h"
#include "loadText.h"
#include "library.h"
#include "camera.h"
#include "fieldmanager.h"
#include "audience.h"
#include "barun.h"
#include "sky.h"
#include "RawMouse.h"
#include "ringRender.h"
#include "territoryRender.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_TIME	              (60 * 8)
#define LOAD_UI		              ("data\\TEXT\\UI\\Title.txt")
#define LOAD_OBJECT		          ("data/TEXT/Stage.txt")
#define LOAD_TERRITORY	          ("data/TEXT/Territory.txt")
#define MOVE_XZ					  (0.2f);
#define MIDLLE					  (15.0f)

// 回転カメラ用
#define ROTATIONCAMERA_POSVHEIGHT (700.0f)
#define ROTATIONCAMERA_POSR       (D3DXVECTOR3(0.0f,400.0f,0.0f))
#define ROTATIONCAMERA_ROTSPEED   (0.003f)
#define ROTATIONCAMERA_LENGTH     (900.0f)

// タイトルの歯車用
//#define TITLE_GEAR_DRAWADDTIVE
#define TITLE_GEAR_MAX            (5)
#define TITLE_GEAR_TEXIDX         (3)
#define TITLE_GEAR_PRIORITY       (5)

#define TITLE_GEAR_POS0           (D3DXVECTOR3(300.0f, 360.0f, 0.0f))
#define TITLE_GEAR_POS1           (D3DXVECTOR3(365.0f, 140.0f, 0.0f))
#define TITLE_GEAR_POS2           (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 260.0f, 0.0f))
#define TITLE_GEAR_POS3           (D3DXVECTOR3(920.0f, 360.0f, 0.0f))
#define TITLE_GEAR_POS4           (D3DXVECTOR3(985.0f, 160.0f, 0.0f))

#define TITLE_GEAR_SIZE0          (D3DXVECTOR3(125.0f, 125.0f, 0.0f))
#define TITLE_GEAR_SIZE1          (D3DXVECTOR3(150.0f, 150.0f, 0.0f))
#define TITLE_GEAR_SIZE2          (D3DXVECTOR3(200.0f, 200.0f, 0.0f))
#define TITLE_GEAR_SIZE3          (D3DXVECTOR3(150.0f, 150.0f, 0.0f))
#define TITLE_GEAR_SIZE4          (D3DXVECTOR3(125.0f, 125.0f, 0.0f))

#define TITLE_GEAR_ROTSPEED0      (-D3DX_PI * 0.004f)
#define TITLE_GEAR_ROTSPEED1      (D3DX_PI * 0.002f)
#define TITLE_GEAR_ROTSPEED2      (D3DX_PI * 0.001f)
#define TITLE_GEAR_ROTSPEED3      (D3DX_PI * 0.002f)
#define TITLE_GEAR_ROTSPEED4      (-D3DX_PI * 0.004f)

#define TITLE_GEAR_TEXHEIGHT0     (-1.0f)
#define TITLE_GEAR_TEXHEIGHT1     (1.0f)
#define TITLE_GEAR_TEXHEIGHT2     (1.0f)
#define TITLE_GEAR_TEXHEIGHT3     (1.0f)
#define TITLE_GEAR_TEXHEIGHT4     (-1.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitle::CTitle()
{
	m_pLoadObject = NULL;
	m_pLoadTerritory = NULL;
	m_pRotationCamera = NULL;
	m_pFieldManager = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CTitle::Init(void)
{
	//ベースの初期化
	CModeBase::Init();

	//UI情報の読み込み処理
	LoadUI(LOAD_UI);

	// タイトル用の歯車を生成
	CreateTitleGear();

	//読み込み処理
	if (m_pLoadObject == NULL) { m_pLoadObject = CLoadTextObject::Create(LOAD_OBJECT); }
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }

	// 地面の生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	// カメラの生成
	CreateCamera();

	//初期化
	m_SoundState = SOUNDSTATE_NONE;

	//観客の生成
	CAudience::Create();

	//風船の生成
	CBarun::LoadTex();
	SetBarun();

	// 空の生成
	CSky::Load();
	CSky::Create();

	// リング描画クラスを生成
	CRingRender::Create();

	// テリトリー描画クラスを生成
	CTerritoryRender::Create();
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// カメラの破棄
	if (m_pRotationCamera != NULL)
	{
		m_pRotationCamera->Uninit();
		delete m_pRotationCamera;
		m_pRotationCamera = NULL;
	}

	// 地面の破棄
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	// オブジェクトの破棄
	if (m_pLoadObject != NULL)
	{
		m_pLoadObject->Uninit();
		delete m_pLoadObject;
		m_pLoadObject = NULL;
	}

	// テリトリーの破棄
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	//ベースの破棄
	CModeBase::Uninit();

	//風船テクスチャの破棄
	CBarun::UnloadTex();

	// 空テクスチャの破棄
	CSky::UnLoad();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	// カメラの更新
	if (m_pRotationCamera != NULL)
	{
		m_pRotationCamera->Update();
	}

	//サウンドの取得
	CSound *pSound = CManager::GetSound();

	//	タイトルのカウンター加算
	m_nCounter++;

	//インプットの取得　
	CGamePad	*pGamePad = CManager::GetInputGamePad();
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();
	CInputXPad * pXPad = CManager::GetXPad();
	CRawMouse *pRawMouse = CManager::GetRawMouse();					//RawMouseの取得

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (pKeyboard->GetKeyboardTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true ||
			pRawMouse->GetTrigger(nCnt, CRawMouse::RIMS_BUTTON_RIGHT) == true)
		{
			if (CFade::GetFadeMode() != CFade::FADE_OUT)
			{
				//決定音
				pSound->PlaySound(CSound::SOUND_LABEL_SE007);
				CFade::SetFade(CManager::MODE_SELECT);
			}
		}
	}
	if ((m_nCounter > FADE_TIME))
	{
		if (CFade::GetFadeMode() != CFade::FADE_OUT)
		{
			CFade::SetFade(CManager::MODE_TUTORIAL);
		}
	}

	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	// カメラの設定
	if (m_pRotationCamera != NULL)
	{
		m_pRotationCamera->Set();
	}

	// すべて描画
	CScene::DrawAll();
}

//=============================================================================
// タイトル用の歯車を生成する処理
//=============================================================================
void CTitle::CreateTitleGear(void)
{
	D3DXVECTOR3 pos[TITLE_GEAR_MAX] =
	{
		TITLE_GEAR_POS0,
		TITLE_GEAR_POS1,
		TITLE_GEAR_POS2,
		TITLE_GEAR_POS3,
		TITLE_GEAR_POS4,
	};
	D3DXVECTOR3 Size[TITLE_GEAR_MAX] =
	{
		TITLE_GEAR_SIZE0,
		TITLE_GEAR_SIZE1,
		TITLE_GEAR_SIZE2,
		TITLE_GEAR_SIZE3,
		TITLE_GEAR_SIZE4,
	};
	float fRotSpeed[TITLE_GEAR_MAX] =
	{
		TITLE_GEAR_ROTSPEED0,
		TITLE_GEAR_ROTSPEED1,
		TITLE_GEAR_ROTSPEED2,
		TITLE_GEAR_ROTSPEED3,
		TITLE_GEAR_ROTSPEED4,
	};
	float fTexHeight[TITLE_GEAR_MAX] =
	{
		TITLE_GEAR_TEXHEIGHT0,
		TITLE_GEAR_TEXHEIGHT1,
		TITLE_GEAR_TEXHEIGHT2,
		TITLE_GEAR_TEXHEIGHT3,
		TITLE_GEAR_TEXHEIGHT4,
	};

	for (int nCntGear = 0; nCntGear < TITLE_GEAR_MAX; nCntGear++)
	{
		CTitleGear2D *pGear2D = CTitleGear2D::Create(pos[nCntGear], Size[nCntGear], fRotSpeed[nCntGear], fTexHeight[nCntGear], TITLE_GEAR_PRIORITY);
		if (pGear2D == NULL) { continue; }
		pGear2D->BindTexture(GetTexture()[TITLE_GEAR_TEXIDX]);
	}
}

//=============================================================================
// 回転カメラを生成する処理
//=============================================================================
void CTitle::CreateCamera(void)
{
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 1.0f;
	viewport.MinZ = 0.0f;
	m_pRotationCamera = CRotationCamera::Create(ROTATIONCAMERA_POSVHEIGHT, ROTATIONCAMERA_POSR,
		ROTATIONCAMERA_ROTSPEED, ROTATIONCAMERA_LENGTH);
	m_pRotationCamera->SetViewport(viewport);
}

//=============================================================================
// 風船の生成処理
//=============================================================================
void CTitle::SetBarun(void)
{
	int nCntColor = 0;
	D3DXVECTOR3 move = INITIALIZE_VECTOR3;

	for (int nCnt = 0; nCnt < 120; nCnt++)
	{
		nCntColor = (nCntColor + 1) % 5;
		int nRand = rand() % 30 + 10;
		move.y = (float)nRand * 0.1f;
		move.z += MOVE_XZ;
		move.x += MOVE_XZ;
		if (move.z > 5.0f) { move.z = -3.0f; }
		if (move.x > 5.0f) { move.x = -3.0f; }

		nCntColor = (nCntColor + 1) % 5;
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), 300.0f, 1600.0f), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), 250.0f, 1500.0f), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), 200.0f, 1400.0f), (nCntColor + 1) % 5, move);

		nCntColor = (nCntColor + 1) % 5;
		move.z += MOVE_XZ;
		move.x += MOVE_XZ;
		if (move.z > 5.0f) { move.z = -6.0f; }
		if (move.x > 5.0f) { move.x = -6.0f; }

		nCntColor = (nCntColor + 1) % 5;
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), 50.0f, -1600.0f), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), 0.0f, -1500.0f), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), -50.0f, -1400.0f), (nCntColor + 1) % 5, move);

		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), -150.0f, -1600.0f), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), -250.0f, -1500.0f), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(-1200.0f + (nCnt * MIDLLE), -200.0f, -1400.0f), (nCntColor + 1) % 5, move);

		nRand = rand() % 30 + 10;
		move.y = (float)nRand * 0.1f;
		nCntColor = (nCntColor + 1) % 5;
		move.z += MOVE_XZ;
		move.x += MOVE_XZ;
		if (move.z > 5.0f) { move.z = -3.0f; }
		if (move.x > 5.0f) { move.x = -3.0f; }

		nCntColor = (nCntColor + 1) % 5;
		CBarun::Create(D3DXVECTOR3(-1400.0f, 50.0f, -1200.0f + (nCnt * MIDLLE)), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(-1500.0f, 150.0f, -1200.0f + (nCnt * MIDLLE)), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(-1600.0f, 200.0f, -1200.0f + (nCnt * MIDLLE)), (nCntColor + 1) % 5, move);

		nCntColor = (nCntColor + 1) % 5;
		CBarun::Create(D3DXVECTOR3(-1400.0f, -50.0f, -1200.0f + (nCnt * MIDLLE)), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(-1500.0f, -150.0f, -1200.0f + (nCnt * MIDLLE)), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(-1600.0f, -200.0f, -1200.0f + (nCnt * MIDLLE)), (nCntColor + 1) % 5, move);

		nCntColor = (nCntColor + 1) % 5;
		move.z += MOVE_XZ;
		move.x += MOVE_XZ;
		if (move.z > 5.0f) { move.z = -6.0f; }
		if (move.x > 5.0f) { move.x = -6.0f; }

		nCntColor = (nCntColor + 1) % 5;
		CBarun::Create(D3DXVECTOR3(1400.0f, 200.0f, 1200.0f - (nCnt * MIDLLE)), nCntColor, move);
		CBarun::Create(D3DXVECTOR3(1500.0f, 250.0f, 1200.0f - (nCnt * MIDLLE)), (nCntColor + 1) % 5, move);
		CBarun::Create(D3DXVECTOR3(1600.0f, 300.0f, 1200.0f - (nCnt * MIDLLE)), (nCntColor + 1) % 5, move);
	}
}



//*****************************************************************************
// タイトルロゴ用の歯車クラスの処理(CTitleGear2D)
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CTitleGear2D::CTitleGear2D(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_fRot = 0.0f;
	m_fRotSpeed = 0.0f;
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitleGear2D::~CTitleGear2D()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTitleGear2D *CTitleGear2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Size, float fRotSpeed, float fTexHeight, int nPriority, CScene2D::TYPE type)
{
	// メモリの確保
	CTitleGear2D *pTitleGear2D = NULL;
	pTitleGear2D = new CTitleGear2D(nPriority);
	if (pTitleGear2D == NULL) { return NULL; }

	// 各種値の設定
	pTitleGear2D->SetRotSpeed(fRotSpeed);

	// 初期化処理
	if (FAILED(pTitleGear2D->Init()))
	{
		pTitleGear2D->Uninit();
		pTitleGear2D = NULL;
		return NULL;
	}

	// 大きさの設定
	pTitleGear2D->Set(pos, Size);

	// テクスチャ座標の設定
	pTitleGear2D->SetTexUV(0.0f, 1.0f, 0.0f, fTexHeight);

	// ポリゴンの大きさを計算しておく
	pTitleGear2D->CalcPolygonScale();

	return pTitleGear2D;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitleGear2D::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitleGear2D::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitleGear2D::Update(void)
{
	// 向きを回転させる
	m_fRot += m_fRotSpeed;
	RotationPolygon();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitleGear2D::Draw(void)
{
#ifdef TITLE_GEAR_DRAWADDTIVE
	// レンダリングクラスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// 加算合成する設定に
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 共通の描画処理
	CScene2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
#else
	// 共通の描画処理
	CScene2D::Draw();
#endif
}

//=============================================================================
// ポリゴンの大きさを計算する処理
//=============================================================================
void CTitleGear2D::CalcPolygonScale(void)
{
	// 大きさを取得
	D3DXVECTOR3 Size = GetSize();

	// ポリゴンの対角線の角度求める
	m_fAngle = (float)atan2(Size.x, Size.y);

	// 対角線の長さを求める
	m_fLength = sqrtf((Size.x * Size.x) + (Size.y * Size.y));
}

//=============================================================================
// ポリゴンを回転させる処理
//=============================================================================
void CTitleGear2D::RotationPolygon(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == NULL) { return; }
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の回転を考慮して座標を計算
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + GetPos();
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 現在の向きを設定する処理
//=============================================================================
void CTitleGear2D::SetRot(const float fRot)
{
	m_fRot = fRot;
}

//=============================================================================
// 回転スピードを設定する処理
//=============================================================================
void CTitleGear2D::SetRotSpeed(const float fRotSpeed)
{
	m_fRotSpeed = fRotSpeed;
}

//=============================================================================
// ポリゴンの角度を設定する処理
//=============================================================================
void CTitleGear2D::SetAngle(const float fAngle)
{
	m_fAngle = fAngle;
}

//=============================================================================
// ポリゴンの長さを設定する処理
//=============================================================================
void CTitleGear2D::SetLength(const float fLength)
{
	m_fLength = fLength;
}

//=============================================================================
// 現在の向きを取得する処理
//=============================================================================
float CTitleGear2D::GetRot(void)
{
	return m_fRot;
}

//=============================================================================
// 回転スピードを取得する処理
//=============================================================================
float CTitleGear2D::GetRotSpeed(void)
{
	return m_fRotSpeed;
}

//=============================================================================
// ポリゴンの角度を取得する処理
//=============================================================================
float CTitleGear2D::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
// ポリゴンの長さを取得する処理
//=============================================================================
float CTitleGear2D::GetLength(void)
{
	return m_fLength;
}