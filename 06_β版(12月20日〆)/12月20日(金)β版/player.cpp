//=============================================================================
//
// �v���C���[�̏��� [player.cpp]
// Author : ���я��� & �R���֎j & NiwaHodaka
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"
#include "collision.h"
#include "model.h"
#include "Territory.h"
#include "library.h"
#include "line.h"
#include "loadEffect.h"
#include "RawMouse.h"
#include "UI.h"
#include "score.h"
#include "mine.h"
#include "debuglog.h"
#include "bullet.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �R���W�����֌W
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_PLAYERMOVING  (12.0f)	
#define BLOW_ROBOTMOVING   (30.0f)	
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23) //23

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�ÓI�����o�ϐ��錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
{
	m_move = INITIALIZE_VECTOR3;
	m_Angle = INITIALIZE_VECTOR3;
	m_rot = INITIALIZE_VECTOR3;
	m_posOld = INITIALIZE_VECTOR3;
	m_bSuperArmor = false;
	m_bSprint = false;
	m_bSprintMotion = false;
	m_bSprint = false;
	m_bWalk = true;
	m_bMouse = false;
	m_bStateFlag = false;
	m_fMouse = 0.0f;
	m_fSpeed = 0.0f;
	m_fInertia = 0.0f;
	m_fBlowAngle = 0.0f;
	m_fBlowLength = 0.0f;
	m_nDownCount = 0;
	m_nRobotDownCount = 0;
	m_nCountTime = 0;
	m_nNumPlayer = 0;
	m_nControllerIndx = 0;
	m_nControllerType = 0;
	m_nDamageCounter = 0;
	m_nDamageCount = 0;
	m_PlayerState = PLAYERSTATE_NONE;
	m_pModel = NULL;
	m_pMotion = NULL;
	m_pLoadEffect = NULL;
	m_bWalkFrag = false;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPlayer::~CPlayer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType)
{
	//�v���C���[�̃i���o�[���擾
	m_nNumPlayer = nNumPlayer;

	//�L�����N�^�[�̏�����
	CCharacter::Init(nNumPlayer,ModelTxt, MotionTxt, m_CharcterType,nType);
	CCharacter::SetPos(pos);

	//���f���̏����擾
	m_pModel = CCharacter::GetModel();

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CPlayer::Uninit(void)
{
	//�L�����N�^�[�̔j��
	CCharacter::Uninit();

	if (m_pLoadEffect != NULL)
	{//�G�t�F�N�g�̔j��
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}
	m_bMouse = false;
}

//=============================================================================
// �X�V����
//=============================================================================
void  CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾

	CGame *pGame = CManager::GetGame();								// �Q�[���̎擾
	CEventCamera *pEveCam = pGame->GetEveCam();						// �C�x���g�J�����̎擾
	if (pEveCam == NULL)											// �C�x���g�J�����������Ă�����
	{	
		//�L�����N�^�[�̍X�V
		CCharacter::Update();

		//	���C���̃J�E���g�_�E������
		CountDownLineTime();

		//	���C�������̊J�n(�J�E���g�A�b�v)
		if (m_bBlockStartTerritory)
		{
			m_nCountTime++;
			if ((m_nCountTime % NOT_GETTIME) == 0) { m_bBlockStartTerritory = false; }
		}

		//�v���C���[�̏��
		switch (m_PlayerState)
		{
		case PLAYERSTATE_NONE:		//�ʏ���
									//������p�ɂ���
			m_bWalk = true;
			//�X�v�����g�g�p��
			m_bSprintMotion = true;
			break;

		case PLAYERSTATE_WALK:		//�ړ����
			//�ړ��G�t�F�N�g
			m_pLoadEffect->SetPos(D3DXVECTOR3(GetPos().x, GetPos().y + 20.0f, GetPos().z));
			//�ʏ�̈ړ��͂�����
			m_pLoadEffect->Update();
			m_bWalkFrag = true;
			break;

		case PLAYERSTATE_ACTION:	//�A�N�V�������
			pSound->StopSound(CSound::SOUND_LABEL_SE018);
			break;

		case PLAYERSTATE_BLOWAWAY:	//������΂���Ă���
									// �_���[�W�J�E���g���Z
			m_nDamageCount++;

			if (m_nDamageCount >= MAX_DAMAGECOUNT)
			{
				m_nDamageCount = 0;
				m_bWalk = true;
				m_bSprintMotion = true;
				m_PlayerState = PLAYERSTATE_NONE;
			}

			// ������΂�
			m_fBlowLength -= BLOW_MOVING_CUT;
			if (m_fBlowLength >= 0.0f)
			{
				m_move.x -= sinf(m_fBlowAngle) * m_fBlowLength;
				m_move.z -= cosf(m_fBlowAngle) * m_fBlowLength;
			}
			break;

		case PLAYERSTATE_DAMAGE:
			// �_���[�W�J�E���g���Z
			m_nDownCount++;

			if (m_nDownCount == 70)
			{//70�b��������N���オ��
				m_pMotion->SetNumMotion(5);
			}
			if (m_nDownCount >= 110)
			{//110�b�œ�����悤�ɂȂ�
				m_bWalk = true;
				m_bSprintMotion = true;
				m_nDownCount = 0;
				m_bSuperArmor = false;
				m_PlayerState = PLAYERSTATE_NONE;
				m_nDamageCounter = 0;
			}
			break;

		case PLAYERSTATE_ROBOTDAMAGE:	// ���ǉ�(�悵�낤)
										// �_���[�W�J�E���g���Z
			m_nRobotDownCount++;

			//70�b��������N���オ��
			if (m_nRobotDownCount == 70)
			{
				m_pMotion->SetNumMotion(5);
			}
			if (m_nRobotDownCount >= 110)
			{//110�b�œ�����悤�ɂȂ�
				m_bWalk = true;
				m_bSprintMotion = true;
				m_nRobotDownCount = 0;
				m_bSuperArmor = false;
				m_PlayerState = PLAYERSTATE_NONE;
				m_nDamageCounter = 0;
			}
			// ������΂�
			m_fBlowLength -= BLOW_MOVING_CUT;
			if (m_fBlowLength >= 0.0f)
			{
				m_move.x -= sinf(m_fBlowAngle) * m_fBlowLength;
				m_move.z -= cosf(m_fBlowAngle) * m_fBlowLength;
			}
			break;

		}

		//�}�`������������̏���
		UpdateShapeComplete();
		//�Q�[���̏�Ԃ��擾
		int nGameState = CGame::GetGameState();

		if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
		{
			//�v���C���[�̈ړ�����
			PlayerMove();
		}
		else if (nGameState == CGame::GAMESTATE_END)
		{//�Q�[���I�����Ƀ��[�V�������j���[�g�����ɂ���
			if (m_bCharaMotionState == false)
			{
				m_PlayerState = PLAYERSTATE_NONE;

				//�T�E���h���~�߂�
				pSound->StopSound(CSound::SOUND_LABEL_SE018);
				pSound->StopSound(CSound::SOUND_LABEL_SE030);
				pSound->StopSound(CSound::SOUND_LABEL_SE022);
				pSound->StopSound(CSound::SOUND_LABEL_SE024);

				//�ҋ@���[�V����
				m_pMotion->SetNumMotion(0);
				m_bCharaMotionState = true;
			}
		}

		if (m_PlayerState != PLAYERSTATE_WALK)
		{//�����Ă��Ȃ�������
			m_bWalkFrag = false;
		}

		// �L�����N�^�[�̏�Ԃ�ݒ�
		SetCharaState(m_PlayerState);	

		// �����Ă��邩�ǂ���
		SetWalkFrag(m_bWalkFrag);
	}
	else
	{
		//�����T�E���h���~�߂�
		pSound->StopSound(CSound::SOUND_LABEL_SE018);
	}

	CDebugProc::Print("�Ԃ���у_���[�W : %d\n", m_nDamageCount);
	CDebugProc::Print("�X�e�[�g : %d\n", m_PlayerState);
	CDebugProc::Print("�_���J���@: %d\n", m_nDamageCounter);
	CDebugProc::Print("�_���[�W�_�E�� : %d\n", m_nDownCount);
	CDebugProc::Print("\n���݂̃��[�V���� : %d\n", m_pMotion->GetNumMotion());

}

