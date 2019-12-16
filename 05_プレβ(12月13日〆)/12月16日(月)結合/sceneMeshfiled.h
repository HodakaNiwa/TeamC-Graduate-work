//=============================================================================
//
// ���b�V���t�B�[���h���� [sceneMeshfiled.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _SCENEMESHFILED_H_
#define _SCENEMESHFILED_H_

#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_TEX	(1)

//=============================================================================
// ���b�V���t�B�[���h�N���X
//=============================================================================
class CSceneMeshFiled : public CScene
{
public:
	CSceneMeshFiled();
	~CSceneMeshFiled();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSceneMeshFiled * Create(const D3DXVECTOR3 pos);
	static void Load(void);
	static void Unload(void);
	float GetHight(D3DXVECTOR3 pos);

#ifdef _DEBUG
	void SetHight(D3DXVECTOR3 pos, float fValue, float fRange);
#endif


private:
	//�ϐ��錾
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size);
	void SaveData(void);
	void LoadData(void);
	void SetNor(void);		//�@�����X�V����

#ifdef _DEBUG
	void UpdatePrimitiv(void);
	void SetPosY(int nVertex);
#endif

	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9	m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;			// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3					m_pos;				//���S���W
	D3DXVECTOR3					m_rot;				//����
	int							m_nHight;			//�|��̐�
	int							m_nWidth;			//����̐�
	int							m_nIndex;			//�C���f�b�N�X��
	int							m_nPolygon;			//�|���S����
	int							m_nPrimitiv;		//���_��
	int							m_nSelectPrimitiv;	//���ݑI������Ă��钸�_
	bool						m_bUpdatePrimitiv;	//���_��񂪍X�V���ꂢ�邩�ǂ���

	D3DXVECTOR3					m_LoadPos[3000];	//�ǂݍ��ݗp�̒��_

	D3DXVECTOR3 m_nor[2];
};
#endif
