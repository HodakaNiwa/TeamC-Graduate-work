
//=============================================================================
//
// ���f������ [model.cpp]
// Author : Kobayashi & Yamashita
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
#include "player.h"
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
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)

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

//���̐F�̐ݒ�
D3DXCOLOR CPlayer::m_CountryColor[CPlayer::TYPE_MAX] =
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

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_Angle = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_nDamageCount = 0;
	m_fBlowAngle = 0.0f;
	m_fBlowLength = 0.0f;
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
HRESULT CPlayer::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer)
{
	//�v���C���[�̃i���o�[���擾
	m_nNumPlayer = nNumPlayer;

	//������
	m_bWalk = true;

	//�L�����N�^�[�̏�����
	CCharacter::Init(ModelTxt, MotionTxt);
	CCharacter::SetPos(pos);

	//���f���̏����擾
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

	//���[�V�����̏����擾
	m_pMotion = CCharacter::GetMotion();

	//�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	////�G�t�F�N�g�̏�����
	//if (m_pLoadEffect == NULL)
	//{
	//	m_pLoadEffect = CLoadEffect::Create(1, pos, 8);
	//}

	ResetLine();	//���C���̏�����

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
	CCharacter::Uninit();

	if (m_pLoadEffect != NULL)
	{
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

	CCharacter::Update();

	//���C���̃J�E���g�_�E������
	CountDownLineTime();

	int nGameState = CManager::GetGameState();

	if (nGameState != 1 && nGameState != 3)
	{
		//�v���C���[�̈ړ�����
		PlayerMove();
	}
	else if(nGameState == 4)
	{

	}

	switch (m_PlayerState)
	{
	case PLAYERSTATE_NONE:

		//������p�ɂ���
		m_bWalk = true;
		break;

	case PLAYERSTATE_WALK:
		break;

	case PLAYERSTATE_BLOWAWAY:
		// �_���[�W�J�E���g���Z
		m_nDamageCount++;
		if (m_nDamageCount >= MAX_DAMAGECOUNT)
		{
			m_nDamageCount = 0;
			m_bWalk = true;
			m_PlayerState = PLAYERSTATE_NONE;
			m_pMotion->SetNumMotion(m_PlayerState);
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

	//���Ԃ̉Z
	if (m_bBlockStartTerritory)
	{
		m_nCountTime++;
		if ((m_nCountTime % NOT_GETTIME) == 0) { m_bBlockStartTerritory = false; }
	}

	//�}�`������������̏���
	UpdateShapeComplete();
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CPlayer::Draw(void)
{
	CCharacter::Draw();

	if (m_pLoadEffect != NULL)
	{
		m_pLoadEffect->Draw();
	}
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nNumPlayer, TYPE type, PLAYERTYPE playertype)
{
	CPlayer *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		//�������𓮓I�m��
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{//NULL�`�F�b�N
			pPlayer->SetType(type);
			pPlayer->m_PlayerType = playertype;
			pPlayer->Init(pos, ModelTxt,MotionTxt, nNumPlayer);
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
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CGamePad * pGamePad = CManager::GetInputGamePad();				//�Q�[���p�b�h�̎擾
	CInputXPad * pXPad = CManager::GetXPad();
	float fRot = pXPad->GetStickRot(0, m_nNumPlayer);
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CRawMouse *pRawMouse = CManager::GetRawMouse();

	int MouseX = pRawMouse->GetMouse(0).lLastX;
	int MouseY = pRawMouse->GetMouse(0).lLastY;
	int MouseZ = pInputMouse->GetIZ();

	//�}�E�X�p
	float Mouse = atan2f((float)MouseX, (float)MouseY * -1.0f);

	//�g���b�N�{�[���p
	//float Mouse = atan2f((float)MouseX * -1.0f, (float)MouseY * -1.0f);


	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);

	//�����ƈʒu�̎擾
	D3DXVECTOR3 rot = pGameCamera->GetRot();
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�X�e�[�^�X�̎擾
	int   nAttack = GetAttack();
	float fAttackSpeed = GetAttackSpeed();
	float fRange = GetRange();
	float fSpeed = GetSpeed();
	float fInertia = GetInertia();
	float fLineTime = GetLineTime();

	//�O��̈ʒu���i�[
	m_posOld = pos;

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
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.5f)) * SPEED;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.5f)) * SPEED;
			m_Angle.y = rot.y + (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
	{
		m_move.x -= sinf(rot.y) * SPEED;
		m_move.z -= cosf(rot.y) * SPEED;
		m_Angle.y = rot.y;
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
	{
		m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * fSpeed;
		m_Angle.y = D3DX_PI + rot.y;
	}

	//�R���g���[���[�iXInput�j
	if (pXPad->GetStick(0, m_nNumPlayer) == true)
	{

		//�ړ����
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			m_PlayerState = PLAYERSTATE_WALK;
			m_move.x -= sinf(fRot + rot.y) * fSpeed;
			m_move.z -= cosf(fRot + rot.y) * fSpeed;
			m_Angle.y = rot.y + fRot;

			//�ړ��G�t�F�N�g
			m_pLoadEffect->SetPos(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z));
			//�ʏ�̈ړ��͂�����
			m_pLoadEffect->Update();
		}

		//����
		if (m_bWalk == true)
		{//�ړ����[�V����
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);

			m_pMotion->SetNumMotion(1);
			m_bWalk = false;
		}
	}
	else if (MouseX != 0 || MouseY != 0)
	{//�}�E�X
		// �ړ����
		if (m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			m_PlayerState = PLAYERSTATE_WALK;
			m_move.x -= sinf((Mouse + rot.y)) * fSpeed;
			m_move.z -= cosf(Mouse + rot.y) * fSpeed;
			m_Angle.y = rot.y + Mouse;

			//�ړ��G�t�F�N�g
			m_pLoadEffect->SetPos(D3DXVECTOR3(pos.x, pos.y + 60.0f, pos.z));
			//�ʏ�̈ړ��͂�����
			m_pLoadEffect->Update();
		}

		if (m_bWalk == true)
		{//�ړ����[�V����
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);
			m_pMotion->SetNumMotion(1);
			m_bWalk = false;
		}

		//�X�s�[�hUP�̎��͂�����
		//m_pLoadEffect->OnTimeEffect();
	}
	else
	{
		if (m_PlayerState == PLAYERSTATE_WALK && (m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f)
			&& m_PlayerState != PLAYERSTATE_BLOWAWAY)
		{
			pSound->StopSound(CSound::SOUND_LABEL_SE018);
			//�j���[�g������Ԃɂ���
			m_PlayerState = PLAYERSTATE_NONE;
			m_pMotion->SetNumMotion(0);
		}
	}
	if (pInputKeyboard->GetKeyboardTrigger(DIK_2) == true)
	{
		//�X�s�[�hUP�̎��͂�����
		m_pLoadEffect->OnTimeEffect();
	}
	D3DXVECTOR3 DiffAngle;

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

	// �ړ�������
	pos += m_move;

	//����
	m_move.x += (0.0f - m_move.x) * fInertia;
	m_move.z += (0.0f - m_move.z) * fInertia;

	CCharacter::SetPos(pos);
	CCharacter::SetRot(m_rot);
	pSound->SetVolume(CSound::SOUND_LABEL_SE018, 50.0f);
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
		if (CollisionCylinderCollider((CCylinderCollider*)pCollider, pos, posOld, Move, ColRange) == true)
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
bool CPlayer::CollisionCylinderCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
{
	bool bLand = false;
	if (pCylinderCollider->Collision(&pos, &posOld, &Move, CYLINDER_COLRADIUS, CYLINDER_COLHEIGHT, this, &bLand) == true)
	{
		if (bLand == true)
		{

		}

		// �e���G���v���C���[�������ꍇ�����𐁂���΂�
		CScene *pParent = pCylinderCollider->GetParent();
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER)
		{
			CCharacter *pCharacter = (CCharacter*)pParent;
			if (pCharacter == NULL) { return true; }
			D3DXVECTOR3 AnotherPos = pCharacter->GetPos();
			BlowAway(AnotherPos);
		}
		else if (pParent->GetObjType() == OBJTYPE_MINE)
		{// �n����������
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this)
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
	if (pShere->Collision(&pos, CYLINDER_COLRADIUS, this) == true && pShere->GetParent() != this)
	{// �����ȊO�̍U����������������
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_MINE)
		{// �n����������
			CMine *pMine = (CMine*)pParent;
			if (pMine->GetParentChara() != this)
			{// �����ȊO���o�����n���Ȃ�_���[�W

			}
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
// �e���g���[�̎擾�X�V����
//=============================================================================
void CPlayer::CountDownLineTime(void)
{
	if (m_apTerritory[TERRITORY_TOP] != NULL)
	{
		//����`�����Ԃ����炷
		m_nLineTime--;

		if (m_nLineTime < 0)
		{
			m_nLineTime = m_nMaxLineTime;					//�`��ł��鎞�Ԃ����Z�b�g����
			GetTerritory(m_apTerritory[TERRITORY_TOP]);		//�N�_�̂ݓ_��������
			ResetList();									//���X�g������������
			UninitOrtbitLine();								//��Ղ̔j��
			UninitLine();										//���C���̔j��
		}
	}
}

//=============================================================================
//�@�}�`������������̏���
//=============================================================================
void CPlayer::UpdateShapeComplete(void)
{
	if (!m_bMakeShape) { return; }

	//���Ԃ̉Z
	m_nCntTimeCopyLine++;

	if ((m_nCntTimeCopyLine % LINE_TIME) == 0)
	{
		m_bMakeShape = false;
		UninitCopyLine();
	}
}

//=============================================================================
// �e���g���[�����X�g�ɒǉ�
//=============================================================================
void CPlayer::AddTerritoryList(CTerritory * pTerritory)
{
	//�}�`���������Ă��邩�ǂ����̔���
	if (m_nCountTerritory >= MIN_TERRITORY)
	{
		if (true == ChackSamePos(pTerritory, TERRITORY_TOP))
		{
			ShapeComplete(pTerritory);	//�}�`�����������Ƃ��̏���
			ChackInShape();				//�}�`���Ƀe���g���[���邩����
			CreateEndLine();			//�Ō�̐��̐���
			ResetList();				//���X�g�̏�����
			UninitOrtbitLine();			//��Ղ̔j��
			ResetLine();				//���C���̏�����
			return;
		}
	}

	//���X�g�̒ǉ�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apTerritory[nCnt] == NULL)
		{
			UninitOrtbitLine();			//��Ղ̔j��

			if (m_bBlockStartTerritory)
			{//�擾��A�����N�_�����Ȃ��悤�ɂ���
				D3DXVECTOR3 Pos = pTerritory->GetPos();
				D3DXVECTOR3 OldStartPos = INITIALIZE_VECTOR3;
				if (m_pOldStartTerritory != NULL) { OldStartPos = m_pOldStartTerritory->GetPos(); }

				if ((nCnt != TERRITORY_TOP) && (Pos != OldStartPos))
				{
					m_apTerritory[nCnt] = pTerritory;
					m_nCountTerritory++;
					CreateOrbitLine();			//��Ղ̐���
				}
			}
			else
			{//�ʏ�
				m_apTerritory[nCnt] = pTerritory;
				m_nCountTerritory++;
				CreateOrbitLine();				//��Ղ̐���
			}

			//���C���𐶐�����
			if ((nCnt != 0))
			{
				if (m_apLine[nCnt - 1] == NULL)
				{
					if ((m_apTerritory[nCnt - 1] != NULL) && (m_apTerritory[nCnt] != NULL))
					{
						m_apLine[nCnt - 1] = CLine::Create(m_apTerritory[nCnt - 1]->GetPos(), m_apTerritory[nCnt]->GetPos(), m_nNumPlayer, nCnt - 1);
					}
				}
			}
			break;
		}
		else if (m_apTerritory[nCnt] != NULL)
		{//�����e���g���[�̏��ł͂Ȃ����ǂ���
			if (true == ChackSamePos(pTerritory, nCnt)) { break; }
		}
	}
}

//=============================================================================
// �e���g���[�Ő}�`���o���Ă��邩�ǂ���
//=============================================================================
bool CPlayer::ChackSamePos(CTerritory * pTerritory, int nNumTerritory)
{
	//�ϐ��錾
	bool bMakeShape = false;

	D3DXVECTOR3 TopPos = m_apTerritory[nNumTerritory]->GetPos();
	D3DXVECTOR3 CurPos = pTerritory->GetPos();

	if (TopPos == CurPos) { bMakeShape = true; }

	return bMakeShape;
}

//=============================================================================
// �e���g���[�����������Ƃ��̏���
//=============================================================================
void CPlayer::ShapeComplete(CTerritory * pTerritory)
{
	//�}�`������
	m_bBlockStartTerritory = true;
	m_pOldStartTerritory = pTerritory;
	m_bMakeShape = true;
	m_nLineTime = m_nMaxLineTime;		//�`��ł��鎞�Ԃ����Z�b�g����

										//�e���g���[�̎擾
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL) { GetTerritory(m_apTerritory[nCnt]); }
	}
}