//=============================================================================
// �`�揈��
//=============================================================================
void  CPlayer::Draw(void)
{
	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾���ǉ�(�悵�낤)
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾���ǉ�(�悵�낤)
	if (pEveCam == NULL)	// �C�x���g�J�����������Ă����灩�ǉ�(�悵�낤)
	{
		//�L�����N�^�[�̕`��
		CCharacter::Draw();

		if (m_pLoadEffect != NULL)
		{//�G�t�F�N�g�̕`��
			m_pLoadEffect->Draw();
		}
	}
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, TYPE type, CHARCTERTYPE charctertype)
{
	CPlayer *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//�������𓮓I�m��
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{//NULL�`�F�b�N
			pPlayer->m_CharcterType = charctertype;
			pPlayer->Init(pos, ModelTxt,MotionTxt, nNumPlayer,type);
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
	return pPlayer;
}

//=============================================================================
//
//=============================================================================
void  CPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//�@�v���C���[�̈ړ�����
//=============================================================================
void  CPlayer::PlayerMove(void)
{
	//�e���̎擾
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾
	
	CManager *pManager = NULL;									
	CGame *pGame = pManager->GetGame();								//�Q�[���̎擾
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//�Q�[���J�����̎擾
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//�����̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();							//�ʒu�̎擾
	
	//�X�e�[�^�X�̎擾
	float fSpeed = GetSpeed();					//����
	//float fInertia = GetInertia();			//����
	
	//�O��̈ʒu���i�[
	m_posOld = pos;

	if (m_nControllerType == 0)
	{//PAD
		PlayerMovePad(rot, pos);
	}
	else if (m_nControllerType == 1)
	{//�}�E�X
		PlayerMoveMouse(rot, pos);
	}
	else
	{//�L�[�{�[�h
		PlayerMoveKeyboard(rot, pos);
	}

	D3DXVECTOR3 DiffAngle = INITIALIZE_VECTOR3;

	// �v���C���[�̊p�x���C��
	DiffAngle.y = m_Angle.y - m_rot.y;   //�������v�Z

	if (DiffAngle.y > D3DX_PI)
	{//D3DX_PI�𒴂���
		DiffAngle.y -= D3DX_PI * 2.0f;
	}
	if (DiffAngle.y < -D3DX_PI)
	{//-D3DX_PI�𒴂���
		DiffAngle.y += D3DX_PI * 2.0f;
	}

	//������{���Œ���
	m_rot.y += DiffAngle.y * 0.1f;

	if (m_rot.y > D3DX_PI)
	{//D3DX_PI�𒴂���
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{//-D3DX_PI�𒴂���
		m_rot.y += D3DX_PI * 2.0f;
	}
	
	//�ʒu��ݒ�
	CCharacter::SetPos(pos);

	//������ݒ�
	CCharacter::SetRot(m_rot);

	//�{�����[���̐ݒ�
	pSound->SetVolume(CSound::SOUND_LABEL_SE018, 50.0f);
}

//=============================================================================
//�@�v���C���[�̃L�[�{�[�h�ł̈ړ�����
//=============================================================================
void CPlayer::PlayerMoveKeyboard(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos)
{
	//�e���̎擾
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();								//�Q�[���̎擾
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//�Q�[���J�����̎擾
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//�����̎擾
																	//�X�e�[�^�X�̎擾
	float fSpeed = GetSpeed();										//����
															
	if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE  && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
	{
		//�v���C���[�̈ړ������i�L�[�{�[�h�j
		if (pInputKeyboard->GetKeyboardPress(DIK_A) == true)
		{
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
				{//�|���S��������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.25f);
				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
				{//�|���S���������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.5f);
				}
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
			{
				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
				{//�|���S��������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.25f);
				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
				{//�|���S���������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.5f);
				}
			}

			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && 
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_D) == true)
		{
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
				{//�|���S�����E��Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.25f);
				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
				{//�|���S�����E���Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�����E�Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.5f);
				}
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_DAMAGE)
			{
				if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
				{//�|���S�����E��Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.25f);
				}
				else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
				{//�|���S�����E���Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�����E�Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.5f);
				}
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER ||
				m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && (m_PlayerState != PLAYERSTATE_ACTION
					&& m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_W) == true)
		{
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_move.x -= sinf(rot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(rot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = rot.y;
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_DAMAGE)
			{
				m_move.x -= sinf(rot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(rot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = rot.y;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER ||
				m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && (m_PlayerState != PLAYERSTATE_ACTION
					&& m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_S) == true)
		{
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * (fSpeed * m_fSpeed);
				m_Angle.y = D3DX_PI + rot.y;
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_DAMAGE)
			{
				m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * (fSpeed * m_fSpeed);
				m_Angle.y = D3DX_PI + rot.y;
			}

			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER ||
				m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && (m_PlayerState != PLAYERSTATE_ACTION
					&& m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
		}
		else
		{
			if (m_PlayerState == PLAYERSTATE_WALK || (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && 
				m_PlayerState == PLAYERSTATE_ACTION) && 
				(m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f)
				&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
			{
				pSound->StopSound(CSound::SOUND_LABEL_SE018);
				//�j���[�g������Ԃɂ���
				m_PlayerState = PLAYERSTATE_NONE;
				m_pMotion->SetNumMotion(PLAYERSTATE_NONE);
			}
		}
	}
	
	if (m_bWalk == true)
	{//�ړ����[�V����
		if (m_PlayerState == PLAYERSTATE_WALK)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
			m_pMotion->SetNumMotion(PLAYERSTATE_WALK);
			m_bWalk = false;
		}
	}
	if (m_bSprintMotion == true)
	{//�X�v�����g���[�V����

		if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState == PLAYERSTATE_ACTION)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
			m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);
			m_bSprintMotion = false;
		}
	}

	if (pInputKeyboard->GetKeyboardPress(DIK_P) == true)
	{
		//�e���g���[�擾�𒆒f
		Interruption();
	}

	//�ړ�����
	pos += m_move;

	//����
	m_move.x += (0.0f - m_move.x) * m_fInertia;
	m_move.z += (0.0f - m_move.z) * m_fInertia;
}

//=============================================================================
//�@�v���C���[�̃R���g���[���[(�X�e�B�b�N)�ł̈ړ�����
//=============================================================================
void  CPlayer::PlayerMovePad(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos)
{
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾
	CInputXPad * pXPad = CManager::GetXPad();						//Xinput�̎擾
	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();								//�Q�[���̎擾
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//�Q�[���J�����̎擾

	//�X�e�[�^�X�̎擾		
	float fSpeed = GetSpeed();										//����
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//�����̎擾

	//�R���g���[���[�iXInput�j
	float fRot = pXPad->GetStickRot(0, m_nControllerIndx);				//�X�e�B�b�N�̎擾
	if (pXPad->GetStick(0, m_nControllerIndx) == true)
	{
		//�ړ����
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
		{
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && (m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}

			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_move.x -= sinf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + fRot;
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && (m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_move.x -= sinf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + fRot;
			}
		}
	}
		//�v���C���[�̈ړ������i�\���L�[�j
	else if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_LEFT, m_nControllerIndx) == true)
	{
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
		{
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_UP, m_nControllerIndx) == true)
				{//�|���S��������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.25f);
				}
				else if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_DOWN, m_nControllerIndx) == true)
				{//�|���S���������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.5f);
				}
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && (m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_UP, m_nControllerIndx) == true)
				{//�|���S��������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.25f);
				}
				else if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_DOWN, m_nControllerIndx) == true)
				{//�|���S���������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�������Ɉړ�
					m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y - (D3DX_PI * 0.5f);
				}
			}

			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE)
				&& (m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
		}
	}
	else if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_RIGHT, m_nControllerIndx) == true)
	{
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
		{
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_UP, m_nControllerIndx) == true)
				{//�|���S�����E��Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.25f);
				}
				else if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_DOWN, m_nControllerIndx) == true)
				{//�|���S�����E���Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�����E�Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.5f);
				}
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && (m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_UP, m_nControllerIndx) == true)
				{//�|���S�����E��Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.25f);
				}
				else if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_DOWN, m_nControllerIndx) == true)
				{//�|���S�����E���Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.75f);
				}
				else
				{//�|���S�����E�Ɉړ�
					m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * (fSpeed * m_fSpeed);
					m_Angle.y = rot.y + (D3DX_PI * 0.5f);
				}
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER ||
				m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && (m_PlayerState != PLAYERSTATE_ACTION
					&& m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
		}
	}
	else if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_UP, m_nControllerIndx) == true)
	{
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
		{
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_move.x -= sinf(rot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(rot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = rot.y;
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_DAMAGE)
			{
				m_move.x -= sinf(rot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(rot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = rot.y;

			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER ||
				m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && (m_PlayerState != PLAYERSTATE_ACTION
					&& m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
		}
	}
	else if (pXPad->GetPress(XINPUT_GAMEPAD_DPAD_DOWN, m_nControllerIndx) == true)
	{
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE  && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
		{
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * (fSpeed * m_fSpeed);
				m_Angle.y = D3DX_PI + rot.y;
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_DAMAGE)
			{
				m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * (fSpeed * m_fSpeed);
				m_Angle.y = D3DX_PI + rot.y;
			}

			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER ||
				m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && (m_PlayerState != PLAYERSTATE_ACTION
					&& m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
		}
	}
	else
	{
		if (m_PlayerState == PLAYERSTATE_WALK || (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState == PLAYERSTATE_ACTION)
			&& (m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f)
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE018);
			//�j���[�g������Ԃɂ���
			m_PlayerState = PLAYERSTATE_NONE;
			m_pMotion->SetNumMotion(PLAYERSTATE_NONE);
		}
	}

	if (pXPad->GetTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER, m_nControllerIndx) == true)
	{
		//�e���g���[�擾�𒆒f
		Interruption();
	}

	if (m_bWalk == true)
	{//�ړ����[�V����
		if (m_PlayerState == PLAYERSTATE_WALK)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
			m_pMotion->SetNumMotion(PLAYERSTATE_WALK);
			m_bWalk = false;
		}
	}
	if (m_bSprintMotion == true)
	{//�X�v�����g���[�V����

		if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState == PLAYERSTATE_ACTION)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
			m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);
			m_bSprintMotion = false;
		}
	}
	//�ړ�����
	pos += m_move;

	//����
	m_move.x += (0.0f - m_move.x) * m_fInertia;
	m_move.z += (0.0f - m_move.z) * m_fInertia;

}

