//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�G�l�~�[���� [enemy.cpp]
//			Author : Kobayashi_Sho-hei / ���� ����
//���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"
#include "library.h"
#include "collision.h"
#include "line.h"
#include "AIController.h"
#include "model.h"
#include "territory.h"
#include "mine.h"
#include <windows.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SPEED (1.0f)

//	�����蔻��E�_���[�W�֘A
#define CYLINDER_COLRADIUS (20.0f)
#define CYLINDER_COLHEIGHT (100.0f)
#define BLOW_MOVING        (12.0f)
#define BLOW_MOVING_CUT    (1.5f)
#define MAX_DAMAGECOUNT    (23)

// �ÓI�����o�ϐ�
//float* CEnemy::m_tmp[4] = {};


//*****************************************************************************
//	�R���X�g���N�^
//*****************************************************************************
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_move = INITIALIZE_VECTOR3;
	m_nDamageCount = 0;
	m_fBlowAngle = 0.0f;
	m_fBlowLength = 0.0f;
	m_rot = INITIALIZE_VECTOR3;
	m_posOld = INITIALIZE_VECTOR3;
	m_nDamageCounter = 0;
	m_bSuperArmor = false;
	m_bTarget = false;
	m_bCheck = false;
	m_state = STATE_NONE;
	m_nLevel = 0;
}

//*****************************************************************************
//	�f�X�g���N�^
//*****************************************************************************
CEnemy::~CEnemy() { }
//=============================================================================
//	��������
//=============================================================================
CEnemy *CEnemy::Create(int nNum, TYPE type, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40], CHARCTERTYPE charatype)
{
	return NULL;	//	���g�p����Ȃ����
}

//=============================================================================
// ����������	���e�h���N���X�̏������������g���Ă���
//=============================================================================
HRESULT CEnemy::Init(int nNum, D3DXVECTOR3 pos, char ModelTxt[40], char MotionTxt[40],int nType)
{ return S_OK; }

HRESULT CEnemy::Init(void)
{ return S_OK; }