//=============================================================================
// �}�`���Ƀe���g���[�����邩�ǂ���
//=============================================================================
void CPlayer::ChackInShape(void)
{
	//�ϐ��錾
	D3DXVECTOR3 * pListPos = new D3DXVECTOR3[m_nCountTerritory];
	D3DXVECTOR3 * pVecA = new D3DXVECTOR3[m_nCountTerritory];
	bool bEnter = false;

	//�ʒu���擾
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL)
		{
			pListPos[nCnt] = m_apTerritory[nCnt]->GetPos();
		}
	}

	//------------------------------
	// �����v���̔���
	//------------------------------
	//VecA�����߂�
	for (int nCntVec = 0; nCntVec < m_nCountTerritory; nCntVec++)
	{
		int nCnt = nCntVec + 1;
		if ((nCnt % m_nCountTerritory) == 0) { nCnt = 0; }

		pVecA[nCntVec] = pListPos[nCnt] - pListPos[nCntVec];
	}

	//�e���g���[���擾����
	CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY); //�I�u�W�F�N�g�|�C���^�̐擪���擾
	CScene * pScene = pSceneTop;							 //�g�b�v�̈ʒu��ۑ�����
	CTerritory * m_pTerritory = NULL;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			CTerritory * pTerritory = (CTerritory *)pScene;

			if (pTerritory->GetPlayerNumber() != m_nNumPlayer)
			{
				D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();
				int nCountEnter = 0;

				for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
				{
					if (m_apTerritory[nCnt] != NULL)
					{
						D3DXVECTOR3 VecB = TerritoryPos - pListPos[nCnt];

						//�O�ς����߂�
						float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pVecA[nCnt].x, pVecA[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

						//�x�N�g�����傫�����ǂ���
						if (fCross >= 0)
						{
							nCountEnter++;
						}
						else
						{
							break;
						}
					}
				}

				//�͈͓��Ȃ�e���g���[�̎擾
				if (nCountEnter == m_nCountTerritory)
				{
					GetTerritory(pTerritory);
					bEnter = true;
				}
			}
		}

		pScene = pSceneNext;	//�|�C���^��i�߂�
	}

	if (bEnter)
	{
		//�������̔j��
		if (pListPos != NULL)
		{
			delete[] pListPos;
			pListPos = NULL;
		}
		if (pVecA != NULL)
		{
			delete[] pVecA;
			pVecA = NULL;
		}
		return;
	}

	//------------------------------
	// ���v���̔���
	//------------------------------

	//VecA�����߂�
	for (int nCntVec = m_nCountTerritory - 1; nCntVec >= 0; nCntVec--)
	{
		int nCnt = nCntVec - 1;
		if (nCntVec == 0) { nCnt = m_nCountTerritory - 1; }
		pVecA[nCntVec] = pListPos[nCnt] - pListPos[nCntVec];
	}

	//�e���g���[���擾����
	pSceneTop = CScene::GetTop(TERRITORY_PRIORITY); //�I�u�W�F�N�g�|�C���^�̐擪���擾
	pScene = pSceneTop;							 //�g�b�v�̈ʒu��ۑ�����
	m_pTerritory = NULL;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();	//���̃I�u�W�F�N�g�̃|�C���^��ۑ�����

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			CTerritory * pTerritory = (CTerritory *)pScene;

			if (pTerritory->GetPlayerNumber() != m_nNumPlayer)
			{
				D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();
				int nCountEnter = 0;

				for (int nCnt = m_nCountTerritory - 1; nCnt >= 0; nCnt--)
				{
					if (m_apTerritory[nCnt] != NULL)
					{
						D3DXVECTOR3 VecB = TerritoryPos - pListPos[nCnt];

						//�O�ς����߂�
						float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pVecA[nCnt].x, pVecA[nCnt].z), &D3DXVECTOR2(VecB.x, VecB.z));

						//�x�N�g�����傫�����ǂ���
						if (fCross >= 0)
						{
							nCountEnter++;
						}
						else
						{
							break;
						}
					}
				}

				//�͈͓��Ȃ�e���g���[�̎擾
				if (nCountEnter == m_nCountTerritory) { GetTerritory(pTerritory); }
			}
		}

		pScene = pSceneNext;	//�|�C���^��i�߂�
	}

	//�������̔j��
	if (pListPos != NULL)
	{
		delete[] pListPos;
		pListPos = NULL;
	}
	if (pVecA != NULL)
	{
		delete[] pVecA;
		pVecA = NULL;
	}
}

