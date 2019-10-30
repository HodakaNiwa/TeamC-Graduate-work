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
#include "AIController.h"
#include "territory.h"
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
HRESULT CEnemy::Init(D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CCharacter::Init(ModelTxt, MotionTxt);
	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);

	//	�e���g���[���̎擾 / �m��
	m_nMax = 0;
	int nNum = 0;
	CScene * pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);
	CScene * pScene = pSceneTop;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			m_pTerritory = (CTerritory*)pScene;
			if (m_nMax == 0)
			{
				//	�e���g���[�ő吔���m��
				m_nMax = m_pTerritory->GetMaxObj();
				m_TerritoryInfo = new CEnemy::TERRITORY_INFO[m_nMax];
			}
			//	�e�e���g���[���̈ʒu���L�� / ���̃e���g���[�ԍ���
			m_TerritoryInfo[nNum].pos = m_pTerritory->GetPos();
			nNum += 1;
		}
		pScene = pSceneNext;
	}
	
	//	�e�e���g���[�̃t���O��������
	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		m_TerritoryInfo[nCnt].bFlag = false;
	}

	//	���[�v�����p
	m_bBreak = false;
	//	�e���g���[�ʉߐ��L�^
	m_nTargetCnt = 0;

	//	�G�l�~�[�ƃe���g���[�Ԃ̋������v�Z
	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		m_TerritoryInfo[nCnt].fDistance = CAIController::dist(m_TerritoryInfo[nCnt].pos, m_beforePos);
		m_TerritoryInfo[nCnt].fChange = CAIController::dist(m_TerritoryInfo[nCnt].pos, m_beforePos);
	}

	//	������Z�����Ƀ\�[�g
	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		for (int nCnt02 = 0; nCnt02 < m_nMax; nCnt02++)
		{
			if (m_TerritoryInfo[nCnt].fChange < m_TerritoryInfo[nCnt02].fChange)
			{
				float fChange = m_TerritoryInfo[nCnt].fChange;
				m_TerritoryInfo[nCnt].fChange = m_TerritoryInfo[nCnt02].fChange;
				m_TerritoryInfo[nCnt02].fChange = fChange;
			}
		}
	}
	//	��ԒZ���������Z�o
	while (m_bBreak != true)
	{
		for (int nCnt = 0; nCnt < m_nMax; nCnt++)
		{
			for (int nInitTarget = 0; nInitTarget < m_nMax; nInitTarget++)
			{					//	��ԒZ������ == ���������Ԃ̋��_�ԍ�
				if (m_TerritoryInfo[nCnt].fChange == m_TerritoryInfo[nInitTarget].fDistance)
				{
					m_nTargetNum = nInitTarget;
					m_bBreak = true;
					break;
				}
			}
			break;
		}
	}
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
	//	�e���g���[���̉��
	if (m_TerritoryInfo != NULL)
	{
		delete[] m_TerritoryInfo;
		m_TerritoryInfo = NULL;
	}

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
	D3DXVECTOR3 pos = CCharacter::GetPos();
	m_posOld = pos;

	
	//!	---<<AI�֘A>---

	m_nAreaNum = m_pTerritory->RequestErea(pos);

	//	���p�x�v�Z
	for (int nCnt = 0; nCnt < m_nMax; nCnt++)
	{
		m_TerritoryInfo[nCnt].fRadian = (float)atan2(m_TerritoryInfo[nCnt].pos.z - pos.z, m_TerritoryInfo[nCnt].pos.x - pos.x);
	}
	//	�����̍��W�̗\���l���v�Z����
	D3DXVECTOR3 nextPos;
	nextPos.x = (float)cos(m_TerritoryInfo[m_nTargetNum].fRadian) * SPEED + pos.x;
	nextPos.z = (float)sin(m_TerritoryInfo[m_nTargetNum].fRadian) * SPEED + pos.z;

	//	���ړI�n�͈͓��ɓ�����
	if (pos.x <= m_TerritoryInfo[m_nTargetNum].pos.x + 35.0f && pos.x >= m_TerritoryInfo[m_nTargetNum].pos.x - 35.0f &&
		pos.z <= m_TerritoryInfo[m_nTargetNum].pos.z + 35.0f && pos.z >= m_TerritoryInfo[m_nTargetNum].pos.z - 35.0f)
	{
		//	[[�t���O�𗧂Ă�]]
		m_TerritoryInfo[m_nTargetNum].bFlag = true;
		//	[[�ʉߋL�^�J�E���g�A�b�v]]
		m_nTargetCnt += 1;
		//	[[�ēx�������v�Z]]
		for (int nCnt = 0; nCnt < m_nMax; nCnt++)
		{
			m_TerritoryInfo[nCnt].fDistance = CAIController::dist(m_TerritoryInfo[nCnt].pos, pos);
			m_TerritoryInfo[nCnt].fChange = CAIController::dist(m_TerritoryInfo[nCnt].pos, pos);
		}
		//	[[�Z���������ɕς���]]
		for (int nCnt = 0; nCnt < m_nMax; nCnt++)
		{
			for (int nCnt2 = 0; nCnt2 < m_nMax; nCnt2++)
			{
				if (m_TerritoryInfo[nCnt].fChange < m_TerritoryInfo[nCnt2].fChange)
				{
					float fChange = m_TerritoryInfo[nCnt].fChange;
					m_TerritoryInfo[nCnt].fChange = m_TerritoryInfo[nCnt2].fChange;
					m_TerritoryInfo[nCnt2].fChange = fChange;
				}
			}
		}

		// [[���n�_����߂�������T��]]
		m_bBreak = false;
		int nNextNum = 1;//	���̋߂��^�[�Q�b�g���_�ԍ�(0�͌��ݎ���������ꏊ)

		if (m_nTargetCnt == m_nMax)	//	�ʉ߉񐔂��t���O�����Z�b�g
		{
			m_nTargetCnt = 0;
			for (int nCnt = 0; nCnt < m_nMax; nCnt++)
			{
				m_TerritoryInfo[nCnt].bFlag = false;
			}
		}

		while (m_bBreak != true)
		{
			for (int nCnt = 0; nCnt < m_nMax; nCnt++)
			{
				if (m_TerritoryInfo[nCnt].bFlag == false)
				{
					if (m_TerritoryInfo[nCnt].fDistance == m_TerritoryInfo[nNextNum].fChange)
					{
						m_nTargetNum = nCnt;
						m_bBreak = true;
						break;
					}
				}
			}
			nNextNum += 1;	//	���łɃt���O�������Ă���ꍇ�͎��̋ߏ��
		}
	}

	//m_beforePos.x = pos.x;
	//m_beforePos.z = pos.z;

	if (pos.x > nextPos.x) { pos.x -= 4.0f; }
	else if (pos.x < nextPos.x) { pos.x += 4.0f; }
	if (pos.z > nextPos.z) { pos.z -= 4.0f; }
	else if (pos.z < nextPos.z) { pos.z += 4.0f; }

	// �����_�܂ł̊p�x / ���g�̎�����
	float fDestAngle = atan2f(pos.x - m_TerritoryInfo[m_nTargetNum].pos.x, pos.z - m_TerritoryInfo[m_nTargetNum].pos.z);
	float fAngle = fDestAngle - m_rot.y;


	if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
	if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }

	if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
	if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }

	//	�ړ������Ɋp�x����
	m_rot.y += fAngle * 0.1f;

	//	�ʒu�E��]���̔��f
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
