//=============================================================================
//
// �e�L�X�g�̓ǂݍ��ݏ��� [LoadText.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "loadText.h"
#include "manager.h"
#include "Territory.h"
#include "objbase.h"
#include "sceneOrbit.h"
#include "particle.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//�[���x�N�g��
#define FILE_NAME_ORBIT ("data//TEXT//ORBIT//orbit.txt")


//=============================================================================
// ��������
//=============================================================================
CLoadText * CLoadText::Create(void)
{
	//�C���X�^���X�̐���
	CLoadText * pLoadText = NULL;

	pLoadText->Init();
	return pLoadText;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadText::CLoadText()
{
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadText::~CLoadText(){}

//=============================================================================
// ��̓ǂݍ���
//=============================================================================
char * CLoadText::ReadLine(FILE * pFile, char * pDest)
{
	//�ϐ��錾
	char * pStrCur = NULL;

	while (1)
	{//�s���΂�
		fgets(pDest, 256, pFile);					//��s�������o��
		pStrCur = pDest;							//�擪�̕������ۊǂ���

		if (false == ((*pStrCur != '\0') && (*pStrCur != '\n') && (*pStrCur != '#'))) { continue; }

		while (1)
		{//��ɗL���ȕ������Ȃ����`�F�b�N����
			if ((*pStrCur != '\n') && (*pStrCur != '\t') && (*pStrCur != ' ') && (*pStrCur != '#') && (*pStrCur != '\0'))
			{
				break;
			}

			pStrCur++;	//�ꕶ�����i�߂�
		}
		break;
	}

	return pStrCur;
}

//=============================================================================
// �擪�|�C���^�̕�����i�߂�
//=============================================================================
char * CLoadText::AddChar(char * pStrCur, char * aStrCur, int nAddChar)
{
	pStrCur += nAddChar;						//�擪�A�h���X�𕶎������i�߂�
	strcpy(aStrCur, pStrCur);					//�������ۊǂ���

	return pStrCur;
}

//=============================================================================
// ������𔲂��o��
//=============================================================================
int CLoadText::PopString(char * pStrCur, char * aStrCur)
{
	//�ϐ��錾
	int nCounterWord = 0;	//�i�񂾕������J�E���g����
	while (1)
	{
		aStrCur++;
		nCounterWord++;

		if ((*aStrCur == ' ') || (*aStrCur == '\t') || (*aStrCur == '#') || (*aStrCur == '\n'))
		{
			aStrCur[0] = '\0';
			nCounterWord++;
			break;
		}
	}

	return nCounterWord;
}

//=============================================================================
// �k�������̒ǉ�
//=============================================================================
void CLoadText::AddNull(char * pLine)
{
	//�ϐ��錾
	char * cTop = &pLine[0];
	int nCounter = 0;

	while (1)
	{
		cTop++;
		nCounter++;

		if ((*cTop == '\n') || (*cTop == '\t') || (*cTop == ' ') || (*cTop == '#'))
		{
			pLine[nCounter] = '\0';	//���������� \0( �k������ ) �ɒu��������
			break;
		}
	}
}

//*****************************************************************************
//
// �I�u�W�F�N�g���ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLoadTextObject * CLoadTextObject::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextObject * pLoadTextObject = new CLoadTextObject;
	pLoadTextObject->SetLoadTextFileName(pFileName);			//�ǂݍ��ރe�L�X�g�t�@�C�����̕ۊ�
	pLoadTextObject->Init();

	return pLoadTextObject;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextObject::CLoadTextObject() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextObject::~CLoadTextObject() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextObject::Init(void)
{
	//�ϐ��̏�����
	m_pStrCur = NULL;					//������̐擪�ւ̃|�C���^
	m_aLine[256] = {};						//������ǂݍ��ݗp�i��s���j
	m_aStrCur[256] = {};						//�����񔲂��o���p
	m_TexInfo.ppFileName = NULL;
	m_ObjInfo.nMaxModel = 0;
	int nTex = 0;
	int nModeltype = 0;
	int nType = 0;
	D3DXVECTOR3 Pos = VECTOR_ZERO;
	D3DXVECTOR3 Rot = VECTOR_ZERO;
	D3DXVECTOR3 Scale = VECTOR_ZERO;


	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//�e�L�X�g�̓ǂݍ���

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
			strcpy(m_aStrCur, m_pStrCur);

			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)		//�e�N�X�`���̍ő吔�̎擾
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//�e�N�X�`���t�@�C�����𓮓I�m�ۂ���
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				//���������������𓮓I�m�ۂ���
				if (m_TexInfo.ppFileName != NULL)
				{
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
					{
						m_TexInfo.ppFileName[nCntTex] = new char[256];
					}
				}

				//�e�N�X�`���̃t�@�C�������擾
				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("�e�L�X�g�t�@�C���|�C���^��NULL�ł�"), TEXT("CLoadTextObject::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);				//�t�@�C�����̍Ō��NULL��ǉ�����
				}

				//�e�N�X�`���̓ǂݍ��ݏ���
				CObjBase::LoadTex(m_TexInfo);

			}
			else if (memcmp(m_aStrCur, "MAX_MODEL = ", 12) == 0)		//���f���t�@�C���̍ő吔
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
				m_ObjInfo.nMaxModel = atoi(m_pStrCur);

				//���f���t�@�C�������������𓮓I�m�ۂ���
				if (m_ObjInfo.ppModelFileName == NULL)
				{
					m_ObjInfo.ppModelFileName = new char *[m_ObjInfo.nMaxModel];
				}
				if (m_ObjInfo.ppModelFileName != NULL)
				{
					for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)
					{
						m_ObjInfo.ppModelFileName[nCntModel] = new char[256];
					}
				}

				//���f���̃t�@�C������ۑ�����
				for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)
				{
					if (m_ObjInfo.ppModelFileName[nCntModel] == NULL)
					{
						MessageBox(NULL, TEXT("���f���t�@�C���|�C���^��NULL�ł�"), TEXT("CLoadTextObject::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "MODELNAME = ", 12) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
					strcpy(m_ObjInfo.ppModelFileName[nCntModel], m_pStrCur);
					AddNull(m_ObjInfo.ppModelFileName[nCntModel]);			//�t�@�C�����̍Ō��NULL��ǉ�����
				}

				//���f���̓ǂݍ���
				CObjBase::LoadModel(m_ObjInfo);
			}
			else if (memcmp(m_aStrCur, "SET", 3) == 0)
			{
				while (memcmp(m_aStrCur, "END_SET", 7) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "MODELTYPE = ", 12) == 0)			//���f���^�C�v�̎擾
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
						nModeltype = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 6);
						nTex = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "POS = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "ROT = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "SCALE = ", 8) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "TYPE = ", 7) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 7);
						nType = atoi(m_pStrCur);
					}
				}

				//�I�u�W�F�N�g�𐶐�����
				CObjBase::Create(Pos, Rot, Scale, nTex, nModeltype, (CObjBase::TYPE)nType);
				CObjBase::SetAddMaxObj();
			}
		}
		fclose(pFile);	//�t�@�C���ǂݍ��݂��I���
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("CLoadTextObject::Init()"), MB_OK);
	}

	//����������
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextObject::Uninit(void)
{
	//�e�N�X�`�����̔j��
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] == NULL) { continue; }

		delete[] m_TexInfo.ppFileName[nCntTex];
		m_TexInfo.ppFileName[nCntTex] = NULL;
	}

	//�e�N�X�`���̃t�@�C���|�C���^�̔j��
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}

	//�I�u�W�F�N�g����j������
	for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)		//���f���t�@�C�����̔j��
	{
		if (m_ObjInfo.ppModelFileName[nCntModel] == NULL) { continue; }

		delete[] m_ObjInfo.ppModelFileName[nCntModel];
		m_ObjInfo.ppModelFileName[nCntModel] = NULL;
	}
	if (m_ObjInfo.ppModelFileName != NULL)
	{
		delete[] m_ObjInfo.ppModelFileName;
		m_ObjInfo.ppModelFileName = NULL;
	}

	//���f���E�e�N�X�`���̔j��
	CObjBase::UnloadModel();
	CObjBase::UnloadTex();
}

