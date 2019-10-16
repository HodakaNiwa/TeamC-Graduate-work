//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
//					�L�����N�^�[�Ǘ� [character.h]
//			Author : Kobayashi_Sho-hei / ���� ����
//	���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@���@��
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//	---<<#include>>---
#include "main.h"
#include "motion.h"
#include "scene.h"

//	---<<�}�N��>>---

//	---<<�O���錾>>---
class CModel;
class CMotion;
//==============================================
//				�L�����N�^�[�Ǌ��N���X
//==============================================
class CCharacter : public CScene
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
	HRESULT Init(char ModelTxt[40], char MotionTxt[40]);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Unload(void);
	

	//	---<<���f�����̓ǂݍ���>>---
	void ModelSetLode(void);	
	//	---<<���[�V�������̓ǂݍ���>>---
	void MotionSetLode(void);

	//	---<<�O���f�[�^�̓ǂݍ��݂ɕK�v�ȏ���>>---
	static char *ReadLine(FILE *pFile, char *pDst);
	static char *GetLIneTop(char *pSrc);
	static int PopString(char *pSrc, char *pDst);

	//	---<<Get�֐�>>---
	CModel** GetModel() { return m_ppModel; }
	//	---<<Set�֐�>>---

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetRot(void) { return m_Rot; }

private:
	D3DXVECTOR3					m_Pos;							//�ʒu
	D3DXVECTOR3					m_Scale;							//�ړ���
	D3DXVECTOR3					m_Rot;							//����
	D3DXMATRIX					m_mtxWorld;						//���[���h�}�g���b�N�X

	//	---<<���f���֘A�ϐ�>>---
	CModel **m_ppModel;
	char* m_cModelTxt[40];						//	�ǂݍ��ރe�L�X�g�t�@�C����
	static LPDIRECT3DTEXTURE9 m_pTexture;		
	 char *m_pUVTexName;					//	UV�e�N�X�`����
	 int m_nPartsNum;						//	�p�[�c��
	 char **m_aPartsName;					//	�e�p�[�c��
	 int *m_aIndex;						//	�C���f�b�N�X
	 int *m_nParent;						//	�e�ԍ�
	 MODEL_OFFSET *m_aOffset;				//	�e�p�[�c�̈ʒu�����

	//	---<<���[�V�����֘A�ϐ�>>---
	CMotion *m_pMotion;
	char* m_cMotionTxt[40];						//	�ǂݍ��ރe�L�X�g�t�@�C����
	CMotion::MOTION_INFO *m_pMotionInfo;
	int m_nNumMotionMAX;
	int m_nNumKey;
};

#endif