//=============================================================================
//�@�v���C���[��MOUSE�ł̈ړ�����
//=============================================================================
void  CPlayer::PlayerMoveMouse(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos)
{
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾

	//RawMouse�̎擾
	CRawMouse *pRawMouse = CManager::GetRawMouse();					

	//�}�E�X�̊p�x���擾
	int MouseX = pRawMouse->GetMouse(m_nControllerIndx).lLastX;
	int MouseY = pRawMouse->GetMouse(m_nControllerIndx).lLastY;

	//�}�E�X�p
	//float Mouse = atan2f((float)MouseX, (float)MouseY * -1.0f);

	//�g���b�N�{�[���p
	float Mouse = atan2f((float)MouseX * -1.0f, (float)MouseY * -1.0f);

	//�X�e�[�^�X�̎擾
	float fSpeed = GetSpeed();				//����
	//float fInertia = GetInertia();			//����

	if (pRawMouse->GetTrigger(m_nControllerIndx,CRawMouse::RIMS_SLIDE_UP) == true ||
		pRawMouse->GetTrigger(m_nControllerIndx, CRawMouse::RIMS_SLIDE_LEFT) == true || 
		pRawMouse->GetTrigger(m_nControllerIndx, CRawMouse::RIMS_SLIDE_RIGHT) == true || 
		pRawMouse->GetTrigger(m_nControllerIndx, CRawMouse::RIMS_SLIDE_DOWN) == true)
	{
		m_fMouse = Mouse;
		m_bMouse = true;
	}
	
	if (m_bMouse == true)
	{
		//�ړ����
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE && m_PlayerState != PLAYERSTATE_ROBOTDAMAGE)
		{
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER ||
				m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) && (m_PlayerState != PLAYERSTATE_ACTION
					&& m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				(m_PlayerState != PLAYERSTATE_ACTION && m_PlayerState != PLAYERSTATE_DAMAGE))
			{
				m_move.x -= sinf((m_fMouse + CameraRot.y)) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(m_fMouse + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + m_fMouse;
			}
			else if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_DAMAGE)
			{
				m_move.x -= sinf((m_fMouse + CameraRot.y)) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(m_fMouse + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + m_fMouse;
			}

			if (m_bWalk == true)
			{//�ړ����[�V����
				if (m_PlayerState == PLAYERSTATE_WALK)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
					m_pMotion->SetNumMotion(PLAYERSTATE_WALK);
					m_bWalk = false;
				}
			}
			if (m_bSprintMotion == true)
			{//�X�v�����g���[�V����
				if (m_PlayerState == PLAYERSTATE_ACTION)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
					m_pMotion->SetNumMotion(PLAYERSTATE_ACTION);
					m_bSprintMotion = false;
				}
			}
		}
		else
		{
			if (m_PlayerState == PLAYERSTATE_WALK || (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState == PLAYERSTATE_ACTION) &&
				(m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f)
				&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
			{
				pSound->StopSound(CSound::SOUND_LABEL_SE018);
				//�j���[�g������Ԃɂ���
				m_PlayerState = PLAYERSTATE_NONE;
				m_pMotion->SetNumMotion(PLAYERSTATE_NONE);
				m_bWalk = true;
			}
		}
	}

	if (pRawMouse->GetTrigger(m_nControllerIndx, CRawMouse::RIMS_BUTTON_LEFT) == true)
	{
		//�e���g���[�擾�𒆒f
		Interruption();
	}

	//�ړ�����
	pos += m_move;

	//����
	m_move.x += (0.0f - m_move.x) * m_fInertia;
	m_move.z += (0.0f - m_move.z) * m_fInertia;
}

