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
	CSceneX();
	CSceneX(int nPriority, OBJTYPE objtype);
	~CSceneX();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneX * Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, char FileName[80], LPDIRECT3DTEXTURE9 pTexture);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };				//���S���W�̐ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };				//���S���W�̌���
	void SetScale(D3DXVECTOR3 scale);							//�X�P�[��
	void SetvtxMax(D3DXVECTOR3 vtxMax) { m_vtxMax = vtxMax; }	//�ő�l
	void SetvtxMin(D3DXVECTOR3 vtxMin) { m_vtxMin = vtxMin; }	//�ŏ��l
	void SetMat(float Mat) { m_Mat_a = Mat; };					
	D3DXVECTOR3 GetPos(void) { return m_pos; };
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	D3DXVECTOR3 GetScale(void) { return m_Scale; };
	float GetMat_a(void) { return m_Mat_a; };
	D3DXVECTOR3 GetvtxMin(void) { return m_vtxMin; };
	D3DXVECTOR3 GetvtxMax(void) { return m_vtxMax; };

protected:
	void Load(char FileName[80], LPDIRECT3DTEXTURE9 pTexture);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 pTexture);
	DWORD GetNumMat(void) { return m_nNumMat; }
	LPD3DXBUFFER GetBuffMat(void) { return m_pBuffMat; }
	D3DXVECTOR3			m_vtxMax;								//���_�̍ő�l
	D3DXVECTOR3			m_vtxMin;								//���_�̍ŏ��l
	D3DXVECTOR3			m_pos;									//���S���W
	D3DXVECTOR3			m_rot;									//����
	D3DXVECTOR3			m_Scale;								//�X�P�[��
	D3DXMATRIX			m_mtxWorld;								//���[���h�}�g���b�N�X
	float				m_Mat_a;								//�}�e���A���̃��l

private:
	//�ϐ��錾
	LPDIRECT3DTEXTURE9 * m_ppTexture;							// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH			 m_pMesh;								//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		 m_pBuffMat;							//�}�e���A�����ւ̃|�C���^
	DWORD				 m_nNumMat;								//�}�e���A�����̐�

	void RequestMatrix(D3DXVECTOR3 rot, D3DXVECTOR3 pos);
};
#endif