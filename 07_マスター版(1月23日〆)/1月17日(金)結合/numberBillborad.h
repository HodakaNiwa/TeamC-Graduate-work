//=============================================================================
//
// �i���o�[�r���{�[�h���� [scenebillboard.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _NUMBERBILLBORD_H_
#define _NUMBERBILLBORD_H_

#include "main.h"

//=============================================================================
// �r���{�[�h�N���X
//=============================================================================
class CNumberBillborad
{
public:
	CNumberBillborad();
	~CNumberBillborad();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CNumberBillborad * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	D3DXMATRIX * GetMtxWorld(void) { return &m_mtxWorld; };		//���[���h�}�g���b�N�X�̎擾
	void SetSize(D3DXVECTOR3 size);
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetSize(void) { return m_size; }				//�T�C�Y�̐ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXCOLOR GetColor(void) { return m_Color; }
	void SetUV(D3DXVECTOR2 uvMIn, D3DXVECTOR2 uvMax);
	void SetColor(D3DXCOLOR color);
	static void LoadTex(void);									//�e�N�X�`���̓ǂݍ���
	static void UnloadTex(void);								//�e�N�X�`���̔j��

private:
	//�����o�֐�
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetColorABillborad(float colA);

	//�ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9		m_pTexture;							// �e�N�X�`���ւ̃|�C���^

	//�����o�ϐ�
	D3DXVECTOR3				m_pos;								//���S���W
	D3DXVECTOR3				m_size;								//�T�C�Y
	D3DXVECTOR3				m_rot;								//�����̐ݒ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;							//���[���h�}�g���b�N�X
	D3DXCOLOR				m_Color;							//�F
};
#endif