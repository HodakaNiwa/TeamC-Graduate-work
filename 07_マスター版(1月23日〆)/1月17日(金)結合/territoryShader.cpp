//*****************************************************************************
//
//     �e���g���[�V�F�[�_�[�̏���[territoryShader.cpp]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#include "shader.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTerritoryShader::CTerritoryShader(char *pFileName) : CShader(pFileName)
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
//    �f�X�g���N�^
//=============================================================================
CTerritoryShader::~CTerritoryShader()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTerritoryShader *CTerritoryShader::Create(char *pFileName)
{
	// �������̍쐬
	CTerritoryShader *pTerritoryShader = NULL;
	pTerritoryShader = new CTerritoryShader(pFileName);
	if (pTerritoryShader == NULL) { return NULL; }

	// ����������
	if (FAILED(pTerritoryShader->Init()))
	{
		pTerritoryShader->Uninit();
		delete pTerritoryShader;
		return NULL;
	}

	return pTerritoryShader;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTerritoryShader::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return E_FAIL; }

	// �G�t�F�N�g�t�@�C���̐���
	if (FAILED(D3DXCreateEffectFromFile(pDevice, m_pFileName, NULL, NULL, 0, NULL, &m_pEffect, NULL)))
	{
		return E_FAIL;
	}

	// �e��p�����[�^�̃n���h�����擾
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
//    �I������
//=============================================================================
void CTerritoryShader::Uninit(void)
{
	DIRECT_RELEASE(m_pEffect);
}

//=============================================================================
//    �V�F�[�_�[�Ɋe��l��ݒ肷��
//=============================================================================
void CTerritoryShader::SetParamToEffect(void)
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