//=============================================================================
//
// �O�p�`�|���S��3D�̏��� [sceneTriangle3D.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _SCENE_TRAIANGLE3D_H_
#define _SCENE_TRAIANGLE3D_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 3D�|���S���N���X
//=============================================================================
class CSceneTriangle3D : public CScene
{
public:
	CSceneTriangle3D();
	~CSceneTriangle3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneTriangle3D * Create(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);
	void SetColor(D3DXCOLOR col);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }	//�o�b�t�@�̎擾

private:
	void SetWorldMatrix(void);

	//�����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;								//���[���h�}�g���b�N�X
	int m_nLife;													//�\������
	D3DXCOLOR m_col;												//�F
};
#endif
