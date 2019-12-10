//====================================================================================
//
// 第9戦力UI処理	[robotUI.cpp]
// Author;荒谷由朗
//
//====================================================================================
#include "robotUI.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "character.h"
#include "library.h"
#include "debuglog.h"
#include "scene.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define WARNING_TEX_FILENAME_0 "data/TEXTURE/game/Warning000.jpg"	// 危険警告

// 危険警告
#define ROBOTUI_WARNING_POS		(D3DXVECTOR3(SCREEN_WIDTH/2,70.0f,1.0f))
#define ROBOTUI_WARNING_COL		(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define ROBOTUI_WARNING_WIDTH	(SCREEN_WIDTH)
#define ROBOTUI_WARNING_HEIGHT	(70.0f)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CRobotUI::m_apTexture[ROBOTUI_TEXTURE_MAX] = {};

//*****************************************************************************
//     CRobotUIの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CRobotUI::CRobotUI()
{
	m_pRobotWarning = NULL;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRobotUI::~CRobotUI()
{
}

//=============================================================================
//    生成処理
//=============================================================================
CRobotUI *CRobotUI::Create(int nNum)
{
	CRobotUI *pRobotUI = NULL;
	if (pRobotUI != NULL) { return NULL; }

	// メモリを確保
	pRobotUI = new CRobotUI;
	if (pRobotUI == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pRobotUI->Init(nNum)))
	{
		//pRobotUI->Uninit();
		//delete pRobotUI;
		//return NULL;
	}

	if (pRobotUI != NULL)
	{
		pRobotUI->m_nNum = nNum;
	}

	return pRobotUI;	// インスタンスのアドレスを返す
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CRobotUI::Load(void)
{
	char aFileName[ROBOTUI_TEXTURE_MAX][256]
	{
		WARNING_TEX_FILENAME_0,
	};

	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return E_FAIL; }

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < ROBOTUI_TEXTURE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, aFileName[nCntTex], &m_apTexture[nCntTex]);
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの開放処理
//=============================================================================
void CRobotUI::UnLoad(void)
{
	for (int nCntTex = 0; nCntTex < ROBOTUI_TEXTURE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRobotUI::Init(int nNum)
{
	// 危険警告を生成
	CreateWarning(nNum);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRobotUI::Uninit(void)
{
	// 危険警告を開放
	ReleaseWarning();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRobotUI::Update(void)
{
	switch (m_nNum)
	{
	case 0:			// 危険警告
		UpdateWarning();
		break;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CRobotUI::Draw(void)
{
}

//=============================================================================
//    危険警告を開放する処理
//=============================================================================
void CRobotUI::CreateWarning(int nNum)
{
	m_pRobotWarning = CRobotUI2D::Create(ROBOTUI_WARNING_POS, ROBOTUI_WARNING_COL,
		(float)ROBOTUI_WARNING_WIDTH, ROBOTUI_WARNING_HEIGHT, (float)nNum);
	if (m_pRobotWarning == NULL) { return; }
	m_pRobotWarning->BindTexture(m_apTexture[0]);
}

//=============================================================================
//    危険警告を開放する処理
//=============================================================================
void CRobotUI::ReleaseWarning(void)
{
	if (m_pRobotWarning != NULL)
	{
		m_pRobotWarning->Uninit();
		m_pRobotWarning = NULL;
	}
}

//=============================================================================
//    危険警告の動き
//=============================================================================
void CRobotUI::UpdateWarning(void)
{
	if (m_pRobotWarning != NULL)
	{// 危険警告のNULLチェック
		CGame *pGame = CManager::GetGame();		// ゲームの取得

		bool bEveCamFlag = pGame->GetEVeCamFlag();	// イベントカメラの死亡フラグを取得
		if (bEveCamFlag == true)
		{// イベントカメラが消えていたら消す
			Uninit();
		}
	}
}

//*****************************************************************************
//     CRobotUI2Dの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CRobotUI2D::CRobotUI2D(): CScene(4, CScene::OBJTYPE_ROBOT)
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
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRobotUI2D::~CRobotUI2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRobotUI2D *CRobotUI2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fTexU, float fTexV, float fTexWidth, float fTexHeight, float fRot, int nNum)
{
	// メモリを確保
	CRobotUI2D *pRobotUI2D = NULL;
	pRobotUI2D = new CRobotUI2D;
	if (pRobotUI2D == NULL) { return NULL; }

	// 各種値の設定
	pRobotUI2D->SetPos(pos);              // ポリゴンの座標
	pRobotUI2D->SetRot(fRot);             // ポリゴンの向き
	pRobotUI2D->SetCol(col);              // ポリゴンの色
	pRobotUI2D->SetWidth(fWidth);         // ポリゴンの幅
	pRobotUI2D->SetHeight(fHeight);       // ポリゴンの高さ
	pRobotUI2D->SetTexU(fTexU);           // ポリゴンの左上テクスチャU座標
	pRobotUI2D->SetTexV(fTexV);           // ポリゴンの左上テクスチャV座標
	pRobotUI2D->SetTexWidth(fTexWidth);   // ポリゴンのテクスチャ座標の横幅
	pRobotUI2D->SetTexHeight(fTexHeight); // ポリゴンのテクスチャ座標の縦幅

	// 初期化処理
	if (FAILED(pRobotUI2D->Init()))
	{
		pRobotUI2D->Uninit();
		delete pRobotUI2D;
		return NULL;
	}

	return pRobotUI2D;		// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRobotUI2D::Init(void)
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
void CRobotUI2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRobotUI2D::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CRobotUI2D::Draw(void)
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
//    頂点バッファ生成処理
//=============================================================================
void CRobotUI2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
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
void CRobotUI2D::SetScale(float fWidth, float fHeight)
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
void CRobotUI2D::SetVtxBuffValue(void)
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
		m_Pos.z = 0.0f;
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
void CRobotUI2D::SetVtxBuffPos(void)
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
void CRobotUI2D::SetVtxBuffCol(void)
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
void CRobotUI2D::SetVtxBuffTex(void)
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