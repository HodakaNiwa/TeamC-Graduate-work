//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�X�s�[�h�^ [typeSpeed.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "enemy.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "inputmouce.h"
#include "loadEffect.h"
#include "line.h"
#include "sceneOrbit.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//�X�v�����g����
#define RECAST (10)				//�X�v�����g�̃��L���X�g����
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTypeSpeed::CTypeSpeed() 
{ 
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTypeSpeed::~CTypeSpeed() { }

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTypeSpeed *CTypeSpeed::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypeSpeed *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypeSpeed;

		if (pSpeedType != NULL)
		{
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt);
		}
		else
		{
			MessageBox(0, "�v���C���[��NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "�v���C���[��NULL����Ȃ��ł�", "�x��", MB_OK);
	}
	return pSpeedType;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTypeSpeed::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	m_bWalk = true;
	
	m_nEnemyNo = nChara;	//	�L�����ԍ��̋L��(������)

	CCharacter::Init(nChara,ModelTxt, MotionTxt);	//	������
	CCharacter::SetPos(pos);						//	�ʒu���f

	InitSort(pos);	//	�Q�[���J�n���̋ߏ�̃G���A�E�e���g���[��������
	
	m_pModel = CCharacter::GetModel();	//	���f�����̎擾
	m_pMotion = CCharacter::GetMotion();//	���[�V�������̎擾

	//	---���C���ϐ��̏�����---
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;


	//	�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	//	�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}

	
	ResetLine();	//	���C���̏�����

	
	m_fSpeed = 1.0f;				//	�������x
	m_nSprintCounter = 0;
	m_nSprintTimer = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_bRecast = false;
	m_bSprint = false;
	m_nCreateTime = (rand() % 4);	//	�n�_�ɖ߂�܂ł̎��Ԓ���
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTypeSpeed::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTypeSpeed::Uninit(void)
{
	if (m_pLoadEffect != NULL)
	{//�G�t�F�N�g�̔j��
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	//�v���C���[�̏I������
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTypeSpeed::Update(void)
{
	SprintUpdate();		//	�X�L������
	CEnemy::Update();
}

//=============================================================================
// �X�v�����g����
//=============================================================================
void  CTypeSpeed::SprintUpdate(void)
{
	
	if (m_bRecast == false && m_bCheckS == false)
	{
		m_bCheckS = true;
		m_state = STATE_ACTION;	//�X�v�����g���
		m_fSpeed = 1.5f;		//�X�s�[�h�^�̑�����1.5�{�ɂ���
		m_bSprint = true;		//�X�v�����g���ɂ���
	}

	//�X�v�����g��
	if (m_bSprint == true)
	{
		m_nSprintCounter++;

		if (m_nSprintCounter % 60 == 0)
		{
			m_nSprintTimer++;
			
			if (m_nSprintTimer >= MAX_SPRINTTIMER)	//	��莞�ԂŌ��̑��x��
			{
				m_fSpeed = 1.0f;					//�X�s�[�h�����Ƃɖ߂�
				m_nSprintTimer = 0;					//�X�v�����g�^�C�}�[��������
				m_bRecast = true;					//���L���X�g���ɂ���
				m_bWalk = true;						//�ړ����[�V�������ł����Ԃɂ���
				m_state = STATE_NONE;				//�ړ����
				m_bSprint = false;
			}
		}
	}

	//���L���X�g��
	if (m_bRecast == true)
	{
		m_nRecastCounter++;
		if (m_nRecastCounter % 60 == 0)
		{
			m_nRecastTimer++;
			if (m_nRecastTimer <= RECAST)
			{//10�b�ȉ���������X�v�����g���g�p�ł��Ȃ�
				m_bSprint = false;
			}
			else
			{//10�b��������X�v�����g���g�p�ł���悤�ɂ���
				m_bCheckS = false;
				m_bRecast = false;		//���L���X�g������Ȃ�
				m_nRecastTimer = 0;		//���L���X�g�^�C�}�[�̏�����
				m_bSprint = false;

			}
		}
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTypeSpeed::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
//
//=============================================================================
void  CTypeSpeed::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
// ���C���̐�������
//=============================================================================
void CTypeSpeed::CreateOrbitLine(void)
{
	if (m_pOrbitLine == NULL)
	{
		m_pOrbitLine = CSceneOrbit::Create(CSceneOrbit::TYPE_PLAYER, CCharacter::GetPos());
		m_pOrbitLine->SetMtxParent(&m_pModel[10]->GetMtxWorld());
		m_pOrbitLine->SetMtxParentEnd(&m_pModel[1]->GetMtxWorld());
	}
}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CTypeSpeed::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}