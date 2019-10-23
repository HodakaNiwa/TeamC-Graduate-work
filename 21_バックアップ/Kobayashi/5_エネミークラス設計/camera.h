//=============================================================================
//
// �J�������� [camera.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//=============================================================================
// �O���錾
//=============================================================================
class CInputKeyboard;
class CPlayer;
class CModel;
//=============================================================================
// �J�����N���X
//=============================================================================
class CCamera
{
public:
	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

	void SetViewport(D3DVIEWPORT9 viewport);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetPosV(void) { return m_posV; };		//���_�̎擾
	D3DXVECTOR3 GetPosR(void) { return m_posR; };		//�����_�̎擾
	void ReSetCamera(void);								//�J�����̐ݒ�
	int GetViewAngle(void) { return m_nViewAngle; };	//��p�̎擾
	bool ReflectObject(D3DXVECTOR3 ObjectPos);				//�J�����Ɏʂ��Ă��邩�ǂ����̔���

protected:

	D3DXVECTOR3 m_posV;				//���_
	D3DXVECTOR3 m_posVDest;			//�ړI�̎��_
	D3DXVECTOR3 m_posR;				//�����_
	D3DXVECTOR3 m_posRDest;			//�ړI�̒����_
	D3DXVECTOR3 m_vecU;				//������x�N�g��
	D3DXVECTOR3 m_Rot;				//����
	D3DXVECTOR3 m_RotDest;			//�ړI�̌���
	D3DXMATRIX  m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX  m_mtxView;			//�r���[�}�g���N�X
	float		m_Lngth;			//����
	float		m_Lngth2;			//�ύX����
	float		m_fAngle;			//�p�x
	int			m_nViewAngle;		//��p
	bool		m_bRot;				//��]���Ă��邩�ǂ���
	bool        m_bHight;			//�������ω��������ǂ���
	D3DVIEWPORT9 m_Viewport;

private:
	void RemakeAngle(float * pAngle);
	void DebugUpdate(CInputKeyboard * pInputKeyboard);
};

//=============================================================================
// �Q�[���J�����N���X
//=============================================================================
class CGameCamera : public CCamera
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_CLEAR,
	}TYPE;

	CGameCamera();
	~CGameCamera();
	void Init(CPlayer *pPlayer = NULL);
	void Uninit(void);
	void Update(void);
	void Set(void);
	void SetType(TYPE type) { m_type = type; };

private:
	void TypeNormal(void);
	void TypeClear(void);
	float	m_fOldHight;	//�O��̍���
	bool	m_bSetRdest;
	TYPE	m_type;
	CPlayer *m_pTargetPlayer;
};

//=============================================================================
// �^�C�g���J�����N���X
//=============================================================================
class CTitleCamera : public CCamera
{
public:
	//�J�����̃A���O���^�C�v
	typedef enum
	{
		TYPE_00,
		TYPE_01,
		TYPE_MAX,
	}TYPE;

	CTitleCamera();
	~CTitleCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

private:
	int m_nCounterTime;	//���Ԃ̃J�E���^�[
	TYPE m_type;		//�J�����̃^�C�v
	bool m_bSetCamera;	//�J�����̈ʒu��ݒ肵�����ǂ���
};

//=============================================================================
// �����L���O�J�����N���X
//=============================================================================
class CRankingCamera : public CCamera
{
public:
	CRankingCamera();
	~CRankingCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

private:
	float m_fSpeed;
	float m_fCountDistance;
	float m_fMove;
};
#endif