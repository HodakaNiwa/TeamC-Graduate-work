//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�G�l�~�[���� [enemy.cpp]
//			Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "enemy.h"
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�ÓI�����o�ϐ��錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CSceneX *CEnemy::m_apSceneX = NULL;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPEED (4.0f)
#define MODEL_RENG (50)
#define MAX_BUNDCNT (120)
#define MAX_BULLET (50)

//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEnemy::CEnemy()
{
	m_Angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CEnemy::~CEnemy()
{

}



//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos,char ModelTxt[40], char MotionTxt[40])
{
	CCharacter::Init(ModelTxt, MotionTxt);
	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);

	
	//m_beforePos = CCharacter::GetPos();	//	�ȑO�̏ꏊ
	//	�e���_�̈ʒu����(��ł�)
	m_objPos[0] = D3DXVECTOR3(100.0f, 0.0f, 100.0f);
	m_objPos[1] = D3DXVECTOR3(-200.0f, 0.0f, 200.0f);
	m_objPos[2] = D3DXVECTOR3(500.0f, 0.0f, 300.0f);
	m_objPos[3] = D3DXVECTOR3(-400.0f, 0.0f, -400.0f);
	m_objPos[4] = D3DXVECTOR3(500.0f, 0.0f, -500.0f);
	m_nNum = 0;
	m_nNext = 0;
	return S_OK;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(void)
{

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void  CEnemy::Uninit(void)
{

	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CEnemy::Update(void)
{
	CCharacter::Update();

	Move();	//	�G�l�~�[�ړ�����
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CEnemy::Draw(void)
{
	//���[�e�V����
	//�ʒu

	CCharacter::Draw();
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	CManager::GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		//�������𓮓I�m��
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{//NULL�`�F�b�N

		 //����������
			pEnemy->Init(pos, ModelTxt, MotionTxt);
		}
		else
		{
			MessageBox(0, "territory��NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "territory��NULL����Ȃ��ł�", "�x��", MB_OK);
	}
	return pEnemy;
}

//=============================================================================
//
//=============================================================================
void  CEnemy::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{

}

//=============================================================================
//�@�v���C���[�̈ړ�����
//=============================================================================
void  CEnemy::Move(void)
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾
	CGamePad * pGamePad = CManager::GetInputGamePad();				//�Q�[���p�b�h�̎擾
	CInputXPad * pXPad = CManager::GetXPad();


	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();
	CGameCamera *pGameCamera = pGame->GetGameCamera(0);
	D3DXVECTOR3 rot = pGameCamera->GetRot();


	D3DXVECTOR3 pos = CCharacter::GetPos();
	m_posOld = pos;


	//!	---<<��AI>>---
	//	�p�x�v�Z
	//m_Info[m_nNum].fDistance = dist(m_objPos[m_nNum], m_beforePos);
	//m_Info[m_nNum].fRadian = atan2(m_objPos[m_nNum].z - m_beforePos.z, m_objPos[m_nNum].x - m_beforePos.x);
	for (int nCnt = 0; nCnt < BASE_MAX; nCnt++)
	{
		m_Info[nCnt].fRadian = atan2(m_objPos[nCnt].z - m_beforePos.z, m_objPos[nCnt].x - m_beforePos.x);

	}
	//	���̍��W�̗\���l���v�Z����
	D3DXVECTOR3 nextPos;
	nextPos.x = cos(m_Info[m_nNum].fRadian) * SPEED + m_beforePos.x;
	nextPos.z = sin(m_Info[m_nNum].fRadian) * SPEED + m_beforePos.z;

	//	�ړI�n�͈͓��ɓ�����
	if (pos.x <= m_objPos[m_nNum].x + 10 && pos.x >= m_objPos[m_nNum].x - 10 &&
		pos.z <= m_objPos[m_nNum].z + 10 && pos.z >= m_objPos[m_nNum].z - 10)
	{
		//	�����v�Z
		for (int nCnt = 0; nCnt < BASE_MAX; nCnt++)
		{
			m_Info[nCnt].fDistance = dist(m_objPos[nCnt], m_beforePos);
			m_Info[nCnt].fChange = dist(m_objPos[nCnt], m_beforePos);
		}
		//	�Z���������ɕς���
		for (int nCnt = 0; nCnt < BASE_MAX; nCnt++)
		{
			for (int nCnt2 = 0; nCnt2 < BASE_MAX; nCnt2++)
			{
				if (m_Info[nCnt].fChange < m_Info[nCnt2].fChange)
				{
					float fChange = m_Info[nCnt].fChange;
					m_Info[nCnt].fChange = m_Info[nCnt2].fChange;
					m_Info[nCnt2].fChange = fChange;
				}
			}
		}
		//	��������v���Ă��邩
		m_nNum += 1;
		if (m_nNum == BASE_MAX)
		{
			m_nNum = 0;
		}
		/*for (int nCnt = 0; nCnt < BASE_MAX; nCnt++)
		{
			if (m_Info[nCnt].fDistance == m_Info[m_nNum].fChange)
			{
				m_nNum = nCnt;
				break;
			}
		}*/
		
	}
	
		m_beforePos.x = pos.x;
		m_beforePos.z = pos.z;
		pos.x = nextPos.x;
		pos.z = nextPos.z;


#if 0
	if (pInputKeyboard->GetKeyboardPress(DIK_LEFT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//�|���S��������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.25f)) * SPEED;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.25f)) * SPEED;
			m_Angle.y = rot.y - (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//�|���S���������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.75f)) * SPEED;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.75f)) * SPEED;
			m_Angle.y = rot.y - (D3DX_PI * 0.75f);
		}
		else
		{//�|���S�������Ɉړ�
			m_move.x -= sinf(rot.y + (-D3DX_PI * 0.5f)) * SPEED;
			m_move.z -= cosf(rot.y + (-D3DX_PI * 0.5f)) * SPEED;
			m_Angle.y = rot.y - (D3DX_PI * 0.5f);
		}
	}
	else if (pInputKeyboard->GetKeyboardPress(DIK_RIGHT) == true)
	{
		if (pInputKeyboard->GetKeyboardPress(DIK_UP) == true)
		{//�|���S�����E��Ɉړ�
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.25f)) * SPEED;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.25f)) * SPEED;
			m_Angle.y = rot.y + (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetKeyboardPress(DIK_DOWN) == true)
		{//�|���S�����E���Ɉړ�
			m_move.x -= sinf(rot.y + (D3DX_PI * 0.75f)) * SPEED;
			m_move.z -= cosf(rot.y + (D3DX_PI * 0.75f)) * SPEED;
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
		m_move.x -= sinf(rot.y + D3DX_PI * 1.0f) * SPEED;
		m_move.z -= cosf(rot.y + D3DX_PI * 1.0f) * SPEED;
		m_Angle.y = D3DX_PI + rot.y;
	}
#endif

	D3DXVECTOR3 DiffAngle;

	// �G�l�~�[�̊p�x���C��
	//DiffAngle.y = m_Info[0].fRadian - m_rot.y;   //�������v�Z

	//	�����v�Z
	DiffAngle.y = m_Angle.y - m_rot.y;   

	if (DiffAngle.y > D3DX_PI) { DiffAngle.y -= D3DX_PI * 2.0f; }
	if (DiffAngle.y < -D3DX_PI) { DiffAngle.y += D3DX_PI * 2.0f; }

	//	������{���Œ���
	m_rot.y += DiffAngle.y * 0.1f;

	if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2.0f; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2.0f; }

	//	�ړ��ʉ��Z
	pos += m_move;	

	//	����
	m_move.x += (0.0f - m_move.x) * 1.0f;
	m_move.z += (0.0f - m_move.z) * 1.0f;

	CCharacter::SetPos(pos);
	CCharacter::SetRot(m_rot);
}


//=============================================================================
//�@�G�l�~�[�ƃe���g���[�̓����蔻��
//=============================================================================
void CEnemy::Collision(void)
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
bool CEnemy::CollisionCollider(CCollider *pCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
bool CEnemy::CollisionBoxCollider(CBoxCollider *pBoxCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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

//	2�_�Ԃ̋��������߂�
float CEnemy::dist(D3DXVECTOR3 obj, D3DXVECTOR3 enemy)
{
	float fOut, fInX, fInZ;

	//	�e���W�̍����v�Z
	fInX = enemy.x - obj.x;
	fInZ = enemy.z - obj.z;

	//	2�_�Ԃ̋������v�Z����
	fOut = sqrt((fInX*fInX) + (fInZ*fInZ));

	return fOut;
}
