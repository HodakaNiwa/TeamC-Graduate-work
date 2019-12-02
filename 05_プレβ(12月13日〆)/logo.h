//=============================================================================
//
// ���S�̏��� [Logo.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _LOGO_H_
#define _LOGO_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// 2D�|���S���N���X
//=============================================================================
class CLogo: public CScene2D
{
public:
	CLogo(int nPriority, CScene::OBJTYPE type);
	~CLogo();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:
};

#endif