//=============================================================================
//
// �t�F�[�h�̏��� [fade.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene.h"
#include "manager.h"


//=============================================================================
// �t�F�[�h�N���X
//=============================================================================
class CFade : public CScene
{
public:
	typedef enum
	{
		FADE_NONE = 0,
		FADE_IN,
		FADE_OUT,
	}FADE;

	//�����o�֐�
	CFade();
	~CFade();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	//static CStock * Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);

	static void SetFade(CManager::MODE ModeNext);
	static FADE GetFadeMode(void);

private:
	void CreateLogo(const LPDIRECT3DDEVICE9 pDevice);
	void ReleaseLogo(void);
	void UpdateLogo(void);
	void DrawLogo(const LPDIRECT3DDEVICE9 pDevice);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffLogo;	// ���_�o�b�t�@�ւ̃|�C���^(���S�p)
	LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`���ւ̃|�C���^(���S�p)
	static FADE				m_fade;						// �t�F�[�h�̏��
	static D3DXCOLOR		m_Coloer;					// �F�̐ݒ�
	static CManager::MODE	m_ModeNext;				//���̃��[�h
};
#endif