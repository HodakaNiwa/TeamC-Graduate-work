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
#include "mine.h"

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

//	�����蔻��E�_���[�W�֘A
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)


//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_move = INITIALIZE_VECTOR3;
	m_Angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nDamageCount = 0;
	m_fBlowAngle = 0.0f;
	m_fBlowLength = 0.0f;
	m_rot = INITIALIZE_VECTOR3;
	m_posOld = INITIALIZE_VECTOR3;
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
	CCharacter::Init(nNum, ModelTxt, MotionTxt, m_CharcterType);
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
	m_bCheckS = false;

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

	//�Q�[���̏�Ԃ��擾
	int nGameState = CGame::GetGameState();
	float aaa = m_fSpeed;
	if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
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

	//	�}�`������������̏���
	UpdateShapeComplete();

	//	���݂̏��
	switch (m_state)
	{
	case STATE_NONE://	�ʏ�
					//	���s�\
		m_bWalk = true;
		m_bSprintMotion = true;
		break;

	case STATE_WALK://	�ړ����
					//m_bSprintMotion = true;

		break;

	case STATE_ACTION: //�A�N�V�������
					   //m_bWalk = true;

		break;

	case STATE_BLOWAWAY:	//������΂���Ă���
							// �_���[�W�J�E���g���Z
		m_nDamageCount++;

		if (m_nDamageCount >= MAX_DAMAGECOUNT)
		{
			m_nDamageCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;

			//if (m_bSprintMotion == true)
			{
				m_state = STATE_NONE;
				m_pMotion->SetNumMotion(m_state);
			}

		}

		// ������΂�
		m_fBlowLength -= BLOW_MOVING_CUT;
		if (m_fBlowLength >= 0.0f)
		{
			D3DXVECTOR3 pos = CCharacter::GetPos();
			pos.x -= sinf(m_fBlowAngle) * m_fBlowLength;
			pos.z -= cosf(m_fBlowAngle) * m_fBlowLength;
			CCharacter::SetPos(pos);
		}

		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CEnemy::Draw(void)
{
	CCharacter::Draw();
	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	CManager::GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//=============================================================================
//�I�u�W�F�N�g�̐�������
//=============================================================================
CEnemy *CEnemy::Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],CHARCTERTYPE charatype)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;

		if (pEnemy != NULL)
		{
			pEnemy->m_CharcterType = charatype;
			pEnemy->SetType(type);
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
//=============================================================================
//	���������\�[�g����
//=============================================================================
void CEnemy::InitSort(D3DXVECTOR3 pos)
{
	for (int nCntArea = 0; nCntArea < AREA_MAX; nCntArea++)
	{
		m_nAreaTerrNum[nCntArea] = 0;
	}

	for (int nCntTe = 0; nCntTe < 10; nCntTe++)
	{
		//m_apTerritory[nCntTe] = NULL;
	}
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
	m_posOld = pos;				//	�����t���[���O�̈ʒu
	float fSpeed = GetSpeed();	//	����

	if (m_bFinish == true)	//!	[[�n�_�ւ̈ړ����� / ���̌�A�i�ދ��_�����߂�]]
	{
		//	���p�x�v�Z
		m_nTerrStart.fRadian = (float)atan2(m_nTerrStart.pos.z - pos.z, m_nTerrStart.pos.x - pos.x);

		//	���ړI�n�͈͓��ɓ�����
		if (pos.x <= m_nTerrStart.pos.x + 25.0f && pos.x >= m_nTerrStart.pos.x - 25.0f &&
			pos.z <= m_nTerrStart.pos.z + 25.0f && pos.z >= m_nTerrStart.pos.z - 25.0f)
		{
			//	�����̍Čv�Z
			DisSort(pos);

			//	���C�����̏����� / �V���Ȏn�_�E�I�_�����߂�
			m_bFinish = false;
			m_nTerrStart.pos = m_AreaInfo[m_nAreaNow][m_nTargetNum].pos;
		}

		MotionAction();	//	���[�V�����A�N�V����(�L�������m�̂Ԃ��蓙)

						//	�ړ����Z����
		if (m_state == STATE_WALK || m_state == STATE_ACTION)
		{
			pos.x += (float)cos(m_nTerrStart.fRadian) * (fSpeed * m_fSpeed);
			pos.z += (float)sin(m_nTerrStart.fRadian) * (fSpeed * m_fSpeed);
		}
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
	else //! [[�n�_���������_�̌v�Z]]
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
				if (m_nLineTime <= (12 - m_nCreateTime) * 100)	// �������Ԃ������Ă�����
				{
					m_nCreateTime = (rand() % 4);
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

			if (m_nTargetCnt == m_nAreaTerrNum[m_nAreaNow])	//	�ʉ߉񐔂��t���O�����Z�b�g
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
				m_nAreaNow += 1;
				if (m_nAreaNow == AREA_MAX)
				{
					m_nAreaNow = 0;
				}
			}

			DisSort(pos);	//	�����̍Čv�Z
		}

		MotionAction();	//	���[�V�����A�N�V����(�L�������m�̂Ԃ��蓙)

						//	�ړ����Z����
		if (m_state == STATE_WALK || m_state == STATE_ACTION)
		{
			pos.x += (float)cos(m_AreaInfo[m_nAreaNow][m_nTargetNum].fRadian) * (fSpeed * m_fSpeed);
			pos.z += (float)sin(m_AreaInfo[m_nAreaNow][m_nTargetNum].fRadian) * (fSpeed * m_fSpeed);
		}
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
//�@�����ԃ\�[�g����
//=============================================================================
void CEnemy::DisSort(D3DXVECTOR3 pos)
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
}

//=============================================================================
//�@������я���
//=============================================================================
void CEnemy::BlowAway(D3DXVECTOR3 AnotherPos)
{
	// ���W���擾
	D3DXVECTOR3 pos = GetPos();

	// ������Ԍ������v�Z
	m_fBlowAngle = CFunctionLib::CalcAngleToDest(pos.x, pos.z, AnotherPos.x, AnotherPos.z);

	// ������ԋ�����ݒ�
	m_fBlowLength = BLOW_MOVING;

	// ������я�Ԃɂ���
	m_state = STATE_BLOWAWAY;
	m_pMotion->SetNumMotion(STATE_BLOWAWAY);

	// ������ς���
	float fRot = m_fBlowAngle + D3DX_PI;
	fRot = CFunctionLib::CheckRotationOverPiX(fRot);
	m_rot.y = fRot;
	m_Angle = D3DXVECTOR3(0.0f, fRot, 0.0f);
}

//=============================================================================
//�@���[�V�����A�N�V����(�L�������m�Ԃ�������)
//=============================================================================
void CEnemy::MotionAction(void)
{
	CSound *pSound = CManager::GetSound();	//	�T�E���h�̎擾

											//�ړ����
	if (m_state != STATE_BLOWAWAY)
	{
		if (m_state != STATE_ACTION && m_bSprint != true)
		{
			//�ړ����
			m_state = STATE_WALK;
		}
		if (m_bSprint == true)
		{
			//�ړ����
			m_state = STATE_ACTION;
		}
	}
	if (m_bWalk == true)
	{//�ړ����[�V����
		if (m_state == STATE_WALK)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
			m_pMotion->SetNumMotion(STATE_WALK);
			m_bWalk = false;
		}
	}
	if (m_bSprintMotion == true)
	{//�X�v�����g���[�V����
		if (m_state == STATE_ACTION)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
			m_pMotion->SetNumMotion(STATE_ACTION);
			m_bSprintMotion = false;
		}
	}
}

//=============================================================================
// ���C���̐��� / �j��
//=============================================================================
#if 1
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
#endif
//=============================================================================
// ����֘A
//=============================================================================
#if 1
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

//=============================================================================
//�@�V�����_�[�R���C�_�[�Ƃ̓����蔻�菈��
//=============================================================================
bool CEnemy::CollisionCylinderyCollider(CCylinderCollider *pCylinderCollider, D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &Move, D3DXVECTOR3 &ColRange)
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
			BlowAway(AnotherPos);
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
bool CEnemy::CollisionPlayerAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
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
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER)
		{
			//if (m_bCollision == true)
			{
				m_pMotion->SetNumMotion(4);
			}

		}
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER)
		{
			//m_pMotion->SetNumMotion(3);
		}
		return true;
	}

	return false;
}
#endif