//*****************************************************************************
//
//     コライダー管轄の処理[colliderManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CColliderManager::CColliderManager()
{
	m_nNumCollider = 0;
	m_pCollider = NULL;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CColliderManager::~CColliderManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CColliderManager *CColliderManager::Create(int nNumCollider)
{
	// メモリを確保
	CColliderManager *pColliderManager = NULL;
	pColliderManager = new CColliderManager;
	if (pColliderManager == NULL)return NULL;

	// 初期化処理
	if (FAILED(pColliderManager->Init(nNumCollider)))
	{
		pColliderManager->Uninit();
		delete pColliderManager;
		pColliderManager = NULL;
		return NULL;
	}

	return pColliderManager;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CColliderManager::Init(int nNumCollider)
{
	// コライダーの数を設定
	SetNumCollider(nNumCollider);

	// コライダーの数だけメモリを確保しておく
	MakeMemoryCollider();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CColliderManager::Uninit(void)
{
	// コライダーの終了処理
	if (m_pCollider != NULL)
	{
		ReleaseCollider();
	}

	// 自身のポインタを削除
	delete this;
}

//=============================================================================
//    コライダーを開放する処理
//=============================================================================
void CColliderManager::MakeMemoryCollider(void)
{
	// メモリを確保する
	if (m_pCollider != NULL) { return; }
	m_pCollider = new CCollider*[m_nNumCollider];
	if (m_pCollider == NULL) { return; }

	// メモリを確保した分空にしておく
	for (int nCntCol = 0; nCntCol < m_nNumCollider; nCntCol++)
	{
		m_pCollider[nCntCol] = NULL;
	}
}

//=============================================================================
//    コライダーを開放する処理
//=============================================================================
void CColliderManager::ReleaseCollider(void)
{
	// コライダーの数だけ終了処理
	for (int nCntCol = 0; nCntCol < m_nNumCollider; nCntCol++)
	{
		if (m_pCollider[nCntCol] != NULL)
		{
			m_pCollider[nCntCol]->Uninit();
			m_pCollider[nCntCol] = NULL;
		}
	}

	// メモリの開放
	delete[] m_pCollider;
	m_pCollider = NULL;
}

//=============================================================================
//    コライダーの数を設定する
//=============================================================================
void CColliderManager::SetNumCollider(const int nNumCollider)
{
	m_nNumCollider = nNumCollider;
}

//=============================================================================
//    コライダークラスへのポインタを設定する
//=============================================================================
void CColliderManager::SetCollider(CCollider *pCollider, int nIdx)
{
	m_pCollider[nIdx] = pCollider;
}

//=============================================================================
//    コライダーの数を取得する
//=============================================================================
int CColliderManager::GetNumCollider(void)
{
	return m_nNumCollider;
}

//=============================================================================
//    コライダークラスへのポインタを取得する
//=============================================================================
CCollider **CColliderManager::GetCollider(void)
{
	return m_pCollider;
}