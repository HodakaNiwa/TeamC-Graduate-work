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
#include "ring.h"
#include "tutorial.h"
#include "ringRender.h"
#include "shadow.h"	//��

//=============================================================================
// �}�N����`
//=============================================================================
#define SaveTextName ("data/TEXT/SaveTerritory.txt")		//�ۑ�����e�L�X�g��
#define UPDATE_LENGTH	(80.0f)								//�X�V��Ԃ�L���ɂ���͈�
#define GET_LENGTH		(40.0f)								//�擾�͈�		
#define MIN_TERRITORY	(3)									//�擾�e���g���[�̍Œᐔ
#define FIELD_SIZE		(D3DXVECTOR2(4000.0f, 4000.0f))		//�t�B�[���h�̃T�C�Y
#define SHADOW_SIZE		(D3DXVECTOR3(43.0f, 0.0f, 43.0f))	//�e�̃T�C�Y�@��

// �����O�p
#define RING_MODELIDX   (1)
#define RING_DIFFERENCE (D3DX_PI * 0.8f)
#define RING_OFFSET     (D3DXVECTOR3(0.0f,18.0f,0.0f))
#define RING_SCALE      (D3DXVECTOR3(1.6f, 1.6f,1.6f))
#define RING_SCALEDOWN  (0.25f)
#define RING_ADDPOSY    (15.0f)
#define RING_ADDANGLE   (D3DX_PI * 0.005f)

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
D3DXCOLOR CTerritory::m_DefaultColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
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
	pTerritory->CreateShadow();						//�e�̐���

	//���f���E�e�N�X�`���̊��蓖��
	pTerritory->BindModel(m_pMesh[ModelNum], m_pBuffMat[ModelNum], m_pNumMat[ModelNum], m_ppTexture[nTexNum]);

	// �����O���쐬���鏈��
	pTerritory->CreateRing();

	return pTerritory;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTerritory::CTerritory() : CSceneX(3, OBJTYPE_TERRITORY)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
	m_nCountTime = 0;
	m_nRingColIndx = 0;
	m_nTex = 0;
	m_nModelType = 0;
	m_nPoint = 0;
	m_nOldNumPlayer = 0;
	m_nErea = 0;
	m_fLength = 0.0f;
	m_pLoadEffect = NULL;
	m_ppRing = NULL;
	m_pbChangeColRing = false;
	m_bGetTerritory = false;
	m_Color = INITIALIZE_VECTOR4;
	m_OldColor = INITIALIZE_VECTOR4;
	m_state = STATE_NONE;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTerritory::CTerritory(int nPriority, OBJTYPE objtype) : CSceneX(nPriority, objtype)
{
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rot = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(0, 0, 0);
	m_nCountTime = 0;
	m_nRingColIndx = 0;
	m_nTex = 0;
	m_nModelType = 0;
	m_nPoint = 0;
	m_nOldNumPlayer = 0;
	m_nErea = 0;
	m_fLength = 0.0f;
	m_pLoadEffect = NULL;
	m_ppRing = NULL;
	m_pbChangeColRing = false;
	m_bGetTerritory = false;
	m_Color = INITIALIZE_VECTOR4;
	m_OldColor = INITIALIZE_VECTOR4;
	m_state = STATE_NONE;
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
	m_Color = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);	//�F
	m_OldColor = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);	//�F
	m_nNumPlayer = -1;								//�v���C���[�ԍ�
	m_nOldNumPlayer = -1;							//�O��̃v���C���[�ԍ�
	m_fLength = 0.0f;								//����
	m_bGetTerritory = false;						//�e���g���[���擾�������ǂ���
	m_nErea = 0;									//�G���A�ԍ�
	m_ppRing = NULL;
	m_pLoadEffect = NULL;
	m_state = STATE_NONE;							//�e���g���[�̏�ԁ@
	m_nCountTime = 0;								//���Ԃ̃J�E���^�[
	m_nRingColIndx = 0;
	m_pbChangeColRing = NULL;

	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(0, m_pos, 8);
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

	// �����O�̊J��
	ReleaseRing();

	m_nMaxObj--;
}

//=============================================================================
// �X�V����
//=============================================================================
void CTerritory::Update(void)
{
	CSceneX::Update();

	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();

		int nNumPlayer = pGame->GetNumPlay();
		nNumPlayer += pGame->GetEnemyNum();

		for (int nCntPlayer = 0; nCntPlayer < nNumPlayer; nCntPlayer++)
		{
			CCharacter * pPlayer = CManager::GetGame()->GetChara(nCntPlayer);
			if (pPlayer == NULL) { continue; }

			D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

			//�v���C���[�Ƃ̋��������߂�
			m_fLength = CFunctionLib::Vec2Length(m_pos.x, m_pos.z, PlayerPos.x, PlayerPos.z);

			//�X�V�͈͂ɂ��邩�ǂ���
			if (m_fLength < GET_LENGTH)
			{
				pPlayer->AddTerritoryList(this);	//�v���C���[�Ɏ��g�̏���n��
			}
		}
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{
		CCharacter * pPlayer = CManager::GetTutorial()->GetTutorialPlayer();
		if (pPlayer == NULL) { return; }

		D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

		//�v���C���[�Ƃ̋��������߂�
		m_fLength = CFunctionLib::Vec2Length(m_pos.x, m_pos.z, PlayerPos.x, PlayerPos.z);

		//�X�V�͈͂ɂ��邩�ǂ���
		if (m_fLength < GET_LENGTH)
		{
			pPlayer->AddTerritoryList(this);	//�v���C���[�Ɏ��g�̏���n��
		}
	}

	//�����O�̍X�V�����@
	UpdateGetRing();
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
	if (m_pLoadEffect != NULL)
	{
		m_pLoadEffect->Draw();
	}
}

