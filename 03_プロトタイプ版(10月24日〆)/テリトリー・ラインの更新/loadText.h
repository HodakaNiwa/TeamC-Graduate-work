//=============================================================================
//
// �e�L�X�g�̓ǂݍ��ݏ��� [LoadText.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _LOADTEXT_H_
#define _LOADTEXT_H_

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "scene2D.h"
#include "sceneOrbit.h"

//=============================================================================
// �e�L�X�g�ǂݍ��݃N���X
//=============================================================================
class CLoadText
{
public:
	//�e�N�X�`���̏��
	typedef struct
	{
		int		nMaxTex;			//�e�N�X�`���̍ő吔
		char ** ppFileName;			//�t�@�C�����i�[����
	}TEX_INFO;

	CLoadText();
	~CLoadText();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	static CLoadText * Create(void);
	TEX_INFO GetTexInfo(void) { return m_TexInfo; };									//�e�N�X�`�����
	void SetLoadTextFileName(char * pFileName) { m_pLoadTextFileName = pFileName; };	//�ǂݍ��ރe�L�X�g�t�@�C���ڂ̕ۊ�

protected:
	char * ReadLine(FILE * pFile, char * pDest);
	char * AddChar(char * pStrCur, char * aStrCur, int nAddChar);
	int PopString(char * pStrCur, char * aStrCur);
	void AddNull(char * pLine);
	char * m_pStrCur;					//������̐擪�ւ̃|�C���^
	char  m_aLine[256];				//������ǂݍ��ݗp�i��s���j
	char  m_aStrCur[256];				//�����񔲂��o���p
	TEX_INFO m_TexInfo;				//�e�N�X�`�����
	char * m_pLoadTextFileName;		//�e�L�X�g�t�@�C����

};

//=============================================================================
// �X�e�[�W�I�u�W�F�N�g���ǂݍ��݃N���X
//=============================================================================
class CLoadTextObject : public CLoadText
{
public:
	typedef struct
	{
		int nMaxModel;				//���f���̂̍ő吔
		char ** ppModelFileName;	//���f���̃t�@�C�����i�[
	}OBJ_INFO;

	CLoadTextObject();
	~CLoadTextObject();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextObject * Create(char * pFileName);
	OBJ_INFO GetObjInfo(void) { return m_ObjInfo; };

private:
	OBJ_INFO m_ObjInfo;
};

//=============================================================================
// �e���g���[���ǂݍ��݃N���X
//=============================================================================
class CLoadTextTerritory : public CLoadText
{
public:
	typedef struct
	{
		int nMaxModel;				//���f���̂̍ő吔
		char ** ppModelFileName;	//���f���̃t�@�C�����i�[
	}OBJ_INFO;

	CLoadTextTerritory();
	~CLoadTextTerritory();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextTerritory * Create(char * pFileName);
	OBJ_INFO GetObjInfo(void) { return m_ObjInfo; };

private:
	OBJ_INFO m_ObjInfo;
};

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ��
//=============================================================================
class CLoadTextTexture : public CLoadText
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 size;
		int nTex;
		CScene2D::TYPE type;
	}POLYGON_INFO;

	CLoadTextTexture();
	~CLoadTextTexture();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextTexture * Create(char * pFileName);
	POLYGON_INFO GetPolygonInfo(int nCntPolygon) { return m_pPolygonInfo[nCntPolygon]; }
	int GetMaxPolygon(void) { return m_nMaxPolygon; }

private:
	POLYGON_INFO * m_pPolygonInfo;
	int m_nMaxPolygon;
	int m_nCountPoly;
};

//=============================================================================
// �O�Ղ̓ǂݍ��݃N���X
//=============================================================================
class CLoadTextOrbit : public CLoadText
{
public:
	CLoadTextOrbit();
	~CLoadTextOrbit();
	HRESULT Init(void);
	void Uninit(void);
	CSceneOrbit::ORBIT_INFO * GetOrbit(void) { return m_pOrbit; };	//�O�Տ��̎擾
	static CLoadTextOrbit * Create(void);

private:
	CSceneOrbit::ORBIT_INFO * m_pOrbit;	//�O�Ճ|�C���^
};
#endif