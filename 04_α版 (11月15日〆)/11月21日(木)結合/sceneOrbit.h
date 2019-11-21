//=============================================================================
//
// ���b�V���t�B�[���h���� [sceneMeshfiled.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CLoadTextOrbit;

//=============================================================================
// �O�ՃN���X
//=============================================================================
class CSceneOrbit: public CScene
{
public:
	//�O�Ղ̏��
	typedef struct
	{
		int							nTex;					//�e�N�X�`���̔ԍ�
		D3DXVECTOR3					pos[2];					//�I�t�Z�b�g
		int							nPrimitiv;				//���_��
		int							nPorigon;				//�|���S����
		D3DXCOLOR					color;					//�F
		float						fUV;					//UV�̕�����
		float						fAnim;					//�A�j���[�V������
	}ORBIT_INFO;

	//��Ղ̎��
	typedef enum
	{
		TYPE_PLAYER = 0,
		TYPE_STAR,
		TYPE_MAX,
	}TYPE;

	CSceneOrbit();
	~CSceneOrbit();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneOrbit * Create(TYPE type, D3DXVECTOR3 pos);
	static void Load(void);
	static void Unload(void);
	void SetMtxParent(D3DXMATRIX * pMtx) { m_pMtxParent = pMtx; };			//�}�g���b�N�X�̎n�_
	void SetMtxParentEnd(D3DXMATRIX * pMtx) { m_pMtxParentEnd = pMtx; };	//�}�g���b�N�X�̏I�_
	void SetColor(D3DXCOLOR col);											//�F�̐ݒ�

private:
	//�ϐ��錾
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SetType(TYPE type) { m_type = type; };				//�^�C�v�̐ݒ�

	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9	* m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	static ORBIT_INFO *			m_pOrbitInfo;				//�O�Ղ̍\���̃|�C���^
	static int					m_nMaxTexture;				//�e�N�X�`���̍ő吔
	static CLoadTextOrbit *		m_pLoadText;				//�e�L�X�g�̏��
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;					//���[���h�}�g���b�N�X
	D3DXMATRIX *				m_pMtxParent;				//�e�}�g���b�N�X�̏��
	D3DXMATRIX *				m_pMtxParentEnd;			//�e�}�g���b�N�X�̏��
	D3DXVECTOR3	*				m_aPosVertex;				//���_�̌v�Z�p
	TYPE						m_type;						//�^�C�v
	D3DXVECTOR3					m_pos;						//�ʒu
};
#endif