//=============================================================================
// �����O�̍X�V����
//=============================================================================
void CTerritory::UpdateGetRing(void)
{//��
	if (m_state != STATE_GET) { return; }

	//�ϐ��錾
	bool bEndScale = false;
	bool bEndPos = false;

	//���Ԃ̉��Z
	m_nCountTime++;

	//�����O�����_���[�̎擾
	CRingRender * pRingrender = NULL;
	CManager::MODE mode = CManager::GetMode();

	if (mode == CManager::MODE_GAME)
	{
		pRingrender = CManager::GetGame()->GetRingRender();
	}
	else if (mode == CManager::MODE_TUTORIAL)
	{
		pRingrender = CManager::GetTutorial()->GetRingRender();
	}

	if (pRingrender == NULL) { return; }

	// ���[���h�}�g���b�N�X�o�b�t�@�����b�N���A���[���h�}�g���b�N�X�f�[�^�ւ̃|�C���^���擾
	CRingRender::WORLDMATRIX *pWorld = NULL;
	LPDIRECT3DVERTEXBUFFER9 pWorldBuff = pRingrender->GetWorldBuff();
	pWorldBuff->Lock(0, 0, (void**)&pWorld, 0);

	// ���_�J���[�o�b�t�@�����b�N���A���_�J���[�f�[�^�ւ̃|�C���^���擾
	CRingRender::COLORDATA *pColor = NULL;
	LPDIRECT3DVERTEXBUFFER9 pColorBuff = pRingrender->GetColorBuff();
	pColorBuff->Lock(0, 0, (void**)&pColor, 0);

	//�|�C���^��i�߂�
	if (m_ppRing[0] != NULL)
	{
		int nId = m_ppRing[0]->GetID();
		pWorld += nId;
		pColor += nId;
	}

	//���X�g�ɒǉ�����
	for (int nCnt = 0; nCnt < m_nPoint; nCnt++)
	{
		if (m_ppRing[nCnt] == NULL) { continue; }

		if (m_nCountTime > 20)
		{
			if (((m_nCountTime % 35) == 0) && (m_nRingColIndx == nCnt))
			{
				m_pbChangeColRing[m_nRingColIndx] = true;
				m_nRingColIndx++;	//ID��i�߂�
				m_nCountTime++;	//���Ԃ�i�߂�
			}
		}
		else if (m_nCountTime < 20)
		{
			//�X�P�[���̕ύX
			D3DXVECTOR3 Scale = m_ppRing[nCnt]->GetScale();
			Scale += D3DXVECTOR3(0.04f - (0.01f * nCnt), 0.04f - (0.01f * nCnt), 0.04f - (0.01f * nCnt));
			m_ppRing[nCnt]->SetScale(Scale);

			//�ʒu�̕ύX
			D3DXVECTOR3 Pos = m_ppRing[nCnt]->GetPos();
			Pos.y += (5.1f - (nCnt * 2.0f));
			m_ppRing[nCnt]->SetPos(Pos);
		}

		if (m_pbChangeColRing[nCnt])
		{
			//�F�̕ύX
			pColor->col = m_Color;

			// �X�P�[���̕ύX
			D3DXVECTOR3 Scale = m_ppRing[nCnt]->GetScale();
			Scale -= D3DXVECTOR3(0.04f - (0.01f * nCnt), 0.04f - (0.01f * nCnt), 0.04f - (0.01f * nCnt));

			//�X�P�[���̐ݒ�
			float fScale = RING_SCALE.x - (RING_SCALEDOWN * nCnt);

			if (Scale.x < fScale)
			{
				Scale = D3DXVECTOR3(fScale, fScale, fScale);
				if (nCnt == m_nPoint - 1) { bEndScale = true; }
			}
			m_ppRing[nCnt]->SetScale(Scale);

			//�ʒu�̕ύX
			D3DXVECTOR3 Pos = m_ppRing[nCnt]->GetPos();
			Pos.y -= (5.1f - (nCnt * 2.0f));

			float fOffsetY = RING_OFFSET.y + (RING_ADDPOSY * nCnt);

			if (Pos.y < fOffsetY)
			{
				Pos.y = fOffsetY;
				if (nCnt == m_nPoint - 1) { bEndPos = true; }
			}

			m_ppRing[nCnt]->SetPos(Pos);
		}

		//�Đݒ肷��
		pRingrender->AddList(m_ppRing[nCnt], pWorld, pColor);

		//�|�C���^��i�߂�
		pColor++;
		pWorld++;
	}

	if ((m_pbChangeColRing[m_nPoint - 1] == true) && (bEndPos == true) && (bEndScale == true))
	{//�Ō�̃t���O���L����������
		m_state = STATE_NONE;		//�ʏ��Ԃɂ���
		m_nCountTime = 0;
		m_nRingColIndx = 0;

		for (int nCnt = 0; nCnt < m_nPoint; nCnt++)
		{//�t���O������������
			m_pbChangeColRing[nCnt] = false;
		}
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
	CBoxCollider *pBox = CBoxCollider::Create(m_pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), 27.0f, 68.0f, 20.0f, false);
	pBox->SetParent(this);
	//pBox->SetParentMtxWorld(&GetMtx());
	pColManager->SetCollider(pBox, 0);
	SetCOlliderManager(pColManager);
}

