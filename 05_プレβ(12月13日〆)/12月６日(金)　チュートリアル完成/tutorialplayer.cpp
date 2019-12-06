
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
#include "collision.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TUTORIALPLAYER_SPEED	(3.2f) //3.2

//���X�g�̍ő吔
#define AIMPOS_LIST00	(4)
#define AIMPOS_LIST01	(7)
#define AIMPOS_LIST02	(3)
#define AIMPOS_LIST03	(1)
#define AIMPOS_LIST04	(1)
#define AIMPOS_LIST05	(2)
#define AIMPOS_LIST06	(1)
#define AIMPOS_LIST07	(1)

// �R���W�����֌W
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)

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
	m_fSpeed = TUTORIALPLAYER_SPEED;
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

	//�v���C���[�̏��
	switch (m_PlayerState)
	{
	case PLAYERSTATE_DAMAGE:
		// �_���[�W�J�E���g���Z
		m_nDownCount++;

		if (m_nDownCount == 70)
		{
			m_pMotion->SetNumMotion(5);
		}
		if (m_nDownCount >= 120)
		{//110�b�œ�����悤�ɂȂ�
			m_bWalk = true;
			m_bSprintMotion = true;
			m_nDownCount = 0;
			m_PlayerState = PLAYERSTATE_NONE;
			m_nDamageCounter = 0;
		}
		break;
	}

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
	if (GetDeth() == true) { return; }

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

	//3�ڂ̃��X�g���쐬
	m_AimPosList[2].nMaxList = AIMPOS_LIST02;
	m_AimPosList[2].List = new D3DXVECTOR3[m_AimPosList[2].nMaxList];

	m_AimPosList[2].List[0] = D3DXVECTOR3(-200.0f, 0.0f, 150.0f);
	m_AimPosList[2].List[1] = D3DXVECTOR3(-300.0f, 0.0f, 300.0f);
	m_AimPosList[2].List[2] = D3DXVECTOR3(0.0f, 0.0f, 380.0f);

	//4�ڂ̃��X�g���쐬
	m_AimPosList[3].nMaxList = AIMPOS_LIST03;
	m_AimPosList[3].List = new D3DXVECTOR3[m_AimPosList[3].nMaxList];

	m_AimPosList[3].List[0] = D3DXVECTOR3(250.0f, 0.0f, 0.0f);

	//5�ڂ̃��X�g���쐬
	m_AimPosList[4].nMaxList = AIMPOS_LIST04;
	m_AimPosList[4].List = new D3DXVECTOR3[m_AimPosList[4].nMaxList];

	m_AimPosList[4].List[0] = D3DXVECTOR3(-300.0f, 0.0f, 300.0f);

	//6�ڂ̃��X�g���쐬
	m_AimPosList[5].nMaxList = AIMPOS_LIST05;
	m_AimPosList[5].List = new D3DXVECTOR3[m_AimPosList[5].nMaxList];

	m_AimPosList[5].List[0] = D3DXVECTOR3(250.0f, 0.0f, 0.0f);
	m_AimPosList[5].List[1] = D3DXVECTOR3(-600.0f, 0.0f, 150.0f);

	//7�ڂ̃��X�g���쐬
	m_AimPosList[6].nMaxList = AIMPOS_LIST06;
	m_AimPosList[6].List = new D3DXVECTOR3[m_AimPosList[6].nMaxList];

	m_AimPosList[6].List[0] = D3DXVECTOR3(0.0f, 0.0f, 180.0f);

	//8�ڂ̃��X�g���쐬
	m_AimPosList[7].nMaxList = AIMPOS_LIST07;
	m_AimPosList[7].List = new D3DXVECTOR3[m_AimPosList[7].nMaxList];

	m_AimPosList[7].List[0] = D3DXVECTOR3(-600.0f, 0.0f, 150.0f);
}