//*****************************************************************************
//
// �e���g���[���ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLoadTextTerritory * CLoadTextTerritory::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextTerritory * pLoadTextObject = new CLoadTextTerritory;
	pLoadTextObject->SetLoadTextFileName(pFileName);			//�ǂݍ��ރe�L�X�g�t�@�C�����̕ۊ�
	pLoadTextObject->Init();

	return pLoadTextObject;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextTerritory::CLoadTextTerritory() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextTerritory::~CLoadTextTerritory() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextTerritory::Init(void)
{
	//�ϐ��̏�����
	m_pStrCur = NULL;					//������̐擪�ւ̃|�C���^
	m_aLine[256] = {};					//������ǂݍ��ݗp�i��s���j
	m_aStrCur[256] = {};				//�����񔲂��o���p
	m_TexInfo.ppFileName = NULL;
	m_ObjInfo.nMaxModel = 0;
	int nTex = 0;
	int nModeltype = 0;
	int nType = 0;
	int nPoint = 0;
	D3DXVECTOR3 Pos = VECTOR_ZERO;
	D3DXVECTOR3 Rot = VECTOR_ZERO;
	D3DXVECTOR3 Scale = VECTOR_ZERO;


	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//�e�L�X�g�̓ǂݍ���

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
			strcpy(m_aStrCur, m_pStrCur);

			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)		//�e�N�X�`���̍ő吔�̎擾
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//�e�N�X�`���t�@�C�����𓮓I�m�ۂ���
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				//���������������𓮓I�m�ۂ���
				if (m_TexInfo.ppFileName != NULL)
				{
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
					{
						m_TexInfo.ppFileName[nCntTex] = new char[256];
					}
				}

				//�e�N�X�`���̃t�@�C�������擾
				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("�e�L�X�g�t�@�C���|�C���^��NULL�ł�"), TEXT("CLoadTextTerritory::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);				//�t�@�C�����̍Ō��NULL��ǉ�����
				}

				//�e�N�X�`���̓ǂݍ��ݏ���
				CTerritory::LoadTex(m_TexInfo);

			}
			else if (memcmp(m_aStrCur, "MAX_MODEL = ", 12) == 0)		//���f���t�@�C���̍ő吔
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
				m_ObjInfo.nMaxModel = atoi(m_pStrCur);

				//���f���t�@�C�������������𓮓I�m�ۂ���
				if (m_ObjInfo.ppModelFileName == NULL)
				{
					m_ObjInfo.ppModelFileName = new char *[m_ObjInfo.nMaxModel];
				}
				if (m_ObjInfo.ppModelFileName != NULL)
				{
					for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)
					{
						m_ObjInfo.ppModelFileName[nCntModel] = new char[256];
					}
				}

				//���f���̃t�@�C������ۑ�����
				for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)
				{
					if (m_ObjInfo.ppModelFileName[nCntModel] == NULL)
					{
						MessageBox(NULL, TEXT("���f���t�@�C���|�C���^��NULL�ł�"), TEXT("CLoadTextTerritory::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "MODELNAME = ", 12) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
					strcpy(m_ObjInfo.ppModelFileName[nCntModel], m_pStrCur);
					AddNull(m_ObjInfo.ppModelFileName[nCntModel]);			//�t�@�C�����̍Ō��NULL��ǉ�����
				}

				//���f���̓ǂݍ���
				CTerritory::LoadModel(m_ObjInfo);
			}
			else if (memcmp(m_aStrCur, "SET", 3) == 0)
			{
				while (memcmp(m_aStrCur, "END_SET", 7) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "MODELTYPE = ", 12) == 0)			//���f���^�C�v�̎擾
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 12);
						nModeltype = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 6);
						nTex = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "POS = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "ROT = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Rot.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "SCALE = ", 8) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						Scale.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "POINT = ", 8) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 8);
						nPoint = atoi(m_pStrCur);
					}
				}

				//�I�u�W�F�N�g�𐶐�����
				CTerritory::Create(Pos, Rot, Scale, nTex, nModeltype, nPoint);
				CTerritory::SetAddMaxObj();
			}
		}
		fclose(pFile);	//�t�@�C���ǂݍ��݂��I���
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("CLoadTextTerritory::Init()"), MB_OK);
	}

	//����������
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextTerritory::Uninit(void)
{
	//�e�N�X�`�����̔j��
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] == NULL) { continue; }

		delete[] m_TexInfo.ppFileName[nCntTex];
		m_TexInfo.ppFileName[nCntTex] = NULL;
	}

	//�e�N�X�`���̃t�@�C���|�C���^�̔j��
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}

	//�I�u�W�F�N�g����j������
	for (int nCntModel = 0; nCntModel < m_ObjInfo.nMaxModel; nCntModel++)		//���f���t�@�C�����̔j��
	{
		if (m_ObjInfo.ppModelFileName[nCntModel] == NULL) { continue; }

		delete[] m_ObjInfo.ppModelFileName[nCntModel];
		m_ObjInfo.ppModelFileName[nCntModel] = NULL;
	}
	if (m_ObjInfo.ppModelFileName != NULL)
	{
		delete[] m_ObjInfo.ppModelFileName;
		m_ObjInfo.ppModelFileName = NULL;
	}

	//���f���E�e�N�X�`���̔j��
	CTerritory::UnloadModel();
	CTerritory::UnloadTex();
}

