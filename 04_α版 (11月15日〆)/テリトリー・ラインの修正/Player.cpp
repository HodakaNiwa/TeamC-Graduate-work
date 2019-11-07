
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
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPEED			(2.0f)
#define MODEL_RENG		(50)
#define MAX_BUNDCNT		(120)
#define MAX_BULLET		(50)


//�F
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//��
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//��
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//�I�����W
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//��

////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////�ÓI�����o�ϐ��錾
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
////���̐F�̐ݒ�
//D3DXCOLOR CPlayer::m_CountryColor[CPlayer::TYPE_MAX] =
//{
//	COLOR_WHITE,	//���{
//	COLOR_BULE,		//���V�A
//	COLOR_RED,		//�A�����J
//	COLOR_YELLOW,	//�u���W��
//	COLOR_ORENGE,	//�C�M���X
//	COLOR_PAPULE,	//�C�^���A
//	COLOR_BRACK,	//�j���[�W�[�����h
//	COLOR_GREEN,	//��A�t���J
//};

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CPlayer::CPlayer()
{
	m_Angle = D3DXVECTOR3(0.0f,0.0f,0.0f);
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

	//�G�t�F�N�g�̏�����
	if (m_pLoadEffect == NULL)
	{
		m_pLoadEffect = CLoadEffect::Create(1, pos, 8);
	}

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

		switch (m_PlayerState)
		{
		case PLAYERSTATE_NONE:

			//������p�ɂ���
			m_bWalk = true;
			break;

		case PLAYERSTATE_WALK:
			
			break;
		}
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
		m_PlayerState = PLAYERSTATE_WALK;
		if (m_bWalk == true)
		{//�ړ����[�V����
			m_pMotion->SetNumMotion(1);
			m_bWalk = false;
		}
		m_move.x -= sinf(fRot + rot.y) * fSpeed;
		m_move.z -= cosf(fRot + rot.y) * fSpeed;
		m_Angle.y = rot.y + fRot;
	}
	else if (MouseX != 0 || MouseY != 0)
	{//�}�E�X
		// �ړ����
		m_PlayerState = PLAYERSTATE_WALK;
		if (m_bWalk == true)
		{//�ړ����[�V����
			m_pMotion->SetNumMotion(1);
			m_bWalk = false;
		}
		m_move.x -= sinf((Mouse + rot.y)) * fSpeed;
		m_move.z -= cosf(Mouse + rot.y) * fSpeed;
		m_Angle.y = rot.y + Mouse;
	}
	else
	{
		if (m_PlayerState == PLAYERSTATE_WALK && (m_move.x <= 0.1f || m_move.x >= -0.1f || m_move.z <= 0.1f || m_move.z >= -0.1f))
		{
			//�j���[�g������Ԃɂ���
			m_PlayerState = PLAYERSTATE_NONE;
			m_pMotion->SetNumMotion(0);

		}
	}


	//pInputMouse->SetNumMouse(m_nNumPlayer);
	

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

	pos += m_move;

	//����
	m_move.x += (0.0f - m_move.x) * fInertia;
	m_move.z += (0.0f - m_move.z) * fInertia;

	CCharacter::SetPos(pos);
	CCharacter::SetRot(m_rot);
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
			int nData = 0;
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