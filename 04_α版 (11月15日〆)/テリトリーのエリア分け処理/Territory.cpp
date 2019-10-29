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
#include "collision.h"
#include <stdio.h>
#include <stdlib.h>
#include "library.h"
#include "game.h"
#include "Player.h"
#include "loadEffect.h"
#include "camera.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SaveTextName ("data/TEXT/SaveTerritory.txt")		//�ۑ�����e�L�X�g��
#define UPDATE_LENGTH	(80.0f)								//�X�V��Ԃ�L���ɂ���͈�
#define GET_LENGTH		(40.0f)								//�擾�͈�		
#define MIN_TERRITORY	(3)									//�擾�e���g���[�̍Œᐔ
#define FIELD_SIZE		(D3DXVECTOR2(4000.0f, 4000.0f))		//�t�B�[���h�̃T�C�Y

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
D3DXCOLOR CTerritory::m_DefaultColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
D3DXVECTOR2 CTerritory::m_pEreaPos[MAX_EREAPOS] = {};

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
	pTerritory->SetvtxMax(m_pVtxMax[ModelNum]);		//�ő�l
	pTerritory->SetvtxMin(m_pVtxMin[ModelNum]);		//�ŏ��l
	pTerritory->SetScale(scale);					//�傫���̐ݒ�
	pTerritory->SetTexNum(nTexNum);					//�e�N�X�`���ԍ�
	pTerritory->SetModelType(ModelNum);				//���f���ԍ�
	pTerritory->SetPoint(nPoint);					//�|�C���g�̕ۑ�
	//pTerritory->CreateCollider();

	//���f���E�e�N�X�`���̊��蓖��
	pTerritory->BindModel(m_pMesh[ModelNum], m_pBuffMat[ModelNum], m_pNumMat[ModelNum], m_ppTexture[nTexNum]);

	return pTerritory;
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
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_OldColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	m_nNumPlayer = -1;								//�v���C���[�ԍ�
	m_nOldNumPlayer = -1;							//�O��̃v���C���[�ԍ�
	m_fLength = 0.0f;								//����
	m_bGetTerritory = false;						//�e���g���[���擾�������ǂ���
	m_nErea = 0;									//�G���A�ԍ�
	
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(0, m_pos, 8);
		//m_pLoadEffect->Init();
	}

	CSceneX::Init();


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTerritory::Uninit(void)
{
	CSceneX::Uninit();

	if (m_pLoadEffect != NULL)
	{
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CTerritory::Update(void)
{
	CSceneX::Update();

	for (int nCntPlayer = 0; nCntPlayer < MAX_NUMPLAYER; nCntPlayer++)
	{
		CPlayer * pPlayer = CManager::GetGame()->GetPlayer(nCntPlayer);
		D3DXVECTOR3 PlayerPos = INITIALIZE_VECTOR3;
		if (pPlayer != NULL) { PlayerPos = pPlayer->GetPos(); }

		//�v���C���[�Ƃ̋��������߂�
		m_fLength = CFunctionLib::Vec2Length(m_pos.x, m_pos.z, PlayerPos.x, PlayerPos.z);

		//�X�V�͈͂ɂ��邩�ǂ���
		if (m_fLength < GET_LENGTH)
		{
			pPlayer->AddTerritoryList(this);	//�v���C���[�Ɏ��g�̏���n��
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CTerritory::CreateGetEffect(void)
{
	if (m_pLoadEffect != NULL)
	{
		m_pLoadEffect->SetPos(D3DXVECTOR3(m_pos.x, 50.0f, m_pos.z));
		m_pLoadEffect->OnTimeEffect();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTerritory::Draw(void)
{
	SetDiffuseColor(m_Color);
	CSceneX::Draw();
	SetDiffuseColor(m_DefaultColor);

	if (m_pLoadEffect != NULL)
	{
		m_pLoadEffect->Draw();
	}
}

//=============================================================================
//�@�擾��Ԃ̍X�V����
//=============================================================================
void CTerritory::SetDiffuseColor(D3DXCOLOR Color)
{
	//�}�e���A�����̎擾
	LPD3DXBUFFER ModelMat = CSceneX::GetBuffMat();
	DWORD nNumMat = CSceneX::GetNumMat();

	//�}�e���A���|�C���^�̎擾
	D3DXMATERIAL * pMat = (D3DXMATERIAL*)ModelMat->GetBufferPointer();

	//�}�e���A���J���[�̕ύX
	for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
	{
		pMat[nCntMat].MatD3D.Diffuse = Color;
	}
}

//=============================================================================
//�@�v���C���[�ԍ��̕ۑ�
//=============================================================================
void CTerritory::SetPlayerNumber(int nNumPlayer)
{
	m_nOldNumPlayer = m_nNumPlayer;	//�O��̏���ۑ�
	m_nNumPlayer = nNumPlayer;
}

//=============================================================================
// �G���A�̔ԍ��U�荇����
//=============================================================================
void CTerritory::SetErea(void)
{
	//���_�����m�ۂ���
	int nMaxVertex = (HIGHT_DIVISION + 1) * (WIDTH_DIVISION + 1);

	for (int nCnt = 0; nCnt < nMaxVertex; nCnt++)
	{
		m_pEreaPos[nCnt] = D3DXVECTOR2(0.0f, 0.0f);
	}

	//�G���A�̃T�C�Y�����߂�
	D3DXVECTOR2 EreaSize = D3DXVECTOR2(0.0f, 0.0f);
	EreaSize.x = FIELD_SIZE.x / WIDTH_DIVISION;
	EreaSize.y = FIELD_SIZE.y / HIGHT_DIVISION;

	//�G���A�̎n�_�����߂�
	D3DXVECTOR2 FirstPos = D3DXVECTOR2(0.0f, 0.0f);
	FirstPos.x = (FIELD_SIZE.x * 0.5f) * -1;
	FirstPos.y = (FIELD_SIZE.y * 0.5f) * -1;

	//�e���_���W�ɃG���A���W��ݒ肷��
	int nCountPos = 0;					//���W�̔ԍ�

	for (int nCntHight = 0; nCntHight < HIGHT_DIVISION + 1; nCntHight++)
	{
		for (int nCntWidth = 0; nCntWidth < WIDTH_DIVISION + 1; nCntWidth++)
		{
			m_pEreaPos[nCountPos].x = FirstPos.x + (EreaSize.x * nCntWidth);
			m_pEreaPos[nCountPos].y = FirstPos.y + (EreaSize.y * nCntHight);
			nCountPos++;
		}
	}

	//�e���g���[�ɃG���A�ԍ��̐U�蕪��
	//�G�̃I�u�W�F�N�g�|�C���^�̎擾
	CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);	 //�I�u�W�F�N�g�|�C���^�̐擪���擾			
	CScene * pScene = pSceneTop;							 //�g�b�v�̈ʒu��ۑ�����

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();					//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			CTerritory * pTerritory = (CTerritory*)pScene;		//�G�N���X�ɃL���X�g����
			D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();	//�e���g���[�̈ʒu���擾����
			int nCountErea = 0;

			for (int nCntHight = 0; nCntHight < HIGHT_DIVISION; nCntHight++)
			{
				for (int nCntWidth = 0; nCntWidth < WIDTH_DIVISION; nCntWidth++)
				{
					int nNumPos = nCntWidth + (nCntHight * (WIDTH_DIVISION + 1));

					if ((TerritoryPos.x > m_pEreaPos[nNumPos].x) &&
						(TerritoryPos.x < m_pEreaPos[nNumPos + 1].x) &&
						(TerritoryPos.z > m_pEreaPos[nNumPos].y) &&
						(TerritoryPos.z < m_pEreaPos[nNumPos + WIDTH_DIVISION + 1].y))
					{
						pTerritory->m_nErea = nCountErea;	//�G���A�ԍ��̊��蓖��
					}

					nCountErea++;
				}
			}
		}

		pScene = pSceneNext;		//�|�C���^��i�߂�
	}
}

//=============================================================================
// �ǂ̃G���A�ɋ���̂����肷��
//=============================================================================
int CTerritory::RequestErea(D3DXVECTOR3 Pos)
{
	//�ϐ��錾
	int nErea = 0;
	int nCountErea = 0;

	for (int nCntHight = 0; nCntHight < HIGHT_DIVISION; nCntHight++)
	{
		for (int nCntWidth = 0; nCntWidth < WIDTH_DIVISION; nCntWidth++)
		{
			int nNumPos = nCntWidth + (nCntHight * (WIDTH_DIVISION + 1));

			if ((Pos.x > m_pEreaPos[nNumPos].x) &&
				(Pos.x < m_pEreaPos[nNumPos + 1].x) &&
				(Pos.z > m_pEreaPos[nNumPos].y) &&
				(Pos.z < m_pEreaPos[nNumPos + WIDTH_DIVISION + 1].y))
			{
				nErea = nCountErea;	//�G���A�ԍ��̊��蓖��
				break;
			}

			nCountErea++;
		}
	}

	return nErea;
}

//=============================================================================
// �����蔻��̐���
//=============================================================================
void CTerritory::CreateCollider(void)
{
	// �R���C�_�[�̐���
	CColliderManager *pColManager = CColliderManager::Create(1);
	if (pColManager == NULL) { return; }
	CBoxCollider *pBox = CBoxCollider::Create(m_pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 27.0f, 68.0f, 20.0f, true);
	pBox->SetParent(this);
	//pBox->SetParentMtxWorld(&GetMtx());
	pColManager->SetCollider(pBox, 0);
	SetCOlliderManager(pColManager);
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
//�@�I�u�W�F�N�g����
//=============================================================================
CTerritory * CTerritory::RequestPointer(int nCountObj)
{
	//�G�̃I�u�W�F�N�g�|�C���^�̎擾
	CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);	 //�I�u�W�F�N�g�|�C���^�̐擪���擾			
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
		CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);	 //�I�u�W�F�N�g�|�C���^�̐擪���擾			
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