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
#define MAX_TERRITORY	(10)
#define TRIANGLE_VERTEX	(3)
#define FIRST_LINETIME	(60 * 20)	//���C���̎��Ԃ̏�����
#define MIN_TERRITORY	(3)			//�e���g���[�̍Œ����
#define TERRITORY_TOP	(0)			//�e���g���[�̐擪���
#define NOT_GETTIME		(60)		//�e���g���[�̎擾�ł��Ȃ�����
#define LINE_TIME		(90)		//���C���������Ȃ�����

//	---<<�O���錾>>---
class CModel;
class CMotion;
class CTerritory;
class CLine;
class CSceneOrbit;

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

	typedef enum
	{//�v���C���[�̃^�C�v
		CHARCTERTYPE_SPEED = 0,
		CHARCTERTYPE_POWER,
		CHARCTERTYPE_TECHNIQUE,
		CHARCTERTYPE_MAX
	}CHARCTERTYPE;

	typedef enum
	{//���̃^�C�v
		TYPE_JAPAN = 0,		//���{
		TYPE_RUSSIA,		//���V�A
		TYPE_AMERICA,		//�A�����J
		TYPE_BRAZIL,		//�u���W��
		TYPE_ENGLAND,		//�C�M���X
		TYPE_ITALY,			//�C�^���A
		TYPE_NEWZEALAND,	//�j���[�W�[�����h
		TYPE_SOUTHAFRICA,	//��A�t���J
		TYPE_MAX,
	}TYPE;

	typedef struct
	{
		D3DXVECTOR3 pos[TRIANGLE_VERTEX];
		D3DXVECTOR3 VecA[TRIANGLE_VERTEX];
	}TRAIANGLE;


	//	---<<�R���X�g���N�^�E�f�X�g���N�^>>---
	CCharacter(int nPriority, OBJTYPE objType);
	~CCharacter();

	static CCharacter* Create();

	//	---<<��Պ֐�>>---
	HRESULT Init(int nCharaNum, char ModelTxt[40], char MotionTxt[40],int nType, int nCuntry);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Unload(void);


	//	---<<���f�����̓ǂݍ���>>---
	void ModelSetLode(void);
	//	---<<���[�V�������̓ǂݍ���>>---
	void MotionSetLode(void);
	//	---<<�e�N�X�`���̓ǂݍ���>>---
	void TextureSetLode(int nStartIdx, int nNumTex);

	//	---<<�O���f�[�^�̓ǂݍ��݂ɕK�v�ȏ���>>---
	static char *ReadLine(FILE *pFile, char *pDst);
	static char *GetLIneTop(char *pSrc);
	static int PopString(char *pSrc, char *pDst);
	static D3DXCOLOR m_CountryColor[CCharacter::TYPE_MAX];	//���̐F

															//	---<<Get�֐�>>---
	CModel** GetModel() { return m_ppModel; }
	CMotion *GetMotion() { return m_pMotion; }

	//	---<<Set�֐�>>---
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }
	D3DXVECTOR3 GetRot(void) { return m_Rot; }

	int GetAttack(void) { return m_nAttack; }					//�U���͂̎擾
	float GetAttackSpeed(void) { return m_fAttackSpeed; }		//�U�����x�̎擾
	float GetRange(void) { return m_fRange; }					//�˒��̎擾
	float GetSpeed(void) { return m_fSpeed; }					//�ړ��ʂ̎擾
	float GetInertia(void) { return m_fInertia; }				//�����̎擾
	float GetLineTime(void) { return m_fLineTime; }				//����`�����Ԃ̎擾

	void AddTerritoryList(CTerritory * pTerritory);
	void CutLine(int nID);

	//���̃^�C�v Geter & Set
	void SetType(int nType) { m_nType = nType; }
	int GetType(void) { return m_nType; }

	//�L�����^�C�v�̎擾
	CHARCTERTYPE GetCharcterType(void) { return m_CharcterType; }

	//���U���g�p�L�����^�C�v�ƍ��̎擾
	static CHARCTERTYPE GetCharcterTypeResult(int nType) { return m_CharcterTypeResult[nType]; }
	int GetCuntryResult(int nNumCuntry) { return m_nCuntry[nNumCuntry]; }

