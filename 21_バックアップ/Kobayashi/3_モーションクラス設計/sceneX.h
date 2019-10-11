//=============================================================================
//
// X�t�@�C������ [sceneX.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// X�t�@�C���N���X
//=============================================================================
class CSceneX : public CScene
{
public:

	typedef enum
	{
		TYPE_HART = 0,
		TYPE_OBJ,
		TYPE_ARROW,
	}TYPE;

	CSceneX();
	CSceneX(int nPriority, OBJTYPE objtype);
	~CSceneX();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneX * Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, char FileName[80], LPDIRECT3DTEXTURE9 pTexture, TYPE type);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };					//���S���W�̐ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };					//���S���W�̌���
	void SetMat(float Mat) { m_Mat_a = Mat; };					//���S���W�̌���
	D3DXVECTOR3 GetPos(void) { return m_pos; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	float GetMat_a(void) { return m_Mat_a; };
	D3DXVECTOR3 GetvtxMin(void) { return m_vtxMin; };
	D3DXVECTOR3 GetvtxMax(void) { return m_vtxMax; };

protected:
	void Load(char FileName[80], LPDIRECT3DTEXTURE9 pTexture);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	D3DXVECTOR3			m_vtxMin;		//���_�̍ŏ��l
	D3DXVECTOR3			m_vtxMax;		//���_�̍ő�l

private:
	//�ϐ��錾

	LPDIRECT3DTEXTURE9 * m_ppTexture;		// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			 m_pMesh;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		 m_pBuffMat;		//�}�e���A�����ւ̃|�C���^
	DWORD				 m_nNumMat;			//�}�e���A�����̐�

	//�����o�ϐ�
	D3DXVECTOR3				m_pos;				//���S���W
	D3DXVECTOR3				m_rot;				//����
	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	float					m_Mat_a;			//�}�e���A���̃��l
};
#endif