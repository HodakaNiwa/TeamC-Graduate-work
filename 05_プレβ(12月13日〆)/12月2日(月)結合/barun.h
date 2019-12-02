//=============================================================================
//
// ���D���� [barun.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _BARUN_H_
#define _BARUN_H_

#include "main.h"
#include "sceneBillBorad.h"

//=============================================================================
// �r���{�[�h�N���X
//=============================================================================
class CBarun : public CSceneBillborad
{
public:
	CBarun();
	CBarun(int nPriority, OBJTYPE ObjType);
	~CBarun();
	HRESULT Init(int nType, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBarun * Create(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move);
	static void LoadTex(void);
	static void UnloadTex(void);

private:

	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_move;
};
#endif