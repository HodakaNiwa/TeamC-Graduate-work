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
	CMoveUI();
	~CMoveUI();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMoveUI * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, LPDIRECT3DTEXTURE9 pTexture);
	void UpdateMoveLeft(float fMove);
	void UpdateMoveRight(float fMove);
	void UpdateMoveUp(float fMove);
	void UpdateMoveDown(float fMove);
	void UpdateScaleUp(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move);
	void UpdateScaleDown(D3DXVECTOR3 AimSize, D3DXVECTOR2 Move);
	void UpdateMoveDest(D3DXVECTOR3 AimPos, float fSpeed);

private:
};

#endif