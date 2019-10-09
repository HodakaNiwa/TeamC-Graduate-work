//=============================================================================
//
// �e�L�X�g�̓ǂݍ��ݏ��� [LoadText.cpp]
// Author : �������� Sato_Asumi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "loadText.h"
#include "manager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define VECTOR_ZERO (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//�[���x�N�g��

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