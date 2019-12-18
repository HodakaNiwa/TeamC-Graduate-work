//=============================================================================
//
// 選択処理のプレイヤーの処理 [charMultRendering.cpp]
// Author : 佐藤 安純　[ SATO ASUMI ]
//
//=============================================================================
#include "charMultRendering.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "title.h"
#include "player.h"
#include "tutorial.h"
#include "collision.h"
#include "model.h"
#include "Territory.h"
#include "library.h"
#include "line.h"
#include "loadEffect.h"
#include "RawMouse.h"
#include "select.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)
#define MINE_MODEL_NAME	("data/MODEL/mine000.x")
#define RIGHT_HAND		(5)

//ビューポートの位置設定
#define POS_V	(D3DXVECTOR3(0.0f, 70.0f, -100.0f))
#define POS_R	(D3DXVECTOR3(0.0f, 70.0f, 0.0f))
#define POS_U	(D3DXVECTOR3(0.0f, 1.0f, 0.0f))

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CCharMultRender::CCharMultRender(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
{
	m_pModel = NULL;
	m_pMotion = NULL;
	m_pModelMine = NULL;
	m_pTextureMT = NULL;
	m_pBuffMT = NULL;
	m_pRenderMT = NULL;
	m_nNumPlayer = 0;
	m_bStopMotion = false;
	m_PlayerType = PLAYERTYPE_MAX;
	m_move = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_PosV = INITIALIZE_VECTOR3;
	m_PosR = INITIALIZE_VECTOR3;

	// ビューポートの初期化
	m_viewportMT.X = 0;						// Xの設定
	m_viewportMT.Y = 0;						// Yの設定
	m_viewportMT.Width = SCREEN_WIDTH;		// 幅の設定
	m_viewportMT.Height = SCREEN_HEIGHT;	// 高さの設定
	m_viewportMT.MinZ = 0.0f;				// 手前
	m_viewportMT.MaxZ = 1.0f;				// 奥
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CCharMultRender::~CCharMultRender(){}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCharMultRender::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, int nCuntry)
{
	//プレイヤーのナンバーを取得
	m_nNumPlayer = nNumPlayer;
	m_MotionType = MOTION_TYPE_NONE;
	m_pModel = NULL;
	m_pMotion = NULL;
	m_pModelMine = NULL;
	m_bStopMotion = false;

	//キャラクターの初期化
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType, nCuntry);
	CCharacter::SetPos(pos);

	//モデルの情報を取得
	m_pModel = CCharacter::GetModel();

	//モーションの情報を取得
	m_pMotion = CCharacter::GetMotion();

	//マルチレンダリング用テクスチャの生成
	InitMultTexture();
	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCharMultRender::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void  CCharMultRender::Uninit(void)
{
	CCharacter::Uninit();

	//テクスチャの破棄
	if (m_pTextureMT != NULL)
	{
		m_pTextureMT->Release();
		m_pTextureMT = NULL;
	}

	//レンダラーの破棄
	if (m_pRenderMT != NULL)
	{
		m_pRenderMT->Release();
		m_pRenderMT = NULL;
	}

	//バッファの破棄
	if (m_pBuffMT != NULL)
	{
		m_pBuffMT->Release();
		m_pBuffMT = NULL;
	}

	if (m_pMotion != NULL)
	{
		m_pMotion = NULL;
	}

	if (m_pModel != NULL)
	{
		m_pModel = NULL;
	}

	if (m_pModelMine != NULL)
	{
		m_pModelMine->Uninit();
		delete m_pModelMine;
		m_pModelMine = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void  CCharMultRender::Update(void)
{
	if (m_pMotion == NULL) { return; }

	if (m_MotionType == MOTION_TYPE_SELECT)
	{
		//モーション情報の取得
		int nKey = m_pMotion->GetKey();
		int nFram = m_pMotion->GetnCountMotion();

		switch (m_CharcterType)
		{
		case CCharacter::CHARCTERTYPE_POWER:
			if ((nKey == 1) && (nFram == 16)) { m_bStopMotion = true; }
			break;

		case CCharacter::CHARCTERTYPE_SPEED:
			if ((nKey == 1) && (nFram == 12)) { m_bStopMotion = true; }
			break;

		case CCharacter::CHARCTERTYPE_TECHNIQUE:
			//地雷の生成
			if (m_pModelMine == NULL)
			{
				m_pModelMine = CModel::Create(D3DXVECTOR3(0.0f, -9.0f, 0.0f), MINE_MODEL_NAME, D3DXVECTOR3(2.5f, 2.5f, 2.5f));
				m_pModelMine->SetRot(D3DXVECTOR3(D3DX_PI * -0.5f, 0.0f, 0.0f));
				m_pModelMine->SetParent(m_pModel[RIGHT_HAND]);
			}

			if ((nKey == 2) && (nFram == 13)) { m_bStopMotion = true; }
			break;
		}
	}
	else
	{
		m_bStopMotion = false;

		//地雷の破棄
		if (m_pModelMine != NULL)
		{
			m_pModelMine->Uninit();
			delete m_pModelMine;
			m_pModelMine = NULL;
		}
	}

	if (m_bStopMotion) { return; }
	CCharacter::Update();

	if (m_pMotion->GetNumMotion() == m_MotionType) { return; }

	//モーションの設定
	switch (m_MotionType)
	{
	case MOTION_TYPE_LOSE:
		m_pMotion->SetNumMotion(MOTION_TYPE_LOSE);
		break;

	case MOTION_TYPE_SELECT:
		m_pMotion->SetNumMotion(MOTION_TYPE_SELECT);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void  CCharMultRender::Draw(void)
{
	//変数宣言
	PDIRECT3DSURFACE9 pRenderOrg;	//レンダラーの保存用変数
	LPDIRECT3DSURFACE9 pBuffOrg;	//バッファの保存用変数
	D3DVIEWPORT9 viewportOrg;
	D3DXMATRIX  mtxProjectionOrg;	//プロジェクションマトリックス
	D3DXMATRIX  mtxViewOrg;			//ビューマトリクス
	D3DXMATRIX  mtxProjection;		//プロジェクションマトリックス
	D3DXMATRIX  mtxView;			//ビューマトリクス

									//デバイスの取得
	CRenderer * pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//現在の状態を保存する
	pDevice->GetViewport(&viewportOrg);							//ビューポートの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewOrg);				//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionOrg);	//プロジェクションマトリックスの取得
	pDevice->GetRenderTarget(0, &pRenderOrg);
	pDevice->GetDepthStencilSurface(&pBuffOrg);

	//設定処理
	pDevice->SetRenderTarget(0, m_pRenderMT);
	pDevice->SetDepthStencilSurface(m_pBuffMT);

	//ターゲット・Zバッファのクリア
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	pDevice->SetViewport(&m_viewportMT);	//ビューポートの設定

	D3DXMatrixIdentity(&mtxView);			//ビューマトリックスの初期化

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView, &m_PosV, &m_PosR, &POS_U);

	// ビューマトリックスを設定
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		2500.0f);										//カメラの描画範囲

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	CCharacter::Draw();

	//地雷の描画
	if (m_pModelMine != NULL) { m_pModelMine->Draw(1.0f); }

	//デフォルトに戻す
	pDevice->SetViewport(&viewportOrg);							//ビューポートの取得
	pDevice->SetTransform(D3DTS_VIEW, &mtxViewOrg);				//ビューマトリックスの取得
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionOrg);	//プロジェクションマトリックスの取得
	pDevice->SetRenderTarget(0, pRenderOrg);
	pDevice->SetDepthStencilSurface(pBuffOrg);
}

//=============================================================================
//オブジェクトの生成処理
//=============================================================================
CCharMultRender *CCharMultRender::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],CHARCTERTYPE charctertype, int nNumCharaIdx, int nCuntry)
{
	CCharMultRender *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//メモリを動的確保
		pPlayer = new CCharMultRender;
		if (pPlayer != NULL)
		{//NULLチェック
			pPlayer->m_CharcterType = charctertype;
			pPlayer->Init(pos, ModelTxt,MotionTxt, nNumCharaIdx, nCuntry);
		}
		else
		{
			MessageBox(0, "プレイヤーがNULLでした", "警告", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "プレイヤーがNULLじゃないです", "警告", MB_OK);
	}
	return pPlayer;
}

