//*****************************************************************************
//
//     ���ʃ��C�u�����̏���[library.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// ���ʂŎg�������̒�`
#define STRING_TAB       "\t"                     // Tab�̕�����
#define STRING_ENTER     "\n"                     // ���s�R�[�h
#define STRING_SPACE     " "                      // �󔒂̕�����
#define STRING_COMMENT   "#"                      // �R�����g���J�n����镶��
#define SCRIPT           "SCRIPT"                 // �X�N���v�g�ǂݍ��݊J�n�̍��}
#define END_SCRIPT       "END_SCRIPT"             // �X�N���v�g�ǂݍ��ݏI���̍��}

//*****************************************************************************
//     ���ʊ֐��N���X�̒�`
//*****************************************************************************
class CFunctionLib
{
public:    // �N�ł��A�N�Z�X�\
	CFunctionLib();
	~CFunctionLib();

	static char *ReadLine(FILE *pFile, char *pDest);
	static int Memcmp(char *pSrc, char *pComp);
	static char *GetLineTop(char *pSrc);
	static int PopString(char *pSrc, char *pDest);
	static char *HeadPutout(char *pDest, char *pHead);
	static int ReadInt(char *pSrc, char *pHead);
	static float ReadFloat(char *pSrc, char *pHead);
	static char *ReadString(char *pSrc, char *pDest, char *pHead);
	static bool ReadBool(char *pSrc, char *pHead);
	static D3DXVECTOR3 ReadVector3(char *pSrc, char *pHead);
	static D3DXCOLOR ReadVector4(char *pSrc, char *pHead);
	static char *WriteInt(int nData, char *pDest);
	static char *WriteFloat(float fData, char *pDest);
	static char *WriteFloat3(float *pData, char *pDest);
	static float RotationVectorX(float fRot, float X, float Y);
	static float RotationVectorY(float fRot, float X, float Y);
	static float Vec2Cross(D3DXVECTOR2* Vec1, D3DXVECTOR2* Vec2);
	static float Vec2Length(float fValueX1, float fValueY1, float fValueX2, float fValueY2);
	static float Vec3Length(D3DXVECTOR3* Vec1, D3DXVECTOR3* Vec2);
	static D3DXVECTOR3* Vec3Direction(D3DXVECTOR3* Dest, D3DXVECTOR3* Start, D3DXVECTOR3* End);
	static float CalcAngleToDest(float fStartX, float fStartY, float fDestX, float fDestY);
	static D3DXVECTOR3 *CalcReflectVector3(D3DXVECTOR3 *Out, D3DXVECTOR3 *AdvanceVec, D3DXVECTOR3 *Normal);
	static D3DXVECTOR3* CalcRadianToDegree(D3DXVECTOR3* Dest);
	static D3DXVECTOR3* CalcDegreeToRadian(D3DXVECTOR3* Dest);
	static D3DXVECTOR3 *CheckRotationPi(D3DXVECTOR3 *Dest);
	static float CheckRotationOverPiX(float X);
	static float CheckRotationOverPiY(float Y);
	static float CheckRotationOverPiZ(float Z);
	static float CheckRotationOverMinusPiX(float X);
	static float CheckRotationOverMinusPiY(float Y);
	static float CheckRotationOverMinusPiZ(float Z);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};


//*****************************************************************************
//    �t�@�C���ۑ��N���X�̒�`
//*****************************************************************************
class CFileSaver
{
public:    // �N�ł��A�N�Z�X�\
	CFileSaver(char *pSaveFileName);
	~CFileSaver();

	static CFileSaver *Create(char *pSaveFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Print(char *fmt, ...);

	void SetSavefileName(char *pFileName);
	char *GetSavefileName(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	FILE *m_pFile;               // �t�@�C���|�C���^
	char m_aSaveFileName[256];   // �ۑ�����t�@�C����
};


//*****************************************************************************
//    �t�@�C���ǂݍ��݃N���X�̒�`
//*****************************************************************************
class CFileLoader
{
public:    // �N�ł��A�N�Z�X�\
	CFileLoader(char *pLoadFileName);
	~CFileLoader();

	static CFileLoader *Create(char *pLoadFileName);

	HRESULT Init(void);
	void Uninit(void);
	char *GetString(char *pSrc);

	void SetLoadfileName(char *pFileName);
	char *GetLoadfileName(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	FILE *m_pFile;               // �t�@�C���|�C���^
	char m_aLoadFileName[256];   // �ǂݍ��ރt�@�C����
};

#endif