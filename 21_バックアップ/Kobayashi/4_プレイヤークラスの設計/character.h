//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�L�����N�^�[�Ǘ� [character.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//	---<<#include>>---
#include "main.h"
#include "motion.h"

//	---<<�}�N��>>---

//	---<<�O���錾>>---
class CModel;
class CMotion;

//==============================================
//				�L�����N�^�[�Ǌ��N���X
//==============================================
class CCharacter
{
public:

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	}MODEL_OFFSET;


	//	---<<�R���X�g���N�^�E�f�X�g���N�^>>---
	CCharacter();
	~CCharacter();

	static CCharacter* Create();

	//	---<<��Պ֐�>>---
	HRESULT Init(char FileName[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//	---<<���f�����̓ǂݍ���>>---
	void ModelSetLode(void);	
	//	---<<���[�V�������̓ǂݍ���>>---
	void MotionSetLode(void);

	//	---<<�O���f�[�^�̓ǂݍ��݂ɕK�v�ȏ���>>---
	static char *ReadLine(FILE *pFile, char *pDst);
	static char *GetLIneTop(char *pSrc);
	static int PopString(char *pSrc, char *pDst);

private:
	//	---<<���f���֘A�ϐ�>>---
	CModel **m_ppModel;
	char* m_TextName[40];						//	�ǂݍ��ރe�L�X�g�t�@�C����
	static LPDIRECT3DTEXTURE9 m_pTexture;		
	static char *m_pUVTexName;					//	UV�e�N�X�`����
	static int m_nPartsNum;						//	�p�[�c��
	static char **m_aPartsName;					//	�e�p�[�c��
	static int *m_aIndex;						//	�C���f�b�N�X
	static int *m_nParent;						//	�e�ԍ�
	static MODEL_OFFSET *m_aOffset;				//	�e�p�[�c�̈ʒu�����

	//	---<<���[�V�����֘A�ϐ�>>---
	CMotion *m_pMotion;
	static CMotion::MOTION_INFO *m_pMotionInfo;
	int m_nNumMotionMAX;
	int m_nNumKey;

	D3DXVECTOR3					m_Pos;							//�ʒu

};

#endif