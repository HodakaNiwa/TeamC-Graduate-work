//*****************************************************************************
//
//     空の処理[sky.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sky.h"
#include "manager.h"
#include "library.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SKY_BG_TEXFILENAME      "data/TEXTURE/Bg/bg000.jpg"			// 背景テクスチャのファイルパス名
#define SKY_MESHSPHER_POS       (D3DXVECTOR3(0.0f,-9000.0f,0.0f))	// メッシュスフィアの位置
#define SKY_MESHSPHER_COL       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))	// メッシュスフィアの色
#define SKY_MESHSPHERE_XBLOCK   (30)								// メッシュスフィアの横の分割数
#define SKY_MESHSPHERE_YBLOCK   (30)								// メッシュスフィアの縦の分割数
#define SKY_MESHSPHERE_RADIUS   (15000.0f)							// メッシュスフィアの半径
#define SKY_MESHSPHERE_PRIORITY (2)									// メッシュスフィアの処理優先順位

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CSky::m_pTexture = NULL;

//=============================================================================
//    コンストラクタ
//=============================================================================
CSky::CSky(int nPriority, OBJTYPE objType) : CMeshSphere(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CSky::~CSky()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSky *CSky::Create(void)
{
	// メモリ確保
	CSky *pSky = NULL;
	pSky = new CSky(SKY_MESHSPHERE_PRIORITY);
	if (pSky == NULL)
	{
		return NULL;
	}

	// 各種値の代入
	pSky->SetPos(SKY_MESHSPHER_POS);			// メッシュスフィアの座標
	pSky->SetRot(INITIALIZE_VECTOR3);			// メッシュスフィアの向き
	pSky->SetCol(SKY_MESHSPHER_COL);			// メッシュスフィアの色
	pSky->SetRadius(SKY_MESHSPHERE_RADIUS);		// メッシュスフィアの半径
	pSky->SetXBlock(SKY_MESHSPHERE_XBLOCK);		// 横の分割数
	pSky->SetYBlock(SKY_MESHSPHERE_YBLOCK);		// 縦の分割数

	// 初期化処理
	if (FAILED(pSky->Init()))
	{
		pSky->Uninit();
		return NULL;
	}

	return pSky;
}

//=============================================================================
//    テクスチャの読み込み処理
//=============================================================================
HRESULT CSky::Load(void)
{
	if (FAILED(D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), SKY_BG_TEXFILENAME, &m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    テクスチャの開放処理
//=============================================================================
void CSky::UnLoad(void)
{
	DIRECT_RELEASE(m_pTexture);
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSky::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CMeshSphere::Init()))
	{
		return E_FAIL;
	}

	// テクスチャの割り当て
	BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSky::Uninit(void)
{
	CMeshSphere::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSky::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSky::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// ライティングをしない設定に
	DWORD Lighting;
	pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 共通の描画処理
	CMeshSphere::Draw();

	// ライティングの設定を戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
}