//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�n���^ [typeSweeper.h]
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
#include "mine.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RECASTTIME (10)				//���L���X�g����
#define MAX_MINE_POINT (100)		//�}�C�����g�p�ł���|�C���g
#define REDUCED_MINE_POINT (20)		//�}�C�����g�p�������Ɍ���|�C���g
#define RECOVERY_MINE_POINT (2)		//�}�C���g�p�|�C���g�̉񕜗�
#define RECOVERY_TIME (3)			//�g�p�|�C���g�񕜂܂ł̎���

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
	CTypeSweeper *pSpeedType = NULL;
	if (pSpeedType == NULL)
	{
		pSpeedType = new CTypeSweeper;

		if (pSpeedType != NULL)
		{
			pSpeedType->SetType(country);
			pSpeedType->m_CharcterType = type;
			pSpeedType->Init(nChara, pos, ModelTxt, MotionTxt, country);
			//�����ǉ����܂���
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
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nButtonCounter = 0;
	m_bRecast = false;
	m_nReCounter = 0;
	m_nReTimer = 0;
	m_nMinePoint = MAX_MINE_POINT;
	m_nInstallationCounter = 0;
	m_nInstallationTimer = 0;
	m_nLineNum = 2;
	m_nTiming = 0;
	m_bStop = false;

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
	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();

	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	{
		ActionUpdate();	//	�X�L������
		CEnemy::Update();
	}
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
// �X�v�����g����
//=============================================================================
void  CTypeSweeper::ActionUpdate(void)
{
	if (m_nMinePoint == MAX_MINE_POINT)	//	���^�����Ƃ肠�����n����u��
	{
		Process();

	}

	if (m_apTerritory[0] != NULL)
	{
		m_nTiming++;
		if (m_nTiming == 50) { Process(); }
		if (m_nTiming == 100) { Process(); }
	}
	else { m_nTiming = 0; }

	//	�|�C���g�̉�
	m_nReCounter++;
	if (m_nReCounter % 180 == 0 && m_nMinePoint < MAX_MINE_POINT)
	{
		m_nReCounter = 0;

		m_nMinePoint += RECOVERY_MINE_POINT;	//	�񕜒l

		if (m_nMinePoint >= MAX_MINE_POINT)		//	����l���z�����Ȃ�
		{
			m_nMinePoint = MAX_MINE_POINT;
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
		if (m_nMinePoint >= REDUCED_MINE_POINT)
		{
			//	�ݒu
			CMine::Create(CCharacter::GetPos(), pCharacter);

			//	�g�p�\�|�C���g�����炷
			m_nMinePoint -= REDUCED_MINE_POINT;
			if (m_nMinePoint <= 0)	//	0�ȉ��ɂ����Ȃ�
			{
				m_nMinePoint = 0;
			}
		}
	}
}

