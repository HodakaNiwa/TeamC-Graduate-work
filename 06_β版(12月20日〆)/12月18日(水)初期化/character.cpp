//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�L�����N�^�[�Ǘ� [character.cpp]
//			Author : Kobayashi_Sho-hei / ���� ���� & Hodaka Niwa
//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��

//	---<<#include>>---
#include "character.h"
#include "model.h"
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "collision.h"
#include "line.h"
#include "library.h"
#include "territory.h"
#include "sound.h"
#include "UI.h"
#include "score.h"
#include "sceneTriangle3D.h"
#include "game.h"
#include "select.h"
#include "lightorbit.h"
#include "scoreGauge.h"
#include "robot.h"
#include "point.h" 

//	---<<�}�N����`>>---
#define CHARACTER_COLLISION_POS    (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define CHARACTER_COLLISION_HEIGHT (100.0f)
#define CHARACTER_COLLISION_RADIUS (20.0f)

//�F
#define COLOR_RED	 (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_BULE	 (D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f))	//��
#define COLOR_GREEN	 (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_WHITE	 (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//��
#define COLOR_BRACK	 (D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	//��
#define COLOR_YELLOW (D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f))	//��
#define COLOR_ORENGE (D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f))	//�I�����W
#define COLOR_PAPULE (D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f))	//��

#define FAR_POS			(D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))

//#define LIGHTORBITDRAW

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�ÓI�����o�ϐ��錾
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CCharacter::CHARCTERTYPE CCharacter::m_CharcterTypeResult[8] = {};		//���U���g�p�̃L�����^�C�v
int CCharacter::m_nCuntry[8] = {};										//���U���g�p�̍��^�C�v

//���̐F�̐ݒ�
D3DXCOLOR CCharacter::m_CountryColor[CCharacter::TYPE_MAX] =
{
	COLOR_RED,		//���{
	COLOR_BULE,		//���V�A
	COLOR_GREEN,	//�A�����J
	COLOR_YELLOW,	//�u���W��
	COLOR_ORENGE,	//�C�M���X
	COLOR_PAPULE,	//�C�^���A
	COLOR_BRACK,	//�j���[�W�[�����h
	COLOR_WHITE,	//��A�t���J
};

//==============================================
//					�R���X�g���N�^
//==============================================
CCharacter::CCharacter(int nPriority, OBJTYPE objType) : CScene(nPriority,objType)
{
	m_pOldStartTerritory = NULL;
	m_pMotion = NULL;
	m_pMotionInfo = NULL;
	m_nCountGetTerritry = 0;
	m_nCntTimeCopyLine = 0;
	m_nLineTime = 0;
	m_nNumPlayerNo = -1;
	m_nType = -1;
	m_nButtonCounter = 0;
	m_nRecastCounter = 0;
	m_nRecastTimer = 0;
	m_nState = 0;
	m_nNumMotionMAX = 0;
	m_nNumKey = 0;
	m_nAttack = 0;
	m_nGetScorePoint = 0;
	m_nDestPlayer = 0;
	m_nPointCounter = 0;
	m_nCountMakeShape = 0;
	m_nCountGetTerritry = 0;
	m_nCountRobbtedTerritory = 0;
	m_fAttackSpeed = 0.0f;
	m_fRange = 0.0f;
	m_fSpeed = 0.0f;
	m_fInertia = 0.0f;
	m_fLineTime = 0.0f;
	m_bRecast = false;
	m_bBlockStartTerritory = false;
	m_bMakeShape = false;
	m_bCharaMotionState = false;
	m_bSprint = false;
	m_CharcterType = CHARCTERTYPE_MAX;
	m_Pos = INITIALIZE_VECTOR3;
	m_Rot = INITIALIZE_VECTOR3;
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//==============================================
//					�f�X�g���N�^
//==============================================
CCharacter::~CCharacter(){}

//=============================================================================
// ���f���̔j��
//=============================================================================
void CCharacter::Unload(void)
{

}


//==============================================
//					������
////==============================================
HRESULT CCharacter::Init(int nCharaNum, char ModelTxt[40], char MotionTxt[40],int nType, int nCuntry)
{
	//����������
	m_Pos = INITIALIZE_VECTOR3;
	m_Rot = INITIALIZE_VECTOR3;
	m_nCountTerritory = 0;
	m_ppUVTexName = NULL;
	m_bCharaMotionState = false;
	m_nDestPlayer = 0;
	m_nGetScorePoint = 0;
	m_nPointCounter = 0;
	m_nCountMakeShape = 0;			//�}�`���������
	m_nCountGetTerritry = 0;		//�e���g���[�̎擾��
	m_nCountRobbtedTerritory = 0;	//�e���g���[�̒D��ꂽ��
	m_nLineTime = FIRST_LINETIME;
	m_nCntTimeCopyLine = 0;
	m_bSprint = false;

	m_cModelTxt[0] = &ModelTxt[0];
	m_cMotionTxt[0] = &MotionTxt[0];
	ModelSetLode();
	int nData = 0;

	m_ppModel = NULL;
	m_pMotion = NULL;

	if (m_ppModel == NULL)
	{
		m_ppModel = new CModel*[m_nPartsNum];		//	�p�[�c���m�ۂ���
	}
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		m_ppModel[nCnt] = NULL;
	}

	CSelect *pSelect = CManager::GetSelect();
	int nPlayer[4] = {};

	/*for (int nCntCountry = 0; nCntCountry < 4; nCntCountry++)
	{
	m_nCuntry[nCntCountry] = pSelect->GetPlayerCuntry(nCntCountry);
	}*/

	for (int nCntTerritory = 0; nCntTerritory < MAX_TERRITORY; nCntTerritory++)
	{//�e���g���[�E���C���̏�����
		m_apTerritory[nCntTerritory] = NULL;
		m_apLine[nCntTerritory] = NULL;
		m_apCopyLine[nCntTerritory] = NULL;
	}

	// �ǂݍ��ރe�N�X�`���̐���ݒ�
	m_nLoadTexNum = 0;
	if (nType == CHARCTERTYPE_SPEED)
	{
		m_nLoadTexNum = 9;
	}
	if (nType == CHARCTERTYPE_POWER)
	{
		m_nLoadTexNum = 8;
	}
	if (nType == CHARCTERTYPE_TECHNIQUE)
	{
		m_nLoadTexNum = 10;
	}

	// �e�N�X�`���̓ǂݍ���
	if (nCuntry < 0) { nCuntry = 0; }
	TextureSetLode(nCuntry * m_nLoadTexNum, m_nLoadTexNum);

	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_ppModel[nCnt] == NULL)
		{
			//	���f���p�[�c�̏����l�����߂�
			D3DXVECTOR3 Pos = D3DXVECTOR3(m_aOffset[nCnt].pos.x, m_aOffset[nCnt].pos.y, m_aOffset[nCnt].pos.z);
			char * Name = m_aPartsName[nCnt];

			m_ppModel[nCnt] = CModel::Create(D3DXVECTOR3(m_aOffset[nCnt].pos.x, m_aOffset[nCnt].pos.y, m_aOffset[nCnt].pos.z), m_aPartsName[nCnt], D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			//	���f��UV�e�N�X�`�������߂適�ǉ�(�悵�낤)
			if (nCharaNum != ROBOT_CHARNUM)
			{
				m_ppModel[nCnt]->BindTexture(m_ppTexture[m_nTexIdx[nCnt]]);
			}
			else
			{
				m_ppModel[nCnt]->BindTexture(NULL);
			}

			//	�e���f���p�[�c�̐e�����߂�
			if (nCnt == 0)
			{
				m_ppModel[nCnt]->SetParent(NULL);
			}
			else
			{
				int nParent = m_nParent[nCnt];
				m_ppModel[nCnt]->SetParent(m_ppModel[m_nParent[nCnt]]);
			}
		}
	}
	MotionSetLode();
	m_pMotion = CMotion::Create();
	m_pMotion->SetMaxModel(m_nPartsNum);
	m_pMotion->SetModel(m_ppModel);
	m_pMotion->SetMotion(m_pMotionInfo);
	m_pMotion->SetMaxMotion(m_nNumMotionMAX);
	m_pMotion->Init();

	m_nNumPlayerNo = nCharaNum;

	// �R���C�_�[���쐬���鏈��
	CreateCylinderCollider();


	return S_OK;
}