//=============================================================================
// �����O���쐬���鏈��
//=============================================================================
void CTerritory::CreateRing(void)
{
	if (m_ppRing != NULL) { return; }

	// ���������m�ۂ���
	m_ppRing = new CRing*[m_nPoint];
	if (m_ppRing == NULL) { return; }

	//�t���O�̃��������m�ۂ���@
	m_pbChangeColRing = new bool[m_nPoint];

	// �C���X�^���X�̃A�h���X�𐶐�
	float fAngle = 0.0f;
	for (int nCntPoint = 0; nCntPoint < m_nPoint; nCntPoint++)
	{// ���g�̃|�C���g�̉񐔕��J�肩����

	 //�t���O�̏����� 
		m_pbChangeColRing[nCntPoint] = false;

	 // �����O�̊p�x�����炵�Ă���
		fAngle = RING_DIFFERENCE * nCntPoint;

		// �����O�̈ʒu�����炵�Ă���
		D3DXVECTOR3 Offset = RING_OFFSET;
		Offset.y += RING_ADDPOSY * nCntPoint;

		// �傫����ύX
		D3DXVECTOR3 Scale = RING_SCALE;
		Scale.x -= RING_SCALEDOWN * nCntPoint;
		Scale.y -= RING_SCALEDOWN * nCntPoint;
		Scale.z -= RING_SCALEDOWN * nCntPoint;

		// ���������m��
		m_ppRing[nCntPoint] = NULL;
		m_ppRing[nCntPoint] = CRing::Create(GetPos() + Offset, Scale, m_pMesh[RING_MODELIDX], m_pBuffMat[RING_MODELIDX], m_pNumMat[RING_MODELIDX], NULL);
		if (m_ppRing[nCntPoint] == NULL) { continue; }
	}
}

//=============================================================================
// �����O���J�����鏈��
//=============================================================================
void CTerritory::ReleaseRing(void)
{
	if (m_ppRing == NULL) { return; }
	for (int nCntPoint = 0; nCntPoint < m_nPoint; nCntPoint++)
	{// ���g�̃|�C���g�̉񐔕��J�肩����
		if (m_ppRing[nCntPoint] != NULL)
		{// ���������m�ۂ���Ă���
			m_ppRing[nCntPoint]->Uninit();
			m_ppRing[nCntPoint] = NULL;
		}
	}
	delete[] m_ppRing;
	m_ppRing = NULL;

	if (m_pbChangeColRing != NULL)
	{
		delete[] m_pbChangeColRing;
		m_pbChangeColRing = NULL;
 	}
}

//=============================================================================
// �e�̐���
//=============================================================================
void CTerritory::CreateShadow(void)
{//��
	CShadow::Create(D3DXVECTOR3(m_pos.x, 0.1f, m_pos.z), SHADOW_SIZE);
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
// �e���g���[�̐F�����Z�b�g����
//=============================================================================
void CTerritory::ResetColorTerritory(void)
{
	//�I�u�W�F�N�g�|�C���^�̎擾
	CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);	//�I�u�W�F�N�g�|�C���^�̐擪���擾			
	CScene * pScene = pSceneTop;								//�g�b�v�̈ʒu��ۑ�����
	CTerritory * pTerritory = NULL;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();					//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			pTerritory = (CTerritory*)pScene;
			pTerritory->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));	//�F�����Ƃɖ߂�
			pTerritory->m_nNumPlayer = -1;								//�v���C���[�ԍ�������
			pTerritory->m_nOldNumPlayer = -1;							//�O��̃v���C���[�ԍ�������

			if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{//�����O�̐F��ύX����@��
				CManager::GetTutorial()->GetRingRender()->ResetColorRing();
			}
		}

		pScene = pSceneNext;		//�|�C���^��i�߂�
	}
}


//=============================================================================
//�@�I�u�W�F�N�g����
//=============================================================================
CTerritory * CTerritory::RequestPointer(int nCountObj)
{
	//�I�u�W�F�N�g�|�C���^�̎擾
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