//=============================================================================
//
//=============================================================================
void  CCharMultRender::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
// テクスチャの初期化処理
//=============================================================================
void CCharMultRender::InitMultTexture(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderOrg = NULL;
	LPDIRECT3DSURFACE9 pBuffOrg = NULL;
	m_pTextureMT = NULL;
	m_pBuffMT = NULL;
	m_pRenderMT = NULL;

	// Textureの作成
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureMT, NULL);

	// レンダラーの取得
	m_pTextureMT->GetSurfaceLevel(0, &m_pRenderMT);

	// バッファの生成
	pDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT, NULL);

	pDevice->GetRenderTarget(0, &pRenderOrg);		// 現在のレンダラーターゲットを保存
	pDevice->GetDepthStencilSurface(&pBuffOrg);		// 現在のバッファを保存
	pDevice->SetRenderTarget(0, m_pRenderMT);		// レンダラーの設定
	pDevice->SetDepthStencilSurface(m_pBuffMT);		// バッファの設定
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 元に戻す
	pDevice->SetRenderTarget(0, pRenderOrg);		// レンダラー
	pDevice->SetDepthStencilSurface(pBuffOrg);		// バッファ

	// ビューポートの初期化
	m_viewportMT.X = 0;						// Xの設定
	m_viewportMT.Y = 0;						// Yの設定
	m_viewportMT.Width = SCREEN_WIDTH;		// 幅の設定
	m_viewportMT.Height = SCREEN_HEIGHT;	// 高さの設定
	m_viewportMT.MinZ = 0.0f;				// 手前
	m_viewportMT.MaxZ = 1.0f;				// 奥

	//視点、注視点の設定
	D3DXVECTOR3 Pos = CCharacter::GetPos();
	m_PosV = Pos + POS_V;
	m_PosR = Pos + POS_R;
}