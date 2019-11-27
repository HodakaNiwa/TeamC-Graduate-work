//*****************************************************************************
//
//     �A�E�g���C���V�F�[�_�[�̏���[outlineShader.cpp]
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
COutlineShader::COutlineShader(char *pFileName) : CShader(pFileName)
{
	D3DXMatrixIdentity(&m_MtxView);
	D3DXMatrixIdentity(&m_MtxProjection);
	D3DXMatrixIdentity(&m_MtxWorld);
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
COutlineShader::~COutlineShader()
{

}

//=============================================================================
//    ��������
//=============================================================================
COutlineShader *COutlineShader::Create(char *pFileName)
{
	// �������̍쐬
	COutlineShader *pOutlineShader = NULL;
	pOutlineShader = new COutlineShader(pFileName);
	if (pOutlineShader == NULL) { return NULL; }

	// ����������
	if (FAILED(pOutlineShader->Init()))
	{
		pOutlineShader->Uninit();
		delete pOutlineShader;
		return NULL;
	}

	return pOutlineShader;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT COutlineShader::Init(void)
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
	m_hMtxWorld = m_pEffect->GetParameterByName(NULL, "g_MtxWorld");

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void COutlineShader::Uninit(void)
{
	DIRECT_RELEASE(m_pEffect);
}

//=============================================================================
//    �V�F�[�_�[�Ɋe��l��ݒ肷��
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