//*****************************************************************************
//
// �e�N�X�`���̃t�@�C�����ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLoadTextTexture * CLoadTextTexture::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextTexture * pLoadTextObject = new CLoadTextTexture;

	pLoadTextObject->SetLoadTextFileName(pFileName);	//�ǂݍ��ރt�@�C������ۊǂ���
	pLoadTextObject->Init();

	return pLoadTextObject;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextTexture::CLoadTextTexture() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextTexture::~CLoadTextTexture() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextTexture::Init(void)
{
	//�ϐ��̏�����
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
	m_TexInfo.ppFileName = NULL;
	m_pPolygonInfo = NULL;
	m_nCountPoly = 0;

	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//�e�L�X�g�̓ǂݍ���

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
			strcpy(m_aStrCur, m_pStrCur);

			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)
			{
				//�e�N�X�`���̍ő吔�̎擾
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//�e�N�X�`���t�@�C�����𓮓I�m�ۂ���
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				if (m_TexInfo.ppFileName != NULL)
				{
					//���������������𓮓I�m�ۂ���
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++) { m_TexInfo.ppFileName[nCntTex] = new char[256]; }
				}

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("�e�L�X�g�t�@�C���|�C���^��NULL�ł�"), TEXT("CLoadTextTexture::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }		//�e�N�X�`���̃t�@�C�������擾

					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);			//�t�@�C�����̍Ō��NULL��ǉ�����
				}
			}
			else if (memcmp(m_aStrCur, "MAX_UI = ", 9) == 0)
			{
				//�e�N�X�`���̍ő吔�̎擾
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 9);
				m_nMaxPolygon = atoi(m_pStrCur);

				//�������𓮓I�m�ۂ���
				if (m_pPolygonInfo == NULL)
				{
					m_pPolygonInfo = new POLYGON_INFO[m_nMaxPolygon];
				}
			}
			else if (memcmp(m_aStrCur, "SET_UI", 6) == 0)
			{
				while (memcmp(m_aStrCur, "END_UI", 6) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);
					
					if (memcmp(m_aStrCur, "POS = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						m_pPolygonInfo[m_nCountPoly].pos.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						m_pPolygonInfo[m_nCountPoly].pos.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						m_pPolygonInfo[m_nCountPoly].pos.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "SIZE = ", 7) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
						int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						m_pPolygonInfo[m_nCountPoly].size.x = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						nWord = PopString(m_pStrCur, &m_aStrCur[0]);
						m_pPolygonInfo[m_nCountPoly].size.y = (float)atof(m_aStrCur);

						m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
						m_pPolygonInfo[m_nCountPoly].size.z = (float)atof(m_aStrCur);
					}
					else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 6);
						m_pPolygonInfo[m_nCountPoly].nTex = atoi(m_pStrCur);
					}
					else if (memcmp(m_aStrCur, "TYPE = ", 7) == 0)
					{
						m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 7);
						m_pPolygonInfo[m_nCountPoly].type = (CScene2D::TYPE)atoi(m_pStrCur);
						m_nCountPoly++;
					}
				}
				
			}
		}
		fclose(pFile);	//�t�@�C���ǂݍ��݂��I���
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("CLoadTextTexture::Init()"), MB_OK);
	}

	//����������
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextTexture::Uninit(void)
{
	//�e�N�X�`�����̔j��
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] == NULL) { continue; }

		delete[] m_TexInfo.ppFileName[nCntTex];
		m_TexInfo.ppFileName[nCntTex] = NULL;
	}

	//�e�N�X�`���̃t�@�C���|�C���^�̔j��
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}

	//�|���S������j��
	if (m_pPolygonInfo != NULL)
	{
		delete[] m_pPolygonInfo;
		m_pPolygonInfo = NULL;
	}
}

