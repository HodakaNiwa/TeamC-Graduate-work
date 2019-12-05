
//=============================================================================
//
//�@�`���[�g���A���v���C���[���� [tutorialplayer.cpp]
// Author : Kobayashi & Yamashita & Niwa
//
//=============================================================================
#include "tutorialplayer.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "game.h"
#include "model.h"
#include "Territory.h"
#include "library.h"
#include "line.h"
#include "loadEffect.h"
#include "UI.h"
#include "score.h"
#include "mine.h"
#include "debuglog.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// �R���W�����֌W
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)

//���X�g�̍ő吔
#define AIMPOS_LIST00	(4)
#define AIMPOS_LIST01	(7)
#define AIMPOS_LIST02	(3)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�ÓI�����o�ϐ��錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CTutorialPlayer::CTutorialPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority,objType)
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
	m_bWalk = true;
	m_fInertia = 0.0f;
	m_nDownCount = 0;
	m_pLoadEffect = NULL;
	m_bMovePlayer = false;
	m_bMoveEnd = false;
	m_nNumPos = 0;
	m_nNumList = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CTutorialPlayer::~CTutorialPlayer(){}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer,int nType)
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

	//�ړI�n�̐ݒ�
	SetAimPos();
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorialPlayer::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CTutorialPlayer::Uninit(void)
{
	//�L�����N�^�[�̔j��
	CCharacter::Uninit();

	if (m_pLoadEffect != NULL)
	{//�G�t�F�N�g�̔j��
		m_pLoadEffect->Uninit();
		delete m_pLoadEffect;
		m_pLoadEffect = NULL;
	}

	//���X�g�̔j��
	for (int nCnt = 0; nCnt < MAX_LIST; nCnt++)
	{
		for (int nCntList = 0; nCntList < m_AimPosList[nCnt].nMaxList; nCntList++)
		{
			delete[] m_AimPosList[nCnt].List;
			m_AimPosList[nCnt].List = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void  CTutorialPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();							//�T�E���h�̎擾

	//�L�����N�^�[�̍X�V
	CCharacter::Update();

	////�v���C���[�̏��
	//switch (m_PlayerState)
	//{
	//case PLAYERSTATE_NONE:			//�ʏ���
	//	m_bWalk = true;				//������p�ɂ���
	//	m_bSprintMotion = true;		//�X�v�����g�g�p��
	//	break;

	//case PLAYERSTATE_WALK:	//�ړ����
	//	break;

	//case PLAYERSTATE_ACTION: //�A�N�V�������
	//	break;

	//case PLAYERSTATE_BLOWAWAY:	//������΂���Ă���
	//							// �_���[�W�J�E���g���Z
	//	m_nDamageCount++;
	//	if (m_nDamageCount <= MAX_DAMAGECOUNT)
	//	{
	//		m_PlayerState = PLAYERSTATE_BLOWAWAY;
	//	}
	//	if (m_nDamageCount >= MAX_DAMAGECOUNT)
	//	{
	//		m_nDamageCount = 0;
	//		m_bWalk = true;
	//		m_bSprintMotion = true;
	//		m_PlayerState = PLAYERSTATE_NONE;
	//	}

	//	// ������΂�
	//	m_fBlowLength -= BLOW_MOVING_CUT;
	//	if (m_fBlowLength >= 0.0f)
	//	{
	//		m_move.x -= sinf(m_fBlowAngle) * m_fBlowLength;
	//		m_move.z -= cosf(m_fBlowAngle) * m_fBlowLength;
	//	}

	//	break;

	//case PLAYERSTATE_DAMAGE:
	//	// �_���[�W�J�E���g���Z
	//	m_nDownCount++;

	//	if (m_nDownCount == 70)
	//	{
	//		m_pMotion->SetNumMotion(5);
	//	}
	//	if (m_nDownCount >= 120)
	//	{//110�b�œ�����悤�ɂȂ�
	//		m_bWalk = true;
	//		m_bSprintMotion = true;
	//		m_nDownCount = 0;
	//		m_bSuperArmor = false;
	//		m_PlayerState = PLAYERSTATE_NONE;
	//		m_nDamageCounter = 0;
	//	}
	//	break;
	//}

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

	////�Q�[���̏�Ԃ��擾
	//int nGameState = CGame::GetGameState();

	//if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
	//{
	//	//�v���C���[�̈ړ�����
	//}
	//else if (nGameState == CGame::GAMESTATE_END)
	//{//�Q�[���I�����Ƀ��[�V�������j���[�g����
	//	if (m_bCharaMotionState == false)
	//	{
	//		pSound->StopSound(CSound::SOUND_LABEL_SE018);
	//		m_pMotion->SetNumMotion(0);
	//		m_bCharaMotionState = true;
	//	}
	//}

	//�ړ�����
	UpdateMoveTutorial();

#if _DEBUG
	DebugMove();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CTutorialPlayer::Draw(void)
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
CTutorialPlayer *CTutorialPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, TYPE type, CHARCTERTYPE charctertype)
{
	CTutorialPlayer *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//�������𓮓I�m��
		pPlayer = new CTutorialPlayer;
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
void  CTutorialPlayer::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size){}

//=============================================================================
//�@������я���
//=============================================================================
void CTutorialPlayer::BlowAway(D3DXVECTOR3 AnotherPos)
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
// �f�o�b�O�p�̈ړ�����
//=============================================================================
void CTutorialPlayer::DebugMove(void)
{
	CInputKeyboard * pKeyboard = CManager::GetInputkeyboard();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�ړ�����
	if (pKeyboard->GetKeyboardPress(DIK_UP) == true)
	{
		pos.z += 1.5f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_DOWN) == true)
	{
		pos.z -= 1.5f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_LEFT) == true)
	{
		pos.x -= 1.5f;
	}
	if (pKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
	{
		pos.x += 1.5f;
	}
	
	//�ʒu�ݒ�
	CCharacter::SetPos(pos);
}

//=============================================================================
// �ړI�n�̈ʒu�ݒ�
//=============================================================================
void CTutorialPlayer::SetAimPos(void)
{
	for (int nCnt = 0; nCnt < MAX_LIST; nCnt++)
	{
		m_AimPosList[nCnt].List = NULL;
		m_AimPosList[nCnt].nMaxList = 0;
	}

	//1�ڂ̃��X�g�쐬
	m_AimPosList[0].nMaxList = AIMPOS_LIST00;
	m_AimPosList[0].List = new D3DXVECTOR3[m_AimPosList[0].nMaxList];

	m_AimPosList[0].List[0] = D3DXVECTOR3(10.0f, 0.0f, 250.0f);
	m_AimPosList[0].List[1] = D3DXVECTOR3(-190.0f, 0.0f, 250.0f);
	m_AimPosList[0].List[2] = D3DXVECTOR3(-190.0f, 0.0f, 20.0f);
	m_AimPosList[0].List[3] = D3DXVECTOR3(20.0f, 0.0f, 20.0f);

	//�Q�ڂ̃��X�g���쐬
	m_AimPosList[1].nMaxList = AIMPOS_LIST01;
	m_AimPosList[1].List = new D3DXVECTOR3[m_AimPosList[1].nMaxList];

	m_AimPosList[1].List[0] = D3DXVECTOR3(-10.0f, 0.0f, -20.0f);
	m_AimPosList[1].List[1] = D3DXVECTOR3(-250.0f, 0.0f, -10.0f);
	m_AimPosList[1].List[2] = D3DXVECTOR3(-230.0f, 0.0f, 300.0f);
	m_AimPosList[1].List[3] = D3DXVECTOR3(150.0f, 0.0f, 300.0f);
	m_AimPosList[1].List[4] = D3DXVECTOR3(160.0f, 0.0f, 150.0f);
	m_AimPosList[1].List[5] = D3DXVECTOR3(140.0f, 0.0f, -10.0f);
	m_AimPosList[1].List[6] = D3DXVECTOR3(-60.0f, 0.0f, -20.0f);

	//�Q�ڂ̃��X�g���쐬
	m_AimPosList[2].nMaxList = AIMPOS_LIST02;
	m_AimPosList[2].List = new D3DXVECTOR3[m_AimPosList[2].nMaxList];

	m_AimPosList[2].List[0] = D3DXVECTOR3(-200.0f, 0.0f, 150.0f);
	m_AimPosList[2].List[1] = D3DXVECTOR3(-300.0f, 0.0f, 300.0f);
	m_AimPosList[2].List[2] = D3DXVECTOR3(0.0f, 0.0f, 380.0f);
}

//=============================================================================
// �`���[�g���A���p�̈ړ�����
//=============================================================================
void CTutorialPlayer::UpdateMoveTutorial(void)
{
	if (!m_bMovePlayer) { return; }
	
	//������Ԃɂ���
	SetMotion(PLAYERSTATE_WALK);

	//�ϐ��錾
	bool bBigerX = false;
	bool bBigerZ = false;
	bool bSetPotisionX = false;
	bool bSetPotisionZ = false;

	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CCharacter::GetPos();
	if (Pos.x > m_AimPosList[m_nNumList].List[m_nNumPos].x) { bBigerX = true; }
	if (Pos.z > m_AimPosList[m_nNumList].List[m_nNumPos].z) { bBigerZ = true; }

	//�ړI�l�܂ł̊p�x�����߂�
	float fAngle = CFunctionLib::CalcAngleToDest(Pos.x, Pos.z, m_AimPosList[m_nNumList].List[m_nNumPos].x, m_AimPosList[m_nNumList].List[m_nNumPos].z);

	//�ړ�����
	Pos.x += sinf(fAngle) * TUTORIALPLAYER_SPEED;
	Pos.z += cosf(fAngle) * TUTORIALPLAYER_SPEED;

	//�l�̏C��
	if (bBigerX)
	{//X�l���ړI�n���傫��
		if (Pos.x <= m_AimPosList[m_nNumList].List[m_nNumPos].x)
		{ 
			Pos.x = m_AimPosList[m_nNumList].List[m_nNumPos].x;
			bSetPotisionX = true;
		}
	}
	else
	{//X�l���ړI�n��菬����
		if (Pos.x >= m_AimPosList[m_nNumList].List[m_nNumPos].x) 
		{ 
			Pos.x = m_AimPosList[m_nNumList].List[m_nNumPos].x; 
			bSetPotisionX = true;
		}
	}

	if (bBigerZ)
	{//Y�l���ړI�n���傫��
		if (Pos.z <= m_AimPosList[m_nNumList].List[m_nNumPos].z) 
		{ 
			Pos.z = m_AimPosList[m_nNumList].List[m_nNumPos].z; 
			bSetPotisionZ = true;
		}
	}
	else
	{//Y�l���ړI�n��菬����
		if (Pos.z >= m_AimPosList[m_nNumList].List[m_nNumPos].z)
		{ 
			Pos.z = m_AimPosList[m_nNumList].List[m_nNumPos].z; 
			bSetPotisionZ = true;
		}
	}

	//�p�x�C��
	if (fAngle == 0.0f)
	{
		fAngle = D3DX_PI;
	}
	else if (fAngle >= D3DX_PI)
	{
		fAngle = 0.0f;
	}

	//�ʒu�ݒ�
	CCharacter::SetRot(D3DXVECTOR3(0.0f, -fAngle, 0.0f));
	CCharacter::SetPos(Pos);

	//�t���O����
	if (bSetPotisionX && bSetPotisionZ)
	{
		if (m_nNumPos != m_AimPosList[m_nNumList].nMaxList - 1)
		{//���X�g�̒l��菬����������
			m_nNumPos++;
		}
		else
		{//�ړI�n�܂ł��ǂ蒅������
			m_nNumPos = 0;
			m_bMovePlayer = false;
			m_bMoveEnd = true;
			SetMotion(PLAYERSTATE_NONE);	//�j���[�g�������[�V�����ɂ���
		}
	}
}

//=============================================================================
// ���[�V�����ݒ�
//=============================================================================
void CTutorialPlayer::SetMotion(PLAYERSTATE PlayerState)
{
	if (m_pMotion != NULL)
	{
		if (m_pMotion->GetNumMotion() != PlayerState)
		{
			m_pMotion->SetNumMotion(PlayerState);
			m_PlayerState = PlayerState;
		}
	}
}

//=============================================================================
// ���C���̐�������
//=============================================================================
void CTutorialPlayer::CreateOrbitLine(void){}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CTutorialPlayer::UninitOrtbitLine(void){}

