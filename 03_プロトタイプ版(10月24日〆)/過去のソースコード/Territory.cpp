//=============================================================================
//
// �e���g���[���� [territory.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "Territory.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define SaveTextName ("data/TEXT/SaveTerritory.txt")		//�ۑ�����e�L�X�g��
#define OBJ_PRIORITY (3)

//=============================================================================
// �O���錾
//=============================================================================
LPDIRECT3DTEXTURE9 * CTerritory::m_ppTexture = NULL;
LPD3DXMESH * CTerritory::m_pMesh = NULL;
LPD3DXBUFFER * CTerritory::m_pBuffMat = NULL;
DWORD *	CTerritory::m_pNumMat = NULL;
D3DXVECTOR3 * CTerritory::m_pVtxMax = NULL;
D3DXVECTOR3 * CTerritory::m_pVtxMin = NULL;
int CTerritory::m_nMaxTex = 0;
int CTerritory::m_nMaxModel = 0;
int CTerritory::m_nCountObj = 0;
int CTerritory::m_nMaxObj = 0;

//=============================================================================
// ��������
//=============================================================================
CTerritory * CTerritory::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexNum, int ModelNum, int nPoint)
{
	//�C���X�^���X�̐���
	CTerritory * pTerritory = new CTerritory;

	//����������
	pTerritory->Init();
	pTerritory->SetPos(pos);						//�ʒu�̐ݒ�
	pTerritory->SetRot(rot);						//�����̐ݒ�
	pTerritory->SetvtxMax(m_pVtxMax[ModelNum]);	//�ő�l
	pTerritory->SetvtxMin(m_pVtxMin[ModelNum]);	//�ŏ��l
	pTerritory->SetScale(scale);					//�傫���̐ݒ�
	pTerritory->SetTexNum(nTexNum);				//�e�N�X�`���ԍ�
	pTerritory->SetModelType(ModelNum);			//���f���ԍ�
	pTerritory->SetPoint(nPoint);					//�|�C���g�̕ۑ�

	//���f���E�e�N�X�`���̊��蓖��
	pTerritory->BindModel(m_pMesh[ModelNum], m_pBuffMat[ModelNum], m_pNumMat[ModelNum], m_ppTexture[nTexNum]);

	return pTerritory;
}

//=============================================================================
// ���f���̔j��
//=============================================================================
void CTerritory::UnloadModel(void)
{
	//���b�V���̔j��
	if (m_pMesh != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}

		delete[] m_pMesh;
		m_pMesh = NULL;
	}

	//�o�b�t�@�̔j��
	if (m_pBuffMat != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}

		delete[] m_pBuffMat;
		m_pBuffMat = NULL;
	}

	//�}�e���A���̔j��
	if (m_pNumMat != NULL)
	{
		delete m_pNumMat;
		m_pNumMat = NULL;
	}

	//�ő�l�̔j��
	if (m_pVtxMax != NULL)
	{
		delete m_pVtxMax;
		m_pVtxMax = NULL;
	}

	//�ŏ��l�̔j��
	if (m_pVtxMin != NULL)
	{
		delete m_pVtxMin;
		m_pVtxMin = NULL;
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTerritory::CTerritory() : CSceneX(3, OBJTYPE_TERRITORY)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTerritory::CTerritory(int nPriority, OBJTYPE objtype) : CSceneX(nPriority, objtype)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTerritory::~CTerritory(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTerritory::Init(void)
{
	//�ϐ��̏�����
	m_nTex = 0;
	m_nModelType = 0;
	m_nPoint = 1;

	CSceneX::Init();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTerritory::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTerritory::Update(void)
{
	CSceneX::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTerritory::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
void CTerritory::LoadTex(CLoadTextTerritory::TEX_INFO TexInfo)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̍ő吔
	m_nMaxTex = TexInfo.nMaxTex;

	//�e�N�X�`���������𓮓I�m��
	if (m_ppTexture == NULL)
	{
		m_ppTexture = new LPDIRECT3DTEXTURE9[m_nMaxTex];
	}

	//�e�N�X�`���𐶐�����
	for (int nCntTex = 0; nCntTex < m_nMaxTex; nCntTex++)
	{
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, &TexInfo.ppFileName[nCntTex][0], &m_ppTexture[nCntTex]);
	}
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CTerritory::UnloadTex(void)
{
	if (m_ppTexture != NULL)
	{
		for (int nCntTex = 0; nCntTex < m_nMaxTex; nCntTex++)
		{
			m_ppTexture[nCntTex]->Release();
			m_ppTexture[nCntTex] = NULL;
		}

		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}
}

//=============================================================================
//�@���f���̓ǂݍ���
//=============================================================================
void CTerritory::LoadModel(CLoadTextTerritory::OBJ_INFO ObjInfo)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���f���̍ő吔
	m_nMaxModel = ObjInfo.nMaxModel;

	//���f���̓��I�m��
	if ((m_pMesh == NULL) && (m_pBuffMat == NULL) && (m_pNumMat == NULL))
	{
		m_pMesh = new LPD3DXMESH[m_nMaxModel];
		m_pBuffMat = new LPD3DXBUFFER[m_nMaxModel];
		m_pNumMat = new DWORD[m_nMaxModel];
		m_pVtxMax = new D3DXVECTOR3[m_nMaxModel];
		m_pVtxMin = new D3DXVECTOR3[m_nMaxModel];
	}

	//���f���̐���
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&ObjInfo.ppModelFileName[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pBuffMat[nCntModel],
			NULL,
			&m_pNumMat[nCntModel],
			&m_pMesh[nCntModel]);

		//�ϐ��錾
		int		nNumVtx = 0;	//���_��
		DWORD	sizeFVF = 0;	//���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *	pVtxBuff = 0;	//���_�o�b�t�@�ւ̃|�C���^

								//���_�����擾
		nNumVtx = m_pMesh[nCntModel]->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nCntModel]->GetFVF());

		m_pVtxMax[nCntModel] = D3DXVECTOR3(-100000.0f, 0.0f, -100000.0f);	//���_�̍ő�l
		m_pVtxMin[nCntModel] = D3DXVECTOR3(100000.0f, 0.0f, 100000.0f);		//���_�̍ŏ��l

																			//���_�o�b�t�@�����b�N
		m_pMesh[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

														//�ő�l���r
			if (m_pVtxMax[nCntModel].x < vtx.x)
			{
				m_pVtxMax[nCntModel].x = vtx.x;
			}
			if (m_pVtxMax[nCntModel].y < vtx.y)
			{
				m_pVtxMax[nCntModel].y = vtx.y;
			}
			if (m_pVtxMax[nCntModel].z < vtx.z)
			{
				m_pVtxMax[nCntModel].z = vtx.z;
			}

			//�ŏ��l���r
			if (m_pVtxMin[nCntModel].x > vtx.x)
			{
				m_pVtxMin[nCntModel].x = vtx.x;
			}
			if (m_pVtxMin[nCntModel].y > vtx.y)
			{
				m_pVtxMin[nCntModel].y = vtx.y;
			}
			if (m_pVtxMin[nCntModel].z > vtx.z)
			{
				m_pVtxMin[nCntModel].z = vtx.z;
			}
			//�T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		m_pMesh[nCntModel]->UnlockVertexBuffer();
	}
}

