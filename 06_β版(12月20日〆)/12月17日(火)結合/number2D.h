//=============================================================================
//
// �����̏��� [number2D.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _NUMBER2D_H_
#define _NUMBER2D_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �i���o�[�N���X
//=============================================================================
class CNumber2D
{
public:
	//�����o�֐�
	CNumber2D();
	~CNumber2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CNumber2D * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, D3DXCOLOR col);
	void SetNumber(const int nNumber);
	void SetbUse(bool bUse);
	void SetPos(D3DXVECTOR3 pos);
	void SetSize(D3DXVECTOR3 size);					//�T�C�Y�̐ݒ�
	void ResetPos(void);
	void SetColor(D3DXCOLOR col);
	D3DXVECTOR3 GetPos(void) { return m_pos; };		//�ʒu�̎擾
	D3DXVECTOR3 GetSize(void) { return m_Size; };	//�T�C�Y�̎擾

	//�e�N�X�`���̐ݒ�
	static HRESULT Load(void);
	static void Unload(void);

private:
	//�֐��錾

	//�ÓI�����o�ϐ��錾
	static LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���̃|�C���^

	//�ϐ��錾
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;					// �|���S���̈ʒu
	D3DXVECTOR3				m_Other;				// �|���S���̑Ίp��
	D3DXVECTOR3				m_Size;					// �|���S���̃T�C�Y
	bool					m_bUse;					// �g�p���Ă��邩�ǂ���
	D3DXVECTOR3				m_HoldPos;				// ���W�̕ۑ�
};
#endif