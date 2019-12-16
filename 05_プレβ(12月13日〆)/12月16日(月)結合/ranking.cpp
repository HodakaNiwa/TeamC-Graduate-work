//=============================================================================
//
// ランキングの処理 [ranking.cpp]
// Author : Shoji Takahiro
//=============================================================================
#include "ranking.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "loadText.h"
#include "number2D.h"
#include "camera.h"
#include "fieldmanager.h"
#include "score.h"
#include "RawMouse.h"
#include "sky.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FAREM_COUNT	(60 * 10)	// フェードのフレーム数
#define LOAD_UI		("data\\TEXT\\UI\\Ranking.txt")
#define LOAD_OBJ			("data/TEXT/Stage.txt")
#define LOAD_TERRITORY		("data/TEXT/Territory.txt")

// タイトルの歯車用
#define RANKING_BG_TEXIDX     (3)
#define RANKING_BG_PRIORITY   (5)

#define RANKING_BG_POS0       (D3DXVECTOR3(640.0f, 100.0f, 0.0f))
#define RANKING_BG_POS1       (D3DXVECTOR3(640.0f, 180.0f, 0.0f))
#define RANKING_BG_POS2       (D3DXVECTOR3(640.0f, 240.0f, 0.0f))
#define RANKING_BG_POS3       (D3DXVECTOR3(640.0f, 300.0f, 0.0f))
#define RANKING_BG_POS4       (D3DXVECTOR3(640.0f, 360.0f, 0.0f))
#define RANKING_BG_POS5       (D3DXVECTOR3(640.0f, 420.0f, 0.0f))
#define RANKING_BG_POS6       (D3DXVECTOR3(640.0f, 480.0f, 0.0f))
#define RANKING_BG_POS7       (D3DXVECTOR3(640.0f, 540.0f, 0.0f))
#define RANKING_BG_POS8       (D3DXVECTOR3(640.0f, 600.0f, 0.0f))

#define RANKING_BG_SIZE0      (D3DXVECTOR3(560.0f, 50.0f, 0.0f))
#define RANKING_BG_SIZE1      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE2      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE3      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE4      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE5      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE6      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE7      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))
#define RANKING_BG_SIZE8      (D3DXVECTOR3(560.0f, 30.0f, 0.0f))

#define RANKING_BG_ROTSPEED0  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED1  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED2  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED3  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED4  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED5  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED6  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED7  (D3DX_PI * 0.000f)
#define RANKING_BG_ROTSPEED8  (D3DX_PI * 0.000f)

#define RANKING_BG_TEXHEIGHT0 (1.0f)
#define RANKING_BG_TEXHEIGHT1 (1.0f)
#define RANKING_BG_TEXHEIGHT2 (1.0f)
#define RANKING_BG_TEXHEIGHT3 (1.0f)
#define RANKING_BG_TEXHEIGHT4 (1.0f)
#define RANKING_BG_TEXHEIGHT5 (1.0f)
#define RANKING_BG_TEXHEIGHT6 (1.0f)
#define RANKING_BG_TEXHEIGHT7 (1.0f)
#define RANKING_BG_TEXHEIGHT8 (1.0f)

#define RANKING_BG_MOVE0 (0.0f)
#define RANKING_BG_MOVE1 (60.0f)
#define RANKING_BG_MOVE2 (60.0f)
#define RANKING_BG_MOVE3 (60.0f)
#define RANKING_BG_MOVE4 (60.0f)
#define RANKING_BG_MOVE5 (60.0f)
#define RANKING_BG_MOVE6 (60.0f)
#define RANKING_BG_MOVE7 (60.0f)
#define RANKING_BG_MOVE8 (60.0f)

#define RANKING_ADDPOS (1200.0f)

#define RANKING_TITLE		("data/TEXTURE/Ranking/RANKING_BG_000.png")
#define RANKING_001			("data/TEXTURE/Ranking/RANKING_BG_001.png")
#define RANKING_002			("data/TEXTURE/Ranking/RANKING_BG_002.png")
#define RANKING_003			("data/TEXTURE/Ranking/RANKING_BG_003.png")
#define RANKING_004			("data/TEXTURE/Ranking/RANKING_BG_004.png")
#define RANKING_005			("data/TEXTURE/Ranking/RANKING_BG_005.png")
#define RANKING_006			("data/TEXTURE/Ranking/RANKING_BG_006.png")
#define RANKING_007			("data/TEXTURE/Ranking/RANKING_BG_007.png")
#define RANKING_008			("data/TEXTURE/Ranking/RANKING_BG_008.png")

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************


