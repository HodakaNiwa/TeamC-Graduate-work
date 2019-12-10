//*****************************************************************************
//
//     地雷の処理[mine.h]
//     Author:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MINE_H_
#define _MINE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************


//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CLoadEffect;

//*****************************************************************************
//    地雷クラスの定義
//*****************************************************************************
class CMine : public CSceneX
{
public:    // 誰でもアクセス可能
	//--------------------
	//	状態
	//--------------------
	typedef enum
	{
		TYPE_NONE = 0,	// 通常状態
		TYPE_STARTUP,	// 起動状態
		TYPE_EXPLOSION,	// 爆発状態
		TYPE_MAX
	}TYPE;

	CMine(int nPriority = 3, OBJTYPE objtype = OBJTYPE_MINE);
	~CMine();

	static CMine *Create(D3DXVECTOR3 pos, CScene *pParentChara);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------
	//	Set & Get
	//--------------------
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetType(const int nType) { m_nType = nType; }
	void SetScaleUp(const float fScaleUp) { m_fScaleUp = fScaleUp; }
	void SetExplosionTimer(const int nExplosionTimer) { m_nExplosionTimer = nExplosionTimer; }
	void SetParentChara(CScene *pParentChara) { m_pParentChara = pParentChara; }

	int GetType(void) { return m_nType; }
	int GetLife(void) { return m_nLife; }
	float GetScaleUp(void) { return m_fScaleUp; }
	int GetExplosionTimer(void) { return m_nExplosionTimer; }
	CScene *GetParentChara(void) { return m_pParentChara; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	//--------------------
	//	関数
	//--------------------
	void None(void);
	void StartUp(void);
	void Explosion(void);
	void CreateStartUpCollider(void);
	void CreateExplosionEffect(void);
	void CreateExplosionCollider(void);

	//--------------------
	//	メンバ変数
	//--------------------
	int m_nType;							// 状態
	int m_nLife;							// 寿命
	float m_fScaleUp;						// どれくらい大きくするか
	int m_nExplosionTimer;					// 爆発までの時間を計測する
	CScene *m_pParentChara;					// 自身を生成したキャラへのポインタ
	CLoadEffect * m_pLoadEffect;			// エフェクトの情報


	//--------------------
	//	静的メンバ変数
	//--------------------
	static LPD3DXMESH m_pMesh;
	static LPD3DXBUFFER m_pBuffMat;
	static DWORD m_nNumMat;
};

#endif