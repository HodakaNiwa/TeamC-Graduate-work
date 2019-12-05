//=============================================================================
//
// �^�C�}�[�̏��� [timer.h]
// Author : �R���֎j
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CNumber2D;

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TIMENUMBER	(1)

//=============================================================================
// �X�R�A�N���X
//=============================================================================
class CTimer : public CScene
{
public:
	typedef enum
	{
		TYPE_GAME,
		TYPE_COUNTER,
		TYPE_RANKING,
		TYPE_NONE
	}TYPE;

	//�����o�֐�
	CTimer(int nPriority,OBJTYPE objType);
	virtual ~CTimer();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	static CTimer * Create(TYPE type, int Priority, D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static void AddTime(int nTime);
	static int GetTime(void);
	void SetSizePos(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) { m_size = size; m_pos = pos; };

protected:
	static int m_nTime;
	static int m_nTimeData[MAX_TIMENUMBER];
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_pos;
};

//=============================================================================
// �X�R�A�Q�[��
//=============================================================================
class CCountDownTimer : public CTimer
{
public:
	CCountDownTimer(int Priority, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CCountDownTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void TimeCountUpdate(void);
private:
	//�֐��錾
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	CNumber2D * m_pNumber[MAX_TIMENUMBER];			//�i���o�[�̃|�C���^
	static int	m_nNumber[MAX_TIMENUMBER];			//�e�����ԍ�
	int m_nCounter;
	int m_nCountDown;
	int m_nDeathCount;
	bool m_bDeath;
	bool m_bCountStop;
};

//=============================================================================
// �X�R�A�Q�[��
//=============================================================================
class CGameTimer : public CTimer
{
public:
	CGameTimer(int Priority, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CGameTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void TimeGameUpdate(void);
private:
	//�֐��錾
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	CNumber2D * m_pNumber[MAX_TIMENUMBER];			//�i���o�[�̃|�C���^
	static int	m_nNumber[MAX_TIMENUMBER];			//�e�����ԍ�
	int m_nTimeCounter;								//�^�C�}�[�J�E���^�[
	int m_nTime;									//�^�C�}�[
	bool m_bTimeStop;								//�^�C���X�g�b�v
	int m_nDeathGameCount;							
};
#endif
