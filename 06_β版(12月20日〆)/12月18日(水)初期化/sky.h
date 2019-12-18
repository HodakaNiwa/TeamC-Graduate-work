//*****************************************************************************
//
//     ���b�V���X�t�B�A�̏���[meshSphere.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "meshSphere.h"

//*****************************************************************************
//    ��N���X�̒�`
//*****************************************************************************
class CSky : public CMeshSphere
{
public:    // �N�ł��A�N�Z�X�\
	CSky(int nPriority = 1, OBJTYPE objType = OBJTYPE_NONE);
	~CSky();

	static CSky *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_pTexture;

};

#endif