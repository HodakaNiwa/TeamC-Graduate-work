//*****************************************************************************
//
//     �C���X�^���V���O�V�F�[�_�[�̏���[instancing.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "shader.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
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
//    �f�X�g���N�^
//=============================================================================
CInstancingShader::~CInstancingShader()
{

}

//=============================================================================
//    ��������
//=============================================================================
CInstancingShader *CInstancingShader::Create(char *pFileName)
{
	// �������̍쐬
	CInstancingShader *pInstancingShader = NULL;
	pInstancingShader = new CInstancingShader(pFileName);
	if (pInstancingShader == NULL) { return NULL; }

	// ����������
	if (FAILED(pInstancingShader->Init()))
	{
		pInstancingShader->Uninit();
		delete pInstancingShader;
		return NULL;
	}

	return pInstancingShader;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CInstancingShader::Init(void)
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
//    �I������
//=============================================================================
void CInstancingShader::Uninit(void)
{
	DIRECT_RELEASE(m_pEffect);
}

//=============================================================================
//    �V�F�[�_�[�Ɋe��l��ݒ肷��
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