//=============================================================================
//�@�v���C���[�ƃe���g���[�̓����蔻��
//=============================================================================
void CPlayer::Collision(void)
{
	//�e��l�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();
	D3DXVECTOR3 posOld = m_posOld;
	D3DXVECTOR3 Move = INITIALIZE_VECTOR3;
	D3DXVECTOR3 ColRange;
	//����J�n
	CCollider *pCol = NULL;
	CCollider *pColNext = NULL;

	for (int nCntType = 0; nCntType < CCollider::TYPE_MAX; nCntType++)
	{
		pCol = CCollider::GetTop(nCntType);

		while (pCol != NULL)
		{
			pColNext = pCol->GetNext();
			CollisionCollider(pCol, pos, posOld, m_move, D3DXVECTOR3(15.0f,15.0f,15.0f));
			pCol = pColNext;
		}
	}
	CCharacter::SetPos(pos);
}

//=============================================================================
//�@�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	if (pCollider->GetType() == CCollider::TYPE_BOX)
	{
		if (CollisionBoxCollider((CBoxCollider*)pCollider, pos, posOld, Move, ColRange) == true)
		{
		
		}
	}
	else if (pCollider->GetType() == CCollider::TYPE_CYLINDER)
	{
		if (CollisionCylinderyCollider((CCylinderCollider*)pCollider, pos, posOld, Move, ColRange) == true)
		{
		}
	}
	else if (pCollider->GetType() == CCollider::TYPE_SPHERE_PLAYERATTACK)
	{
		CScene *pParent = pCollider->GetParent();

		if (CollisionPlayerAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, ColRange) == true 
			&& pParent->GetObjType() != OBJTYPE_ROBOT)
		{
		}
		if (CollisionRobotAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, ColRange) == true 
			&& pParent->GetObjType() == OBJTYPE_ROBOT)
		{
		}
	}
	return false;
}