//=============================================================================
// �e���g���[�̎擾����
//=============================================================================
void CPlayer::GetTerritory(CTerritory * pTerritory)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();

	int nOldNumPlayer = pTerritory->GetPlayerNumber();
	pTerritory->SetColor(m_CountryColor[(int)m_type]);	//�F�̕ύX
	pTerritory->SetPlayerNumber(m_nNumPlayer);			//�v���C���[�ԍ��̕ۑ�

	//�O��̃v���C���[�̌��_����
	if (nOldNumPlayer != -1)
	{
		CutTerritoryPoint(pTerritory, nOldNumPlayer);
	}
	//�̒n�l����
	pSound->PlaySound(CSound::SOUND_LABEL_SE017);
	//�_���̒ǉ�����
	AddTerritoryPoint(pTerritory);
	//�G�t�F�N�g�̐���
	pTerritory->CreateGetEffect();
}

//=============================================================================
// �e���g���[�̓��_���폜���鏈��
//=============================================================================
void CPlayer::CutTerritoryPoint(CTerritory * pTerritory, int nOldPlayer)
{
	// ���_���擾
	if (pTerritory == NULL) { return; }
	int nPoint = pTerritory->GetPoint();

	// �Q�[���N���X���擾
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL || CManager::GetMode() != CManager::MODE_GAME) { return; }

	// UI�N���X���擾
	CUI *pUI = pGame->GetUI();
	if (pUI == NULL) { return; }

	// �Q�[���X�R�A�N���X���擾
	CScoreGame *pScoreGame = pUI->GetScoreGame(nOldPlayer);
	if (pScoreGame == NULL) { return; }

	// �X�R�A�����_
	pScoreGame->AddScore(-nPoint);
}

