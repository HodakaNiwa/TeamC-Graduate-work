//=============================================================================
//
// �^�C�g������ [title.h]
// Author : ��������
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "sound.h"
#include "modebase.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLoadTextObject;
class CLoadTextTerritory;
class CRotationCamera;
class CFieldManager;

//=====================
// ��{�N���X
//=====================
class CTitle : public CModeBase
{
public:
	typedef enum
	{
		SOUNDSTATE_NONE = 0,
		SOUNDSTATE_01,
		SOUNDSTATE_02,
		SOUNDSTATE_MAX
	}SOUNDSTATE;
	CTitle();								// �R���X�g���N�^
	~CTitle();								// �f�X�g���N�^
	void	Init(void);						// ����������
	void	Uninit(void);					// �I������
	void	Update(void);					// �X�V����
	void	Draw(void);						// �`�揈��

private:
	//--------------------
	//	�֐�
	//--------------------
	void CreateTitleGear(void);
	void CreateCamera(void);

	//--------------------
	//	�����o�ϐ�
	//--------------------
	CLoadTextObject *m_pLoadObject;			// �I�u�W�F�N�g�̓ǂݍ���
	CLoadTextTerritory *m_pLoadTerritory;	// �e���g���[�̓ǂݍ���
	CRotationCamera *m_pRotationCamera;		// ��]�J����
	CFieldManager *m_pFieldManager;			// �n�ʊǊ��N���X
	SOUNDSTATE m_SoundState;
};


//================================
// �^�C�g�����S�p�̎��ԃN���X
//================================
class CTitleGear2D : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	CTitleGear2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CTitleGear2D();

	static CTitleGear2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 Size, float fRotSpeed, float fTexHeight, int nPriority = 6, CScene2D::TYPE type = CScene2D::TYPE_GEAR);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRot(const float fRot);
	void SetRotSpeed(const float fRotSpeed);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);

	float GetRot(void);
	float GetRotSpeed(void);
	float GetAngle(void);
	float GetLength(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CalcPolygonScale(void);
	void RotationPolygon(void);

	float m_fRot;        // ���݂̌���
	float m_fRotSpeed;   // ��]�X�s�[�h
	float m_fAngle;      // �|���S���̌���
	float m_fLength;     // �|���S���̒���
};

#endif