//=============================================================================
//�@�I�u�W�F�N�g����
//=============================================================================
CTerritory * CTerritory::RequestPointer(int nCountObj)
{
	//�G�̃I�u�W�F�N�g�|�C���^�̎擾
	CScene * pSceneTop = CScene::GetTop(OBJ_PRIORITY);	 //�I�u�W�F�N�g�|�C���^�̐擪���擾			
	CScene * pScene = pSceneTop;						 //�g�b�v�̈ʒu��ۑ�����
	CTerritory * m_pTerritory = NULL;
	int nNumObject = 0;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();					//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			if (nNumObject == nCountObj)
			{//�ԍ�����v������|�C���^��ۑ�
				m_pTerritory = (CTerritory*)pScene;
				break;
			}

			nNumObject++;	//�ԍ��̉��Z
		}

		pScene = pSceneNext;		//�|�C���^��i�߂�
	}

	return m_pTerritory;
}

//=============================================================================
// �e�L�X�g�ۑ�����
//=============================================================================
void CTerritory::Save(HWND hWnd)
{
	//�ϐ��錾
	FILE * pFile = NULL;

	//�e�N�X�g�̍쐬
	pFile = fopen(SaveTextName, "w");

	//�����o������
	if (pFile != NULL)
	{
		//�G�̃I�u�W�F�N�g�|�C���^�̎擾
		CScene * pSceneTop = CScene::GetTop(OBJ_PRIORITY);	 //�I�u�W�F�N�g�|�C���^�̐擪���擾			
		CScene * pScene = pSceneTop;						 //�g�b�v�̈ʒu��ۑ�����

		while (pScene != NULL)
		{
			CScene * pSceneNext = pScene->GetpNext();					//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

			if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
			{
				if (pScene->GetDeth() == false)
				{
					CTerritory * pObject = (CTerritory*)pScene;						//�G�N���X�ɃL���X�g����
					int ModelType = pObject->GetModelType();
					int nTexNum = pObject->GetTex();
					D3DXVECTOR3 Pos = pObject->GetPos();
					D3DXVECTOR3 Rot = pObject->GetRot();
					D3DXVECTOR3 Scale = pObject->GetScale();
					int nPoint = pObject->GetPoint();

					fprintf(pFile, "SET\n");
					fprintf(pFile, "\tMODELTYPE = %d\n", ModelType);							//���f���̃^�C�v
					fprintf(pFile, "\tTEX = %d\n", nTexNum);									//�e�N�X�`���ԍ�
					fprintf(pFile, "\tPOS = %.1f %.1f %.1f\n", Pos.x, Pos.y, Pos.z);			//�ʒu
					fprintf(pFile, "\tROT = %.1f %.1f %.1f\n", Rot.x, Rot.y, Rot.z);			//����
					fprintf(pFile, "\tSCALE = %.1f %.1f %.1f\n", Scale.x, Scale.y, Scale.z);	//�X�P�[��
					fprintf(pFile, "\tPOINT = %d\n", nPoint);									//�|�C���g
					fprintf(pFile, "END_SET\n");
					fprintf(pFile, "\n");
				}
			}

			pScene = pSceneNext;		//�|�C���^��i�߂�
		}

		//�������݂��I������
		fclose(pFile);

		//���b�Z�[�W�{�b�N�X�̕\��
		MessageBox(hWnd, "�Z�[�u���������܂���", "���������ʒm", MB_OK);
	}
}