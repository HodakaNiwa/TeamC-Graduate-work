//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�X�s�[�h�^ [typeSpeed.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "enemy.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "line.h"
#include "model.h"
#include "debuglog.h"
#include "ripples.h" 

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_SPRINTTIMER (8)		//�X�v�����g����
#define RECAST (20)				//�X�v�����g�̃��L���X�g����

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTypeSpeed::CTypeSpeed(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTypeSpeed::~CTypeSpeed()
{

}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTypeSpeed *CTypeSpeed::Create(int nChara, int nLevel,int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypeSpeed *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypeSpeed;

		if (pSpeedType != NULL)
		{
			pSpeedType->m_nLevel = nLevel;
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt,country);
			pSpeedType->m_CharcterTypeResult[nChara] = type;
			pSpeedType->m_nCuntry[nChara] = country;
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
HRESULT CTypeSpeed::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int country)
{
	m_nEnemyNo = nChara;	//	�L�����ԍ��̋L��(������)

	CCharacter::Init(nChara, ModelTxt, MotionTxt,m_CharcterType, country);	//	������
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
	m_pLoadEffect = NULL;
	m_pLoadEffectSpeed =  NULL;

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
	if (m_pLoadEffectSpeed == NULL)
	{
		m_pLoadEffectSpeed = CLoadEffect::Create(2, D3DXVECTOR3(pos.x, pos.y + 30.0f, pos.z), 6);
	}

	ResetLine();	//	���C���̏�����

	m_fSpeed = 1.0f;				// �������x
	m_nCnt = 0;						// �^�C�}�[����
	m_bSkillFlag = false;			// �X�L���𔭓����Ă��邩
	m_bStop = false;				// �����X�V�̑j�~
	m_nLineNum = 2;					// �Œ჉�C��
	m_state = STATE_NONE;
	m_bSprintMotion = true;
	m_nTimingCnt = 0;
	m_bWalk = true;

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
	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Uninit();
		delete m_pLoadEffectSpeed;
		m_pLoadEffectSpeed = NULL;
	}

	//�v���C���[�̏I������
	CEnemy::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTypeSpeed::Update(void)
{
	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾

	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		if (pEveCam == NULL)
		{
			SprintUpdate();		//	�X�L������
		}

		CEnemy::Update();
	}
	CDebugProc::Print("\n�G�l�~�[�X�e�[�g%d", m_state);
}

//=============================================================================
// �X�L���̔���
//=============================================================================
void  CTypeSpeed::SprintUpdate(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPos();

	// [[AI���x�����ƂɃ^�C�~���O���ς��...]]
	if (m_nLevel == 3)//���x��3�������X�L���g�p�\
	{
		Trigger();// �X�L���̔���
	}
	else // ���x��1��2
	{
		m_nTimingCnt += (m_nLevel * 2);
		if (m_nTimingCnt >= 600)
		{
			Trigger();// �X�L���̔���
		}
	}

	// [[�X�L��������...]]
	if (m_bSkillFlag == true)
	{
		m_nCnt++;
		int nHeight = rand() % 100 + 30;
		m_pLoadEffectSpeed->SetPos(D3DXVECTOR3(pos.x, pos.y + nHeight, pos.z));
		m_pLoadEffectSpeed->OnTimeEffect();
		if (m_nCnt == (MAX_SPRINTTIMER * 60))	//	��莞�ԂŌ��̑��x��
		{
			m_fSpeed = 1.0f;		//	�X�s�[�h�����Ƃɖ߂�
			m_nCnt = 0;				//	�X�v�����g�^�C�}�[��������
			m_bWalk = true;			//	�ړ����[�V�������ł����Ԃɂ���
			m_state = STATE_NONE;	
			m_bSkillFlag = false;
		}

		//�g��̃G�t�F�N�g��\��
		if ((m_nCnt % 15) == 0)
		{
			D3DXVECTOR3 Pos = CCharacter::GetPos();
			CRipples::Create(D3DXVECTOR3(Pos.x,1.0f,Pos.z + 20.0f));
		}
		else if ((m_nCnt % 20) == 0)
		{
			D3DXVECTOR3 Pos = CCharacter::GetPos();
			CRipples::Create(D3DXVECTOR3(Pos.x, 1.0f, Pos.z - 20.0f));
		}
	}
	// [[�Ďg�p�܂�...]]
	else
	{
		m_nCnt++;
		if (m_nCnt == (RECAST * 60))
		{
			m_nCnt = 0;
			m_bStop = false;
			m_nTimingCnt = 0;
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTypeSpeed::Draw(void)
{
	CEnemy::Draw();

	if (m_pLoadEffectSpeed != NULL)
	{
		m_pLoadEffectSpeed->Draw();
	}
}

//=============================================================================
// �X�L���̔���
//=============================================================================
void CTypeSpeed::Trigger(void)
{
	if (m_bSkillFlag == false && m_bStop == false)
	{
		m_bStop = true;
		m_state = STATE_ACTION;
		m_fSpeed = 1.5f;		// �X�s�[�h�^�̑�����1.5�{�ɂ���
		m_bSkillFlag = true;		// �X�v�����g��Ԃɂ���
		m_bSprintMotion = true;
		m_nCnt = 0;
	}
}
//=============================================================================
//
//=============================================================================
void  CTypeSpeed::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{ }

