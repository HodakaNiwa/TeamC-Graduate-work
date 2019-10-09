//=============================================================================
//
// ���b�V���t�B�[���h���� [sceneMeshfiled.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "sceneMeshfiled.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FILE_NAME00		("data/TEXT/MeshFiled.txt")
#define TEXTURE_NAME00	("data/TEXTURE/fild.png")
#define MAX_POS			(4)
#define SIZE_POLYGON	(50.0f)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CSceneMeshFiled::m_pTexture = NULL;

//=============================================================================
// ��������
//=============================================================================
CSceneMeshFiled * CSceneMeshFiled::Create(const D3DXVECTOR3 pos)
{
	//�C���X�^���X�̐���
	CSceneMeshFiled * pSceneMeshFiled;
	pSceneMeshFiled = new CSceneMeshFiled;

	//����������
	pSceneMeshFiled->Init();

	//�e�L�X�g��ǂݍ���
	pSceneMeshFiled->LoadData();

	//�e�N�X�`���̓ǂݍ���
	pSceneMeshFiled->Load();

	//�ݒ菈��
	pSceneMeshFiled->Set(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return pSceneMeshFiled;
}

//=============================================================================
// �e�N�X�`���ݒ�
//=============================================================================
void CSceneMeshFiled::Load(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME00,
		&m_pTexture);
}