//=============================================================================
//�@�{�b�N�X�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;

	if (pBoxCollider->Collision(&pos, &posOld, &Move, ColRange, this, &bLand) == true)
	{
		if (bLand == true)
		{
			
		}
		return true;
	}
	return false;
}

//=============================================================================
//�@�V�����_�[�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;
	if (pCylinderCollider->Collision(&pos, &posOld, &Move, CYLINDER_COLRADIUS, CYLINDER_COLHEIGHT, this, &bLand) == true)
	{
		if (bLand == true)
		{

		}

		CScene *pParent = pCylinderCollider->GetParent();
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ROBOT)
		{// �e���G���v���C���[�������ꍇ�����𐁂���΂�
			CCharacter *pCharacter = (CCharacter*)pParent;
			if (pCharacter == NULL) { return true; }
			D3DXVECTOR3 AnotherPos = pCharacter->GetPos();
			m_nDamageCount = 0;
			if (m_bSuperArmor != true)
			{//�X�[�p�[�A�[�}��Ԃ���Ȃ��Ȃ�
				BlowAway(AnotherPos,0);
			}
		}
		else if (pParent->GetObjType() == OBJTYPE_MINE)
		{// �n����������
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this && pMine->GetType() == CMine::TYPE_NONE)
			{// �����ȊO���o�����n���Ȃ�N��
				pMine->SetType(CMine::TYPE_STARTUP);
			}
		}
		else if (pParent->GetObjType() == OBJTYPE_ROBOTBULLET)	
		{// �~�T�C����������
			CBullet *pBullet = (CBullet*)pParent;
			if (pBullet->GetType() == CBullet::TYPE_MOVE)
			{// ����
				pBullet->SetType(CBullet::TYPE_EXPLOSION);
			}
		}

		return true;
	}

	return false;
}