//=============================================================================
// �I������
//=============================================================================
void  CEnemy::Uninit(void)
{
	//�@�}�[�W�\�[�g
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		if (m_tmp[nCnt] != NULL)
		{
			delete[] m_tmp[nCnt];
			m_tmp[nCnt] = NULL;
		}
	}
	//	�e�G���A�̃e���g���[���̉��
	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)
	{
		if (m_AreaInfo[nCnt] != NULL)
		{
			delete[] m_AreaInfo[nCnt];
			m_AreaInfo[nCnt] = NULL;
		}
	}

	//	�X�̃e���g���[���̉��
	if (m_TerritoryInfo != NULL)
	{
		delete[] m_TerritoryInfo;
		m_TerritoryInfo = NULL;
	}

	//	�e�ɐӔC�������ď�������
	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void  CEnemy::Update(void)
{
	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾���ǉ�(�悵�낤)
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾���ǉ�(�悵�낤)
	if (pEveCam == NULL)	// �C�x���g�J�����������Ă����灩�ǉ�(�悵�낤)
	{
		CCharacter::Update();

		//	�Q�[���J�n�܂ōX�V�����Ȃ�
		int nGameState = CGame::GetGameState();
		if (nGameState != CGame::GAMESTATE_FIRSTCOUNTDOWN && nGameState != CGame::GAMESTATE_END)
		{
			AIBasicAction();	//	AI���ʏ���
		}
		//	�Q�[���I����
		else if (nGameState == CGame::GAMESTATE_END)
		{
			if (m_bCharaMotionState == false)
			{
				m_state = STATE_NONE;
				m_pMotion->SetNumMotion(m_state);
				m_bCharaMotionState = true;
			}
		}

		Program_Line();		//	���C���֐��܂Ƃ�
		Program_State();	//	���[�V�����֘A

		// �G�l�~�[�̏�Ԃ�ݒ�
		SetCharaState(m_state);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void  CEnemy::Draw(void)
{
	CGame *pGame = CManager::GetGame();				// �Q�[���̎擾���ǉ�(�悵�낤)
	CEventCamera *pEveCam = pGame->GetEveCam();		// �C�x���g�J�����̎擾���ǉ�(�悵�낤)
	if (pEveCam == NULL)	// �C�x���g�J�����������Ă����灩�ǉ�(�悵�낤)
	{
		CCharacter::Draw();
		D3DXMATRIX mtxWorld;
		D3DXMatrixIdentity(&mtxWorld);
		CManager::GetRenderer()->GetDevice()->SetTransform(D3DTS_WORLD, &mtxWorld);
	}
}
//=============================================================================
//	�f�t�H���g�֐�
//=============================================================================
void  CEnemy::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) { }

//=============================================================================
//	���������\�[�g����
//=============================================================================
void CEnemy::InitSort(D3DXVECTOR3 pos)
{
	for (int nCntArea = 0; nCntArea < AREA_MAX; nCntArea++)
	{
		m_nAreaTerrNum[nCntArea] = 0;
		m_AreaInfo[nCntArea] = NULL;
		m_tmp[nCntArea] = NULL;
	}
	
	//	[[���e���g���[���̎擾 / �m��]]
	int nMax = 0;		//	�e���g���[�ő吔�̋L��
	int nCnt01 = 0;
	CScene* pSceneTop = CScene::GetTop(TERRITORY_PRIORITY);
	CScene* pScene = pSceneTop;
	while (pScene != NULL)
	{
		if (CScene::OBJTYPE_TERRITORY == pScene->GetObjType())
		{
			m_pTerritory = (CTerritory*)pScene;

			if (nMax == 0)//	�e���g���[�̍ő吔��new����
			{
				nMax = m_pTerritory->GetMaxObj();
				m_TerritoryInfo = new CEnemy::TERRITORY_INFO[nMax];
			}

			//	�e�e���g���[�̏����擾
			m_TerritoryInfo[nCnt01].pos = m_pTerritory->GetPos();			//	�ʒu���
			m_TerritoryInfo[nCnt01].nAreaNum = m_pTerritory->GetErea();		//	����U��ꂽ�G���A�ԍ�

			//	�e�G���A���̃e���g���[�̐����J�E���g�A�b�v(m_TerritoryInfo[����U��ꂽ�G���A�ԍ�]�̐�)
			m_nAreaTerrNum[m_TerritoryInfo[nCnt01].nAreaNum] += 1;

			nCnt01 += 1;
		}

		pScene = pScene->GetpNext();	//	���̃e���g���[����
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

		while (nAllTerrCnt != nMax)// �S�Ẵe���g���[���G���A��������܂ő���
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

	for (int nCnt = 0; nCnt < AREA_MAX; nCnt++)	//	�G���A���Ƃ̃e���g���[������
	{
		m_tmp[nCnt] = new float[m_nAreaTerrNum[nCnt]];
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

	//	������Z�����Ƀ}�[�W�\�[�g
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		MergeSort(m_AreaInfo[nAreaCnt], 0, m_nAreaTerrNum[nAreaCnt] - 1, nAreaCnt);
		
	}

	//	�Q�[���J�n�̍ۂɁA�ǂ��̃G���A����ԋ߂��������߂�(�\�[�g����)
	for (int nAreaCnt = 0; nAreaCnt < AREA_MAX; nAreaCnt++)
	{
		for (int nAreaCnt02 = 0; nAreaCnt02 < AREA_MAX; nAreaCnt02++)
		{
			if (m_AreaInfo[nAreaCnt][0].fDisSort < m_AreaInfo[nAreaCnt02][0].fDisSort)
			{
				float fSort = m_AreaInfo[nAreaCnt][0].fDisSort;
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
	MergeSort(m_AreaInfo[m_nAreaNow], 0, m_nAreaTerrNum[m_nAreaNow] - 1, m_nAreaNow);
	

	while (m_bBreak != true)
	{
		for (int nCnt = 0; nCnt < m_nAreaTerrNum[m_nAreaNow]; nCnt++)
		{
			if (m_AreaInfo[m_nAreaNow][nCnt].bFlag == false)
			{
				if (m_AreaInfo[m_nAreaNow][nCnt].fDistance == m_AreaInfo[m_nAreaNow][nNextNum].fDisSort)
				{
					float fDis = m_AreaInfo[m_nAreaNow][nCnt].fDistance;
					m_nTargetNum = nCnt;
					m_bBreak = true;
					// [[���̋��_�����܂�ɉ����ꍇ�͎n�_�ɖ߂�]]
					if (fDis >= 500.0f && m_apTerritory[2] != NULL)
					{
						m_nLineNum = 2;		//	�����Œ�l�ɖ߂�
						m_bFinish = true;	//	�n�_�ɖ߂�
					}
					break;
				}
			}
		}
		nNextNum += 1;	//	���łɃt���O�������Ă���ꍇ�͎��̋ߏ��
	}
}

//=============================================================================
//�@���ʏ���
//=============================================================================
void  CEnemy::AIBasicAction(void)
{
	D3DXVECTOR3 pos = CCharacter::GetPos();
	m_posOld = pos;				//	�����t���[���O�̈ʒu

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

		Program_Motion();	//	���[�V�����A�N�V����(�L�������m�̂Ԃ��蓙)
		Program_Move(pos,m_nTerrStart);	//	�ړ�����
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
			CGame * pGame = CManager::GetGame();
			if (pGame != NULL)
			{
				CCharacter *pChara = pGame->GetChara(m_nEnemyNo);
				if (pChara != NULL)
				{
					switch(pChara->GetCharcterType())
					{
					case CCharacter::CHARCTERTYPE_SPEED:
						
						if (m_state == STATE_ACTION) { LineConnect(((9 + m_nLevel) - (rand() % 4))); }	// �A�N�V������
						else { LineConnect(((7 + m_nLevel) - (rand() % 3))); } // �ʏ펞
						break;

					case CCharacter::CHARCTERTYPE_POWER:
						LineConnect((12 - (rand() % 3)));
						break;

					case CCharacter::CHARCTERTYPE_TECHNIQUE:
						LineConnect((12 - (rand() % 3)));
						break;
					}
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
				
				m_nAreaNow += 1;

				if (m_nAreaNow == AREA_MAX)
				{
					m_nAreaNow = 0;
				}
			}

			DisSort(pos);	//	�����̍Čv�Z
		}

		Program_Motion();	//	���[�V�����A�N�V����(�L�������m�̂Ԃ��蓙)
		Program_Move(pos,m_AreaInfo[m_nAreaNow][m_nTargetNum]);	//	�ړ�����
	}
}

//=============================================================================
//�@�ړ��E�ʒu�ύX
//=============================================================================
void CEnemy::Program_Move(D3DXVECTOR3 pos,TERRITORY_INFO territory)
{
#if 1
	if (m_bTarget == false)
	{
		float fSpeed = GetSpeed();	//	����

		//	�ړ����Z����
		if (m_state == STATE_WALK || m_state == STATE_ACTION)
		{
			pos.x += (float)cos(territory.fRadian) * (fSpeed * m_fSpeed);
			pos.z += (float)sin(territory.fRadian) * (fSpeed * m_fSpeed);
		}
		// �����_�܂ł̊p�x / ���g�̎�����
		float fDestAngle = atan2f(pos.x - territory.pos.x, pos.z - territory.pos.z);
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
#endif
}
//=============================================================================
// ���C���֐��܂Ƃ�
//=============================================================================
void  CEnemy::Program_Line(void)
{
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
}

//=============================================================================
// ���C�����q���菇
//=============================================================================
void CEnemy::LineConnect(int nRand)
{
	//	[[�ŐV�̎n�_�E�I�_�ɍX�V]]
	if (m_apTerritory[0] != NULL)
	{
		m_nTerrStart.pos = m_apTerritory[0]->GetPos();
	}

	//	[[���C����(m_nLineNum)�{��������n�_�ɖ߂�]]
	if (m_apTerritory[m_nLineNum] != NULL)
	{
		if (m_nLineTime <= nRand * 100 && m_bFinish == false ||	// �������Ԃ������Ă�����
			m_nLineNum == TERRITORY_MAX - 1)					//	�ő僉�C���������Ă�����
		{
			m_nLineNum = 2;		//	�����Œ�l�ɖ߂�
			m_bFinish = true;	//	�n�_�ɖ߂�
		}
		else//	���C����L�΂����Ԃɗ]�T�������
		{
			m_nLineNum += 1;
		}

	}
}
//=============================================================================
// ���[�V���������܂Ƃ�
//=============================================================================
void CEnemy::Program_State(void)
{
#if 1
	switch (m_state)
	{
	case STATE_NONE:		//	[[�ʏ�]]

		m_bWalk = true;
		m_bSprintMotion = true;

		break;

	case STATE_WALK:		//	[[�ړ����]]
		break;

	case STATE_ACTION:		//	[[�A�N�V�������]]

		break;

	case STATE_BLOWAWAY:	//	[[������΂���Ă���]]
		m_nDamageCount++;

		if (m_nDamageCount <= MAX_DAMAGECOUNT)
		{
			m_state = STATE_BLOWAWAY;
		}
		if (m_nDamageCount >= MAX_DAMAGECOUNT)
		{
			m_nDamageCount = 0;
			m_bWalk = true;
			m_bSprintMotion = true;

			m_state = STATE_NONE;
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

	case STATE_DAMAGE:		//	[[�_���[�W���]]

		m_nDamageCount++;
		if (m_nDamageCount == 70)
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
			m_state = STATE_NONE;
		}
		break;
	}
#endif
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
}

//=============================================================================
//�@���[�V�����A�N�V����(�L�������m�Ԃ�������)
//=============================================================================
void CEnemy::Program_Motion(void)
{
	CSound *pSound = CManager::GetSound();	//	�T�E���h�̎擾

	//�ړ����
	if (m_state != STATE_BLOWAWAY && m_state != STATE_DAMAGE)
	{
		if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_state != STATE_ACTION)
		{
			//�ړ����
			m_state = STATE_WALK;
		}
		else if ((m_CharcterType == CCharacter::CHARCTERTYPE_POWER || m_CharcterType == CCharacter::CHARCTERTYPE_TECHNIQUE) &&
			m_state != STATE_ACTION)
		{
			m_state = STATE_WALK;
		}
	}
	if (m_bWalk == true && m_state == STATE_WALK)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE018);//����
		m_pMotion->SetNumMotion(m_state);
		m_bWalk = false;
	}

	if (m_bSprintMotion == true)
	{//�X�v�����g���[�V����
		if (m_CharcterType == CCharacter::CHARCTERTYPE_SPEED && m_state == STATE_ACTION)
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
	return;
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
		// ���p���[�^AI�p�̔���
		if (m_CharcterType == CCharacter::CHARCTERTYPE_POWER)
		{
			CGame * pGame = CManager::GetGame();
			if (pGame != NULL) 
			{ 
				CCharacter *pChara = pGame->GetChara(m_nEnemyNo); 
				if (pChara != NULL)
				{
					CTypePower *pPower = (CTypePower*)pChara;
					if (pPower->CollisionSkillTiming((CCylinderCollider*)pCollider, pos, posOld, Move, ColRange) == true) {}
				}
			}
			
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
		if (pParent->GetObjType() == OBJTYPE_ENEMY || pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ROBOT)
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
				m_state = STATE_DAMAGE;
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
			//�������Ă�Ԃ̓_���[�W���
			m_state = STATE_DAMAGE;

			if (m_nDamageCounter == 0)
			{
				m_bSuperArmor = true;
				m_pMotion->SetNumMotion(4);
				m_nDamageCounter = 1;
			}

		}
		return true;
	}

	return false;
}

void CEnemy::MergeSort(TERRITORY_INFO* pArray, int start, int end, int AreaNum)
{
	//  ��Ɨp�̃f�[�^
	int middle, i, j, k;
	if (start >= end) {
		return;
	}
	//  start��end�̒��Ԓn�_�𕪊��_�Ƃ���
	middle = (start + end) / 2;
	//  �O���������ċA�I�ɐ���
	MergeSort(pArray, start, middle, AreaNum);
	//  �㔼�������ċA�I�ɐ���
	MergeSort(pArray, middle + 1, end, AreaNum);
	k = 0;
	//  ���z�̈�̃f�[�^���}�[�W���Ȃ���R�s�[����B
	for (i = start; i <= middle; i++) {
		m_tmp[AreaNum][k] = pArray[i].fDisSort;
		k++;
	}
	for (j = end; j >= middle + 1; j--) {
		m_tmp[AreaNum][k] = pArray[j].fDisSort;
		k++;
	}
	//  ���[����f�[�^���擾���āA�}�[�W���Ă����B
	i = 0;
	j = end - start;
	for (k = start; k <= end; k++) {
		if (m_tmp[AreaNum][i] <= m_tmp[AreaNum][j]) {
			pArray[k].fDisSort = m_tmp[AreaNum][i];
			i++;
		}
		else {
			pArray[k].fDisSort = m_tmp[AreaNum][j];
			j--;
		}
	}

}

//=============================================================================
//�@�U�����Ƃ̓����蔻�菈��(���{�b�g)
//=============================================================================
bool CEnemy::CollisionRobotAttackSphereCollider(CPlayerAttackSphereCollider *pShere, D3DXVECTOR3 &pos, D3DXVECTOR3 &ColRange)
{
	if (pShere->Collision(&pos, 100.0f, this) == true && pShere->GetParent() != this)
	{// �����ȊO�̍U����������������
		CScene *pParent = pShere->GetParent();
		if (pParent->GetObjType() == OBJTYPE_PLAYER || pParent->GetObjType() == OBJTYPE_ROBOT)
		{
			//�������Ă�Ԃ̓_���[�W���
			m_state = STATE_DAMAGE;

			if (m_nDamageCounter == 0)
			{
				m_bSuperArmor = true;
				m_pMotion->SetNumMotion(4);
				m_nDamageCounter = 1;
			}
		}
		return true;
	}

	return false;
}



#endif