//==============================================
//			�~�����쐬���鏈��
//==============================================
void CCharacter::CreateCylinderCollider(void)
{
	// �R���C�_�[�Ǌ��N���X�𐶐�
	CColliderManager *pColManager = CColliderManager::Create();
	if (pColManager == NULL) { return; }

	// �~�����쐬
	CCylinderCollider *pCylinder = CCylinderCollider::Create(CHARACTER_COLLISION_POS, D3DXVECTOR3(1.0f, 1.0f, 1.0f),
		CHARACTER_COLLISION_RADIUS, CHARACTER_COLLISION_HEIGHT, false);
	if (pCylinder == NULL) { return; }

	// �e��ݒ�
	pCylinder->SetParent(this);
	pCylinder->SetParentMtxWorld(&m_mtxWorld);

	// �R���C�_�[�Ǌ��N���X�̃|�C���^��ݒ�
	pColManager->SetCollider(pCylinder, 0);
	SetCOlliderManager(pColManager);
}


//==============================================
//					�I��
//==============================================
void CCharacter::Uninit()
{
	//!+ ---<<���[�V�������̉��>>---
		//�L�[�t���[���̈ʒu�E��]���̉��(���[�V�����̎�ޕ� / �L�[��)
	for (int nCnt = 0; nCnt < m_nNumMotionMAX; nCnt++)
	{
		for (int nCnt02 = 0; nCnt02 < m_pMotionInfo[nCnt].nNumKey; nCnt02++)
		{
			if (m_pMotionInfo[nCnt].m_pKeyInfo[nCnt02].aKey != NULL)
			{
				delete[] m_pMotionInfo[nCnt].m_pKeyInfo[nCnt02].aKey;
				m_pMotionInfo[nCnt].m_pKeyInfo[nCnt02].aKey = NULL;
			}
		}
	}
	//	�L�[�t���[�����̉��(���[�V�����̎�ޕ�)
	for (int nCnt = 0; nCnt < m_nNumMotionMAX; nCnt++)
	{
		if (m_pMotionInfo[nCnt].m_pKeyInfo != NULL)
		{
			delete[] m_pMotionInfo[nCnt].m_pKeyInfo;
			m_pMotionInfo[nCnt].m_pKeyInfo = NULL;
		}
	}
	//	���[�V�������̉��
	if (m_pMotionInfo != NULL)
	{
		delete[] m_pMotionInfo;
		m_pMotionInfo = NULL;
	}
	//	���
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//!+ ---<<���f�����̉��>>---
	//	�I�t�Z�b�g
	if (m_aOffset != NULL)
	{
		delete[] m_aOffset;
		m_aOffset = NULL;
	}

	//	�y�A�����g���
	if (m_nParent != NULL)
	{
		delete[] m_nParent;
		m_nParent = NULL;
	}

	//	�C���f�b�N�X���
	if (m_aIndex != NULL)
	{
		delete[] m_aIndex;
		m_aIndex = NULL;
	}

	// �g�p����e�N�X�`���̔ԍ�
	if (m_nTexIdx != NULL)
	{
		delete[] m_nTexIdx;
		m_nTexIdx = NULL;
	}

	//	�t�@�C�������(�_�u���|�C���^�������Ă���̂�)
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_aPartsName[nCnt] != NULL)
		{
			delete[] m_aPartsName[nCnt];
			m_aPartsName[nCnt] = NULL;
		}
	}
	if (m_aPartsName != NULL)
	{
		delete[] m_aPartsName;
		m_aPartsName = NULL;
	}

	// �e�N�X�`���̃t�@�C�������
	for (int nCnt = 0; nCnt < m_nTexNum; nCnt++)
	{
		if (m_ppUVTexName[nCnt] != NULL)
		{
			delete[] m_ppUVTexName[nCnt];
			m_ppUVTexName[nCnt] = NULL;
		}
	}
	if (m_ppUVTexName != NULL)
	{
		delete[] m_ppUVTexName;
		m_ppUVTexName = NULL;
	}

	//	���f���p�[�c���(�_�u���|�C���^�������Ă���̂�)
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_ppModel[nCnt] != NULL)
		{
			m_ppModel[nCnt]->Uninit();
			delete m_ppModel[nCnt];
			m_ppModel[nCnt] = NULL;
		}
	}
	if (m_ppModel != NULL)
	{
		delete[] m_ppModel;
		m_ppModel = NULL;
	}

	// �e�N�X�`���̊J��
	if (m_ppTexture != NULL)
	{
		for (int nCntTex = 0; nCntTex < m_nLoadTexNum; nCntTex++)
		{
			if (m_ppTexture[nCntTex] != NULL)
			{
				m_ppTexture[nCntTex]->Release();
				m_ppTexture[nCntTex] = NULL;
			}
		}
		delete[] m_ppTexture;
		m_ppTexture = NULL;
	}

	//���S�t���O�𗧂Ă�
	Release();
}

//==============================================
//					�X�V
//==============================================
void CCharacter::Update()
{
	CInputKeyboard * pInputKeyboard = CManager::GetInputkeyboard();	//�L�[�{�[�h�̎擾

	if (m_pMotion != NULL)
	{
		m_pMotion->Update();
	}

	// �X�R�A�ϓ��̕\��
	if (CManager::GetMode() != CManager::MODE_GAME || m_nGetScorePoint == 0) { return; }
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }
	CUI *pUI = pGame->GetUI();
	if (pUI == NULL) { return; }
	pUI->CreateScoreChange(m_nNumPlayerNo, m_nGetScorePoint);
	m_nGetScorePoint = 0;

#ifdef LIGHTORBITDRAW
	// ���̋O�Ղ�\������
	CreateLightOrbit(pGame);
#endif

	// ��ʃG�t�F�N�g�𐶐�
	pUI->CreateScreenEffect(m_nNumPlayerNo, m_nDestPlayer);
}

