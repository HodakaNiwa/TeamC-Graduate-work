//=============================================================================
//
// �n�ʏ��� [field.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#include "scene3D.h"

//=============================================================================
// 3D�|���S���N���X
//=============================================================================
class CField : public CScene3D
{
public:
	//�t�B�[���h�̏��
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_X_TURN,
		STATE_X_TURNBACK,
		STATE_MOVE_Z,
		STATE_NOT_UPDATE,
		STATE_MAX,
	}STATE;

	typedef enum
	{
		TYPE_BLOCK = 0,
		TYPE_MAINFIELD,
		TYPE_COUNTDOWN,
	}TYPE;

	CField();
	CField(int nPriority, CScene::OBJTYPE obj);
	~CField();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CField * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, TYPE type);

	//Get�ESet
	STATE GetState(void) { return m_state; }
	void SetState(STATE state) { m_state = state; }
	void SetLight(bool bLight) { m_bLight = bLight; }
	void SetType(TYPE type) { m_type = type; }

private:
	void UpdateXturn(void);
	void UpdateXturnBack(void);
	void UpdateMoveZ(void);
	void ChackDraw(void);
	bool m_bLight;
	STATE m_state;
	TYPE  m_type;	// �^�C�v�̐ݒ�
	int	  m_nCountTime;	//���Ԃ̃J�E���g
};
#endif
