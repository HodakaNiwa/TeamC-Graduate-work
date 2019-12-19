//=============================================================================
//
// �Ђъ��ꏈ�� [crack.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _CRACK_H_
#define _CRACK_H_

#include "main.h"
#include "scene3D.h"

//=============================================================================
// �Ђъ���N���X
//=============================================================================
class CCrack : public CScene3D
{
public:
	CCrack();
	CCrack(int nPriority, CScene::OBJTYPE obj);
	~CCrack();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCrack * Create(const D3DXVECTOR3 pos);
	static void LoadTex(void);
	static void UnloadTex(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nLife;		//�\������
	bool m_bDeth;		//���S�t���O
	int m_nCountTime;	//�J�E���^�[
	int m_nPattern;		//�p�^�[��
};
#endif