//==============================================
//					�`��
//==============================================
void CCharacter::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxParent, mtxScale;	//�v�Z

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �A�E�g���C���̕`��
	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_ppModel[nCnt] != NULL)
		{
			m_ppModel[nCnt]->DrawOutline();
		}
	}

	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		if (m_ppModel[nCnt] != NULL)
		{
			m_ppModel[nCnt]->Draw(1.0f);
		}
	}
}

//==============================================
//					���f���̓ǂݍ���
//==============================================
#if 1
void CCharacter::ModelSetLode(void)
{
	FILE* pFile;
	char* pStrCur;		//	�擪������̃|�C���^
	char aLine[256];	//	������̓ǂݍ���
	char aStr[256];		//	������̔����o��
	int nDelay;			//	�ǂݍ��ޕ����ʒu�����炷
	int nCnt = 0;
	int nCntLoadTex = 0;
	m_ppUVTexName = NULL;
	m_ppTexture = NULL;
	m_aPartsName = NULL;
	m_aIndex = NULL;
	m_nParent = NULL;
	m_nTexIdx = NULL;
	m_aOffset = NULL;

	pFile = fopen(m_cModelTxt[0], "r");	//	�ǂݍ��ރe�L�X�g��

	if (pFile != NULL)
	{
		while (1)
		{
			pStrCur = CCharacter::ReadLine(pFile, &aLine[0]);
			strcpy(&aLine[0], pStrCur);

			if (memcmp(pStrCur, "MODEL_LOAD", strlen("MODEL_LOAD")) == 0)
			{
				while (1)
				{
					fgets(&aLine[0], 256, pFile);
					pStrCur = CCharacter::GetLIneTop(&aLine[0]);
					if (memcmp(pStrCur, "NUM_UV = ", strlen("NUM_UV = ")) == 0)	// ---<<�g�p����UV�e�N�X�`���̐��̓ǂݍ���>>---
					{
						// �������i�߂�
						pStrCur += strlen("NUM_UV = ");
						strcpy(aStr, pStrCur);

						// �g�p����UV�e�N�X�`���̐���ǂݍ���
						m_nTexNum = atoi(aStr);

						// �ǂݍ���UV�e�N�X�`���̐��������I�m��
						m_ppUVTexName = new char*[m_nTexNum];
					}

					if (memcmp(pStrCur, "UV_NAME ", strlen("UV_NAME ")) == 0)	// ---<<UV�e�N�X�`���̓ǂݍ���>>---
					{
						pStrCur += strlen("UV_NAME ");
						strcpy(aStr, pStrCur);
						while (1)
						{
							if (pStrCur[strlen(pStrCur) - 1] != 'g')
							{
								pStrCur[strlen(pStrCur) - 1] = '\0';//���s������NULL�����ɕϊ�(�Q�l�T�C�g::http://hisashi.me/blog/?p=714)
							}
							else
							{
								break;
							}
						}
						m_ppUVTexName[nCntLoadTex] = new char[256];	//	������̊m��
						strcpy(m_ppUVTexName[nCntLoadTex], pStrCur);
						nCntLoadTex++;
					}

					if (memcmp(pStrCur, "NUM_MODEL = ", strlen("NUM_MODEL = ")) == 0)	// ---<<�p�[�c���̓ǂݍ���>>---
					{
						pStrCur += strlen("NUM_MODEL = ");
						strcpy(aStr, pStrCur);
						m_nPartsNum = atoi(aStr);
						m_aPartsName = new char*[m_nPartsNum];
						for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
						{
							m_aPartsName[nCnt] = new char[256];
						}
						m_aIndex = new int[m_nPartsNum];
						m_nParent = new int[m_nPartsNum];
						m_nTexIdx = new int[m_nPartsNum];
						m_aOffset = new MODEL_OFFSET[m_nPartsNum];

						for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
						{
							m_aIndex[nCnt] = 0;
							m_nParent[nCnt] = 0;
							m_nTexIdx[nCnt] = 0;
							m_aOffset[nCnt].pos = INITIALIZE_VECTOR3;
							m_aOffset[nCnt].rot = INITIALIZE_VECTOR3;
						}
					}

					if (memcmp(pStrCur, "MODEL_NAME ", strlen("MODEL_NAME ")) == 0)	// ---<<�e�p�[�c�̖��O���擾>>---
					{
						pStrCur += strlen("MODEL_NAME ");

						strcpy(aStr, pStrCur);

						while (1)
						{
							if (pStrCur[strlen(pStrCur) - 1] != 'x') { pStrCur[strlen(pStrCur) - 1] = '\0'; }

							else { break; }

							strcpy(&m_aPartsName[nCnt][0], pStrCur);
						}

						nCnt += 1;
					}

					//�ǂݍ��݂̏I���
					if (memcmp(pStrCur, "END_MODEL_LOAD", strlen("END_MODEL_LOAD")) == 0) { break; }
				}
			}

			nCnt = 0;	//	�����ň��0�ɂ��ǂ�

			if (memcmp(pStrCur, "PARAMETERSET", strlen("PARAMETERSET")) == 0)
			{
				while (1)
				{
					fgets(&aLine[0], 256, pFile);
					pStrCur = GetLIneTop(&aLine[0]);

					if (memcmp(pStrCur, "STATUSSET", strlen("STATUSSET")) == 0)
					{
						while (1)
						{
							fgets(&aLine[0], 256, pFile);
							pStrCur = GetLIneTop(&aLine[0]);
							if (memcmp(pStrCur, "ATTACK = ", strlen("ATTACK = ")) == 0)
							{//�U���͂����߂�
								pStrCur += strlen("ATTACK = ");
								strcpy(aStr, pStrCur);
								m_nAttack = atoi(aStr);
							}
							//	---<<�U�����x�����߂�>>---
							if (memcmp(pStrCur, "ATTACKSPEED = ", strlen("ATTACKSPEED = ")) == 0)
							{
								pStrCur += strlen("ATTACKSPEED = ");
								strcpy(aStr, pStrCur);
								m_fAttackSpeed = (float)atoi(aStr);
							}
							//	---<<�˒�>>---
							if (memcmp(pStrCur, "RANGE =", strlen("RANGE =")) == 0)
							{
								pStrCur += strlen("RANGE =");
								strcpy(aStr, pStrCur);
								m_fRange = (float)atof(aStr);

								//for (int nCntWord = 0; nCntWord < 3; nCntWord++)
								//{
								//	nDelay = PopString(pStrCur, aStr);
								//	pStrCur += nDelay;	//	�ǂݎ�������������炷
								//	strcpy(aStr, pStrCur);

								//	if (nCntWord == 0) { m_aOffset[nCnt].pos.x = (float)atof(aStr); }
								//	else if (nCntWord == 1) { m_aOffset[nCnt].pos.y = (float)atof(aStr); }
								//	else { m_aOffset[nCnt].pos.z = (float)atof(aStr); }
								//}
							}
							//	---<<�ړ���>>---
							if (memcmp(pStrCur, "SPEED =", strlen("SPEED =")) == 0)
							{
								pStrCur += strlen("SPEED =");
								strcpy(aStr, pStrCur);
								m_fSpeed = (float)atof(aStr);

								//for (int nCntWord = 0; nCntWord < 3; nCntWord++)
								//{
								//	nDelay = PopString(pStrCur, aStr);
								//	pStrCur += nDelay;	//	�ǂݎ�������������炷
								//	strcpy(aStr, pStrCur);

								//	if (nCntWord == 0) { m_aOffset[nCnt].rot.x = (float)atof(aStr); }
								//	else if (nCntWord == 1) { m_aOffset[nCnt].rot.y = (float)atof(aStr); }
								//	else { m_aOffset[nCnt].rot.z = (float)atof(aStr); }
								//}
							}
							//	---<<����>>---
							if (memcmp(pStrCur, "INERTIA =", strlen("INERTIA =")) == 0)
							{
								pStrCur += strlen("INERTIA =");
								strcpy(aStr, pStrCur);
								m_fInertia = (float)atof(aStr);
							}
							//	---<<���������鎞��>>---
							if (memcmp(pStrCur, "LINETIME =", strlen("LINETIME =")) == 0)
							{
								pStrCur += strlen("LINETIME =");
								strcpy(aStr, pStrCur);
								m_fLineTime = (float)atof(aStr);
							}
							//	�ǂݍ��ݏI��
							if (memcmp(pStrCur, "END_STATUSSET", strlen("END_STATUSSET")) == 0) { break; }
						}
					}
					//	�ǂݍ��ݏI��
					if (memcmp(pStrCur, "END_PARAMETERSET", strlen("END_PARAMETERSET")) == 0) { break; }
				}
			}

			if (memcmp(pStrCur, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
			{
				while (1)
				{
					fgets(&aLine[0], 256, pFile);
					pStrCur = GetLIneTop(&aLine[0]);

					if (memcmp(pStrCur, "PARTSSET", strlen("PARTSSET")) == 0)
					{
						while (1)
						{
							fgets(&aLine[0], 256, pFile);
							pStrCur = GetLIneTop(&aLine[0]);
							if (memcmp(pStrCur, "INDEX = ", strlen("INDEX = ")) == 0)
							{
								pStrCur += strlen("INDEX = ");
								strcpy(aStr, pStrCur);
								m_aIndex[nCnt] = atoi(aStr);
							}
							if (memcmp(pStrCur, "TEX_INDEX = ", strlen("TEX_INDEX = ")) == 0)
							{
								pStrCur += strlen("TEX_INDEX = ");
								strcpy(aStr, pStrCur);
								m_nTexIdx[nCnt] = atoi(aStr);
							}
							//	---<<�e�p�[�c�̐e�����߂�>>---
							if (memcmp(pStrCur, "PARENT = ", strlen("PARENT = ")) == 0)
							{
								pStrCur += strlen("PARENT = ");
								strcpy(aStr, pStrCur);
								m_nParent[nCnt] = atoi(aStr);
							}
							//	---<<�e�p�[�c�̈ʒu>>---
							if (memcmp(pStrCur, "POS =", strlen("POS =")) == 0)
							{
								pStrCur += strlen("POS =");
								strcpy(aStr, pStrCur);

								for (int nCntWord = 0; nCntWord < 3; nCntWord++)
								{
									nDelay = PopString(pStrCur, aStr);
									pStrCur += nDelay;	//	�ǂݎ�������������炷
									strcpy(aStr, pStrCur);

									if (nCntWord == 0) { m_aOffset[nCnt].pos.x = (float)atof(aStr); }
									else if (nCntWord == 1) { m_aOffset[nCnt].pos.y = (float)atof(aStr); }
									else { m_aOffset[nCnt].pos.z = (float)atof(aStr); }
								}
							}
							//	---<<�e�p�[�c�̌���>>---
							if (memcmp(pStrCur, "ROT =", strlen("ROT =")) == 0)
							{
								pStrCur += strlen("ROT =");
								strcpy(aStr, pStrCur);

								for (int nCntWord = 0; nCntWord < 3; nCntWord++)
								{
									nDelay = PopString(pStrCur, aStr);
									pStrCur += nDelay;	//	�ǂݎ�������������炷
									strcpy(aStr, pStrCur);

									if (nCntWord == 0) { m_aOffset[nCnt].rot.x = (float)atof(aStr); }
									else if (nCntWord == 1) { m_aOffset[nCnt].rot.y = (float)atof(aStr); }
									else { m_aOffset[nCnt].rot.z = (float)atof(aStr); }
								}
							}
							//	�ǂݍ��ݏI��
							if (memcmp(pStrCur, "END_PARTSSET", strlen("END_PARTSSET")) == 0) { nCnt++; break; }
						}
					}
					//	�ǂݍ��ݏI��
					if (memcmp(pStrCur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0) { break; }
				}
			}
			//	�ǂݍ��ݏI��
			if (memcmp(pStrCur, "END_CHARACTERSET", strlen("END_CHARACTERSET")) == 0) { break; }
		}
	}
	fclose(pFile);
}
#endif

