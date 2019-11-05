//=============================================================================
//
// 2D�|���S���̏��� [scene2D.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// 2D�|���S���N���X
//=============================================================================
class CScene2D: public CScene
{
public:
	typedef enum
	{
		TYPE_LOGO = 0,
		TYPE_BOTTUN,
		TYPE_GEAR,
		TYPE_MAX,
	}TYPE;

	CScene2D(int nPriority, CScene::OBJTYPE type);
	CScene2D();
	~CScene2D();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene2D * Create(const D3DXVECTOR3 pos, D3DXVECTOR3 size, TYPE type);

	//�e�N�X�`���̐ݒ�
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	D3DXVECTOR2 GetTex(void);
	void SetTex(float uv, int nPattrn);
	void SetTex2(float uv, float uv2, int nPattrn, int Width);
	void SetTexUV(float u, float u2, float v, float v2);
	void SetTexMove(const float uv);
	void SetTexMove2(const float uv);

	//�ʒu���
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetOtherPos(void);

	//���W�̐ݒ�
	void SetPos(const D3DXVECTOR3 pos, const D3DXVECTOR3 Otherpos);
	void SetRotPos(const D3DXVECTOR3 pos, const float fAngle, const float fRadius);

	//�|���S���̃T�C�Y
	D3DXVECTOR3 GetSize(void);
	void SetSize(const D3DXVECTOR3 size);

	//�g�p���Ă��邩�ǂ���
	bool GetUse(void);
	void SetUse(const bool bUse);

	//�F�̐ݒ�
	void SetColoer(const D3DXCOLOR col);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	D3DXCOLOR GetColor(void) { return m_Color; };

	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

private:
	//�����o�ϐ�
	LPDIRECT3DTEXTURE9			m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3					m_pos;			// �|���S���̈ʒu
	D3DXVECTOR3					m_Other;		// �|���S���̑Ίp��
	D3DXVECTOR3					m_Size;			// �|���S���̃T�C�Y
	D3DXVECTOR2					m_Tex;			// �e�N�X�`����UV�l
	D3DXCOLOR					m_Color;		// �F
	bool						m_bUse;			// �g�p���Ă��邩�ǂ���
};

#endif