//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�L�����N�^�[�Ǘ� [character.cpp]
//			Author : Kobayashi_Sho-hei / ���� ����
//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��

//	---<<#include>>---
#include "character.h"
#include "model.h"
#include "motion.h"
#include "manager.h"
#include "renderer.h"

//	---<<�ÓI�����o�ϐ�>>---


//==============================================
//					�R���X�g���N�^
//==============================================
CCharacter::CCharacter()
{

}


//==============================================
//					�f�X�g���N�^
//==============================================
CCharacter::~CCharacter()
{

}

//=============================================================================
// ���f���̔j��
//=============================================================================
void CCharacter::Unload(void)
{

}


//==============================================
//					������
////==============================================
HRESULT CCharacter::Init(char ModelTxt[40], char MotionTxt[40])
{
	m_cModelTxt[0] = &ModelTxt[0];
	m_cMotionTxt[0] = &MotionTxt[0];
	ModelSetLode();

	m_ppModel = new CModel*[m_nPartsNum];		//	�p�[�c���m�ۂ���

	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		//	���f���p�[�c�̏����l�����߂�
		m_ppModel[nCnt] = CModel::Create(D3DXVECTOR3(m_aOffset[nCnt].pos.x, m_aOffset[nCnt].pos.y, m_aOffset[nCnt].pos.z), m_aPartsName[nCnt], D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		//	���f���p�[�c�����߂�
		m_ppModel[nCnt]->BindModel(m_ppModel[nCnt]->GetMesh(), m_ppModel[nCnt]->GetBuffMat(), m_ppModel[nCnt]->GetNumMat());
		//	���f��UV�e�N�X�`�������߂�
		m_ppModel[nCnt]->BindTexture(m_ppTexture[m_nTexIdx[nCnt]]);
		//	�e���f���p�[�c�̐e�����߂�
		if (nCnt == 0)
		{
			m_ppModel[nCnt]->SetParent(NULL);
		}
		else
		{
			m_ppModel[nCnt]->SetParent(m_ppModel[m_nParent[nCnt]]);
		}
	}

	MotionSetLode();
	m_pMotion = CMotion::Create();
	m_pMotion->SetMaxModel(m_nPartsNum);
	m_pMotion->SetModel(m_ppModel);
	m_pMotion->SetMotion(m_pMotionInfo);
	m_pMotion->SetMaxMotion(m_nNumMotionMAX);
	m_pMotion->Init();

	return S_OK;
}

//==============================================
//					�I��
//==============================================
void CCharacter::Uninit()
{
	//!+ ---<<���[�V�������̉��>>---
	//	�L�[�t���[���̈ʒu�E��]���̉��(���[�V�����̎�ޕ� / �L�[��)
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
	{	if (m_pMotionInfo[nCnt].m_pKeyInfo != NULL)
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
		for (int nCntTex = 0; nCntTex < m_nTexNum; nCntTex++)
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

	//�g��k���s��̍쐬
	//D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nPartsNum; nCnt++)
	{
		m_ppModel[nCnt]->Draw(1.0f);
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
						m_ppTexture = new LPDIRECT3DTEXTURE9[m_nTexNum];
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
							m_ppUVTexName[nCntLoadTex] = new char[256];	//	������̊m��
							strcpy(m_ppUVTexName[nCntLoadTex], pStrCur);
						}

						//�f�o�C�X���̎擾
						LPDIRECT3DDEVICE9 pDevice;
						pDevice = CManager::GetRenderer()->GetDevice();
						D3DXCreateTextureFromFile(pDevice, m_ppUVTexName[nCntLoadTex], &m_ppTexture[nCntLoadTex]);
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
						m_pMotionInfo[nMotionNum].m_pKeyInfo[nKeyNum].aKey = new CMotion::KEY[m_nPartsNum];	//	���f���̃p�[�c���m��
						ClearMotionInfo_KeyInfo_Key(nMotionNum, nKeyNum);

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

//==============================================
////					����
////==============================================
//CCharacter* CCharacter::Create()
//{
//	CCharacter * pCharacter = NULL;
//	if (pCharacter == NULL)
//	{
//		pCharacter = new CCharacter;
//		pCharacter->Init("data\\TEXT\\PLAYER_LODE.txt", "data\\TEXT\\PLAYER_MOTION.txt");
//	}
//
//	return pCharacter;
//}
////=============================================================================
////
////=============================================================================
//void  CCharacter::Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
//{
//
//}