//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CRanking::CRanking()
{
	for (int nCntTex = 0; nCntTex < 9; nCntTex++)
	{
		m_apTextureRanking[nCntTex] = NULL;
		m_apRankingBG[nCntTex] = NULL;
	}

}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking(){}

//=============================================================================
// 初期化処理
//=============================================================================
void CRanking::Init(void)
{
	//ベースの初期化
	CModeBase::Init();

	// 変数を初期化
	m_pCamera = NULL;
	m_pFieldManager = NULL;
	m_pLoadTextObj = NULL;
	m_pLoadTerritory = NULL;
	m_nNumRanking = 0;
	m_nCounterRanking = 0;

	//UI情報の読み込み
	LoadUI(LOAD_UI);

	//テクスチャの読み込み
	CNumber2D::Load();
	CScoreRanking::LoadTex();
	LoadTexture();

	// ランキングBGを生成
	CreateRankingBG(0);

	//カメラの生成
	if (m_pCamera == NULL)
	{
		D3DVIEWPORT9 viewport;
		viewport.X = 0;		//ビューポートの左上X座標
		viewport.Y = 0;		//ビューポートの左上Y座標
		viewport.Width = SCREEN_WIDTH;	//幅
		viewport.Height = SCREEN_HEIGHT;//高さ
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;

		m_pCamera = new CCamera;
		m_pCamera->Init();
		m_pCamera->SetViewport(viewport);
		m_pCamera->SetPosV(D3DXVECTOR3(14.0f, 1000.0f, -600.0f));
		m_pCamera->SetPosR(D3DXVECTOR3(900.0f, 255.0f, 600.0f));
	}

	//地面の生成
	if (m_pFieldManager == NULL)
	{
		m_pFieldManager = new CFieldManager;
		m_pFieldManager->Init();
	}

	if (m_pLoadTextObj == NULL) { m_pLoadTextObj = CLoadTextObject::Create(LOAD_OBJ); }					//オブジェクトの生成
	if (m_pLoadTerritory == NULL) { m_pLoadTerritory = CLoadTextTerritory::Create(LOAD_TERRITORY); }	//テリトリーの読み込み

	// 空の生成
	CSky::Load();
	CSky::Create();

}
//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	//カメラの破棄
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//地面マネージャーの破棄
	if (m_pFieldManager != NULL)
	{
		m_pFieldManager->Uninit();
		delete m_pFieldManager;
		m_pFieldManager = NULL;
	}

	//オブジェクトの破棄
	if (m_pLoadTextObj != NULL)
	{
		m_pLoadTextObj->Uninit();
		delete m_pLoadTextObj;
		m_pLoadTextObj = NULL;
	}

	//テリトリーの破棄
	if (m_pLoadTerritory != NULL)
	{
		m_pLoadTerritory->Uninit();
		delete m_pLoadTerritory;
		m_pLoadTerritory = NULL;
	}

	// テクスチャの開放
	CNumber2D::Unload();
	UnloadTexture();
	CScoreRanking::UnloadTex();
	CSky::UnLoad();

	for (int nCntTex = 0; nCntTex < 9; nCntTex++)
	{
		if (m_apRankingBG[nCntTex] != NULL)
		{
			m_apRankingBG[nCntTex]->Uninit();
			m_apRankingBG[nCntTex] = NULL;
		}
	}

	//ベースの破棄
	CModeBase::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	//サウンドの取得
	CSound *pSound = CManager::GetSound();

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
			if (CFade::GetFadeMode() == CFade::FADE_NONE)
			{
				//決定音
				pSound->PlaySound(CSound::SOUND_LABEL_SE007);
				CFade::SetFade(CManager::MODE_TITLE);
			}
		}
	}

	// カウンターを増加
	m_nCounterRanking++;
	if (m_nCounterRanking == 10)
	{// カウンターが一定の値に達した
		if (m_nNumRanking == 0)
		{
			// ランキングを生成
			CScoreRanking::Create(CScore::TYPE_RANKING, 6, D3DXVECTOR3(1007.5f + 80.0f + RANKING_ADDPOS, 180.0f, 0.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f));
		}
		if (m_nNumRanking < 8)
		{// まだ生成できる
			// ランキングBGを生成
			CreateRankingBG((8 - m_nNumRanking));
			m_nNumRanking++;
			m_nCounterRanking = 0;
		}
	}

	// カウンターを増加
	m_nCounter++;
	if ((m_nCounter > FAREM_COUNT))
	{// カウンターが一定の値に達した
		if (CFade::GetFadeMode() != CFade::FADE_OUT)
		{//タイトルに遷移
			CFade::SetFade(CManager::MODE_TITLE);
			m_nCounter = 0;

		}
	}

	// 地面の更新処理
	if (m_pFieldManager != NULL) { m_pFieldManager->Update(); }

	//音量の調節
	//pSound->SetVolume(CSound::SOUND_LABEL_SE007, 10.0f);

	//デバックキー
	if (pKeyboard->GetKeyboardTrigger(DIK_1) == true) { CFade::SetFade(CManager::MODE_RANKING); }
}
//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{
	//カメラの設定
	if (m_pCamera != NULL) { m_pCamera->Set(); }

	CScene::DrawAll();
}