//=============================================================================
//�@�U�����Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	if (pShere->Collision(&pos, CYLINDER_COLRADIUS, this) == true && pShere->GetParent() != this)
	{// �����ȊO�̍U����������������
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_MINE)
		{// �n����������
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this)
			{// �����ȊO���o�����n���Ȃ�_���[�Wf

				//�������Ă�Ԃ̓_���[�W���
				m_PlayerState = PLAYERSTATE_DAMAGE;

				if (m_nDamageCounter == 0)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE025);	//�n����
					pSound->PlaySound(CSound::SOUND_LABEL_SE019);	//�_�E����
					m_bSuperArmor = true;
					m_pMotion->SetNumMotion(4);
					m_nDamageCounter = 1;
					if (m_nDownCount >= 1)
					{
						m_nDownCount = 0;
					}
				}
			}
		}
		if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ENEMY
			|| pParent->GetObjType() == OBJTYPE_ROBOTBULLET)
		{
			//�������Ă�Ԃ̓_���[�W���
			m_PlayerState = PLAYERSTATE_DAMAGE;
			if (m_nDamageCounter == 0)
			{
				if (pParent->GetObjType() == OBJTYPE_ROBOTBULLET)	
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE025);	//������
				}
				else
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE023);	//�Ռ��g��
				}
				pSound->PlaySound(CSound::SOUND_LABEL_SE019);	//�_�E����
				m_bSuperArmor = true;
				m_pMotion->SetNumMotion(4);
				m_nDamageCounter = 1;
				if (m_nDownCount >= 1)
				{
					m_nDownCount = 0;
				}
			}
		}

		return true;
	}

	return false;
}