//==============================================
//			���[�V�����̓ǂݍ���
//==============================================
#if 1
void CCharacter::MotionSetLode(void)
{

	FILE *pFile;
	char * pStrCur;//�擪������̃|�C���^
	char aLine[256];//������̓ǂݍ���
	char aStr[256];//������̔����o��
	int nWord;
	int nMotionNum = 0;	//	���[�V�����̎�ޕ��J�E���g����
	m_pMotionInfo = NULL;


	pFile = fopen(m_cMotionTxt[0], "r");	//	---<<�e�L�X�g�ǂݍ���>>---

	if (pFile != NULL)
	{
		pStrCur = ReadLine(pFile, &aLine[0]);
		strcpy(&aLine[0], pStrCur);
		if (memcmp(pStrCur, "MOTIONNUM = ", strlen("MOTIONNUM = ")) == 0)	//	---<<���[�V�����ő吔�̎擾>>---
		{
			pStrCur += strlen("MOTIONNUM = ");
			strcpy(aStr, pStrCur);
			m_nNumMotionMAX = atoi(aStr);
			if (m_pMotionInfo == NULL)
			{
				m_pMotionInfo = new CMotion::MOTION_INFO[m_nNumMotionMAX];
				ClearMotionInfo();
			}
		}

		while (1)
		{
			fgets(&aLine[0], 256, pFile);
			pStrCur = GetLIneTop(&aLine[0]);

			if (memcmp(pStrCur, "MOTIONSET", strlen("MOTIONSET")) == 0)
			{

				fgets(&aLine[0], 256, pFile);
				pStrCur = GetLIneTop(&aLine[0]);
				if (memcmp(pStrCur, "LOOP = ", strlen("LOOP = ")) == 0)	//	---<<���[�v���邩���Ȃ���>>---
				{
					pStrCur += strlen("LOOP = ");
					strcpy(aStr, pStrCur);
					bool bLoop;
					if (atoi(aStr) == 1) { bLoop = true; }
					else { bLoop = false; }
					m_pMotionInfo[nMotionNum].m_bLoop = bLoop;
				}

				fgets(&aLine[0], 256, pFile);
				pStrCur = GetLIneTop(&aLine[0]);
				if (memcmp(pStrCur, "NUM_KEY = ", strlen("NUM_KEY = ")) == 0)	//	---<<�L�[�t���[���̐�>>---
				{
					pStrCur += strlen("NUM_KEY = ");
					strcpy(aStr, pStrCur);
					m_nNumKey = atoi(aStr);
					m_pMotionInfo[nMotionNum].nNumKey = atoi(aStr);
					if (m_pMotionInfo[nMotionNum].m_pKeyInfo == NULL)
					{
						m_pMotionInfo[nMotionNum].m_pKeyInfo = new CMotion::KEY_INFO[m_pMotionInfo[nMotionNum].nNumKey];	//	�L�[�̐����m��
						ClearMotionInfo_KeyInfo(nMotionNum);
					}
				}

				int nKeyNum = 0;	//	�L�[�����J�E���g����
				int nParts = 0;		//	�p�[�c���J�E���g����
				while (1)
				{
					fgets(&aLine[0], 256, pFile);
					pStrCur = GetLIneTop(&aLine[0]);

					if (memcmp(pStrCur, "KEYSET", strlen("KEYSET")) == 0)
					{
						if (m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey == NULL)
						{
							m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey = new CMotion::KEY[m_nPartsNum];	//	���f���̃p�[�c���m��
							ClearMotionInfo_KeyInfo_Key(nMotionNum, nKeyNum);
						}

						while (1)
						{
							fgets(&aLine[0], 256, pFile);
							pStrCur = GetLIneTop(&aLine[0]);
							if (memcmp(pStrCur, "FRAME = ", strlen("FRAME = ")) == 0)	//	---<<�t���[�����̎擾>>---
							{
								pStrCur += strlen("FRAME = ");
								strcpy(aStr, pStrCur);
								m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].nFrame = atoi(aStr);	//	���[�V�����̐�����
							}


							if (memcmp(pStrCur, "KEY", strlen("KEY")) == 0)
							{
								while (1)
								{
									if (memcmp(pStrCur, "POS =", strlen("POS =")) == 0)	//	---<<�ʒu���>>---
									{
										pStrCur += strlen("POS =");
										strcpy(aStr, pStrCur);

										for (int nCntWord = 0; nCntWord < 3; nCntWord++)	//	XYZ����
										{
											nWord = PopString(pStrCur, aStr);
											pStrCur += nWord;	//	�����s�ł��ǂݍ��ވʒu��ς���
											strcpy(aStr, pStrCur);

											if (nCntWord == 0) { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fPosX = (float)atof(aStr); }
											else if (nCntWord == 1) { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fPosY = (float)atof(aStr); }
											else { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fPosZ = (float)atof(aStr); }
										}
									}

									if (memcmp(pStrCur, "ROT =", strlen("ROT =")) == 0)	//	---<<��]�����>>---
									{
										pStrCur += strlen("ROT =");
										strcpy(aStr, pStrCur);

										for (int nCntWord = 0; nCntWord < 3; nCntWord++)	//	XYZ����
										{
											nWord = PopString(pStrCur, aStr);
											pStrCur += nWord;	//	�����s�ł��ǂݍ��ވʒu��ς���
											strcpy(aStr, pStrCur);

											if (nCntWord == 0) { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fRotX = (float)atof(aStr); }
											else if (nCntWord == 1) { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fRotY = (float)atof(aStr); }
											else { m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey[nParts].fRotZ = (float)atof(aStr); }
										}
									}

									//	1�p�[�c�̃L�[���擾�I���
									if (memcmp(pStrCur, "END_KEY", strlen("END_KEY")) == 0)
									{
										nParts += 1;
										break;
									}

									fgets(&aLine[0], 256, pFile);
									pStrCur = GetLIneTop(&aLine[0]);
								}
							}

							if (memcmp(pStrCur, "END_KEYSET", strlen("END_KEYSET")) == 0)	//	1�p�[�c�̃L�[�t���[�����擾�I���
							{
								nParts = 0;
								nKeyNum += 1;
								break;
							}
						}
					}
					//	�Ђƒi���I���
					if (memcmp(pStrCur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0) { nMotionNum += 1; break; }
				}
			}
			//	�e�L�X�g�̏I��
			if (memcmp(pStrCur, "END_MOTIONSET", strlen("END_MOTIONSET")) == 0&& nMotionNum == m_nNumMotionMAX) { break; }
		}
	}
	fclose(pFile);
}
#endif

