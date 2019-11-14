//=============================================================================
//
// �|���S������ [polygon.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 3D�|���S���N���X
//=============================================================================
class CScene3D : public CScene
{
public:
	CScene3D();
	CScene3D(int nPriority, CScene::OBJTYPE obj);
	~CScene3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene3D * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; };	//�e�N�X�`���̊�����

	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };					//�ʒu�̍X�V
	void SetColor(D3DXCOLOR col);
	void SetMoveTex(float fMoveX, float fMoveY);
	void SetDivision(D3DXVECTOR2 Division);							//�e�N�X�`���̕�����
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };					//�����̐ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; };						//�ʒu�̎擾
	D3DXVECTOR3 GetRot(void) { return m_rot; };						//�ʒu�̎擾
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }	//�o�b�t�@�̎擾
private:
	//�ϐ��錾

	//�����o�ϐ�
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;				//���S���W
	D3DXVECTOR3				m_rot;				//����
	D3DXVECTOR3				m_size;				//�|���S���̃T�C�Y
	D3DXVECTOR3				m_vtxMin;			//�|���S���̍ŏ��l
	D3DXVECTOR3				m_vtxMax;			//�|���S���̍ŏ��l
	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR2				m_Division;			//������

	D3DXVECTOR3 m_nor[2];
};
#endif
