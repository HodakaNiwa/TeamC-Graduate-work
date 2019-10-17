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

//=============================================================================
// �}�N����`
//=============================================================================
#define SaveTextName ("data/TEXT/SaveTerritory.txt")		//�ۑ�����e�L�X�g��
#define OBJ_PRIORITY (3)
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//��
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//��
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//�I�����W
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//��
#define UPDATE_LENGTH	(80.0f)								//�X�V��Ԃ�L���ɂ���͈�
#define GET_LENGTH		(40.0f)								//�擾�͈�		
#define MIN_TERRITORY	(3)									//�擾�e���g���[�̍Œᐔ

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

//���̐F�̐ݒ�
D3DXCOLOR CTerritory::m_CountryColor[CPlayer::TYPE_MAX] = 
{
	COLOR_WHITE,	//���{
	COLOR_BULE,		//���V�A
	COLOR_RED,		//�A�����J
	COLOR_YELLOW,	//�u���W��
	COLOR_ORENGE,	//�C�M���X
	COLOR_PAPULE,	//�C�^���A
	COLOR_BRACK,	//�j���[�W�[�����h
	COLOR_GREEN,	//��A�t���J
};

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
	pTerritory->CreateCollider();

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
	m_State = STATE_NORMAL;							//���
	m_nNumPlayer = -1;								//�v���C���[�ԍ�
	m_nOldNumPlayer = -1;							//�O��̃v���C���[�ԍ�
	m_fLength = 0.0f;								//����
	m_bGetTerritory = false;						//�e���g���[���擾�������ǂ���
	m_pNext = NULL;									//���X�g�̎��̃|�C���^
	m_pPrev = NULL;									//���X�g�̑O�̃|�C���^

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

	for (int nCntPlayer = 0; nCntPlayer < MAX_NUMPLAYER; nCntPlayer++)
	{
		CPlayer * pPlayer = CManager::GetGame()->GetPlayer(nCntPlayer);
		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

		//�v���C���[�Ƃ̋��������߂�
		m_fLength = CFunctionLib::Vec2Length(m_pos.x, m_pos.z, PlayerPos.x, PlayerPos.z);

		//�X�V�͈͂ɂ��邩�ǂ���
		if (m_fLength < UPDATE_LENGTH)
		{
			//��ԕʍX�V����
			switch (m_State)
			{
			case STATE_NORMAL: UpdateNormal(pPlayer, nCntPlayer); break;	//�ʏ���
			case STATE_START:  UpdateStart(pPlayer, nCntPlayer);  break;	//�n�_�̏��
			case STATE_PASS:   UpdatePass(pPlayer, nCntPlayer);   break;	//�ʉߓ_�̏��
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTerritory::Draw(void)
{
	SetColor(m_Color);
	CSceneX::Draw();
	SetColor(m_DefaultColor);
}

//=============================================================================
//�@�ʏ��Ԃ̍X�V����
//=============================================================================
void CTerritory::UpdateNormal(CPlayer * pPlayer, int nCntPlayer)
{
	//�擾�͈͓��ɂ���̂��ǂ���
	if (m_fLength < GET_LENGTH)
	{
		if (m_bGetTerritory == true) { return; }	//�e���g���[���擾��ԂȂ�X�V���Ȃ�

		//�v���C���[�ԍ��̕ۑ�
		m_nOldNumPlayer = m_nNumPlayer;
		m_nNumPlayer = nCntPlayer;

		//�F�̕ۑ�
		m_OldColor = m_Color;	
		m_Color = m_CountryColor[(int)pPlayer->GetType()];	

		//�v���C���[���n�_��ێ����Ă�����
		if (pPlayer->GetTop() != NULL)
		{//�ʉߓ_��Ԃɂ���
			m_State = STATE_PASS;

			//���X�g�ɒǉ��ɂ���i�Ō���j
			CTerritory * pCur = pPlayer->GetCur();	//���݂̃|�C���^�擾
			pCur->m_pNext = this;					//���݂̎��̃|�C���^���

			//�O��̃e���g���[����ۑ�
			m_pPrev = pCur;

			//�Ō���ɍ���̏���ǉ�����
			pPlayer->SetCur(this);
		}
		else
		{//�n�_��Ԃɂ���
			m_State = STATE_START;

			//���X�g�ɒǉ�
			pPlayer->SetTop(this);	//�擪
			pPlayer->SetCur(this);	//����
		}
	}
	else
	{//�e���g���[���擾���Ă��Ȃ���Ԃɂ���
		m_bGetTerritory = false;
	}
}

//=============================================================================
// �n�_�̍X�V����
//=============================================================================
void CTerritory::UpdateStart(CPlayer * pPlayer, int nCntPlayer)
{
	//�擾�͈͓��ɂ���̂��ǂ���
	if (m_fLength < GET_LENGTH)
	{
		CTerritory * pTop;	//�擪�|�C���^�ۑ��p

		if (nCntPlayer == m_nNumPlayer)
		{//�e���g���[�̊���
			 //�m�[�h���̃��X�g�̃|�C���g�����ׂăv���C���[�ɉ��Z����
			CTerritory * pTerritory = pPlayer->GetTop();	//���X�g�̐擪���擾
			pTop = pTerritory;
			int nCountTerrotpry = 0;

			//���X�g���Ƀe���g���[��3�ȏ゠�邩�ǂ���
			if (false == ChackList(pTop, &nCountTerrotpry)) { return; }

			//�ʒu�ۑ��p�ϐ��̃��������m��
			D3DXVECTOR3 * pPos = new D3DXVECTOR3[nCountTerrotpry];

			int nCnt = 0;
			m_bGetTerritory = true;									//�e���g���[���擾������Ԃɂ���

			//�e���g���[�̎擾����
			while (pTerritory != NULL)
			{
				CTerritory * pTerritoryNext = pTerritory->m_pNext;		//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

				//�ʒu��ۑ�
				pPos[nCnt] = pTerritory->m_pos;

				//�|�C���g�����Z����
				if (pTerritory->m_nNumPlayer != pTerritory->m_nOldNumPlayer)	//�O��Ɠ����e���g���[���������|�C���g�͉��Z����Ȃ�
				{
					//�O��̃v���C���[�̃|�C���g�����炷
					if (m_nOldNumPlayer != -1)
					{
						CPlayer * pPlayer = CManager::GetGame()->GetPlayer(m_nOldNumPlayer);
					}
				}

				//�ʏ��Ԃɂ���
				pTerritory->m_State = STATE_NORMAL;
				pTerritory = pTerritoryNext;		//�|�C���^��i�߂�
				nCnt++;
			}

			//�͈͓��Ƀe���g���[�����邩�ǂ���
			ChackIn(pPos, nCountTerrotpry, m_nNumPlayer);

			//�v���C���[�ŕۑ����Ă��郊�X�g��������
			pPlayer->SetTop(NULL);
			pPlayer->SetCur(NULL);

			//�������̊J��
			if (pPos != NULL)
			{
				delete[] pPos;
				pPos = NULL;
			}
		}
		else
		{//�e���g���[�̔j��
			pTop = DestoroyTerritoryStart();
		}

		//�m�[�h��NULL�ɂ���
		ResetTerritory(pTop);
	}
}

//=============================================================================
// �ʉߓ_�̍X�V����
//=============================================================================
void CTerritory::UpdatePass(CPlayer * pPlayer, int nCntPlayer)
{
	//�擾�͈͓��ɂ���̂��ǂ���
	if (m_fLength < GET_LENGTH)
	{
		if (nCntPlayer != m_nNumPlayer)
		{//�e���g���[�̔j��
			CTerritory * pTop = DestoroyTerritory();

			//�m�[�h��NULL�ɂ���
			ResetTerritory(pTop);
		}
	}
}

//=============================================================================
// �e���g���[�̔j��(�X�^�[�g�n�_�p)
//=============================================================================
CTerritory * CTerritory::DestoroyTerritoryStart(void)
{
	CPlayer * pPlayer = CManager::GetGame()->GetPlayer(m_nNumPlayer);
	CTerritory * pTop = pPlayer->GetTop();
	CTerritory * pTerritory = pTop;

	//�v���C���[�ŕۑ����Ă��郊�X�g��������
	pPlayer->SetTop(NULL);
	pPlayer->SetCur(NULL);

	//�f�[�^�����������Ă���
	while (pTerritory != NULL)
	{
		CTerritory * pTerritoryNext = pTerritory->m_pNext;		//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		pTerritory->m_nNumPlayer = pTerritory->m_nOldNumPlayer;	//���ׂĂ̗̒n��O��̃v���C���[�ɖ߂�
		pTerritory->m_Color = pTerritory->m_OldColor;			//�O��̐F�ɖ߂�
		pTerritory->m_State = STATE_NORMAL;						//�ʏ��Ԃɂ���

		pTerritory = pTerritoryNext;		//�|�C���^��i�߂�
	}

	return pTop;
}

//=============================================================================
// �e���g���[�̔j��
//=============================================================================
CTerritory * CTerritory::DestoroyTerritory(void)
{
	CPlayer * pPlayer = CManager::GetGame()->GetPlayer(m_nNumPlayer);
	CTerritory * pTop = pPlayer->GetTop();
	CTerritory * pTerritory = pTop;
	int nCount = 0;

	//�v���C���[�ŕۑ����Ă��郊�X�g��������
	pPlayer->SetCur(pTop);

	//�f�[�^�����������Ă���
	while (pTerritory != NULL)
	{
		CTerritory * pTerritoryNext = pTerritory->m_pNext;		//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		if (nCount != 0)
		{
			pTerritory->m_nNumPlayer = pTerritory->m_nOldNumPlayer;	//���ׂĂ̗̒n��O��̃v���C���[�ɖ߂�
			pTerritory->m_Color = pTerritory->m_OldColor;				//�O��̐F�ɖ߂�
			pTerritory->m_State = STATE_NORMAL;						//�ʏ��Ԃɂ���
		}

		pTerritory = pTerritoryNext;		//�|�C���^��i�߂�
		nCount++;
	}

	return pTop;
}

//=============================================================================
// �e���g���[�̃m�[�h��������
//=============================================================================
void CTerritory::ResetTerritory(CTerritory * pTop)
{
	//�g�b�v�̈ʒu��ۑ�����
	CTerritory * pTerritory = pTop;

	//pPrev, pNext��NULL�ɂ���
	while (pTerritory != NULL)
	{
		CTerritory * pSceneNext = pTerritory->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����
		
		//pPrev, pNext��NULL�ɂ���
		pTerritory->m_pPrev = NULL;
		pTerritory->m_pNext = NULL;

		//���̃|�C���^�ɐi�߂�
		pTerritory = pSceneNext;
	}
}

//=============================================================================
// ���X�g���̏�ԃ`�F�b�N
//=============================================================================
bool CTerritory::ChackList(CTerritory * pTop, int * nCountTerritory)
{
	//�ϐ��錾
	bool bClear = false;

	CTerritory * pTerritory = pTop;

	while (pTerritory)
	{
		CTerritory * pSceneNext = pTerritory->m_pNext;	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����
		
		//�J�E���^�[��i�߂�
		*nCountTerritory += 1;

		//���̃|�C���^�ɐi�߂�
		pTerritory = pSceneNext;
	}

	if (*nCountTerritory >= MIN_TERRITORY)
	{
		bClear = true;
	}

	return bClear;
}

//=============================================================================
// �͈͓��Ƀe���g���[�����邩�ǂ���
//=============================================================================
void CTerritory::ChackIn(D3DXVECTOR3 * pPos, int nMaxPos, int nNumPlayer)
{
	//�x�N�g���̐������������m��
	D3DXVECTOR3 * pVec = new D3DXVECTOR3[nMaxPos];

	//�x�N�g���̕��������߂�
	for (int nCntVec = 0; nCntVec < nMaxPos; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % nMaxPos) == 0) { nCnt = 0; }

		pVec[nCntVec] = pPos[nCnt] - pPos[nCntVec];
	}

	//�G�̃I�u�W�F�N�g�|�C���^�̎擾
	CScene * pSceneTop = CScene::GetTop(OBJ_PRIORITY);	 //�I�u�W�F�N�g�|�C���^�̐擪���擾			
	CScene * pScene = pSceneTop;						 //�g�b�v�̈ʒu��ۑ�����
	CTerritory * m_pTerritory = NULL;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			CTerritory * pTerritory = (CTerritory *)pScene;

			if ((pTerritory->m_State == STATE_NORMAL) && (pTerritory->m_nNumPlayer != nNumPlayer))
			{
				D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();
				int nCountEnter = 0;

				for (int nCnt = 0; nCnt < nMaxPos; nCnt++)
				{
					D3DXVECTOR3 VecB = TerritoryPos - pPos[nCnt];

					//�O�ς����߂�
					float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pVec[nCnt].x, pVec[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

					//�x�N�g�����傫�����ǂ���
					if (fCross >= 0) { nCountEnter++; }
				}

				if (nCountEnter == nMaxPos)
				{//�͈͓��ɓ����Ă�����

					//�v���C���[���̎擾
					CPlayer * pPlayer = CManager::GetGame()->GetPlayer(nNumPlayer);

					pTerritory->m_Color = m_CountryColor[(int)pPlayer->GetType()];	//�F�̏�������

					//����v���C���[�̓_�������_����
					if ((pTerritory->m_nNumPlayer != -1))
					{
						CPlayer * pOtherPlayer = CManager::GetGame()->GetPlayer(pTerritory->m_nNumPlayer);
					}

					pTerritory->m_State = STATE_NORMAL;		//�ʏ��Ԃɂ���
					pTerritory->m_nNumPlayer = nNumPlayer;	//�v���C���[�ԍ��̒u������
				}
			}
		}

		pScene = pSceneNext;	//�|�C���^��i�߂�
	}

	//�������̔j��
	if (pVec != NULL)
	{
		delete pVec;
		pVec = NULL;
	}
}

//=============================================================================
//�@�擾��Ԃ̍X�V����
//=============================================================================
void CTerritory::SetColor(D3DXCOLOR Color)
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