//=============================================================================
// �e�N�X�`���j��
//=============================================================================
void CSceneMeshFiled::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �f�[�^�����[�h����
//=============================================================================
void CSceneMeshFiled::LoadData(void)
{
	//�ϐ��錾
	FILE *pFile;

	//�e�L�X�g�̓ǂݍ���
	pFile = fopen(FILE_NAME00, "r");

	if (pFile != NULL)
	{
		fscanf(pFile, "%d", &m_nWidth);		//�����̓ǂݍ���
		fscanf(pFile, "%d", &m_nHight);		//�c���̓ǂݍ���
		fscanf(pFile, "%d", &m_nPrimitiv);	//���_�̓ǂݍ���

		for (int nCnt = 0; nCnt < m_nPrimitiv; nCnt++)
		{
			fscanf(pFile, "%f", &m_LoadPos[nCnt].x);		//�����̓ǂݍ���
			fscanf(pFile, "%f", &m_LoadPos[nCnt].y);		//�����̓ǂݍ���
			fscanf(pFile, "%f", &m_LoadPos[nCnt].z);		//�����̓ǂݍ���
		}

		//�ǂݍ��݂��I������
		fclose(pFile);
	}
}

//=============================================================================
// �f�[�^�������o��
//=============================================================================
void CSceneMeshFiled::SaveData(void)
{
	//�ϐ��錾
	FILE *pFile = NULL;

	//�e�L�X�g�̍쐬
	pFile = fopen(FILE_NAME00, "w");

	if (pFile != NULL)
	{//txt�ւ̏�������

		VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		fprintf(pFile, "%d %d %d\n ", m_nHight, m_nWidth, m_nPrimitiv);

		for (int nCntPrimitv = 0; nCntPrimitv < m_nPrimitiv; nCntPrimitv++)
		{//���_���W�̏�������
			fprintf(pFile, "\n%.3f %.3f %.3f \n ", (double)pVtx[0].pos.x, (double)pVtx[0].pos.y, (double)pVtx[0].pos.z);

			//�Y���̈ʒu�܂Ői�߂�
			pVtx++;
		}

		//�������݂��I������
		fclose(pFile);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSceneMeshFiled::CSceneMeshFiled() : CScene(2, OBJTYPE_FLOR)
{
	m_pVtxBuff = NULL;				//���_�o�b�t�@
	m_pIdxBuff = NULL;				//�C���f�b�N�X�o�b�t�@
	m_pos = D3DXVECTOR3(0, 0, 0);	//���S���W
	m_rot = D3DXVECTOR3(0, 0, 0);	//����
	m_nHight = 0;					//�|��
	m_nWidth = 0;					//����
	m_nIndex = 0;					//�C���f�b�N�X��
	m_nPolygon = 0;					//�|���S����
	m_nPrimitiv = 0;				//���_��
	m_nSelectPrimitiv = 0;			//���ݑI������Ă��钸�_
	m_bUpdatePrimitiv = false;		//���_��񂪍X�V����Ă��邩�ǂ���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSceneMeshFiled::~CSceneMeshFiled()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSceneMeshFiled::Init(void)
{
	//�ϐ��̏�����
	m_pVtxBuff = NULL;				//���_�o�b�t�@
	m_pIdxBuff = NULL;				//�C���f�b�N�X�o�b�t�@
	m_pos = D3DXVECTOR3(0, 0, 0);	//���S���W
	m_rot = D3DXVECTOR3(0, 0, 0);	//����
	m_nHight = 0;					//�|��
	m_nWidth = 0;					//����
	m_nIndex = 0;					//�C���f�b�N�X��
	m_nPolygon = 0;					//�|���S����
	m_nPrimitiv = 0;				//���_��
	m_LoadPos[1200] = {};			//����������

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSceneMeshFiled::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//�e�N�X�`���̔j��
	Unload();

	//���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSceneMeshFiled::Update(void)
{
	//�ϐ��錾
	int nCntVertex = 0;

	//�f�o�b�N�\���̎擾
	CDebugProc * pDebugLeft = CRenderer::GetDebugLeft();

	//���_�̍����ύX����
	//UpdatePrimitiv();

	//�f�o�b�N�\���̎擾
	CDebugProc * pDebugRight = CRenderer::GetDebugRight();

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSceneMeshFiled::Draw(void)
{
	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXMATRIX		  mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
		
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
								  0, 0,
								  m_nPrimitiv,
								  0,
								  m_nPolygon);
}

//=============================================================================
// �ݒ菈��
//=============================================================================
void CSceneMeshFiled::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	//�ϐ��錾
	float fWidthSize = 0;		//�����̃T�C�Y
	float fHightSize = 0;		//�c���̃T�C�Y
	D3DXVECTOR3 Vec0[2] = {};	//�x�N�g��0
	D3DXVECTOR3 Vec1[2] = {};	//�x�N�g��1
	int nCntPos = 0;				//�ǂݍ��񂾍��W�z���i�߂�

	//�|���S���p�̖@��
	D3DXVECTOR3 nor[8000] = {};	//�|���S���̖@���x�N�g��
	int nCntNor = 0;			//�@���̔z���i�߂�

	//�ϐ��̑��
	m_pos = pos;

	//���_��, �|���S����, �C���f�b�N�X���̌v�Z
	int LoadPrimitiv = m_nPrimitiv;
	m_nPrimitiv = (m_nWidth + 1) * (m_nHight + 1);					//���_��
	m_nPolygon	= (m_nWidth * m_nHight) * 2 + (4 * (m_nHight - 1));	//�|���S����
	m_nIndex	= m_nPolygon + 2;									//�C���f�b�N�X��

	//���S����̔����̒��������߂�
	fWidthSize = (SIZE_POLYGON * m_nWidth) * 0.5f;	//���̔����̒���
	fHightSize = (SIZE_POLYGON * m_nHight) * 0.5f;	//�c�̔����̒���

	//�����_�����O�N���X���擾
	CRenderer * pRenderer = NULL;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nPrimitiv,	//�m�ۂ��钸�_�o�b�t�@�̐�
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD)* m_nIndex,		//�m�ۂ���C���f�b�N�X�o�b�t�@�̐�
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,										//16�r�b�g(2�o�C�g)�̃f�[�^���m��
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHight = 0; nCntHight <= m_nHight; nCntHight++)
	{//�c�̒��_
		for (int nCntWidth = 0; nCntWidth <= m_nWidth; nCntWidth++)
		{//���̒��_

		 //���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-fWidthSize + (SIZE_POLYGON * nCntWidth), 0.0f, fHightSize - (SIZE_POLYGON * nCntHight));

			if (LoadPrimitiv <= nCntPos)
			{
				pVtx[0].pos.y = 0.0f;
			}
			else
			{
				pVtx[0].pos.y = m_LoadPos[nCntPos].y;
			}

			//�@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//�F�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`���̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (nCntWidth * 1.0f), 0.0f + (nCntHight * 1.0f));

			//�Y���̈ʒu�܂Ői�߂�
			pVtx += 1;
			nCntPos++;		//�z��̃J�E���^�[��i�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�@����ݒ肷��
	SetNor();

	WORD *pIdx;		//�C���f�b�N�X�f�[�^�ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���C���f�b�N�X�o�b�t�@�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntIdx = 0;			//�C���f�b�N�X�̃J�E���^�[

	//�C���f�b�N�X�̐ݒ�
	for (int nCntHight = 0; nCntHight < m_nHight; nCntHight++)
	{//�c�̒��_
		for (int nCntWidth = 0; nCntWidth <= m_nWidth; nCntWidth++, nCntIdx++)
		{//���̒��_
			pIdx[0] = (m_nWidth + 1) + nCntIdx;	//���̒��_�� + �O���[�v�̐�
			pIdx[1] = nCntIdx;

			//�Y���̈ʒu�܂Ői�߂�
			pIdx += 2;

			if (nCntWidth == m_nWidth && nCntHight < m_nHight - 1)
			{//X���̐܂�Ԃ�
				pIdx[0] = nCntIdx;									//�O���[�v�̐�
				pIdx[1] = (m_nWidth + 1) + nCntIdx + 1;	//���̒��_�� + �O���[�v�̐� + 1

				//�Y���̈ʒu�܂Ői�߂�
				pIdx += 2;
			}
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
}


//=============================================================================
// �n�ʂ̍����擾
//=============================================================================
float CSceneMeshFiled::GetHight(D3DXVECTOR3 pos)
{
	//�f�o�b�N�\���̎擾
	CDebugProc * pDebugLeft = CRenderer::GetDebugLeft();

	//�ϐ��錾
	D3DXVECTOR3 VecA = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 VecB = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 VecC = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Vec0 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Vec1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float		fTouch = 0.0f;
	float		fHight = 0.0f;		//������ۑ�����

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�ǂ̃u���b�N�ɏ���Ă��邩�̔���
	int nBlockX = (int)((pos.x + (m_nWidth * SIZE_POLYGON / 2)) / ((m_nWidth * SIZE_POLYGON) / m_nWidth));
	int nBlockZ = (int)((pos.z - (m_nWidth * SIZE_POLYGON / 2)) / ((m_nHight * SIZE_POLYGON) / m_nHight) * -1);

	//�e���_�ԍ������߂�
	int nLU = nBlockX + nBlockZ * (m_nWidth + 1);
	int nRU = nLU + 1;
	int nLD = nLU + (m_nWidth + 1);
	int nRD = nLD + 1;

	//�Ίp���̃x�N�g�������߂�
	VecA = pVtx[nLU].pos - pVtx[nRD].pos;

	//���_�Ǝw��ʒu�̃x�N�g�������߂�
	VecB = pos - pVtx[nLU].pos;

	//�O�ς����߂�
	fTouch = (VecA.z * VecB.x) - (VecA.x * VecB.z);

	if (fTouch <= 0)
	{
		//VecA�����߂�
		VecA = pos - pVtx[nLD].pos;	//���_�ƃv���C���[���W�̍������狁�߂�

		//������ۑ�����
		fHight = pVtx[nLD].pos.y;

		//�@���̐ݒ�
		Vec0 = pVtx[nLD].pos - pVtx[nLU].pos;	//Vec1�����߂�
		Vec1 = pVtx[nLD].pos - pVtx[nRD].pos;	//Vec0�����߂�
	}
	else
	{
		//VecA�����߂�
		VecA = pos - pVtx[nRU].pos;	//���_�ƃv���C���[���W�̍������狁�߂�

		//������ۑ�����
		fHight = pVtx[nRU].pos.y;

		//�@���̐ݒ�
		Vec0 = pVtx[nRU].pos - pVtx[nLU].pos;	//Vec1�����߂�
		Vec1 = pVtx[nRU].pos - pVtx[nRD].pos;	//Vec0�����߂�
	}

	D3DXVec3Cross(&nor, &Vec0, &Vec1);	//�@���̐ݒ�

	 //���K������
	D3DXVec3Normalize(&nor, &nor);

	//���ς����߂�
	VecC.y = ((VecA.x * nor.x) + (VecA.z * nor.z)) / -nor.y;

	//���������Ƃɖ߂�
	VecC.y += fHight;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return VecC.y;
}

#ifdef _DEBUG
//=============================================================================
// ���_�̍X�V����
//=============================================================================
void CSceneMeshFiled::UpdatePrimitiv(void)
{
	//�ҏW��Ԃ��擾����
	if (CRenderer::GetDrawDebugRight() == true)
	{
		CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

		//���_�̑I������
		if (pInputKeyboard->GetKeyboardTrigger(DIK_T) == true)
		{
			m_nSelectPrimitiv = (m_nSelectPrimitiv + (m_nPrimitiv - 1)) % m_nPrimitiv;
		}
		else if(pInputKeyboard->GetKeyboardTrigger(DIK_G) == true)
		{
			m_nSelectPrimitiv = (m_nSelectPrimitiv + 1) % m_nPrimitiv;
		}
		else if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) == true)
		{//���_����ۑ�����
			SaveData();
			SetNor();
			m_bUpdatePrimitiv = false;	//�ŐV��Ԃɂ���
		}

		//�����̍X�V����
		SetPosY(m_nSelectPrimitiv);
	}
}

//=============================================================================
// ���_�̍����ύX
//=============================================================================
void CSceneMeshFiled::SetPosY(int nVertex)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pInputKeyboard->GetKeyboardPress(DIK_Y) == true)
	{
		pVtx[nVertex].pos.y += 1.0f;
		m_bUpdatePrimitiv = true;	//�X�V�K�v��Ԃɂ���
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_H) == true)
	{
		pVtx[nVertex].pos.y -= 1.0f;
		m_bUpdatePrimitiv = true;	//�X�V�K�v��Ԃɂ���
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}
#endif

//=============================================================================
// �@�����Đݒ肷��
//=============================================================================
void CSceneMeshFiled::SetNor(void)
{
	//�ϐ��錾
	D3DXVECTOR3 Vec0[2] = {};	//�x�N�g��0
	D3DXVECTOR3 Vec1[2] = {};	//�x�N�g��1
	D3DXVECTOR3 nor[8000] = {};	//�|���S���̖@���x�N�g��
	int nCntNor = 0;			//�@���̔z���i�߂�

	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHight = 0; nCntHight < m_nHight; nCntHight++)
	{//�c�̒��_
		for (int nCntWidth = 0; nCntWidth < m_nWidth; nCntWidth++)
		{//���̒��_

			//�@���̐ݒ�
			Vec0[0] = pVtx[0].pos - pVtx[0 + m_nWidth + 1].pos;		//Vec1�����߂�
			Vec1[0] = pVtx[0].pos - pVtx[0 + m_nWidth + 2].pos;		//Vec0�����߂�
			Vec0[1] = pVtx[0].pos - pVtx[0 + m_nWidth + 2].pos;		
			Vec1[1] = pVtx[0].pos - pVtx[1].pos;					

			D3DXVec3Cross(&nor[nCntNor], &Vec0[0], &Vec1[0]);		//�@���̐ݒ�
			D3DXVec3Cross(&nor[nCntNor + 1], &Vec0[1], &Vec1[1]);	

			//���K������
			D3DXVec3Normalize(&nor[nCntNor], &nor[nCntNor]);
			D3DXVec3Normalize(&nor[nCntNor + 1], &nor[nCntNor + 1]);

			if (nCntWidth != m_nWidth - 1)
			{
				pVtx++;
			}
			else if (nCntWidth == m_nWidth - 1)
			{
				pVtx += 2;
			}

			nCntNor += 2;	//�v�Z�p�ϐ��̔z���i�߂�
		}
	}

	nCntNor = 0;	//�J�E���^�[������������

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHight = 0; nCntHight <= m_nHight; nCntHight++)
	{//�c�̒��_
		for (int nCntWidth = 0; nCntWidth <= m_nWidth; nCntWidth++)
		{//���̒��_
			if (nCntHight == 0)
			{//�ŏ��̒i
				if (nCntWidth == 0)
				{//���[
					pVtx[0].nor = (nor[nCntNor] + nor[nCntNor + 1]) * 0.5f;
				}
				else if (nCntWidth == m_nWidth)
				{//�E�[
					pVtx[0].nor = nor[nCntNor + 1];
				}
				else
				{//�^��
					pVtx[0].nor = (nor[nCntNor - 1] + nor[nCntNor] + nor[nCntNor + 1]) / 3.0f;
				}
			}
			else if (nCntHight == m_nHight)
			{//�Ō�̒i
				if (nCntWidth == 0)
				{//���[
					pVtx[0].nor = nor[nCntNor];
				}
				else if (nCntWidth == m_nWidth)
				{//�E�[
					pVtx[0].nor = (nor[nCntNor + 1] + nor[nCntNor]) * 0.5f;
				}
				else
				{//�^��
					pVtx[0].nor = (nor[nCntNor - 2] + nor[nCntNor - 1] + nor[nCntNor]) / 3.0f;
				}
			}
			else
			{//�^�񒆂̒i
				if (nCntWidth == 0)
				{//���[
					pVtx[0].nor = (nor[nCntNor + 1] + nor[nCntNor - m_nWidth * 2] + nor[nCntNor]) / 3.0f;
				}
				else if (nCntWidth == m_nWidth)
				{//�E�[
					pVtx[0].nor = (nor[nCntNor + 1] + nor[nCntNor - (m_nWidth * 2)] + nor[nCntNor - (m_nWidth * 2 + 1)]) / 3.0f;
				}
				else
				{//�^��
					pVtx[0].nor = (nor[nCntNor - 1] + nor[nCntNor] + nor[nCntNor + 1] + nor[nCntNor - (m_nWidth * 2 - 2)] + nor[nCntNor - (m_nWidth * 2 - 1)] + nor[nCntNor - m_nWidth * 2]) / 6.0f;
				}
			}

			//�z��̃J�E���^�[��i�߂�
			if (nCntWidth != m_nWidth - 1)
			{
				nCntNor += 2;
			}

			if (nCntWidth == m_nWidth && nCntHight == m_nHight - 1)
			{
				nCntNor -= 2 + (m_nWidth * 2 - 2);
			}

			//�|�C���^���Y���̈ʒu�܂Ői�߂�
			pVtx++;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

#ifdef _DEBUG
//=============================================================================
// ���_�̍�����ݒ肷��
//=============================================================================
void CSceneMeshFiled::SetHight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	VERTEX_3D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�����a�͈͓̔��ɂ��邩�ǂ���
	for (int nCntPrimitiv = 0; nCntPrimitiv < m_nPrimitiv; nCntPrimitiv++)
	{
		//�v���C���[�̋����ƑΏۂ̒��_�܂ł̋���
		float fPosX = pos.x - pVtx[0].pos.x;
		float fPosZ = pos.z - pVtx[0].pos.z;

		float fLength = sqrtf((fPosX * fPosX) + (fPosZ * fPosZ));

		//���a�͈͓̔��ɂ��邩�ǂ���
		if (fLength < fRange)
		{
			float fAngle = (fLength / fRange);
			float fHight = cosf(D3DX_PI * -0.5f * fAngle) * fValue;	//���������߂�

			pVtx[0].pos.y += fHight;
		} 

		//�Y���̈ʒu�܂Ői�߂�
		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�@���̐ݒ�
	SetNor();
}
#endif