//=============================================================================
// �e���g���[�̓��_��ǉ����鏈��
//=============================================================================
void CPlayer::AddTerritoryPoint(CTerritory * pTerritory)
{
	// ���_���擾
	if (pTerritory == NULL) { return; }
	int nPoint = pTerritory->GetPoint();

	// �Q�[���N���X���擾
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL || CManager::GetMode() != CManager::MODE_GAME) { return; }

	// UI�N���X���擾
	CUI *pUI = pGame->GetUI();
	if (pUI == NULL) { return; }

	// �Q�[���X�R�A�N���X���擾
	CScoreGame *pScoreGame = pUI->GetScoreGame(m_nNumPlayer);
	if (pScoreGame == NULL) { return; }

	// �X�R�A�����Z
	pScoreGame->AddScore(nPoint);
}

//=============================================================================
// �e���g���[�̃��X�g��������
//=============================================================================
void CPlayer::ResetList(void)
{
	//�e���g���[�|�C���^�̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apTerritory[nCnt] = NULL;
	}

	//�J�E���^�[�̏�����
	m_nCountTerritory = 0;
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

//=============================================================================
// ���C���̏�����
//=============================================================================
void CPlayer::ResetLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apLine[nCnt] = NULL;
	}
}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CPlayer::UninitLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apLine[nCnt]->Uninit();
			m_apLine[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ���C���̍Ō�̐��̐�������
//=============================================================================
void CPlayer::CreateEndLine(void)
{
	//�ϐ��錾
	int nCountTerritory = 0;

	//�e���g���[�̃��X�g�̍Ō�������߂�
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apTerritory[nCnt] == NULL)
		{
			nCountTerritory = nCnt - 1;
			break;
		}
	}

	//�Ō�̃��C���̐���
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] == NULL)
		{
			if ((m_apTerritory[nCountTerritory] != NULL) && (m_apTerritory[TERRITORY_TOP] != NULL))
			{
				m_apLine[nCnt] = CLine::Create(m_apTerritory[nCountTerritory]->GetPos(), m_apTerritory[TERRITORY_TOP]->GetPos(), m_nNumPlayer, nCnt);
			}
			break;

		}
	}

	//�����������C�����R�s�[����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apCopyLine[nCnt] = m_apLine[nCnt];
			m_apCopyLine[nCnt]->SetCompleteShape(true);
		}
	}
}

