//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : shun yokomichi
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "sound.h"
#include "modebase.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_RANKING				(5)		// �����x�̐�
#define MAX_RANKING_NUMBER		(8)		// �����L���O�̐���
#define RANKING_BG_MAX			(9)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CCamera;
class CFieldManager;
class CLoadTextObject;
class CLoadTextTerritory;
class CNumber;

//================================
// �����L���O���S�N���X
//================================
class CRankingBG : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	CRankingBG(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CRankingBG();

	static CRankingBG *Create(D3DXVECTOR3 pos, D3DXVECTOR3 destPos, D3DXVECTOR3 Size, float fMove, float fRotSpeed, float fTexHeight, int nPriority = 6, CScene2D::TYPE type = CScene2D::TYPE_GEAR);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(const D3DXVECTOR3 fPos) { m_pos = fPos; }
	void SetDestPos(const D3DXVECTOR3 destPos) { m_DestPos = destPos; }
	void SetMove(const float fMove) { m_fMove = fMove; }
	void SetRot(const float fRot) { m_fRot = fRot; }
	void SetRotSpeed(const float fRotSpeed) { m_fRotSpeed = fRotSpeed; }
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }
	void SetLength(const float fLength) { m_fLength = fLength; }

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetDestPos(void) { return m_DestPos; }
	float GetMove(void) { return m_fMove; }
	float GetRot(void) { return m_fRot; }
	float GetRotSpeed(void) { return m_fRotSpeed; }
	float GetAngle(void) { return m_fAngle; }
	float GetLength(void) { return m_fLength; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CalcPolygonScale(void);
	void RotationPolygon(void);

	D3DXVECTOR3 m_pos;      // ���݂̏ꏊ
	D3DXVECTOR3 m_DestPos;  // �ړI�̏ꏊ
	float m_fMove;			// �ړ��X�s�[�h
	float m_fRot;			// ���݂̌���
	float m_fRotSpeed;		// ��]�X�s�[�h
	float m_fAngle;			// �|���S���̌���
	float m_fLength;		// �|���S���̒���
};


//=====================
// ��{�N���X
//=====================
class CRanking : public CModeBase
{
public:
	CRanking();			// �R���X�g���N�^
	~CRanking();		// �f�X�g���N�^
	void Init(void);	// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:				// ���̃N���X�������A�N�Z�X�\
	void LoadTexture(void);
	void UnloadTexture(void);
	void CreateRankingBG(const int nIdx);

	LPDIRECT3DTEXTURE9 m_apTextureRanking[9];				//�e�N�X�`��
	CRankingBG *m_apRankingBG[RANKING_BG_MAX];			//�����L���O�̔w�i

	CCamera *m_pCamera;										//�J����
	CFieldManager *m_pFieldManager;							//�t�B�[���h�}�l�[�W���[
	CLoadTextObject *m_pLoadTextObj;						//�I�u�W�F�N�g�ǂݍ���
	CLoadTextTerritory *m_pLoadTerritory;					// �e���g���[�̓ǂݍ���

	int m_nNumRanking;
	int m_nCounterRanking;
	
};


#endif