protected:
	void CountDownLineTime(void);										//�e���g���[�̎擾�X�V����
	void ResetList(void);												//���X�g�̏�����
	bool ChackSamePos(CTerritory * pTerritory, int nNumTerritory);		//�����ʒu���ǂ���
	void ShapeComplete(CTerritory * pTerritory);						//���������Ƃ��̏���
	void ChackInShape(void);											//�}�`�ɓ����Ă��邩�ǂ����̏���
	void GetTerritory(CTerritory * pTerritory);							//�e���g���[�̎擾
	void ResetLine(void);												//���C���̐�������
	void UninitLine(void);												//���C���̔j��
	void UninitCopyLine(void);											//�R�s�[���C���̔j��
	void CreateEndLine(void);											//�Ō�̐��̐���
	void UpdateShapeComplete(void);										//������̍X�V����
	void InitTraiangle(TRAIANGLE * pTraiangle, int nMaxTraiangle);		//�O�p�`�̍\���̂̏�����
	void MakeTraiangle(int nCnt, int * nCountTraiangle, int * pnFarNumTerritory, D3DXVECTOR3 * pListPos, TRAIANGLE * pTraiangle);				//�O�p�`����鏈��
	void MakePolygon(TRAIANGLE pTraiangle);								//�|���S���̐���
	void BindTexture(LPDIRECT3DTEXTURE9 *pTexture) { m_ppTexture = pTexture; }

	//�e���g���[
	CTerritory * m_apTerritory[MAX_TERRITORY];	//�e���g���[�̃��X�g
	CTerritory * m_pOldStartTerritory;			//�����ʒu�̃e���g���[
	int m_nCountTerritory;						//�J�E���^�[
	int m_nCountTime;							//���ԃJ�E���^�[
	bool m_bBlockStartTerritory;				//�����e���g���[���u���b�N
	bool m_bMakeShape;							//�}�`�������t���O

												//���C��
	CSceneOrbit * m_pOrbitLine;					//���C��
	CLine *		  m_apLine[MAX_TERRITORY];		//���C��
	CLine *		  m_apCopyLine[MAX_TERRITORY];	//�R�s�[���C��
	int			  m_nCntTimeCopyLine;			//���C���̃^�C�}�[
	int m_nLineTime;							//���C���̎���
	int m_nMaxLineTime;							//���C���̎��ԍő吔
	int m_nNumPlayerNo;							//�v���C���[�i���o�[(�G�l�~�[���܂�)

	//���̃^�C�v
	int m_nType;

	//���L���X�g�p�ϐ�
	int m_nButtonCounter;						//�{�^���J�E���^�[
	int m_nRecastCounter;						//���L���X�g�p�J�E���^�[
	int m_nRecastTimer;							//���L���X�g����
	bool m_bRecast;								//���L���X�g���Ă邩

	//�L�����N�^�[�p�ϐ�
	CHARCTERTYPE m_CharcterType;						//�Q�[���p�̃^�C�v
	static CHARCTERTYPE m_CharcterTypeResult[8];		//���U���g�p�̃L�����^�C�v
	static  int m_nCuntry[8];							//���U���g�p�̍��^�C�v
	bool m_bCharaMotionState;							//���[�V������Ԃ̊Ǘ�
private:
	D3DXVECTOR3					m_Pos;							//�ʒu
	D3DXVECTOR3					m_Scale;						//�ړ���
	D3DXVECTOR3					m_Rot;							//����
	D3DXMATRIX					m_mtxWorld;						//���[���h�}�g���b�N�X

	//	---<<���[�V�����֘A�֐�>>---
	void ClearMotionInfo(void);
	void ClearMotionInfo_KeyInfo(int nMotionIdx);
	void ClearMotionInfo_KeyInfo_Key(int nMotionIdx, int nKeyInfoIdx);

	//	---<<�O�Ղ̊֐�>>---
	virtual void CreateOrbitLine(void) = 0;								//��Ղ̐��̐���
	virtual void UninitOrtbitLine(void) = 0;							//���C���̔j��

	//	---<<�R���W�����֘A�֐�>>---
	void CreateCylinderCollider(void);

	//	---<<���f���֘A�ϐ�>>---
	CModel **m_ppModel;
	char* m_cModelTxt[40];					//	�ǂݍ��ރe�L�X�g�t�@�C����
	LPDIRECT3DTEXTURE9 *m_ppTexture;		//	�e�N�X�`���ւ̃|�C���^(�_�u���|�C���^)
	char **m_ppUVTexName;					//	UV�e�N�X�`����
	int m_nTexNum;							//	�g�p����e�N�X�`���̐�
	int m_nLoadTexNum;						//	�ǂݍ��񂾃e�N�X�`���̐�
	int m_nPartsNum;						//	�p�[�c��
	char **m_aPartsName;					//	�e�p�[�c��
	int *m_aIndex;							//	�C���f�b�N�X
	int *m_nParent;							//	�e�ԍ�
	int *m_nTexIdx;							//	�g�p����e�N�X�`���̔ԍ�
	MODEL_OFFSET *m_aOffset;				//	�e�p�[�c�̈ʒu�����

											//	---<<���[�V�����֘A�ϐ�>>---
	CMotion *m_pMotion;
	char* m_cMotionTxt[40];						//	�ǂݍ��ރe�L�X�g�t�@�C����
	CMotion::MOTION_INFO *m_pMotionInfo;
	int m_nNumMotionMAX;
	int m_nNumKey;

	//	---<<�L�����N�^�[�X�e�[�^�X�֘A�ϐ�>>---
	int m_nAttack;							//�U����
	float m_fAttackSpeed;					//�U�����x
	float m_fRange;							//�˒�
	float m_fSpeed;							//�ړ���
	float m_fInertia;						//����
	float m_fLineTime;						//����`���鎞��
	void CutTerritoryPoint(CTerritory * pTerritory, int nOldPlayer);	// �e���g���[�X�R�A�̍폜
	void AddTerritoryPoint(CTerritory * pTerritory);					// �e���g���[�X�R�A�̒ǉ�
};

#endif