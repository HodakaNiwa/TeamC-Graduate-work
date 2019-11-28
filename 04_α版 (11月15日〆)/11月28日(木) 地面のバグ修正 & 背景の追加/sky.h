//*****************************************************************************
//
//     メッシュスフィアの処理[meshSphere.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "meshSphere.h"

//*****************************************************************************
//    空クラスの定義
//*****************************************************************************
class CSky : public CMeshSphere
{
public:    // 誰でもアクセス可能
	CSky(int nPriority = 1, OBJTYPE objType = OBJTYPE_NONE);
	~CSky();

	static CSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

#endif