//==============================================
//					�e�N�X�`���̓ǂݍ���
//==============================================
void CCharacter::TextureSetLode(int nStartIdx, int nNumTex)
{

	//�f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	if (pDevice == NULL) { return; }

	// ���������m��
	m_ppTexture = new LPDIRECT3DTEXTURE9[nNumTex];
	if (m_ppTexture == NULL) { return; }

	// �e�N�X�`���̓ǂݍ���
	int nLoadTex = 0;
	for (int nCntTex = nStartIdx; nCntTex < nStartIdx + nNumTex; nCntTex++, nLoadTex++)
	{
		// ��������������
		m_ppTexture[nLoadTex] = NULL;

		// �t�@�C������ǂݍ���
		D3DXCreateTextureFromFile(pDevice, m_ppUVTexName[nCntTex], &m_ppTexture[nLoadTex]);
	}
}

#if 1
char *CCharacter::ReadLine(FILE *pFile, char *pDst)
{
	fgets(pDst, 256, pFile);

	while (1)
	{
		if (memcmp(pDst, "#", strlen("#")) == 0) { fgets(pDst, 256, pFile); }
		else if (memcmp(pDst, " ", strlen(" ")) == 0){ pDst += 1; }
		else if (memcmp(pDst, "\t", strlen("\t")) == 0) { pDst += 1; }
		else if (memcmp(pDst, "\n", strlen("\n")) == 0) { fgets(pDst, 256, pFile); }
		else { break; }
	}
	return pDst;
}

char *CCharacter::GetLIneTop(char *pSrc)
{
	while (1)
	{
		if (memcmp(pSrc, " ", strlen(" ")) == 0) { pSrc += 1; }
		else if (memcmp(pSrc, "\t", strlen("\t")) == 0) { pSrc += 1; }
		else { break; }
	}

	return pSrc;
}

int CCharacter::PopString(char *pSrc, char *pDst)
{
	int nCnt = 0;

	while (1)
	{
		nCnt += 1;
		if (memcmp(pSrc, " ", strlen(" ")) == 0) { pSrc = "\0"; break; }
		else { pSrc += 1; }
	}

	strcpy(pDst, pSrc);

	return nCnt;
}
#endif

//==============================================
//			���[�V���������N���A���鏈��
//==============================================
void CCharacter::ClearMotionInfo(void)
{
	if (m_pMotionInfo == NULL) { return; }
	for (int nCntMotion = 0; nCntMotion < m_nNumMotionMAX; nCntMotion++)
	{
		m_pMotionInfo[nCntMotion].m_bLoop = false;
		m_pMotionInfo[nCntMotion].nNumKey = 0;
		m_pMotionInfo[nCntMotion].m_pKeyInfo = NULL;
	}
}

//==============================================
//			�L�[�����N���A���鏈��
//==============================================
void CCharacter::ClearMotionInfo_KeyInfo(int nMotionIdx)
{
	if (m_pMotionInfo == NULL) { return; }
	if (m_pMotionInfo[nMotionIdx].m_pKeyInfo == NULL) { return; }
	for (int nCntKeyInfo = 0; nCntKeyInfo < m_pMotionInfo[nMotionIdx].nNumKey; nCntKeyInfo++)
	{
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nCntKeyInfo].aKey = NULL;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nCntKeyInfo].nFrame = 0;
	}
}

//==============================================
//			�L�[���N���A���鏈��
//==============================================
void CCharacter::ClearMotionInfo_KeyInfo_Key(int nMotionIdx, int nKeyInfoIdx)
{
	if (m_pMotionInfo == NULL) { return; }
	if (m_pMotionInfo[nMotionIdx].m_pKeyInfo == NULL) { return; }
	if (m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey == NULL) { return; }
	for (int nCntKey = 0; nCntKey < m_nPartsNum; nCntKey++)
	{
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fPosX = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fPosY = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fPosZ = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fRotX = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fRotY = 0.0f;
		m_pMotionInfo[nMotionIdx].m_pKeyInfo[nKeyInfoIdx].aKey[nCntKey].fRotZ = 0.0f;
	}
}

//=============================================================================
// �e���g���[�̎擾�X�V����
//=============================================================================
void CCharacter::CountDownLineTime(void)
{
	
	if (m_apTerritory[TERRITORY_TOP] != NULL)
	{
		//����`�����Ԃ����炷
		m_nLineTime--;

		if (m_nLineTime < 0)
		{
			m_nLineTime = FIRST_LINETIME;								//�`��ł��鎞�Ԃ����Z�b�g����

			//�l���|�C���g��\��	
			if (m_apTerritory[TERRITORY_TOP]->GetPlayerNumber() != m_nNumPlayerNo)
			{
				int nGetPoint = m_apTerritory[TERRITORY_TOP]->GetPoint();

				// �Q�[���N���X���擾
				CGame *pGame = CManager::GetGame();
				if (pGame != NULL)
				{
					if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((m_apTerritory[TERRITORY_TOP]->GetErea() + 2) % 4))
					{
						nGetPoint += 1;
					}
				}

				CPoint::Create(nGetPoint, m_apTerritory[TERRITORY_TOP]->GetPos(), m_CountryColor[m_nNumPlayerNo]);
			}

			GetTerritory(m_apTerritory[TERRITORY_TOP]);					//�N�_�̂ݓ_��������
			ResetList();												//���X�g������������
			UninitLine();												//���C���̔j��
		}
	}
}

//=============================================================================
// �e���g���[�̎擾�𒆒f���鏈��
//=============================================================================
void CCharacter::Interruption(void)
{
	//�C�ӂɃe���g���[�擾�𒆒f����
	m_nLineTime = FIRST_LINETIME;					//�`��ł��鎞�Ԃ����Z�b�g����
	ResetList();									//���X�g������������
	UninitLine();									//���C���̔j��
}


