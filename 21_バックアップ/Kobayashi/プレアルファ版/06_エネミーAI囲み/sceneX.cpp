//=============================================================================
//
// X�t�@�C������ [sceneX.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//=============================================================================
// ��������
//=============================================================================
CSceneX * CSceneX::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, char FileName[80], LPDIRECT3DTEXTURE9 pTexture)
{
	//�C���X�^���X�̐���
	CSceneX * pSceneX = NULL;

	//����������
	pSceneX->Init();

	pSceneX->Load(FileName, pTexture);						//�t�@�C���ǂݍ���
	pSceneX->Set(pos, D3DXVECTOR3(100.0f, 0.0f, 100.0f));	//�ݒ菈��
	pSceneX->SetRot(rot);									//�����̐ݒ�

	return pSceneX;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX() : CScene(3, OBJTYPE_OBJ)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_nNumMat = 0;
	m_ppTexture = NULL;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneX::CSceneX(int nPriority, OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_nNumMat = 0;
	m_vtxMax = D3DXVECTOR3(0, 0, 0);
	m_vtxMin = D3DXVECTOR3(0, 0, 0);
}

//=============================================================================
// �t�@�C���̓ǂݍ���
//=============================================================================
void CSceneX::Load(char FileName[80], LPDIRECT3DTEXTURE9 pTexture)
{
	//�ϐ��錾
	D3DXMATERIAL	*pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(FileName,
					  D3DXMESH_SYSTEMMEM,
					  pDevice,
					  NULL,
					  &m_pBuffMat,
					  NULL,
					  &m_nNumMat,
					  &m_pMesh);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//�e�N�X�`���̃��������m�ہE������
	if (pTexture == NULL)
	{
		m_ppTexture = NULL;
	}
	else
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		//�e�N�X�`���̐���
		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			m_ppTexture[nCntMat] = NULL;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				m_ppTexture[nCntMat] = pTexture;
			}
		}
	}

	//�ϐ��錾
	int		nNumVtx = 0;	//���_��
	DWORD	sizeFVF = 0;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *	pVtxBuff = 0;	//���_�o�b�t�@�ւ̃|�C���^

	//���_�����擾
	nNumVtx = m_pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	m_vtxMax = D3DXVECTOR3(-100000.0f, 0.0f, -100000.0f);	//���_�̍ő�l
	m_vtxMin = D3DXVECTOR3(100000.0f, 0.0f, 100000.0f);		//���_�̍ŏ��l

	//���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

													//�ő�l���r
		if (m_vtxMax.x < vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (m_vtxMax.y < vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (m_vtxMax.z < vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}

		//�ŏ��l���r
		if (m_vtxMin.x > vtx.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (m_vtxMin.y > vtx.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (m_vtxMin.z > vtx.z)
		{
			m_vtxMin.z = vtx.z;
		}
		//�T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneX::Init(void)
{
	m_pos = D3DXVECTOR3(0, 0, 0);	//�ʒu
	m_rot = D3DXVECTOR3(0, 0, 0);	//����
	m_Mat_a = 1.0f;					//�}�e���A���̃��l
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSceneX::Uninit(void)
{
	// ���b�V���̊J��
	if (m_pMesh != NULL)
	{
		//m_pMesh->Release();
		m_pMesh = NULL;
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		//m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`���̊J��
	if (m_ppTexture != NULL)
	{
		for (int nCntTex = 0; nCntTex < (int)m_nNumMat; nCntTex++)
		{
			if (m_ppTexture[nCntTex] != NULL)
			{
				m_ppTexture[nCntTex] = NULL;
			}
		}

		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneX::Update(void){}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneX::Draw(void)
{
	//�ϐ��錾
	D3DMATERIAL9	matDef;				//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL	*pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans, mtxShadow;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�}�g���b�N�X�����߂�
	RequestMatrix(m_rot, m_pos);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (m_pBuffMat == NULL || m_pMesh == NULL) { return; }

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �e�N�X�`���f�[�^�ւ̃|�C���^���擾
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse.a = m_Mat_a;		//�}�e���A���̃��l�̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �}�e���A���̐ݒ�

		if (m_ppTexture != NULL)
		{
			if (m_ppTexture[nCntMat] != NULL)
			{
				pDevice->SetTexture(0, m_ppTexture[nCntMat]);	//�e�N�X�`���̐ݒ�
			}
			else
			{
				pDevice->SetTexture(0, NULL);	//�e�N�X�`���̐ݒ�
			}
		}
		else
		{
			pDevice->SetTexture(0, NULL);	//�e�N�X�`���̐ݒ�
		}
		m_pMesh->DrawSubset(nCntMat);					// ���f��(�p�[�c)�̕`��
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CSceneX::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	m_pos = pos;
}

//=============================================================================
// ���f���̊��蓖��
//=============================================================================
void CSceneX::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 pTexture)
{
	//���f�����̊��蓖��
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL * pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//�e�N�X�`���̃��������m�ہE������
	if (pTexture == NULL)
	{
		m_ppTexture = NULL;
	}
	else
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		//�e�N�X�`���̐���
		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			m_ppTexture[nCntMat] = NULL;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				m_ppTexture[nCntMat] = pTexture;
			}
		}
	}
}

//=============================================================================
// �X�P�[���̐ݒ�
//=============================================================================
void CSceneX::SetScale(D3DXVECTOR3 scale)
{
	//�X�P�[���̐ݒ�
	m_Scale = scale;

	//�����蔻��̍Đݒ�
	m_vtxMax.x *= m_Scale.x;
	m_vtxMax.y *= m_Scale.y;
	m_vtxMax.z *= m_Scale.z;

	m_vtxMin.x *= m_Scale.x;
	m_vtxMin.y *= m_Scale.y;
	m_vtxMin.z *= m_Scale.z;

}

//=============================================================================
// �}�g���b�N�X�����߂�
//=============================================================================
void CSceneX::RequestMatrix(D3DXVECTOR3 rot, D3DXVECTOR3 pos)
{
	D3DXMATRIX mtxRot; // �v�Z�p�}�g���b�N�X

					   // ��]�s����쐬(D3DXMatrixRotationYawPitchRoll�Q��)
	float fSinPitch = sinf(rot.x);
	float fCosPitch = cosf(rot.x);
	float fSinYaw = sinf(rot.y);
	float fCosYaw = cosf(rot.y);
	float fSinRoll = sinf(rot.z);
	float fCosRoll = cosf(rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// �傫���Ɖ�]�𔽉f����
	m_mtxWorld._11 = mtxRot._11 * m_Scale.x;
	m_mtxWorld._12 = mtxRot._12 * m_Scale.x;
	m_mtxWorld._13 = mtxRot._13 * m_Scale.x;
	m_mtxWorld._21 = mtxRot._21 * m_Scale.y;
	m_mtxWorld._22 = mtxRot._22 * m_Scale.y;
	m_mtxWorld._23 = mtxRot._23 * m_Scale.y;
	m_mtxWorld._31 = mtxRot._31 * m_Scale.z;
	m_mtxWorld._32 = mtxRot._32 * m_Scale.z;
	m_mtxWorld._33 = mtxRot._33 * m_Scale.z;

	// �I�t�Z�b�g�ʒu�𔽉f
	m_mtxWorld._41 = pos.x;
	m_mtxWorld._42 = pos.y;
	m_mtxWorld._43 = pos.z;
}