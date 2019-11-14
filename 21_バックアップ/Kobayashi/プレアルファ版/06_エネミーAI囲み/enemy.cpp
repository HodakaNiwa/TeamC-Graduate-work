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
#include "library.h"
#include "territory.h"
#include "line.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�ÓI�����o�ϐ��錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CSceneX *CEnemy::m_apSceneX = NULL;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPEED (1.0f)
#define MODEL_RENG (50)
#define MAX_BUNDCNT (120)
#define MAX_BULLET (50)

//���̐F�̐ݒ�
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//��
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//��
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//�I�����W
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//��


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
HRESULT CEnemy::Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{

	CCharacter::Init(nNum,ModelTxt, MotionTxt);
	m_pModel = CCharacter::GetModel();
	CCharacter::SetPos(pos);

	m_nLineNum = 2;	//	�Œ���̃��C������������n�_�ɖ߂�(���_��2�q������n�_�ɖ߂�A�n�_�ɖ߂��Ă����烉�C����3�ɂȂ�|�C���g���Z�̏����𖞂�����)
	InitSort(pos);	//	�Q�[���J�n���̋ߏ�̃G���A�E�e���g���[��������

	//���C���ϐ��̏�����
	m_nMaxLineTime = FIRST_LINETIME;
	m_nLineTime = m_nMaxLineTime;
	m_bBlockStartTerritory = false;
	m_pOldStartTerritory = NULL;
	m_nCountTime = 0;
	m_pOrbitLine = NULL;
	m_bMakeShape = false;
	m_nCntTimeCopyLine = 0;

	//�R�s�[���C���̏�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apCopyLine[nCnt] = NULL;
	}

	ResetLine();	//���C���̏�����


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
	//	�e�G���A�̃e���g���[���̉��
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		if (m_AreaInfo[nCnt] != NULL)
		{
			delete[] m_AreaInfo[nCnt];
			m_AreaInfo[nCnt] = NULL;
		}
	}

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

	int nGameState = CManager::GetGameState();

	if (nGameState != 1 && nGameState != 3)
	{
		Move();	//	�G�l�~�[�ړ�����
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
CEnemy *CEnemy::Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40])
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		//�������𓮓I�m��
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{//NULL�`�F�b�N

			pEnemy->SetType(type);
			pEnemy->m_modelType = PLAYERTYPE_SPEED;
			pEnemy->Init(nNum, pos, ModelTxt, MotionTxt);
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
void CEnemy::InitSort(D3DXVECTOR3 pos)
{
	//	�e���g���[���̎擾 / �m��
	m_nMax = 0;
	int nCnt01 = 0;
	CScene* pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);
	CScene* pScene = pSceneTop;

	while (pScene != NULL)
	{
		CScene * pSceneNext = pScene->GetpNext();

		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			m_pTerritory = (CTerritory*)pScene;

			if (m_nMax == 0)//	�e���g���[�̍ő吔��new����
			{
				m_nMax = m_pTerritory->GetMaxObj();
				m_TerritoryInfo = new CEnemy::TERRITORY_INFO[m_nMax];
			}

			//	�e�e���g���[�̏����擾
			m_TerritoryInfo[nCnt01].pos = m_pTerritory->GetPos();			//	�ʒu���
			m_TerritoryInfo[nCnt01].nAreaNum = m_pTerritory->GetErea();		//	����U��ꂽ�G���A�ԍ�

																			//	�e�G���A���̃e���g���[�̐����J�E���g�A�b�v[�G���A�̔ԍ�]
			m_nAreaTerrNum[m_TerritoryInfo[nCnt01].nAreaNum] += 1;

			nCnt01 += 1;
		}
		pScene = pSceneNext;
	}

	//	�G���A���Ƃ̏�񂪗~�����̂ŃG���A�̐���new����
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		m_AreaInfo[nCnt] = new CEnemy::TERRITORY_INFO[m_nAreaTerrNum[nCnt]];
	}

	//	�G���A���ƂɃe���g���[�̏���U�蕪����
	for (int nCntA = 0; nCntA < AREA_MAX; nCntA++)
	{
		int nTerrCnt = 0;		//	�e���g���[�ԍ�
		int nAllTerrCnt = 0;	//	�S�Ẵe���g���[�����邽��

		while (nAllTerrCnt != m_nMax)// �S�Ẵe���g���[���G���A��������܂ő���
		{
			if (nCntA == m_TerritoryInfo[nAllTerrCnt].nAreaNum)
			{
				m_AreaInfo[nCntA][nTerrCnt].pos = m_TerritoryInfo[nAllTerrCnt].pos;
				nTerrCnt += 1;
			}
			nAllTerrCnt += 1;
		}
	}

	//	�e�G���A�̃e���g���[�̃t���O��������
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nTerrCnt = 0; nTerrCnt < m_nAreaTerrNum[nAreaCnt]; nTerrCnt++)	//	�G���A���Ƃ̃e���g���[������
		{
			m_AreaInfo[nAreaCnt][nTerrCnt].bFlag = false;
		}
	}

	//	���[�v�����p
	m_bBreak = false;

	//	�e���g���[�ʉߐ��L�^ / ���݂̃��C����
	m_nTargetCnt = 0;
	m_bFinish = false;

	//	�G�l�~�[�ƃe���g���[�Ԃ̋������v�Z
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nTerrCnt = 0; nTerrCnt < m_nAreaTerrNum[nAreaCnt]; nTerrCnt++)	//	�G���A���Ƃ̃e���g���[������
		{
			m_AreaInfo[nAreaCnt][nTerrCnt].fDistance = CAIController::dist(m_AreaInfo[nAreaCnt][nTerrCnt].pos, pos);
			m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort = CAIController::dist(m_AreaInfo[nAreaCnt][nTerrCnt].pos, pos);

		}
	}

	//	������Z�����Ƀ\�[�g
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		//	��������\�[�g����
		for (int nTerrCnt = 0; nTerrCnt < m_nAreaTerrNum[nAreaCnt]; nTerrCnt++)	//	�G���A���Ƃ̃e���g���[������
		{
			for (int nTerrCnt02 = 0; nTerrCnt02 < m_nAreaTerrNum[nAreaCnt]; nTerrCnt02++)
			{
				if (m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort < m_AreaInfo[nAreaCnt][nTerrCnt02].fDisSort)
				{
					float fDisSort = m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort;
					m_AreaInfo[nAreaCnt][nTerrCnt].fDisSort = m_AreaInfo[nAreaCnt][nTerrCnt02].fDisSort;
					m_AreaInfo[nAreaCnt][nTerrCnt02].fDisSort = fDisSort;
				}
			}
		}
	}

	//	�Q�[���J�n�̍ۂɁA�ǂ��̃G���A����ԋ߂��������߂�(�\�[�g����)
	float fSort = 0.0f;
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nAreaCnt02 = 0; nAreaCnt02 < AREA_MAX; nAreaCnt02++)
		{
			if (m_AreaInfo[nAreaCnt][0].fDisSort < m_AreaInfo[nAreaCnt02][0].fDisSort)
			{
				fSort = m_AreaInfo[nAreaCnt][0].fDisSort;
				m_AreaInfo[nAreaCnt][0].fDisSort = m_AreaInfo[nAreaCnt02][0].fDisSort;
				m_AreaInfo[nAreaCnt02][0].fDisSort = fSort;
			}
		}
	}

	//	��ԋ߂��G���A�A�e���g���[�����܂�
	int nAreaCnt = 0;
	while (m_bBreak != true)
	{
		for (int nInitTarget = 0; nInitTarget < m_nAreaTerrNum[nAreaCnt]; nInitTarget++)//	�G���A���Ƃ̃e���g���[������
		{
			if (m_AreaInfo[0][0].fDisSort == m_AreaInfo[nAreaCnt][nInitTarget].fDistance)
			{
				m_nTargetNum = nInitTarget;
				m_nAreaNow = nAreaCnt;	//	��ԍŏ��ɏ�������G���A�����܂�
				//m_nTerrStart.pos = m_AreaInfo[nAreaCnt][nInitTarget].pos;	//	���C�����q���ۂ̎n�_�E�I�_�ƂȂ�
				m_bBreak = true;		//	���܂����u�ԃ��[�v�𔲂���
				break;
			}
		}
		nAreaCnt += 1;
	}
}
//=============================================================================
//�@�v���C���[�̈ړ�����
//=============================================================================
void  CEnemy::Move(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPos();
	m_posOld = pos;
	
	//!	---<<AI�֘A>---
	

	if (m_bFinish == true)
	{
		//?	���I�_�ƂȂ鋒�_
		m_nTerrStart.fRadian = (float)atan2(m_nTerrStart.pos.z - pos.z, m_nTerrStart.pos.x - pos.x);

		//?	�����̍��W�̗\���l���v�Z����
		if (pos.x <= m_nTerrStart.pos.x + 25.0f && pos.x >= m_nTerrStart.pos.x - 25.0f &&
			pos.z <= m_nTerrStart.pos.z + 25.0f && pos.z >= m_nTerrStart.pos.z - 25.0f)
		{
			// [[���n�_����߂�������T��]]
			m_bBreak = false;
			int nNextNum = 0;//	���̋߂��^�[�Q�b�g���_�ԍ�(0�͌��ݎ���������ꏊ)

			//	[[�ēx�������v�Z]]
			for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
			{
				m_AreaInfo[m_nAreaNow][nCnt].fDistance = CAIController::dist(m_AreaInfo[m_nAreaNow][nCnt].pos, pos);
				m_AreaInfo[m_nAreaNow][nCnt].fDisSort = CAIController::dist(m_AreaInfo[m_nAreaNow][nCnt].pos, pos);

			}

			//	[[�Z���������ɕς���]]
			for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
			{
				for (int nCnt2 = 0; nCnt2 < m_nAreaTerrNum[m_nAreaNow]; nCnt2++)
				{
					if (m_AreaInfo[m_nAreaNow][nCnt].fDisSort < m_AreaInfo[m_nAreaNow][nCnt2].fDisSort)
					{
						float fDisSort = m_AreaInfo[m_nAreaNow][nCnt].fDisSort;
						m_AreaInfo[m_nAreaNow][nCnt].fDisSort = m_AreaInfo[m_nAreaNow][nCnt2].fDisSort;
						m_AreaInfo[m_nAreaNow][nCnt2].fDisSort = fDisSort;
					}
				}
			}

			while (m_bBreak != true)
			{
				for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
				{
					if (m_AreaInfo[m_nAreaNow][nCnt].bFlag == false)
					{
						if (m_AreaInfo[m_nAreaNow][nCnt].fDistance == m_AreaInfo[m_nAreaNow][nNextNum].fDisSort)
						{
							m_nTargetNum = nCnt;
							m_bBreak = true;
							break;
						}
					}
				}
				nNextNum += 1;	//	���łɃt���O�������Ă���ꍇ�͎��̋ߏ��
			}

			//	���C�����̏����� / �V���Ȏn�_�E�I�_�����߂�
			m_bFinish = false;
			m_nTerrStart.pos = m_AreaInfo[m_nAreaNow][m_nTargetNum].pos;
		}
		//	�ړ����Z����
		pos.x += (float)cos(m_nTerrStart.fRadian) * SPEED;
		pos.z += (float)sin(m_nTerrStart.fRadian) * SPEED;

		// �����_�܂ł̊p�x / ���g�̎�����
		float fDestAngle = atan2f(pos.x - m_nTerrStart.pos.x, pos.z - m_nTerrStart.pos.z);
		float fAngle = fDestAngle - m_rot.y;

		if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
		if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }

		//	�ړ������Ɋp�x����
		m_rot.y += fAngle * 0.1f;

		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }

		//	�ʒu�E��]���̔��f
		CCharacter::SetPos(pos);
		CCharacter::SetRot(m_rot);
	}
	// ------------------------------------------------------------------------------------------------------------------
	else
	{
		//	���p�x�v�Z
		for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
		{
			m_AreaInfo[m_nAreaNow][nCnt].fRadian = (float)atan2(m_AreaInfo[m_nAreaNow][nCnt].pos.z - pos.z, m_AreaInfo[m_nAreaNow][nCnt].pos.x - pos.x);
		}

		//	���ړI�n�͈͓��ɓ�����
		if (pos.x <= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.x + 25.0f && pos.x >= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.x - 25.0f &&
			pos.z <= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.z + 25.0f && pos.z >= m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.z - 25.0f)
		{
			//	[[�ŐV�̎n�_�E�I�_�ɍX�V]]
			if (m_apTerritory[0] != NULL)
			{
				m_nTerrStart.pos = m_apTerritory[0]->GetPos();
			}
			
			//	[[���C����?�{��������n�_�ɖ߂�]]
			if (m_apTerritory[m_nLineNum] != NULL)
			{
				if (m_nLineTime <= 800)	// ��������(8�b)�������Ă�����
				{
					m_nLineNum = 2;
					m_bFinish = true;	//	�n�_�ɖ߂�
				}
				else//	���C����L�΂����Ԃɗ]�T�������
				{
					m_nLineNum += 1;
				}
				
			}
			//	[[�t���O�𗧂Ă�]]
			m_AreaInfo[m_nAreaNow][m_nTargetNum].bFlag = true;
			//	[[�ʉߋL�^�J�E���g�A�b�v]]
			m_nTargetCnt += 1;
			// [[���n�_����߂�������T��]]
			m_bBreak = false;
			int nNextNum = 0;//	���̋߂��^�[�Q�b�g���_�ԍ�(0�͌��ݎ���������ꏊ)

			if (m_nTargetCnt == (m_nAreaTerrNum[m_nAreaNow] / 2)-5)	//	�ʉ߉񐔂��t���O�����Z�b�g
			{
				m_nTargetCnt = 0;

				for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
				{
					m_AreaInfo[m_nAreaNow][nCnt].bFlag = false;

				}
				//	����if�������񂩒ʂ����玟�̃G���A�Ɉړ������悤
				//int �Z = 0;
				//�Z++;
				//if (�Z == 3)
				//{

				//}
				/*m_nAreaNow += 1;
				if (m_nAreaNow == AREA_MAX)
				{
					m_nAreaNow = 0;
				}*/
			}

			//	[[�ēx�������v�Z]]
			for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
			{
				m_AreaInfo[m_nAreaNow][nCnt].fDistance = CAIController::dist(m_AreaInfo[m_nAreaNow][nCnt].pos, pos);
				m_AreaInfo[m_nAreaNow][nCnt].fDisSort = CAIController::dist(m_AreaInfo[m_nAreaNow][nCnt].pos, pos);

			}

			//	[[�Z���������ɕς���]]
			for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
			{
				for (int nCnt2 = 0; nCnt2 < m_nAreaTerrNum[m_nAreaNow]; nCnt2++)
				{
					if (m_AreaInfo[m_nAreaNow][nCnt].fDisSort < m_AreaInfo[m_nAreaNow][nCnt2].fDisSort)
					{
						float fDisSort = m_AreaInfo[m_nAreaNow][nCnt].fDisSort;
						m_AreaInfo[m_nAreaNow][nCnt].fDisSort = m_AreaInfo[m_nAreaNow][nCnt2].fDisSort;
						m_AreaInfo[m_nAreaNow][nCnt2].fDisSort = fDisSort;
					}
				}
			}

			while (m_bBreak != true)
			{
				for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
				{
					if (m_AreaInfo[m_nAreaNow][nCnt].bFlag == false)
					{
						if (m_AreaInfo[m_nAreaNow][nCnt].fDistance == m_AreaInfo[m_nAreaNow][nNextNum].fDisSort)
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

		//	�ړ����Z����
		pos.x += (float)cos(m_AreaInfo[m_nAreaNow][m_nTargetNum].fRadian) * SPEED;
		pos.z += (float)sin(m_AreaInfo[m_nAreaNow][m_nTargetNum].fRadian) * SPEED;

		// �����_�܂ł̊p�x / ���g�̎�����
		float fDestAngle = atan2f(pos.x - m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.x, pos.z - m_AreaInfo[m_nAreaNow][m_nTargetNum].pos.z);
		float fAngle = fDestAngle - m_rot.y;

		if (fAngle > D3DX_PI) { fAngle -= D3DX_PI * 2; }
		if (fAngle < -D3DX_PI) { fAngle += D3DX_PI * 2; }

		//	�ړ������Ɋp�x����
		m_rot.y += fAngle * 0.1f;

		if (m_rot.y > D3DX_PI) { m_rot.y -= D3DX_PI * 2; }
		if (m_rot.y < -D3DX_PI) { m_rot.y += D3DX_PI * 2; }

		//	�ʒu�E��]���̔��f
		CCharacter::SetPos(pos);
		CCharacter::SetRot(m_rot);
	}


}

//=============================================================================
// ���C���̐��� / �j��
//=============================================================================
void CEnemy::CreateOrbitLine(void)
{
	if (m_pOrbitLine == NULL)
	{
		m_pOrbitLine = CSceneOrbit::Create(CSceneOrbit::TYPE_PLAYER, CCharacter::GetPos());
		m_pOrbitLine->SetMtxParent(&m_pModel[5]->GetMtxWorld());
		m_pOrbitLine->SetMtxParentEnd(&m_pModel[1]->GetMtxWorld());
	}
}

void CEnemy::UninitOrtbitLine(void)
{
	if (m_pOrbitLine != NULL)
	{
		m_pOrbitLine->Uninit();
		m_pOrbitLine = NULL;
	}
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