//=============================================================================
//�@�}�`������������̏���
//=============================================================================
void CCharacter::UpdateShapeComplete(void)
{
	if (!m_bMakeShape) { return; }

	//���Ԃ̉Z
	m_nCntTimeCopyLine++;

	if (m_nCntTimeCopyLine > LINE_TIME) { UninitCopyLine(); }
}

//=============================================================================
// �e���g���[�����X�g�ɒǉ�
//=============================================================================
void CCharacter::AddTerritoryList(CTerritory * pTerritory)
{
	//�T�E���h�̎擾
	CSound *pSound = CManager::GetSound();
	int nMode = CManager::GetMode();

	//�}�`���������Ă��邩�ǂ����̔���
	if (m_nCountTerritory >= MIN_TERRITORY)
	{
		if (true == ChackSamePos(pTerritory, TERRITORY_TOP))
		{

			int nGetPoint = 0;																					//�����|�C���g
			ShapeComplete(pTerritory, &nGetPoint);																//�}�`�����������Ƃ��̏���
			ChackInShape(&nGetPoint);																			//�}�`���Ƀe���g���[���邩����
			CreateEndLine();																					//�Ō�̐��̐���
			CPoint::Create(nGetPoint, m_apTerritory[TERRITORY_TOP]->GetPos(), m_CountryColor[m_nNumPlayerNo]);	//�l���|�C���g�̕\��
			ResetList();																						//���X�g�̏�����
			ResetLine();																						//���C���̏�����
			m_nCountMakeShape++;																				//�}�`�̊������������Z

			if (nMode != CManager::MODE_TUTORIAL)
			{
				if (m_nPointCounter >= 9)
				{//�e���g���[��9�ȏ�擾�����犽��
					pSound->PlaySound(CSound::SOUND_LABEL_SE027);
					pSound->SetVolume(CSound::SOUND_LABEL_SE027, 3.0f);
				}
			}

			//����������
			m_nPointCounter = 0;
			return;
		}
	}

	//���X�g�̒ǉ�����
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apTerritory[nCnt] == NULL)
		{
			if (m_bBlockStartTerritory)
			{//�擾��A�����N�_�����Ȃ��悤�ɂ���
				D3DXVECTOR3 Pos = pTerritory->GetPos();
				D3DXVECTOR3 OldStartPos = INITIALIZE_VECTOR3;
				if (m_pOldStartTerritory != NULL) { OldStartPos = m_pOldStartTerritory->GetPos(); }

				if ((nCnt != TERRITORY_TOP) && (Pos != OldStartPos))
				{
					m_apTerritory[nCnt] = pTerritory;
					m_nCountTerritory++;
				}
			}
			else
			{//�ʏ�
				m_apTerritory[nCnt] = pTerritory;
				m_nCountTerritory++;
			}

			//���C���𐶐�����
			if ((nCnt != 0))
			{
				if (m_apLine[nCnt - 1] == NULL)
				{
					if ((m_apTerritory[nCnt - 1] != NULL) && (m_apTerritory[nCnt] != NULL))
					{
						m_apLine[nCnt - 1] = CLine::Create(m_apTerritory[nCnt - 1]->GetPos(), m_apTerritory[nCnt]->GetPos(), m_nNumPlayerNo, nCnt - 1);
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
bool CCharacter::ChackSamePos(CTerritory * pTerritory, int nNumTerritory)
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
void CCharacter::ShapeComplete(CTerritory * pTerritory, int * nGetPoint)
{
	//�}�`������
	m_bBlockStartTerritory = true;
	m_pOldStartTerritory = pTerritory;
	m_bMakeShape = true;
	m_nLineTime = FIRST_LINETIME;		//�`��ł��鎞�Ԃ����Z�b�g����

	//�e���g���[�̎擾
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL)
		{
			//�����|�C���g�ɉ��Z
			if (m_apTerritory[nCnt]->GetPlayerNumber() != m_nNumPlayerNo)
			{
				*nGetPoint += m_apTerritory[nCnt]->GetPoint();

				// �Q�[���N���X���擾
				CGame *pGame = CManager::GetGame();
				if (pGame != NULL)
				{
					if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((pTerritory->GetErea() + 2) % 4))
					{
						*nGetPoint += 1;
					}
				}
			}

			m_nPointCounter++;
			GetTerritory(m_apTerritory[nCnt]);
		}
	}
}

//=============================================================================
// �}�`���Ƀe���g���[�����邩�ǂ���
//=============================================================================
void CCharacter::ChackInShape(int * nGetPoint)
{
	//�ϐ��錾
	D3DXVECTOR3 * pListPos = NULL;
	TRAIANGLE * pTraiangle = NULL;
	int * pnFarNumTerritory = NULL;
	float * pfLength = NULL;

	pListPos = new D3DXVECTOR3[m_nCountTerritory];
	pTraiangle = new TRAIANGLE[m_nCountTerritory - 2];	//�O�p�`�̐������������m�ۂ���
	pnFarNumTerritory = new int[m_nCountTerritory - 1];		//������
	pfLength = new float[m_nCountTerritory - 1];			//����
	bool bEnter = false;

	InitTraiangle(pTraiangle, m_nCountTerritory - 2);	//�O�p�`�̍\���̂̏�����

	for (int nCnt = 0; nCnt < m_nCountTerritory - 1; nCnt++)
	{
		pnFarNumTerritory[nCnt] = 0;
		pfLength[nCnt] = 0.0f;
	}

	//�e���g���[�̍��W���擾
	for (int nCnt = 0; nCnt < m_nCountTerritory; nCnt++)
	{
		if (m_apTerritory[nCnt] != NULL) { pListPos[nCnt] = m_apTerritory[nCnt]->GetPos(); }
	}

	//--------------------------------
	// �o�����}�`����O�p�`�𐶐�����
	//--------------------------------
	//�n�_���牓�����Ƀe���g���[��ID���\�[�g����
	for (int nCntTerritory = 1; nCntTerritory < m_nCountTerritory; nCntTerritory++)
	{
		float fLength = CFunctionLib::Vec2Length(pListPos[TERRITORY_TOP].x, pListPos[TERRITORY_TOP].z, pListPos[nCntTerritory].x, pListPos[nCntTerritory].z);
		int NowTerritory = nCntTerritory;

		//�������r�E�\�[�g����
		for (int nCnt = 0; nCnt < m_nCountTerritory - 1; nCnt++)
		{
			//���l�̓���ւ�
			if (fLength > pfLength[nCnt])
			{
				//�����̓���ւ�
				float fCopy = pfLength[nCnt];
				pfLength[nCnt] = fLength;
				fLength = fCopy;

				//���W�ԍ��̓���ւ�
				int nCopy = pnFarNumTerritory[nCnt];
				pnFarNumTerritory[nCnt] = NowTerritory;
				NowTerritory = nCopy;
			}
		}
	}

	//�ł��������_�̗��ׂ̒��_����Ȃ�O�p�`�����
	int nCountTraiangle = 0;
	while (nCountTraiangle != m_nCountTerritory - 2)
	{//�O�p�`�����
		MakeTraiangle(0, &nCountTraiangle, pnFarNumTerritory, pListPos, pTraiangle);
	}

	//----------------------------------------------
	// ���������O�p�`���Ƀe���g���[�����邩�ǂ���
	//----------------------------------------------
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

			if (pTerritory->GetPlayerNumber() != m_nNumPlayerNo)
			{
				D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();

				//�����v���̔���
				for (int nCntTraiangle = 0; nCntTraiangle < m_nCountTerritory - 2; nCntTraiangle++)
				{
					//�}�`���Ƀe���g���[�����邩�ǂ���
					int nCountEnter = 0;
					for (int nCntVertex = 0; nCntVertex < TRIANGLE_VERTEX; nCntVertex++)
					{
						D3DXVECTOR3 VecB = TerritoryPos - pTraiangle[nCntTraiangle].pos[nCntVertex];

						//�O�ς����߂�
						D3DXVECTOR3 VecA = pTraiangle[nCntTraiangle].VecA[nCntVertex];
						float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(VecA.x, VecA.z), &D3DXVECTOR2(VecB.x, VecB.z));

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

					//�͈͓��Ȃ�e���g���[�̎擾
					if (nCountEnter == TRIANGLE_VERTEX)
					{
						//�����|�C���g�ɉ��Z�@��
						if (pTerritory->GetPlayerNumber() != m_nNumPlayerNo)
						{
							*nGetPoint += pTerritory->GetPoint(); //��

							// �Q�[���N���X���擾
							CGame *pGame = CManager::GetGame();
							if (pGame != NULL)
							{
								if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((pTerritory->GetErea() + 2) % 4))
								{
									*nGetPoint += 1;
								}
							}
						}

						m_nPointCounter++;
						GetTerritory(pTerritory);
						bEnter = true;
						break;
					}
				}
			}
		}

		pScene = pSceneNext;	//�|�C���^��i�߂�
	}

	//--------------------------
	// ���v���̔���
	//--------------------------
	if (!bEnter)
	{
		//���v���p��VecA�����߂�
		for (int nCntTraiangle = 0; nCntTraiangle < m_nCountTerritory - 2; nCntTraiangle++)
		{
			for (int nCntVec = TRIANGLE_VERTEX - 1; nCntVec >= 0; nCntVec--)
			{
				int nCnt = nCntVec - 1;
				if (nCntVec == 0) { nCnt = TRIANGLE_VERTEX - 1; }
				pTraiangle[nCntTraiangle].VecA[nCntVec] = pTraiangle[nCntTraiangle].pos[nCnt] - pTraiangle[nCntTraiangle].pos[nCntVec];
			}
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

				if (pTerritory->GetPlayerNumber() != m_nNumPlayerNo)
				{
					D3DXVECTOR3 TerritoryPos = pTerritory->GetPos();

					for (int nCntTraiangle = 0; nCntTraiangle < m_nCountTerritory - 2; nCntTraiangle++)
					{
						//�}�`���Ƀe���g���[�����邩�ǂ���
						int nCountEnter = 0;
						for (int nCntVertex = TRIANGLE_VERTEX - 1; nCntVertex >= 0; nCntVertex--)
						{
							D3DXVECTOR3 VecB = TerritoryPos - pTraiangle[nCntTraiangle].pos[nCntVertex];

							//�O�ς����߂�
							D3DXVECTOR3 VecA = pTraiangle[nCntTraiangle].VecA[nCntVertex];
							float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(VecA.x, VecA.z), &D3DXVECTOR2(VecB.x, VecB.z));

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

						//�͈͓��Ȃ�e���g���[�̎擾
						if (nCountEnter == TRIANGLE_VERTEX)
						{
							//�����|�C���g�ɉ��Z�@��
							if (pTerritory->GetPlayerNumber() != m_nNumPlayerNo)
							{
								*nGetPoint += pTerritory->GetPoint(); //��

								// �Q�[���N���X���擾
								CGame *pGame = CManager::GetGame();
								if (pGame != NULL)
								{
									if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((pTerritory->GetErea() + 2) % 4))
									{
										*nGetPoint += 1;
									}
								}
							}

							m_nPointCounter++;
							GetTerritory(pTerritory); //��
							break;
						}
					}
				}
			}

			pScene = pSceneNext;	//�|�C���^��i�߂�
		}
	}

	//----------------------------------------------
	// �������̔j��
	//----------------------------------------------
	if (pListPos != NULL)
	{//�ʒu�̃��X�g
		delete[] pListPos;
		pListPos = NULL;
	}

	if (pTraiangle != NULL)
	{//�O�p�`�̏��
		delete[] pTraiangle;
		pTraiangle = NULL;
	}

	if (pnFarNumTerritory != NULL)
	{//�n�_���牓�����W�����X�g
		delete[] pnFarNumTerritory;
		pnFarNumTerritory = NULL;
	}

	if (pfLength != NULL)
	{//�n�_�Ƃ̋���
		delete[] pfLength;
		pfLength = NULL;
	}

}

