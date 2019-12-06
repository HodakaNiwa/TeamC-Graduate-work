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
// �}�N����`
//=============================================================================
#define MAX_CAMERAPOS	(4)

//=============================================================================
// �O���錾
//=============================================================================
class CInputKeyboard;
class CPlayer;
class CModel;
class CTerritory;
class CRobot;
class CCharacter;

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
	void SetPosV(D3DXVECTOR3 PosV) { m_posV = PosV; };	//���_�̎擾
	void SetPosR(D3DXVECTOR3 PosR) { m_posR = PosR; };	//���_�̎擾
	void ReSetCamera(void);								//�J�����̐ݒ�
	int GetViewAngle(void) { return m_nViewAngle; };	//��p�̎擾
	bool ReflectObject(D3DXVECTOR3 ObjectPos);			//�J�����Ɏʂ��Ă��邩�ǂ����̔���

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

	D3DXVECTOR3 m_VecA[MAX_CAMERAPOS];
};


//=============================================================================
// ��]�J�����N���X
//=============================================================================
class CRotationCamera : public CCamera
{
public:    // �N�ł��A�N�Z�X�\
	CRotationCamera();
	~CRotationCamera();

	static CRotationCamera *Create(float posVHeight, D3DXVECTOR3 posR, float fRotSpeed, float fLength);

	HRESULT Init(float posVHeight, D3DXVECTOR3 posR, float fRotSpeed, float fLength);
	void Uninit(void);
	void Update(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetRotSpeed(const float fRotSpeed) { m_fRotSpeed = fRotSpeed; }
	float GetRotSpeed(void) { return m_fRotSpeed; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fRotSpeed;
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
	bool CheckTerritoryTopInScreen(CTerritory *pTerritoryTop, D3DXVECTOR3 &pos);
	void CutAddValue(float fCutHeight, float fCutDepth);
	float	m_fOldHight;	//�O��̍���
	bool	m_bSetRdest;
	TYPE	m_type;
	CPlayer *m_pTargetPlayer;
	float   m_fAddHeight;	// �����ɉ�����l
	float   m_fAddDepth;	// ���s�ɉ�����l
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
// ���U���g�J�����N���X
//=============================================================================
class CResultCamera : public CCamera
{
public:
	typedef enum
	{
		STATE_CHAR_IN = 0,
		STATE_CHAR_ZOOM,
		STATE_CHAR_ZOOMOUT,
		STATE_CHAR_STOP,
	}STATE;

	CResultCamera();
	~CResultCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);
	void SetTargetPos(D3DXVECTOR3 Pos) { m_TargetCharPos = Pos; }
	void SetState(STATE state) { m_state = state; }
	STATE GetState(void) { return m_state; }

private:
	void UpdateCharIn(void);
	void UpdateCharZoom(void);
	void UpdateCharZoomOut(void);

	D3DXVECTOR3 m_EndPosV;			//�ŏI�n�_
	D3DXVECTOR3 m_EndPosR;			//�ŏI�����_
	D3DXVECTOR3 m_ZoomOutPos;		//�Y�[���A�E�g�p�̎��_
	D3DXVECTOR3 m_TargetCharPos;	//�^�[�Q�b�g�L�����̈ʒu
	D3DXVECTOR3 m_MoveV;			//���_�̈ړ���
	D3DXVECTOR3 m_MoveR;			//�����_�̈ړ���
	int m_nCountTime;				//���ԃJ�E���^�[
	STATE m_state;					//�X�V���
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

//=============================================================================
// �C�x���g�J�����N���X���ǉ�(�悵�낤)
//=============================================================================
class CEventCamera : public CCamera
{
public:
	typedef enum
	{
		TYPE_ROBOT = 0,
		TYPE_AREABONUS_START,
		TYPE_AREABONUS_FINISH,
		TYPE_DIVISION_START,
		TYPE_DIVISION_FINISH,
		TYPE_MAX
	}TYPE;

	CEventCamera();
	~CEventCamera();
	void Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, CCharacter *pChar = NULL, TYPE type = TYPE_ROBOT);
	void Uninit(void);
	void Update(void);
	void Set(void);

private:
	void RobotEvent(void);
	void AreaBonusEvent_Start(void);
	void AreaBonusEvent_Finish(void);
	void DivisionEvent_Start(void);
	void DivisionEvent_Finish(void);

	TYPE 		m_Type;
	CCharacter	*m_pTargetRobot;
	int			m_nCounter;
};
#endif