//*****************************************************************************
//
// ��Ղ̓ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLoadTextOrbit * CLoadTextOrbit::Create(void)
{
	//�C���X�^���X�̐���
	CLoadTextOrbit * pLoadTextObject = new CLoadTextOrbit;
	pLoadTextObject->Init();

	return pLoadTextObject;
}


//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextOrbit::CLoadTextOrbit() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextOrbit::~CLoadTextOrbit() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextOrbit::Init(void)
{
	//�ϐ��錾
	int nMaxOrbit;
	FILE *pFile;

	//�ϐ��̏�����
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
	m_pOrbit = NULL;

	pFile = fopen(FILE_NAME_ORBIT, "r");	//�e�L�X�g�̓ǂݍ���

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
			strcpy(m_aStrCur, m_pStrCur);

			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)		//�e�N�X�`���̍ő吔
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//�t�@�C�����̃|�C���^�𓮓I�m�ۂ���
				m_TexInfo.ppFileName = new char*[m_TexInfo.nMaxTex];	//�s�����m�ۂ���

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					m_TexInfo.ppFileName[nCntTex] = new char[256];	//���������m�ۂ���
				}

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

					if (memcmp(m_aLine, "FILE_NAME = ", 12) != 0) { continue; }

					m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);
				}
			}
			else if (memcmp(m_aStrCur, "MAX_ORBIT = ", 12) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
				nMaxOrbit = atoi(m_pStrCur);

				//�O�Տ��𓮓I�m�ۂ���
				if (m_pOrbit == NULL)
				{
					m_pOrbit = new CSceneOrbit::ORBIT_INFO[nMaxOrbit];
				}
			}
			else if (memcmp(m_aStrCur, "ORBITSET", 8) == 0)
			{
				while (memcmp(m_aStrCur, "END_ORBITSET", 12) != 0)
				{
					int nIndex = 0;

					while (memcmp(m_aStrCur, "END_SET", 7) != 0)
					{
						m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
						strcpy(m_aStrCur, m_pStrCur);

						if (memcmp(m_aStrCur, "INDEX = ", 8) == 0)		//�C���f�b�N�X�̓ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							nIndex = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
							m_pOrbit[nIndex].nTex = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "POS[0] = ", 9) == 0)	//�I�t�Z�b�g[0]�ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 9);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[0].x = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[0].y = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[0].z = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "POS[1] = ", 9) == 0) //�I�t�Z�b�g[1]�ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 9);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[1].x = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[1].y = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].pos[1].z = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "PRIMITIV = ", 11) == 0) //���_����ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pOrbit[nIndex].nPrimitiv = atoi(m_pStrCur);

							//�|���S�����̐ݒ�
							m_pOrbit[nIndex].nPorigon = m_pOrbit[nIndex].nPrimitiv - 2;
						}
						else if (memcmp(m_aStrCur, "COLOR = ", 8) == 0)	//�F����ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].color.r = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].color.g = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].color.b = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							//nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pOrbit[nIndex].color.a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "UV = ", 5) == 0)		//UV�l�̓ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 5);
							m_pOrbit[nIndex].fUV = (float)atof(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "ANIM = ", 7) == 0)	//�A�j���[�V�����l�̓ǂݍ���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pOrbit[nIndex].fAnim = (float)atof(m_pStrCur);
						}
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);
				}
			}
		}
		fclose(pFile);
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("LoadTextOrbit::Init()"), MB_OK);
	}

	//����������
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextOrbit::Uninit(void)
{
	//�|�C���^�̔j��
	if (m_pOrbit != NULL)
	{
		delete[] m_pOrbit;
		m_pOrbit = NULL;
	}

	//�e�N�X�`�����̔j��
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] == NULL) { continue; }

		delete[] m_TexInfo.ppFileName[nCntTex];
		m_TexInfo.ppFileName[nCntTex] = NULL;
	}

	//�e�N�X�`���̃t�@�C���|�C���^�̔j��
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}
}

