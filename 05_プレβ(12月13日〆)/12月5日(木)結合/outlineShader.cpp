//*****************************************************************************
//
//     アウトラインシェーダーの処理[outlineShader.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "shader.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
COutlineShader::COutlineShader(char *pFileName) : CShader(pFileName)
{
	D3DXMatrixIdentity(&m_MtxView);
	D3DXMatrixIdentity(&m_MtxProjection);
	D3DXMatrixIdentity(&m_MtxWorld);
}

//=============================================================================
//    デストラクタ
//=============================================================================
COutlineShader::~COutlineShader()
{

}

//=============================================================================
//    生成処理
//=============================================================================
COutlineShader *COutlineShader::Create(char *pFileName)
{
	// メモリの作成
	COutlineShader *pOutlineShader = NULL;
	pOutlineShader = new COutlineShader(pFileName);
	if (pOutlineShader == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pOutlineShader->Init()))
	{
		pOutlineShader->Uninit();
		delete pOutlineShader;
		return NULL;
	}

	return pOutlineShader;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT COutlineShader::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// エフェクトファイルの生成
	if (FAILED(D3DXCreateEffectFromFile(pDevice, m_pFileName, NULL, NULL, 0, NULL, &m_pEffect, NULL)))
	{
		return E_FAIL;
	}

	// 各種パラメータのハンドルを取得
	m_hTechnique = m_pEffect->GetTechniqueByName("Render");
	m_hMtxView = m_pEffect->GetParameterByName(NULL, "g_MtxView");
	m_hMtxProjection = m_pEffect->GetParameterByName(NULL, "g_MtxProj");
	m_hMtxWorld = m_pEffect->GetParameterByName(NULL, "g_MtxWorld");

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void COutlineShader::Uninit(void)
{
	DIRECT_RELEASE(m_pEffect);
}

//=============================================================================
//    シェーダーに各種値を設定する
//=============================================================================
void COutlineShader::SetParamToEffect(void)
{
	if (m_pEffect != NULL)
	{
		m_pEffect->SetMatrix(m_hMtxView, &m_MtxView);
		m_pEffect->SetMatrix(m_hMtxProjection, &m_MtxProjection);
		m_pEffect->SetMatrix(m_hMtxWorld, &m_MtxWorld);
	}
}