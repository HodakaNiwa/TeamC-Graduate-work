//*****************************************************************************
//
//     地面シェーダーの処理[fieldShader.cpp]
//     Auther:Hodaka Niwa
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
CFieldShader::CFieldShader(char *pFileName) : CShader(pFileName)
{
	D3DXMatrixIdentity(&m_MtxView);
	D3DXMatrixIdentity(&m_MtxProjection);
	m_pTexture = NULL;
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		m_LightDir[nCntLight] = INITIALIZE_VECTOR3;
		m_LightDiffuse[nCntLight] = INITIALIZE_VECTOR4;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CFieldShader::~CFieldShader()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CFieldShader *CFieldShader::Create(char *pFileName)
{
	// メモリの作成
	CFieldShader *pFieldShader = NULL;
	pFieldShader = new CFieldShader(pFileName);
	if (pFieldShader == NULL) { return NULL; }

	// 初期化処理
	if (FAILED(pFieldShader->Init()))
	{
		pFieldShader->Uninit();
		delete pFieldShader;
		return NULL;
	}

	return pFieldShader;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CFieldShader::Init(void)
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
	char aParamName[64] = "\0";
	char aHandleName[64] = "\0";
	m_hTechnique = m_pEffect->GetTechniqueByName("Render");
	m_hMtxView = m_pEffect->GetParameterByName(NULL, "g_MtxView");
	m_hMtxProjection = m_pEffect->GetParameterByName(NULL, "g_MtxProj");
	m_hTexture = m_pEffect->GetParameterByName(NULL, "g_Texture");
	for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
	{
		strcpy(aHandleName, CFunctionLib::WriteInt(nCntLight, aHandleName));
		strcpy(aParamName, "g_LightDir");
		strcat(aParamName, aHandleName);
		m_hLightDir[nCntLight] = m_pEffect->GetParameterByName(NULL, aParamName);
		strcpy(aParamName, "g_LightDiffuse");
		strcat(aParamName, aHandleName);
		m_hLightDiffuse[nCntLight] = m_pEffect->GetParameterByName(NULL, aParamName);
	}
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CFieldShader::Uninit(void)
{
	DIRECT_RELEASE(m_pEffect);
}

//=============================================================================
//    シェーダーに各種値を設定する
//=============================================================================
void CFieldShader::SetParamToEffect(void)
{
	if (m_pEffect != NULL)
	{
		m_pEffect->SetMatrix(m_hMtxView, &m_MtxView);
		m_pEffect->SetMatrix(m_hMtxProjection, &m_MtxProjection);
		m_pEffect->SetTexture(m_hTexture, m_pTexture);
		for (int nCntLight = 0; nCntLight < MAXLIGHT; nCntLight++)
		{
			m_pEffect->SetVector(m_hLightDir[nCntLight], &D3DXVECTOR4(m_LightDir[nCntLight], 1.0f));
			m_pEffect->SetVector(m_hLightDiffuse[nCntLight], &D3DXVECTOR4(m_LightDiffuse[nCntLight]));
		}
	}
}