//=============================================================================
// テクスチャの読み込み処理
//=============================================================================
void CRanking::LoadTexture(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, RANKING_TITLE, &m_apTextureRanking[0]);		//ランキングタイトル
	D3DXCreateTextureFromFile(pDevice, RANKING_001, &m_apTextureRanking[1]);		//1位
	D3DXCreateTextureFromFile(pDevice, RANKING_002, &m_apTextureRanking[2]);		//2位
	D3DXCreateTextureFromFile(pDevice, RANKING_003, &m_apTextureRanking[3]);		//3位
	D3DXCreateTextureFromFile(pDevice, RANKING_004, &m_apTextureRanking[4]);		//4位
	D3DXCreateTextureFromFile(pDevice, RANKING_005, &m_apTextureRanking[5]);		//5位
	D3DXCreateTextureFromFile(pDevice, RANKING_006, &m_apTextureRanking[6]);		//6位
	D3DXCreateTextureFromFile(pDevice, RANKING_007, &m_apTextureRanking[7]);		//7位
	D3DXCreateTextureFromFile(pDevice, RANKING_008, &m_apTextureRanking[8]);		//8位
}

//=============================================================================
// テクスチャの破棄
//=============================================================================
void CRanking::UnloadTexture(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < RANKING_BG_MAX; nCnt++)
	{
		if (m_apTextureRanking[nCnt] != NULL)
		{
			m_apTextureRanking[nCnt]->Release();
			m_apTextureRanking[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ランキングBGを生成する処理
//=============================================================================
void CRanking::CreateRankingBG(int nIdx)
{
	D3DXVECTOR3 pos[RANKING_BG_MAX] =
	{
		RANKING_BG_POS0,
		RANKING_BG_POS1,
		RANKING_BG_POS2,
		RANKING_BG_POS3,
		RANKING_BG_POS4,
		RANKING_BG_POS5,
		RANKING_BG_POS6,
		RANKING_BG_POS7,
		RANKING_BG_POS8,
	};
	D3DXVECTOR3 Size[RANKING_BG_MAX] =
	{
		RANKING_BG_SIZE0,
		RANKING_BG_SIZE1,
		RANKING_BG_SIZE2,
		RANKING_BG_SIZE3,
		RANKING_BG_SIZE4,
		RANKING_BG_SIZE5,
		RANKING_BG_SIZE6,
		RANKING_BG_SIZE7,
		RANKING_BG_SIZE8,
	};
	float fRotSpeed[RANKING_BG_MAX] =
	{
		RANKING_BG_ROTSPEED0,
		RANKING_BG_ROTSPEED1,
		RANKING_BG_ROTSPEED2,
		RANKING_BG_ROTSPEED3,
		RANKING_BG_ROTSPEED4,
		RANKING_BG_ROTSPEED5,
		RANKING_BG_ROTSPEED6,
		RANKING_BG_ROTSPEED7,
		RANKING_BG_ROTSPEED8,
	};
	float fTexHeight[RANKING_BG_MAX] =
	{
		RANKING_BG_TEXHEIGHT0,
		RANKING_BG_TEXHEIGHT1,
		RANKING_BG_TEXHEIGHT2,
		RANKING_BG_TEXHEIGHT3,
		RANKING_BG_TEXHEIGHT4,
		RANKING_BG_TEXHEIGHT5,
		RANKING_BG_TEXHEIGHT6,
		RANKING_BG_TEXHEIGHT7,
		RANKING_BG_TEXHEIGHT8,
	};
	float fMove[RANKING_BG_MAX] =
	{
		RANKING_BG_MOVE0,
		RANKING_BG_MOVE1,
		RANKING_BG_MOVE2,
		RANKING_BG_MOVE3,
		RANKING_BG_MOVE4,
		RANKING_BG_MOVE5,
		RANKING_BG_MOVE6,
		RANKING_BG_MOVE7,
		RANKING_BG_MOVE8,
	};

	//テクスチャの表示
	float fAddPos = 0.0f;
	if (nIdx != 0)
	{
		fAddPos = RANKING_ADDPOS;
	}
	m_apRankingBG[nIdx] = CRankingBG::Create(pos[nIdx] + D3DXVECTOR3(fAddPos, 0.0f, 0.0f), pos[nIdx], Size[nIdx], fMove[nIdx], fRotSpeed[nIdx], fTexHeight[nIdx], 5);
	if (m_apRankingBG[nIdx] == NULL) { return; }
	m_apRankingBG[nIdx]->BindTexture(m_apTextureRanking[nIdx]);
}


//*****************************************************************************
// ランキングBGクラスの処理(CRankingBG)
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CRankingBG::CRankingBG(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fMove = 0.1f;
	m_fRot = 0.0f;
	m_fRotSpeed = 0.0f;
	m_fAngle = 0.0f;
	m_fLength = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRankingBG::~CRankingBG(){}

//=============================================================================
// 生成処理
//=============================================================================
CRankingBG *CRankingBG::Create(D3DXVECTOR3 pos, D3DXVECTOR3 destPos, D3DXVECTOR3 Size, float fMove, float fRotSpeed, float fTexHeight, int nPriority, CScene2D::TYPE type)
{
	// メモリの確保
	CRankingBG *m_pRankingBG2D = new CRankingBG(nPriority);
	if (m_pRankingBG2D == NULL) { return NULL; }

	// 各種値の設定
	m_pRankingBG2D->SetRotSpeed(fRotSpeed);
	m_pRankingBG2D->SetMove(fMove);
	m_pRankingBG2D->SetPos(pos);
	m_pRankingBG2D->SetDestPos(destPos);

	// 初期化処理
	if (FAILED(m_pRankingBG2D->Init()))
	{
		m_pRankingBG2D->Uninit();
		m_pRankingBG2D = NULL;
		return NULL;
	}

	// 大きさの設定
	m_pRankingBG2D->Set(pos, Size);

	// テクスチャ座標の設定
	m_pRankingBG2D->SetTexUV(0.0f, 1.0f, 0.0f, fTexHeight);

	// ポリゴンの大きさを計算しておく
	m_pRankingBG2D->CalcPolygonScale();

	return m_pRankingBG2D;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRankingBG::Init(void)
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
void CRankingBG::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRankingBG::Update(void)
{
	// 位置を上に動かす
	m_pos.x -= m_fMove;

	// 目的の位置より上に動かしていたら戻す
	if (m_pos.x < m_DestPos.x)
	{
		m_pos.x = m_DestPos.x;
	}

	// ポリゴンの位置を設定
	CScene2D::SetPos(m_pos, GetOtherPos());
}

//=============================================================================
// 描画処理
//=============================================================================
void CRankingBG::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
// ポリゴンの大きさを計算する処理
//=============================================================================
void CRankingBG::CalcPolygonScale(void)
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
void CRankingBG::RotationPolygon(void)
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

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + GetPos();
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}