//=============================================================================
// �O�p�`�̍\���̂̏�����
//=============================================================================
void CCharacter::InitTraiangle(TRAIANGLE * pTraiangle, int nMaxTraiangle)
{
	for (int nCnt = 0; nCnt < nMaxTraiangle; nCnt++)
	{
		for (int nCntVertex = 0; nCntVertex < TRIANGLE_VERTEX; nCntVertex++)
		{
			pTraiangle[nCnt].pos[nCntVertex] = INITIALIZE_VECTOR3;
			pTraiangle[nCnt].VecA[nCntVertex] = INITIALIZE_VECTOR3;
		}
	}
}

//=============================================================================
// �e���g���[�̎擾����
//=============================================================================
void CCharacter::GetTerritory(CTerritory * pTerritory)
{
	CSound *pSound = CManager::GetSound();

	int nOldNumPlayer = pTerritory->GetPlayerNumber();
	pTerritory->SetColor(m_CountryColor[m_nNumPlayerNo]);	//�F�̕ύX
	pTerritory->SetPlayerNumber(m_nNumPlayerNo);			//�v���C���[�ԍ��̕ۑ�
	pTerritory->SetState(CTerritory::STATE_GET);			//�擾��Ԃɂ���

	//�O��̃v���C���[�̌��_����
	if (nOldNumPlayer != -1)
	{
		m_nCountRobbtedTerritory++;	//�e���g���[�̒D��ꂽ�������Z
		CutTerritoryPoint(pTerritory, nOldNumPlayer);
	}
	//�̒n�l����
	pSound->PlaySound(CSound::SOUND_LABEL_SE017);
	//�_���̒ǉ�����
	AddTerritoryPoint(pTerritory, nOldNumPlayer);
	//�G�t�F�N�g�̐���
	pTerritory->CreateGetEffect();
	//�e���g���[�̎擾���̉��Z
	m_nCountGetTerritry++;
}

//=============================================================================
// �e���g���[�̃��X�g��������
//=============================================================================
void CCharacter::ResetList(void)
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
// ���C���̏�����
//=============================================================================
void CCharacter::ResetLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		m_apLine[nCnt] = NULL;
	}
}

//=============================================================================
// ���C���̔j��
//=============================================================================
void CCharacter::UninitLine(void)
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
void CCharacter::CreateEndLine(void)
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
				m_apLine[nCnt] = CLine::Create(m_apTerritory[nCountTerritory]->GetPos(), m_apTerritory[TERRITORY_TOP]->GetPos(), m_nNumPlayerNo, nCnt);
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
void CCharacter::UninitCopyLine(void)
{
	for (int nCnt = 0; nCnt < MAX_TERRITORY; nCnt++)
	{
		if (m_apCopyLine[nCnt] != NULL)
		{
			bool bDeth = m_apCopyLine[nCnt]->ColChange();

			if (bDeth)
			{//���S�t���O���L����������
				m_apCopyLine[nCnt]->Uninit();
				m_apCopyLine[nCnt] = NULL;
				m_nCntTimeCopyLine = 0;
				m_bMakeShape = false;
			}
		}
	}
}

//=============================================================================
// ���C���̐ؒf����
//=============================================================================
void CCharacter::CutLine(int nID)
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
	//���C���ؒf��
	CSound *pSound = CManager::GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_SE029);
	pSound->SetVolume(CSound::SOUND_LABEL_SE029, 30.0f);
}

