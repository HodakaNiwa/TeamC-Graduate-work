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

	static CSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

#endif