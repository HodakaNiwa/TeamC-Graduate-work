#pragma once
//=============================================================================
//
// �~�j�}�b�v�̏��� [minimap.h]
// Author : �R���֎j
//
//=============================================================================
#ifndef _TIMERLOGO_H_
#define _TIMERLOGO_H_

#include "main.h"
#include "scene.h"
#include "scene2D.h"

//�O���錾
class CScene2D;

//=============================================================================
// �~�j�}�b�v�N���X
//=============================================================================
class CTimerLogo : public CScene2D
{
public:
	typedef enum
	{
		TIMERLOGO_START = 0,
		TIMERLOGO_2MIN,
		TIMERLOGO_1MIN,
		TIMERLOGO_30SEC,
		TIMERLOGO_FIN,
		TIMERLOGO_MAX
	}TIMERLOGO;

	CTimerLogo(int nPriority, CScene::OBJTYPE type);
	CTimerLogo();
	~CTimerLogo();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTimerLogo * Create(const D3DXVECTOR3 pos,const D3DXVECTOR3 size,TIMERLOGO timerlogo);

	static HRESULT Load(void);							//�e�N�X�`���ǂݍ���
	static void Unload(void);
private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9			m_pTexture[TIMERLOGO_MAX];		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3					m_Size;			// �|���S���̃T�C�Y
	D3DXCOLOR					m_Color;		// �F
	TIMERLOGO					m_TimerLogo;
};

#endif