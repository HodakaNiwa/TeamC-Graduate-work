//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�n���^ [typeSweeper.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "enemy.h"
#include "manager.h"
#include "camera.h"
#include "input.h"
#include "loadEffect.h"
#include "line.h"
#include "sceneOrbit.h"
#include "model.h"
#include "UI.h"
#include "skilicon.h"
#include "RawMouse.h"
#include "mine.h"
#include "debuglog.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RECASTTIME (10)	// ���L���X�g����
#define MAX_POINT (100)	// �}�C�����g�p�ł���|�C���g
#define COST_POINT (20)	// �}�C�����g�p�������Ɍ���|�C���g
#define RE_POINT (5)	// �}�C���g�p�|�C���g�̉񕜗�

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTypeSweeper::CTypeSweeper()
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTypeSweeper::~CTypeSweeper()
{

}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CTypeSweeper *CTypeSweeper::Create(int nChara, int country, CHARCTERTYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CTypeSweeper *pSweeperType = NULL;
	if (pSweeperType == NULL)
	{
		pSweeperType = new CTypeSweeper;

		if (pSweeperType != NULL)
		{
			pSweeperType->SetType(country);
			pSweeperType->m_CharcterType = type;
			pSweeperType->Init(nChara, pos, ModelTxt, MotionTxt, country);
			//�����ǉ����܂���
			pSweeperType->m_CharcterTypeResult[nChara] = type;
			pSweeperType->m_nCuntry[nChara] = country;

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
	return pSweeperType;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTypeSweeper::Init(int nChara, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int country)
{
	//�v���C���[�̃i���o�[���i�[
	m_nEnemyNo = nChara;

	//������
	m_bWalk = true;
	m_bSprintMotion = false;
	//�L�����N�^�[�̏�����
	CCharacter::Init(nChara,ModelTxt, MotionTxt,m_CharcterType,country);
	CCharacter::SetPos(pos);

	InitSort(pos);	//	�Q�[���J�n���̋ߏ�̃G���A�E�e���g���[��������


	//���f���̎擾
	m_pModel = CCharacter::GetModel();
	
	//���C���ϐ��̏�����
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;
	m_pLoadEffect = NULL;

	//�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z), 8);
	}

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	ResetLine();	//���C���̏�����

	
	//	�e�ϐ��̏�����
	m_fSpeed = 1.0f;
	m_nReCounter = 0;
	m_nMinePoint = MAX_POINT;
	m_nLineNum = 2;
	m_nTiming = 0;
	m_bStop = false;
	m_state = STATE_NONE;
	m_nUse = false;
	m_nActionCnt = 0;
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTypeSweeper::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTypeSweeper::Uninit(void)
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
void  CTypeSweeper::Update(void)
{
	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾

	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		if (pEveCam == NULL)
		{
			ActionUpdate();	//	�X�L������
		}
		CEnemy::Update();
	}

	if (m_nUse == true)
	{
		m_nActionCnt++;
		m_bSuperArmor = true; // �ꎞ�I���G���
		m_bWalk = false;

		if (m_nActionCnt >= 45)// ���[�V�����E�X�e�[�^�X�̐؂�ւ�
		{
			m_nActionCnt = 0;
			m_fSpeed = 1.0f;
			m_nUse = false;
			m_bSuperArmor = false;	// ���G����
			m_bWalk = true;			// �������Ԃ�
			m_state = STATE_NONE;	//�@�X�e�[�^�X���Z�b�g
		}
	}

	CDebugProc::Print("�X�C�[�p�[�@%d\n", m_state);
}



//=============================================================================
// �`�揈��
//=============================================================================
void  CTypeSweeper::Draw(void)
{
	CEnemy::Draw();
}

//=============================================================================
//
//=============================================================================
void  CTypeSweeper::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}
//=============================================================================
// �X�L���̔���
//=============================================================================
void  CTypeSweeper::ActionUpdate(void)
{
	//	[[�ݒu����]]
	if (m_nMinePoint == MAX_POINT)	//	���^�����Ƃ肠�����n����u��
	{
		Process();
	}

	if (m_apTerritory[0] != NULL)
	{
		m_nTiming++;
		if (m_nTiming == 50) { Process(); }
		if (m_nTiming == 150) { Process(); }
	}
	else { m_nTiming = 0; }


	//	[[�|�C���g�̉�]]
	m_nReCounter++;
	if (m_nReCounter % 60 == 0 && m_nMinePoint < MAX_POINT)
	{
		m_nReCounter = 0;

		m_nMinePoint += RE_POINT;			//	�񕜒l

		if (m_nMinePoint >= MAX_POINT)		//	����l���z�����Ȃ�
		{
			m_nMinePoint = MAX_POINT;
		}
	}
}
//=============================================================================
// �n���g�p���̏���
//=============================================================================
void CTypeSweeper::Process()
{
	CGame *pGame = CManager::GetGame();
	CCharacter *pCharacter = pGame->GetChara(m_nEnemyNo);

	//	(�c��|�C���g�������)
	if (m_state != STATE_DAMAGE)
	{
		if (m_nMinePoint >= COST_POINT)
		{
			//	�ݒu
			m_nUse = true;
			m_fSpeed = 0.0f;
			m_state = STATE_ACTION;
			m_pMotion->SetNumMotion(STATE_ACTION);

			if (m_state != STATE_DAMAGE && m_state != STATE_BLOWAWAY)
			{//�_���[�W���Ɛ�����ђ�����Ȃ�������@��
				CMine::Create(CCharacter::GetPos(), pCharacter);
			}

			//	�g�p�\�|�C���g�����炷
			m_nMinePoint -= COST_POINT;
			if (m_nMinePoint <= 0)	//	0�ȉ��ɂ����Ȃ�
			{
				m_nMinePoint = 0;
			}
		}
	}
}