//=============================================================================
// �R�s�[���C���̔j��
//=============================================================================
void CPlayer::UninitCopyLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apCopyLine[nCnt] != NULL)
		{
			m_apCopyLine[nCnt]->Uninit();
			m_apCopyLine[nCnt] = NULL;
		}
	}
}

//=============================================================================
// ���C���̐ؒf����
//=============================================================================
void CPlayer::CutLine(int nID)
{
	//���C���̔j��
	for (int nCnt = nID; nCnt >= 0; nCnt--)
	{
		if (m_apLine[nCnt] != NULL)
		{
			m_apLine[nCnt]->Uninit();
			m_apLine[nCnt] = NULL;
		}
	}

	//���������ւ���
	int nLine = MAX_TERRITORY - nID - 1;

	for (int nCnt = 0; nCnt < nLine; nCnt++)
	{
		int nMoveID = nCnt + nID + 1;

		if ((m_apLine[nCnt] == NULL) && (m_apLine[nMoveID] != NULL))
		{
			m_apLine[nCnt] = m_apLine[nMoveID];	//ID�����炷
			m_apLine[nCnt]->SetID(nCnt);
			m_apLine[nMoveID] = NULL;
		}
	}

	//�ؒf�����̃e���g���[��j������
	for (int nCnt = nID; nCnt >= 0; nCnt--)
	{
		m_apTerritory[nCnt] = NULL;
		m_nCountTerritory--;
	}

	//�e���g���[�̎n�_���ړ�������
	int nNumTerritory = MAX_TERRITORY - (nID + 1);
	for (int nCnt = 0; nCnt < nNumTerritory; nCnt++)
	{
		if ((m_apTerritory[nCnt] == NULL)/* && (m_apTerritory[nCnt + nID] != NULL)*/)
		{
			m_apTerritory[nCnt] = m_apTerritory[nCnt + (nID + 1)];
			m_apTerritory[nCnt + (nID + 1)] = NULL;
		}
	}

	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_SE029);
	pSound->SetVolume(CSound::SOUND_LABEL_SE029,30.0f);

}

