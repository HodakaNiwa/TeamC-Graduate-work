//=============================================================================
//
// �i���o�[�̏��� [number.h]
// Author : Yamashita
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "scene2D.h"

//�i���o�[�N���X
class CNumber
{
public:
	CNumber();											//�R���X�g���N�^
	~CNumber();											//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fWeight, float fHeight);					//����������
	void Uninit(void);								//�I������
	void Update(void);								//�X�V����
	void Draw(void);								//�`�揈��

	//�ÓI�����o�֐�
	static CNumber *Create(D3DXVECTOR3 pos,float fWeight,float fHeight);
	static HRESULT Load(void);						//�e�N�X�`���ǂݍ���
	static void Unload(void);
	
	void SetNumber(int nNumber);
	void SetColNumber(D3DXCOLOR col);
private:
	static LPDIRECT3DTEXTURE9     m_pTexture;
	LPDIRECT3DVERTEXBUFFER9		  m_pVtxBuff;
	D3DXVECTOR3					  m_pos;					// �|���S���̈ʒu

	D3DXCOLOR					  m_col;					// �|���S���̈ʒu
};
#endif