//=============================================================================
//�@�U�����Ƃ̓����蔻�菈��(���{�b�g)���ǉ�(�悵�낤)
//=============================================================================
bool CPlayer::CollisionRobotAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
{
	if (pShere->Collision(&pos, 70.0f, this) == true && pShere->GetParent() != this)
	{// �����ȊO�̍U����������������
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_ROBOT)
		{
			//�������Ă�Ԃ̓_���[�W���
			m_PlayerState = PLAYERSTATE_ROBOTDAMAGE;

			if (m_nDamageCounter == 0)
			{
				m_bSuperArmor = true;
				m_pMotion->SetNumMotion(4);
				m_nDamageCounter = 1;
				if (m_nRobotDownCount >= 1)
				{
					m_nRobotDownCount = 0;
				}
				CCharacter *pCharacter = (CCharacter*)pParent;
				if (pCharacter == NULL) { return true; }
				D3DXVECTOR3 AnotherPos = pCharacter->GetPos();
				m_nDamageCount = 0;
				BlowAway(AnotherPos, 1);
			}
		}
		return true;
	}
	return false;
}

//=============================================================================
//�@������я���
//=============================================================================
void CPlayer::BlowAway(D3DXVECTOR3 AnotherPos, int Num)
{
	// ���W���擾
	D3DXVECTOR3 pos = GetPos();

	// ������Ԍ������v�Z
	m_fBlowAngle = CFunctionLib::CalcAngleToDest(pos.x, pos.z, AnotherPos.x, AnotherPos.z);

	// ������ԋ�����ݒ�	// ���ǉ�(�悵�낤)
	if (Num == 1)
	{// ���{�b�g�̍U���̂Ƃ�
		m_fBlowLength = BLOW_ROBOTMOVING;
	}
	else
	{// ���̑�
		m_fBlowLength = BLOW_PLAYERMOVING;
	}

	if (Num != 1)
	{
		// ������я�Ԃɂ���
		m_PlayerState = PLAYERSTATE_BLOWAWAY;
		m_pMotion->SetNumMotion(m_PlayerState);
	}

	// ������ς���
	float fRot = m_fBlowAngle + D3DX_PI;
	fRot = CFunctionLib::CheckRotationOverPiX(fRot);
	m_rot.y = fRot;
	m_Angle = D3DXVECTOR3(0.0f, fRot, 0.0f);
}

//=============================================================================
// ���C���̐�������
//=============================================================================
void CPlayer::CreateOrbitLine(void)
{
}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CPlayer::UninitOrtbitLine(void)
{
}

