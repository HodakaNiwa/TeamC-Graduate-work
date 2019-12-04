//=============================================================================
//
// �r���{�[�h���� [scenebillboard.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _SCENEBILLBORD_H_
#define _SCENEBILLBORD_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �r���{�[�h�N���X
//=============================================================================
class CSceneBillborad : public CScene
{
public:
	CSceneBillborad();
	CSceneBillborad(int nPriority, OBJTYPE ObjType);
	~CSceneBillborad();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneBillborad * Create(const D3DXVECTOR3 pos);
	D3DXMATRIX * GetMtxWorld(void) { return &m_mtxWorld; };		//���[���h�}�g���b�N�X�̎擾
	void SetSize(D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetSize(void) { return m_size; }				//�T�C�Y�̐ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }
protected:
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetColor(D3DXCOLOR color);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetUV(D3DXVECTOR2 uvMIn, D3DXVECTOR2 uvMax);
	void SetColorABillborad(float colA);
	D3DXVECTOR3		m_pos;		//���S���W
	D3DXVECTOR3		m_size;		//�T�C�Y
	D3DXVECTOR3		m_rot;		//�����̐ݒ�

private:

	//�����o�ϐ�
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXCOLOR				m_Color;	//�F
};
#endif