//=============================================================================
//! �O�p�`����鏈��
//=============================================================================
void CCharacter::MakeTraiangle(int nCnt, int * nCountTraiangle, int * pnFarNumTerritory, D3DXVECTOR3 * pListPos, TRAIANGLE * pTraiangle)
{
	//�n�_���牓�����_�Ƃ��̗��ׂ̍��W�ԍ���ۑ�
	int nVertex = pnFarNumTerritory[nCnt];
	int nVertex2 = pnFarNumTerritory[nCnt] - 1;
	int nVertex3 = pnFarNumTerritory[nCnt] + 1;

	//�l�̏C��
	if (nVertex == 0) { nVertex2 = m_nCountTerritory - 1; }
	if (nVertex == m_nCountTerritory - (1 + *nCountTraiangle)) { nVertex3 = 0; }

	//���W�̕ۑ�
	pTraiangle[*nCountTraiangle].pos[0] = pListPos[nVertex2];
	pTraiangle[*nCountTraiangle].pos[1] = pListPos[nVertex];
	pTraiangle[*nCountTraiangle].pos[2] = pListPos[nVertex3];

	//VecA�̍쐬
	for (int nCntVec = 0; nCntVec < TRIANGLE_VERTEX; nCntVec++)
	{
		int nCntVex2 = nCntVec + 1;
		if ((nCntVex2 % TRIANGLE_VERTEX) == 0) { nCntVex2 = 0; }

		pTraiangle[*nCountTraiangle].VecA[nCntVec] = pTraiangle[*nCountTraiangle].pos[nCntVex2] - pTraiangle[*nCountTraiangle].pos[nCntVec];
	}

	//�O�p�`�̒��Ƀ��X�g�̍��W�����邩�ǂ������肷��
	bool bNoEnter = false;
	for (int nCntList = 0; nCntList < m_nCountTerritory - *nCountTraiangle; nCntList++)
	{
		if (pListPos[nCntList] == FAR_POS) { break; }
		int nCountEnter = 0;
		for (int nCntVec = 0; nCntVec < TRIANGLE_VERTEX; nCntVec++)
		{
			D3DXVECTOR3 VecB = pListPos[nCntList] - pTraiangle[*nCountTraiangle].VecA[nCntVec];

			//�O�ς����߂�
			float fCross = CFunctionLib::Vec2Cross(&D3DXVECTOR2(pTraiangle[*nCountTraiangle].VecA[nCntVec].x, pTraiangle[*nCountTraiangle].VecA[nCntVec].z), &D3DXVECTOR2(VecB.x, VecB.z));

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

		if (nCountEnter == TRIANGLE_VERTEX)
		{//�O�p�`���ɒ��_������Ƃ��ׂ͗ɒ��_�����炵������x���������
			MakeTraiangle(nVertex3, nCountTraiangle, pnFarNumTerritory, pListPos, pTraiangle);	//�O�p�`�����
			bNoEnter = false;
		}
		else
		{
			bNoEnter = true;
		}
	}

	//�g�p�������W�����X�g���珜�O����
	if (bNoEnter)
	{
		//�ʒu���̃��X�g���l�߂�
		for (int nCntPos = nVertex; nCntPos < m_nCountTerritory - 1; nCntPos++)
		{
			D3DXVECTOR3 Pos;
			int nCntSide = nCntPos + 1;

			//���X�g�̓���ւ�
			if (nCntSide != m_nCountTerritory)
			{
				Pos = pListPos[nCntSide];
				pListPos[nCntSide] = FAR_POS;
			}

			pListPos[nCntPos] = Pos;
		}
		if (nVertex == m_nCountTerritory - 1) { pListPos[nVertex] = FAR_POS; }

		//�ł��������_���W���X�g���l�߂�
		for (int nCntPos = nCnt; nCntPos < m_nCountTerritory - (2 + *nCountTraiangle); nCntPos++)
		{
			int nCntSide = nCntPos + 1;
			int nSub;
			if (nCntSide != m_nCountTerritory - 1)
			{
				nSub = pnFarNumTerritory[nCntSide];
				if (nSub > nVertex) { nSub -= 1; }		//�l�̏C��

				pnFarNumTerritory[nCntSide] = pnFarNumTerritory[0];
			}

			pnFarNumTerritory[nCntPos] = nSub;
		}

		//�g�p���Ȃ����X�g��0�Ԗڂ̔ԍ�������
		int FirstVale = m_nCountTerritory - (2 + *nCountTraiangle);
		for (int nCntPos = FirstVale; nCntPos <= m_nCountTerritory - 2; nCntPos++)
		{
			pnFarNumTerritory[nCntPos] = pnFarNumTerritory[0];
		}

		//�O�p�`�|���S���𐶐�����
		MakePolygon(pTraiangle[*nCountTraiangle]);

		//�|�C���^���̐��l�����Z
		*nCountTraiangle += 1;
	}
}

//=============================================================================
// �O�p�`�̃|���S������
//=============================================================================
void CCharacter::MakePolygon(TRAIANGLE pTraiangle)
{
	CSceneTriangle3D * pSceneTriangle = CSceneTriangle3D::Create(pTraiangle.pos[0], pTraiangle.pos[1], pTraiangle.pos[2]);

	CGame * pGame = CManager::GetGame();

	if (pGame == NULL) { return; }
	int nNumPlayer = m_nNumPlayerNo;
	pSceneTriangle->SetColor(m_CountryColor[nNumPlayer]);
}

//=============================================================================
// �e���g���[�̓��_���폜���鏈��
//=============================================================================
void CCharacter::CutTerritoryPoint(CTerritory * pTerritory, int nOldPlayer)
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

	// �Q�[�W��ϓ�������
	CScoreGauge *pScoreGauge = pUI->GetScoreGauge();
	if (pScoreGauge == NULL) { return; }
	pScoreGauge->CutGauge(nOldPlayer, nPoint);
	if (m_nNumPlayerNo == nOldPlayer) { return; }

	// �X�R�A�̕ϓ���ݒ�
	CCharacter *pChara = pGame->GetChara(nOldPlayer);
	if (pChara == NULL) { return; }
	pChara->SetGetScorePoint(pChara->GetGetScorePoint() + nPoint);
	pChara->SetDestPlayer(m_nNumPlayerNo);
}

//=============================================================================
// ���̋O�Ղ���鏈��
//=============================================================================
void CCharacter::CreateLightOrbit(CGame *pGame)
{
	// �L�����N�^�[�N���X���擾
	CCharacter *pChara = pGame->GetChara(m_nDestPlayer);
	if (pChara == NULL) { return; }

	// ���̋O�Ղ𐶐�
	CLightOrbit::Create(m_Pos, pChara->GetPosPointer(), m_nDestPlayer);
}

//=============================================================================
// �e���g���[�̓��_��ǉ����鏈��
//=============================================================================
void CCharacter::AddTerritoryPoint(CTerritory * pTerritory,int nOldPlayer)
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
	CScoreGame *pScoreGame = pUI->GetScoreGame(m_nNumPlayerNo);
	if (pScoreGame == NULL) { return; }

	// �X�R�A�����Z
	pScoreGame->AddScore(nPoint);

	// �{�[�i�X�C�x���g���Ȃ炳��ɉ��Z
	if (pGame->GetAreaBonusEventFlag() == true && pGame->GetAreaBonusIdx() == ((pTerritory->GetErea() + 2) % 4)
		&& m_nNumPlayerNo != nOldPlayer)
	{// �C�x���g�� && �ԍ�����v���Ă��� && �������ۗL���Ă��Ȃ������e���g���[������
		pScoreGame->AddScore(1);
	}

	// ���ʂ̃\�[�g
	pUI->SortRankFromScore();

	// �Q�[�W��ϓ�������
	CScoreGauge *pScoreGauge = pUI->GetScoreGauge();
	if (pScoreGauge == NULL) { return; }
	pScoreGauge->AddGauge(m_nNumPlayerNo, nPoint);
}