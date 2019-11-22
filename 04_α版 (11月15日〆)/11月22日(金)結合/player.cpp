
//=============================================================================
//
// ���f������ [model.cpp]
// Author : Kobayashi & Yamashita & Niwa
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
#include "inputmouce.h"
#include "RawMouse.h"
#include "UI.h"
#include "score.h"
#include "mine.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �R���W�����֌W
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)

//�F
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//��
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//��
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//�I�����W
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//��

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�ÓI�����o�ϐ��錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
{
	m_move = INITIALIZE_VECTOR3;
	m_Angle = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nDamageCount = 0;
	m_fBlowAngle = 0.0f;
	m_fBlowLength = 0.0f;
	m_rot = INITIALIZE_VECTOR3;
	m_posOld = INITIALIZE_VECTOR3;
	m_nDamageCounter = 0;
	m_PlayerState = PLAYERSTATE_NONE;
	m_bSuperArmor = false;
	m_bSprint = false;
	m_bSprintMotion = false;

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPlayer::~CPlayer()
{

}



//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType)
{
	//�v���C���[�̃i���o�[���擾
	m_nNumPlayer = nNumPlayer;

	//�ϐ��̏�����
	m_bWalk = true;

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
}

//=============================================================================
// �X�V����
//=============================================================================
void  CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾

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
	{//�Q�[���I�����Ƀ��[�V�������j���[�g����
		if (m_bCharaMotionState == false)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE018);
			m_pMotion->SetNumMotion(0);
			m_bCharaMotionState = true;
		}
	}
	
	//�v���C���[�̏��
	switch (m_PlayerState)
	{
	case PLAYERSTATE_NONE:	//�ʏ���

		//������p�ɂ���
		m_bWalk = true;
		m_bSprintMotion = true;

		break;

	case PLAYERSTATE_WALK:	//�ړ����

		//m_bSprintMotion = true;

		break;

	case PLAYERSTATE_ACTION: //�A�N�V�������
		//m_nDamageCounter = 0;

		//m_bWalk = true;

		break;

	case PLAYERSTATE_BLOWAWAY:	//������΂���Ă���
		// �_���[�W�J�E���g���Z
		m_nDamageCount++;

		if (m_nDamageCount >= MAX_DAMAGECOUNT)
		{
			m_nDamageCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;
			//if (m_bSprintMotion == true)
			{
				m_PlayerState = PLAYERSTATE_NONE;
				m_pMotion->SetNumMotion(m_PlayerState);
			}
		
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
		m_nDamageCount++;
		if (m_nDamageCount == 60)
		{//60�b��������N���オ��
			m_pMotion->SetNumMotion(5);
		}
		if (m_nDamageCount == 110)
		{//110�b�œ�����悤�ɂȂ�
			m_nDamageCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;
			m_nDamageCounter = 0;
			m_bSuperArmor = false;
			m_PlayerState = PLAYERSTATE_NONE;
		}
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CPlayer::Draw(void)
{
	//�L�����N�^�[�̕`��
	CCharacter::Draw();

	if (m_pLoadEffect != NULL)
	{//�G�t�F�N�g�̕`��
		m_pLoadEffect->Draw();
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
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	
	CManager *pManager = NULL;									
	CGame *pGame = pManager->GetGame();								//�Q�[���̎擾
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);				//�Q�[���J�����̎擾
	D3DXVECTOR3 rot = pGameCamera->GetRot();						//�����̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();							//�ʒu�̎擾

																	//�X�e�[�^�X�̎擾

	float fSpeed = GetSpeed();				//����
	float fInertia = GetInertia();			//����

	
	//�O��̈ʒu���i�[
	m_posOld = pos;

	if (pInputKeyboard->GetKeyboardPress(DIK_H) == true)
	{
		m_nControllerType = 0;
	}
	if (pInputKeyboard->GetKeyboardPress(DIK_J) == true)
	{
		m_nControllerType = 1;
	}

	//�v���C���[�̈ړ������i�L�[�{�[�h�j
	if (pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//�|���S��������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//�|���S���������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.75f);
		}
		else
		{//�|���S�������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * fSpeed;
			m_Angle.y = rot.y - (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//�|���S�����E��Ɉړ�
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * fSpeed;
			m_Angle.y = rot.y + (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//�|���S�����E���Ɉړ�
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * fSpeed;
			m_Angle.y = rot.y + (D3DX_PI * 0.75f);
		}
		else
		{//�|���S�����E�Ɉړ�
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * fSpeed;
			m_Angle.y = rot.y + (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
	{
		m_move.x -= sinf(rot.y) * fSpeed;
		m_move.z -= cosf(rot.y) * fSpeed;
		m_Angle.y = rot.y;
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
	{
		m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_Angle.y = D3DX_PI + rot.y;
	}

	

	if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true)
	{
		//�X�s�[�hUP�̎��͂�����
		m_pLoadEffect->OnTimeEffect();
	}

	if (m_nControllerType == 0)
	{
		PlayerMovePad(rot, pos);
	}
	else if (m_nControllerType == 1)
	{
		PlayerMoveMouse(rot, pos);

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
	
	//�ړ�����
	pos += m_move;

	//����
	m_move.x += (0.0f - m_move.x) * fInertia;
	m_move.z += (0.0f - m_move.z) * fInertia;

	//�ʒu��ݒ�
	CCharacter::SetPos(pos);

	//������ݒ�
	CCharacter::SetRot(m_rot);

	//�{�����[���̐ݒ�
	pSound->SetVolume(CSound::SOUND_LABEL_SE018, 50.0f);
}

//=============================================================================
//�@�v���C���[��PAD�ł̈ړ�����
//=============================================================================
void  CPlayer::PlayerMovePad(D3DXVECTOR3 &CameraRot, D3DXVECTOR3 &pos)
{
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾
	CInputXPad * pXPad = CManager::GetXPad();						//Xinput�̎擾

	//�X�e�[�^�X�̎擾
	int   nAttack = GetAttack();			//�U����
	float fAttackSpeed = GetAttackSpeed();	//�U���X�s�[�h
	float fRange = GetRange();				//�U���͈�
	float fSpeed = GetSpeed();				//����
	float fInertia = GetInertia();			//����
	float fLineTime = GetLineTime();		//���C���������鎞��


	//�R���g���[���[�iXInput�j
	float fRot = pXPad->GetStickRot(0, m_nNumPlayer);				//�X�e�B�b�N�̎擾
	if (pXPad->GetStick(0, m_nNumPlayer) == true)
	{
		//�ړ����
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY && m_PlayerState != PLAYERSTATE_DAMAGE)
		{
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			else if((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				m_PlayerState != PLAYERSTATE_ACTION)
			{
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}
			
			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				m_PlayerState != PLAYERSTATE_ACTION)
			{
				m_move.x -= sinf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + fRot;
			}
			
			else if(m_CharcterType == CCharacter::CHARCTERTYPE_SPEED)
			{
				m_move.x -= sinf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(fRot + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + fRot;
			}
			//�ړ��G�t�F�N�g
			m_pLoadEffect->SetPos(D3DXVECTOR3(pos.x, pos.y + 20.0f, pos.z));
			//�ʏ�̈ړ��͂�����
			m_pLoadEffect->Update();
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
			//m_bWalk = true;
		}
	}
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
	int MouseX = pRawMouse->GetMouse(m_nNumPlayer).lLastX;
	int MouseY = pRawMouse->GetMouse(m_nNumPlayer).lLastY;

	// �ڑ�����Ă���}�E�X�̐����擾
	int nMouseNum = pRawMouse->GetMouseNum();

	//�}�E�X�p
	//float Mouse = atan2f((float)MouseX, (float)MouseY * -1.0f);

	//�g���b�N�{�[���p
	float Mouse = atan2f((float)MouseX * -1.0f, (float)MouseY * -1.0f);

	//�X�e�[�^�X�̎擾
	int   nAttack = GetAttack();			//�U����
	float fAttackSpeed = GetAttackSpeed();	//�U���X�s�[�h
	float fRange = GetRange();				//�U���͈�
	float fSpeed = GetSpeed();				//����
	float fInertia = GetInertia();			//����
	float fLineTime = GetLineTime();		//���C���������鎞��

	if (MouseX != 0 || MouseY != 0)
	{//�}�E�X

		if (m_PlayerState != PLAYERSTATE_BLOWAWAY || m_PlayerState != PLAYERSTATE_DAMAGE)
		{
			if (m_PlayerState != PLAYERSTATE_ACTION && m_bSprint != true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_WALK;
			}
			if (m_bSprint == true)
			{
				//�ړ����
				m_PlayerState = PLAYERSTATE_ACTION;
			}

			if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
				m_PlayerState != PLAYERSTATE_ACTION)
			{//�p���[�^�������͒n���^���A�N�V����������Ȃ�������
				m_move.x -= sinf((Mouse + CameraRot.y)) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(Mouse + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + Mouse;
			}
			else
			{
				m_move.x -= sinf((Mouse + CameraRot.y)) * (fSpeed * m_fSpeed);
				m_move.z -= cosf(Mouse + CameraRot.y) * (fSpeed * m_fSpeed);
				m_Angle.y = CameraRot.y + Mouse;
			}

			//�ړ��G�t�F�N�g
			m_pLoadEffect->SetPos(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z));
			//�ʏ�̈ړ��͂�����
			m_pLoadEffect->Update();

			//�X�s�[�hUP�̎��͂�����
			//m_pLoadEffect->OnTimeEffect();
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
		if ((m_PlayerState == PLAYERSTATE_WALK || m_PlayerState == PLAYERSTATE_ACTION) && (m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f)
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE018);
			//�j���[�g������Ԃɂ���
			m_PlayerState = PLAYERSTATE_NONE;
			m_pMotion->SetNumMotion(PLAYERSTATE_NONE);
		}
	}
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
		if (CollisionPlayerAttackSphereCollider((CPlayerAttackSphereCollider*)pCollider, pos, ColRange) == true)
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
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER)
		{// �e���G���v���C���[�������ꍇ�����𐁂���΂�
			CCharacter *pCharacter = (CCharacter*)pParent;
			if (pCharacter == NULL) { return true; }
			D3DXVECTOR3 AnotherPos = pCharacter->GetPos();

			if (m_bSuperArmor != true)
			{//�X�[�p�[�A�[�}��Ԃ���Ȃ��Ȃ�
				BlowAway(AnotherPos);
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

		return true;
	}

	return false;
}

//=============================================================================
//�@�U�����Ƃ̓����蔻�菈��
//=============================================================================
bool CPlayer::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
{
	m_nDamageCounter;
	if (pShere->Collision(&pos, CYLINDER_COLRADIUS, this) == true && pShere->GetParent() != this)
	{// �����ȊO�̍U����������������
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_MINE)
		{// �n����������
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this)
			{// �����ȊO���o�����n���Ȃ�_���[�W

				//�������Ă�Ԃ̓_���[�W���
				m_PlayerState = PLAYERSTATE_DAMAGE;

				if (m_nDamageCounter == 0)
				{
					m_bSuperArmor = true;
					m_pMotion->SetNumMotion(4);
					m_nDamageCounter = 1;
				}
			}
		}
		if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ENEMY)
		{
			//if (m_bCollision == true)
			{
				m_pMotion->SetNumMotion(4);
			}

		}
		if (pParent->GetObjType() == OBJTYPE_PLAYER)
		{
			//m_pMotion->SetNumMotion(3);
		}
		return true;
	}

	return false;
}

//=============================================================================
//�@������я���
//=============================================================================
void CPlayer::BlowAway(D3DXVECTOR3 AnotherPos)
{
	// ���W���擾
	D3DXVECTOR3 pos = GetPos();

	// ������Ԍ������v�Z
	m_fBlowAngle = CFunctionLib::CalcAngleToDest(pos.x, pos.z, AnotherPos.x, AnotherPos.z);

	// ������ԋ�����ݒ�
	m_fBlowLength = BLOW_MOVING;

	// ������я�Ԃɂ���
	m_PlayerState = PLAYERSTATE_BLOWAWAY;
	m_pMotion->SetNumMotion(PLAYERSTATE_BLOWAWAY);

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
void CPlayer::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
}