//=============================================================================
// �`���[�g���A���p�̈ړ�����
//=============================================================================
void CTutorialPlayer::UpdateMoveTutorial(void)
{
	if (!m_bMovePlayer) { return; }
	if (m_pMotion == NULL) { return; }

	int nNumMotion = m_pMotion->GetNumMotion();
	if (nNumMotion == PLAYERSTATE_DAMAGE || nNumMotion == PLAYERSTATE_MAX) { return; }
	
	//������Ԃɂ���
	SetMotion(PLAYERSTATE_WALK);

	//�ϐ��錾
	bool bBigerX = false;
	bool bBigerZ = false;
	bool bSetPotisionX = false;
	bool bSetPotisionZ = false;

	if(m_AimPosList[m_nNumList].List == NULL){return;}

	//�ʒu�̎擾
	D3DXVECTOR3 Pos = CCharacter::GetPos();
	if (Pos.x > m_AimPosList[m_nNumList].List[m_nNumPos].x) { bBigerX = true; }
	if (Pos.z > m_AimPosList[m_nNumList].List[m_nNumPos].z) { bBigerZ = true; }

	//�ړI�l�܂ł̊p�x�����߂�
	float fAngle = CFunctionLib::CalcAngleToDest(Pos.x, Pos.z, m_AimPosList[m_nNumList].List[m_nNumPos].x, m_AimPosList[m_nNumList].List[m_nNumPos].z);

	//�ړ�����
	Pos.x += sinf(fAngle) * m_fSpeed;
	Pos.z += cosf(fAngle) * m_fSpeed;

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
	else if (fAngle == D3DX_PI)
	{
		fAngle = 0.0f;
	}
	else
	{
		fAngle += D3DX_PI;
	}

	//�ʒu�ݒ�
	CCharacter::SetRot(D3DXVECTOR3(0.0f, fAngle, 0.0f));
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
//�@�R���C�_�[�Ƃ̔���
//=============================================================================
void CTutorialPlayer::Collision(void)
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
			CollisionCollider(pCol, pos, posOld, m_move, D3DXVECTOR3(15.0f, 15.0f, 15.0f));
			pCol = pColNext;
		}
	}
	CCharacter::SetPos(pos);
}

//=============================================================================
//�@�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CTutorialPlayer::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
	}
	return false;
}

//=============================================================================
//�@�{�b�N�X�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CTutorialPlayer::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
bool CTutorialPlayer::CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
bool CTutorialPlayer::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
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
			{// �����ȊO���o�����n���Ȃ�_���[�W

			 //�������Ă�Ԃ̓_���[�W���
				m_PlayerState = PLAYERSTATE_DAMAGE;

				if (m_nDamageCounter == 0)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE025);	//�n����
					pSound->PlaySound(CSound::SOUND_LABEL_SE019);	//�_�E����
																	/*pSound->SetVolume(CSound::SOUND_LABEL_SE025, 20.0f);
																	pSound->SetVolume(CSound::SOUND_LABEL_SE019, 20.0f);*/
					m_pMotion->SetNumMotion(4);
					m_nDamageCounter = 1;
				}
			}
		}
		if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ENEMY)
		{

			//�������Ă�Ԃ̓_���[�W���
			m_PlayerState = PLAYERSTATE_DAMAGE;
			if (m_nDamageCounter == 0)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE023);	//�Ռ��g��
				pSound->PlaySound(CSound::SOUND_LABEL_SE019);	//�_�E����
																/*pSound->SetVolume(CSound::SOUND_LABEL_SE023, 20.0f);
																pSound->SetVolume(CSound::SOUND_LABEL_SE019, 20.0f);*/
				m_pMotion->SetNumMotion(4);
				m_nDamageCounter = 1;
			}

		}

		return true;
	}

	return false;
}


//=============================================================================
// ���C���̐�������
//=============================================================================
void CTutorialPlayer::CreateOrbitLine(void){}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CTutorialPlayer::UninitOrtbitLine(void){}