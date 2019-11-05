//=============================================================================
//
// �ړ��^�C�v��UI�̏��� [moveUI.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _MOVEUI_H_
#define _MOVEUI_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �ړ��^�C�v��UI�N���X
//=============================================================================
class CMoveUI: public CScene2D
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_MOVE_LEFT,
		STATE_MOVE_RIGHT,
		STATE_MOVE_UP,
		STATE_MOVE_DOWN,
	}STATE;

	CMoveUI();
	~CMoveUI();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMoveUI * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 pTexture);

private:
	void UpdateMoveLeft(void);
	void UpdateMoveRight(void);
	void UpdateUp(void);
	void UpdateDown(void);
	STATE m_state;
};

#endif