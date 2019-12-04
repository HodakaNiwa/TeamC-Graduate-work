//*****************************************************************************
//
//     インスタンシングシェーダーの処理[instancing.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "shader.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CInstancingShader::CInstancingShader(char *pFileName) : CShader(pFileName)
{
	D3DXMatrixIdentity(&m_MtxView);
	D3DXMatrixIdentity(&m_MtxProjection);
	D3DXMatrixIdentity(&m_MtxLocal);
	m_pTexture = NULL;
	m_LightDir = INITIALIZE_VECTOR3;
	m_LightDiffuse = INITIALIZE_VECTOR4;
	m_LightAmbient = INITIALIZE_VECTOR4;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CInstancingShader::~CInstancingShader()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CInstancingShader *CInstancingShader::Create(char *pFileName)
{
	// メモリの作成
	CInstancingShader *pInstancingShader = NULL;
	pInstancingShader = new CInstancingShader(pFileName);
	if (pInstancingShader == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pInstancingShader->Init()))
	{
		pInstancingShader->Uninit();
		delete pInstancingShader;
		return NULL;
	}

	return pInstancingShader;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CInstancingShader::Init(void)
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
	m_hMtxLocal = m_pEffect->GetParameterByName(NULL, "g_MtxLocal");
	m_hTexture = m_pEffect->GetParameterByName(NULL, "g_Texture");
	m_hLightDir = m_pEffect->GetParameterByName(NULL, "g_LightDir");
	m_hLightDiffuse = m_pEffect->GetParameterByName(NULL, "g_LightDiffuse");
	m_hLightAmbient = m_pEffect->GetParameterByName(NULL, "g_LightAmbient");

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CInstancingShader::Uninit(void)
{
	DIRECT_RELEASE(m_pEffect);
}

//=============================================================================
//    シェーダーに各種値を設定する
//=============================================================================
void CInstancingShader::SetParamToEffect(void)
{
	if (m_pEffect != NULL)
	{
		m_pEffect->SetMatrix(m_hMtxView, &m_MtxView);
		m_pEffect->SetMatrix(m_hMtxProjection, &m_MtxProjection);
		m_pEffect->SetMatrix(m_hMtxLocal, &m_MtxLocal);
		m_pEffect->SetTexture(m_hTexture, m_pTexture);
		m_pEffect->SetVector(m_hLightDir, &D3DXVECTOR4(m_LightDir, 1.0f));
		m_pEffect->SetVector(m_hLightDiffuse, &D3DXVECTOR4(m_LightDiffuse));
		m_pEffect->SetVector(m_hLightAmbient, &D3DXVECTOR4(m_LightAmbient));
	}
}