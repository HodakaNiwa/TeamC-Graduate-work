#pragma once
//=============================================================================
//
// �~�j�}�b�v�̏��� [minimap.h]
// Author : �R���֎j
//
//=============================================================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "main.h"
#include "scene.h"

//�O���錾
class CScene2D;

//=============================================================================
// �~�j�}�b�v�N���X
//=============================================================================
class CMiniMap
{
public:
	CMiniMap(int nPriority, CScene::OBJTYPE type);
	CMiniMap();
	~CMiniMap();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMiniMap * Create(void);
private:
	//�����o�ϐ�
	LPDIRECT3DTEXTURE9			m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3					m_Size;			// �|���S���̃T�C�Y
	D3DXCOLOR					m_Color;		// �F
	CScene2D                   *m_pScene2D;
};

#endif