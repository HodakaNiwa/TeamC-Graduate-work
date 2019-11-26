//*****************************************************************************
//
//     リングの処理[ring.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "ring.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define RING_ROTSPEED (D3DX_PI * 0.005f)

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CRing::CRing()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CRing::~CRing()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRing *CRing::Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 pTexture)
{
	CRing *pRing = NULL;
	if (pRing != NULL) { return NULL; }

	// メモリを確保
	pRing = new CRing;
	if (pRing == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pRing->Init()))
	{
		pRing->Uninit();
		delete pRing;
		return NULL;
	}

	// 各種値の設定
	pRing->SetPos(pos);
	pRing->SetScale(scale);

	// モデルの割り当て
	pRing->BindModel(pMesh, pBuffMat, nNumMat, pTexture);

	return pRing;	// インスタンスのアドレスを返す
}


//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRing::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CSceneX::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRing::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRing::Update(void)
{
	// 向きの取得
	D3DXVECTOR3 rot = GetRot();

	// 向きを回転させる
	rot.y += RING_ROTSPEED;
	rot.y = CFunctionLib::CheckRotationOverPiX(rot.y);
	rot.y = CFunctionLib::CheckRotationOverMinusPiX(rot.y);

	// 向きの設定
	SetRot(rot);
}

//=============================================================================
//    描画処理
//=============================================================================
void CRing::Draw(void)
{
	//レンダリングクラスを取得
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// 法線を正規化する(大きさによって法線が変わるので)
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	// 共通の描画処理
	CSceneX::Draw();

	// 法線を正規化しない
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
}