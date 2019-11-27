//=============================================================================
//
// ���f����p�̏��� [model.cpp]
// Author : �L�n���u
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "shader.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_OUTLINESHADER_FILENAME "data/outline.hlsl"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
COutlineShader *CModel::m_pOutlineShader = NULL;		// �A�E�g���C���V�F�[�_�[�N���X�ւ̃|�C���^
#ifdef _DEBUG
int CModel::m_nCreateNum = 0;
#endif

//=============================================================================
// ��������
//=============================================================================
CModel * CModel::Create(const D3DXVECTOR3 pos, char FileName[40], D3DXVECTOR3 Scale)
{
	//�C���X�^���X�̐���
	CModel * pModel = NULL;

	if (pModel == NULL)
	{

		pModel = new CModel;
		if (pModel != NULL)
		{
			//����������
			pModel->Init(FileName);

			//�ݒ菈��
			pModel->SetPos(pos);
			pModel->SetFirstPos(pos);
			pModel->SetScale(Scale);

		}
	}

	return pModel;
}

//=============================================================================
// �V�F�[�_�[��ǂݍ��ޏ���
//=============================================================================
HRESULT CModel::ShaderLoad(void)
{
	if (m_pOutlineShader != NULL) { return E_FAIL; }
	m_pOutlineShader = COutlineShader::Create(MODEL_OUTLINESHADER_FILENAME);

	return S_OK;
}

//=============================================================================
// �V�F�[�_�[���J�����鏈��
//=============================================================================
void CModel::ShaderUnLoad(void)
{
	if (m_pOutlineShader != NULL)
	{
		m_pOutlineShader->Uninit();
		delete m_pOutlineShader;
		m_pOutlineShader = NULL;
	}
}

//===============================================================================
//�@�R���X�g���N�^
//===============================================================================
CModel::CModel()
{
	m_pMesh = NULL;								//���b�V�����ւ̃|�C���^
	m_pBuffMat = NULL;							//�}�e���A���̏��ւ̃|�C���^
	m_nNumMat = 0;								//�}�e���A���̏��
	m_VtxMin, m_VtxMax;							//���f���̍ŏ��l�E�ő�l
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	m_FirstPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����ʒu
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�g��k����
	m_pTextures = NULL;
	m_pParent = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);			// ���[���h�}�g���b�N�X�̏�����
	m_Move = INITIALIZE_VECTOR3;				// �ړ���
}
//===============================================================================
//�@�f�X�g���N�^
//===============================================================================
CModel::~CModel(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(char FileName[40])
{
#ifdef _DEBUG
	m_nCreateNum++;
#endif

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	CreateXFile(FileName);		//X�t�@�C���̐���

	int nNumVtx;				//���_��
	DWORD sizeFVF;				//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;				//���_�o�b�t�@�ւ̃|�C���^

	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ʒu
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�����B����
	m_VtxMax = D3DXVECTOR3(-10000, -10000, -10000);			//�ő�l
	m_VtxMin = D3DXVECTOR3(10000, 10000, 10000);			//�ŏ��l

	//���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

		//�ő�l
		if (vtx.x > m_VtxMax.x)
		{
			m_VtxMax.x = vtx.x;
		}
		if (vtx.y > m_VtxMax.y)
		{
			m_VtxMax.y = vtx.y;
		}
		if (vtx.z > m_VtxMax.z)
		{
			m_VtxMax.z = vtx.z;
		}
		//�ŏ��l
		if (vtx.x < m_VtxMin.x)
		{
			m_VtxMin.x = vtx.x;
		}
		if (vtx.y < m_VtxMin.y)
		{
			m_VtxMin.y = vtx.y;
		}
		if (vtx.z < m_VtxMin.z)
		{
			m_VtxMin.z = vtx.z;
		}

		//�T�C�Y���̃|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�o�b�t�@�̃A�����b�N
	m_pMesh->UnlockVertexBuffer();

	m_Rot.y = 0.0f;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{
#ifdef _DEBUG
	m_nCreateNum--;
#endif

	// ���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`���̔j��
	if (m_pTextures != NULL)
	{
		//m_pTextures->Release();
		m_pTextures = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void) {}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(float fAlpha)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans,mtxParent, mtxScale;	//�v�Z
	D3DMATERIAL9 matDef;								//���݂̃}�e���A����ۑ�
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�g��k���s��̍쐬
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{//�e�̏ꍇ
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//�e�ł͂Ȃ��ꍇ
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�e�̃}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`����NULL�ɂ���
		pDevice->SetTexture(0, m_pTextures);

		//�I�u�W�F�N�g(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
//�@���f���̃A�E�g���C����`�悷�鏈��
//=============================================================================
void CModel::DrawOutline(void)
{
	if (m_pOutlineShader == NULL) { return; }

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent, mtxScale;	//�v�Z

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�g��k���s��̍쐬
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{//�e�̏ꍇ
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//�e�ł͂Ȃ��ꍇ
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�e�̃}�g���b�N�X�Ɗ|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	m_pOutlineShader->SetMtxWorld(m_mtxWorld);

	// �v���W�F�N�V�����}�g���b�N�X�ƃr���[�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	m_pOutlineShader->SetMtxProjection(mtxProj);
	m_pOutlineShader->SetMtxView(mtxView);

	// �V�F�[�_�[�ɒl���Z�b�g
	m_pOutlineShader->SetParamToEffect();

	// �\�ʃJ�����O�ɐݒ�
	DWORD CullMode;
	pDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// �`��J�n
	m_pOutlineShader->Begin();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// �����_�����O�p�X�J�n
		m_pOutlineShader->BeginPass(0);

		// �I�u�W�F�N�g(�p�[�c)�̕`��
		m_pMesh->DrawSubset(nCntMat);

		// �����_�����O�p�X�I��
		m_pOutlineShader->EndPass();
	}

	// �`��I��
	m_pOutlineShader->End();

	// �J�����O�̐ݒ��߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, CullMode);
}

//=============================================================================
//�@�}�e���A���J���[�̐ݒ�
//=============================================================================
void CModel::SetColor(D3DXCOLOR col)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �F��t����
	pMat->MatD3D.Diffuse = col;

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//�@X�t�@�C���̓ǂݍ���
//=============================================================================
void CModel::CreateXFile(char FileName[40])
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(&FileName[0],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);
}