//*****************************************************************************
//
// �G�t�F�N�g�ǂݍ��݃N���X
//
//*****************************************************************************

//=============================================================================
// ��������
//=============================================================================
CLoadTextEffect * CLoadTextEffect::Create(char * pFileName)
{
	//�C���X�^���X�̐���
	CLoadTextEffect * pLoadTextEffect = new CLoadTextEffect;

	pLoadTextEffect->SetLoadTextFileName(pFileName);	//�ǂݍ��ރt�@�C������ۊǂ���
	pLoadTextEffect->Init();

	return pLoadTextEffect;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLoadTextEffect::CLoadTextEffect() {}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLoadTextEffect::~CLoadTextEffect() {}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLoadTextEffect::Init(void)
{
	//�ϐ��錾
	int	 nCountParticle = 0;				//�J�E���^�[
	int	 nCountEffect = 0;					//�G�t�F�N�g�J�E���^�[

											//�ϐ��̏�����
	m_pStrCur = NULL;
	m_aLine[256] = {};
	m_aStrCur[256] = {};
	m_TexInfo.ppFileName = NULL;
	m_nMaxEffect = 0;
	m_pEffect = NULL;

	//�t�@�C���錾
	FILE *pFile;

	pFile = fopen(m_pLoadTextFileName, "r");	//�e�L�X�g�̓ǂݍ���

	if (pFile != NULL)
	{
		m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�

		while (memcmp(m_aLine, "END_SCRIPT", 10) != 0)
		{
			m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
			strcpy(m_aStrCur, m_pStrCur);

			//----------------------------------
			//�e�N�X�`���̍ő吔�E���̓ǂݍ���
			//----------------------------------
			if (memcmp(m_aStrCur, "MAX_TEX = ", 10) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
				m_TexInfo.nMaxTex = atoi(m_pStrCur);

				//�e�N�X�`���t�@�C�����𓮓I�m�ۂ���
				if (m_TexInfo.ppFileName == NULL) { m_TexInfo.ppFileName = new char *[m_TexInfo.nMaxTex]; }

				if (m_TexInfo.ppFileName != NULL)
				{
					for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
					{//���������������𓮓I�m�ۂ���
						m_TexInfo.ppFileName[nCntTex] = new char[256];
					}
				}

				for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
				{
					if (m_TexInfo.ppFileName[nCntTex] == NULL)
					{
						MessageBox(NULL, TEXT("�e�N�X�`���t�@�C���|�C���^��NULL�ł�"), TEXT("CLoadTextEffect::Init()"), MB_OK);
						break;
					}

					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "TEXTURE = ", 10) != 0) { continue; }

					//�e�N�X�`���̃t�@�C�������擾
					m_pStrCur = AddChar(m_pStrCur, m_aStrCur, 10);
					strcpy(m_TexInfo.ppFileName[nCntTex], m_pStrCur);
					AddNull(m_TexInfo.ppFileName[nCntTex]);			//�t�@�C�����̍Ō��NULL��ǉ�����
				}

				m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
				strcpy(m_aStrCur, m_pStrCur);
			}

			//----------------------------------
			//�G�t�F�N�g�̍ő吔�E���̏�����
			//----------------------------------
			if (memcmp(m_aStrCur, "MAX_EFFECT = ", 13) == 0)
			{
				m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
				m_nMaxEffect = atoi(m_pStrCur);

				//�������̓��I�m��
				if (m_pEffect == NULL) { m_pEffect = new CEffectTool::Effect[m_nMaxEffect]; }	//�G�t�F�N�g�̌������������m�ۂ���

																								//�ϐ��̏�����
				for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
				{
					if (m_pEffect != NULL)
					{
						m_pEffect[nCntEffect].EffectInfo = new CEffectTool::Effect_Info[MAX_EFFECT];
					}

					if (m_pEffect[nCntEffect].EffectInfo != NULL)
					{
						for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++)
						{
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bCreateEffect = false;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bLoop = false;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bUseGravity = true;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].bZbuffur = true;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].DrawType = CParticle::DRAWTYPE_ADD;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fChangeCol_a = 0.15f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fGravity = 0.01f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].fSpeed = 2.0f;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nInterbalTime = 10;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nCountTime = 0;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nLife = 50;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMaxHight = 3;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nMinHight = 2;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nNumCreate = 1;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].nTexNum = 0;
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].size = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
							m_pEffect[nCntEffect].EffectInfo[nCntParticle].Type = CParticle::TYPE_SPEW;
						}
					}
				}
			}

			//-------------------------------------------
			//�G�t�F�N�g�̐ݒ�
			//-------------------------------------------
			if (memcmp(m_aStrCur, "SETEFFECT", 9) == 0)
			{
				while (memcmp(m_aLine, "END_EFFECT", 10) != 0)
				{
					m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
					strcpy(m_aStrCur, m_pStrCur);

					if (memcmp(m_aStrCur, "SETPARTICLE", 11) != 0) { continue; }
					while (memcmp(m_aStrCur, "END_PARTICLE", 12) != 0)
					{
						if (memcmp(m_aStrCur, "LOOP = ", 7) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bLoop = (atoi(m_pStrCur) != 0);	//���[�v��Ԃ̐ݒ�

							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bCreateEffect = true;			//������Ԃ̐ݒ�
						}
						else if (memcmp(m_aStrCur, "SIZE = ", 7) == 0)						//�T�C�Y�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.x = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.y = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].size.z = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "TEX = ", 6) == 0)						//�e�N�X�`���ԍ��̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 6);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nTexNum = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "DRAWTYPE = ", 11) == 0)					//�`��^�C�v�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].DrawType = (CParticle::DRAWTYPE)atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "TYPE = ", 7) == 0)						//�^�C�v�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Type = (CParticle::TYPE)atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "COLOR = ", 8) == 0)						//�F�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							int nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.r = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.g = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							nWord = PopString(m_pStrCur, &m_aStrCur[0]);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.b = (float)atof(m_aStrCur);

							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], nWord);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].Color.a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "ZBUFFER = ", 10) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bZbuffur = (atoi(m_pStrCur) != 0);	//Z�o�b�t�@�̐ݒ�
						}
						else if (memcmp(m_aStrCur, "LIFE = ", 7) == 0)								//�\�����Ԃ̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nLife = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "CHANGECOLOR = ", 14) == 0)						//�����x�̕ω���
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 14);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fChangeCol_a = (float)atof(m_aStrCur);
						}
						else if (memcmp(m_aStrCur, "USEGRAVITY = ", 13) == 0)
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 13);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].bUseGravity = (atoi(m_pStrCur) != 0);	//�d�͂̏�Ԑݒ�
						}
						else if (memcmp(m_aStrCur, "TIME = ", 7) == 0)									//�������Ԃ̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 7);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nInterbalTime = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "NUMCREATE = ", 12) == 0)								//�����ʂ̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 12);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nNumCreate = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "SPEED = ", 8) == 0)									//�X�s�[�h�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 8);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fSpeed = (float)atof(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "MINHIGHT = ", 11) == 0)								//�����̍ŏ��l�̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nMinHight = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "MAXHIGHT = ", 11) == 0)								//�����̍ő�l�̐ݒu
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 11);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].nMaxHight = atoi(m_pStrCur);
						}
						else if (memcmp(m_aStrCur, "GRAVITY = ", 10) == 0)								//�d�͂̐ݒ�
						{
							m_pStrCur = AddChar(m_pStrCur, &m_aStrCur[0], 10);
							m_pEffect[nCountEffect].EffectInfo[nCountParticle].fGravity = (float)atof(m_pStrCur);
						}

						m_pStrCur = ReadLine(pFile, &m_aLine[0]);			//�����������΂�
						strcpy(m_aStrCur, m_pStrCur);
					}

					//�J�E���^�[��i�߂�
					nCountParticle++;
				}

				nCountEffect++;			//�G�t�F�N�g�J�E���^�[��i�߂�
				nCountParticle = 0;		//�p�[�e�B�N���J�E���^�[�̏�����
			}
		}
		fclose(pFile);	//�t�@�C���ǂݍ��݂��I���
	}
	else
	{
		MessageBox(NULL, TEXT("pFile��NULL�ł�"), TEXT("CLoadTextObject::Init()"), MB_OK);
	}

	//����������
	m_pStrCur = NULL;
	m_aLine[0] = '\0';
	m_aStrCur[0] = '\0';
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLoadTextEffect::Uninit(void)
{
	//�G�t�F�N�g�̏��j��
	for (int nCntEffect = 0; nCntEffect < m_nMaxEffect; nCntEffect++)
	{
		if (m_pEffect[nCntEffect].EffectInfo != NULL)
		{
			delete[] m_pEffect[nCntEffect].EffectInfo;
			m_pEffect[nCntEffect].EffectInfo = NULL;
		}
	}
	if (m_pEffect != NULL)
	{
		delete[] m_pEffect;
		m_pEffect = NULL;
	}

	//�e�N�X�`�����̔j��
	for (int nCntTex = 0; nCntTex < m_TexInfo.nMaxTex; nCntTex++)
	{
		if (m_TexInfo.ppFileName[nCntTex] != NULL)
		{
			delete[] m_TexInfo.ppFileName[nCntTex];
			m_TexInfo.ppFileName[nCntTex] = NULL;
		}
	}

	//�e�N�X�`���̃t�@�C���|�C���^�̔j��
	if (m_TexInfo.ppFileName != NULL)
	{
		delete[] m_TexInfo.ppFileName;
		m_TexInfo.ppFileName = NULL;
	}
}