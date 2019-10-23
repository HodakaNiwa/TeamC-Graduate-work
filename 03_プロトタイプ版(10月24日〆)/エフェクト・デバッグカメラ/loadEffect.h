//=============================================================================
//
// ロードエフェクト処理 [loadEffect.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _LOADEFFECT_H_
#define _LOADEFFECT_H_

#include "main.h"
#include "effecttool.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CSaveText;

//=============================================================================
// 読み込んだエフェクトの生成クラス
//=============================================================================
class CLoadEffect : public CEffectTool
{
public:
	CLoadEffect();
	~CLoadEffect();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CLoadEffect * Create(int nNumEffect, D3DXVECTOR3 pos, int nPriority);

	void SetNumEffect(int nNumEffect) { m_nNumEffect = nNumEffect; };	//エフェクトナンバーの設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };						//位置の設定
	void SetSize(D3DXVECTOR3 size) { m_size = size; };					//サイズの設定
	void SetColor(D3DXCOLOR col) { m_color = col; };					//色の設定
	void SetRemake(bool bRemake) { m_bRemake = bRemake; };				//再設定するかどうか
	void SetPriority(int nPriority) { m_nPriority = nPriority; };		//優先順位の設定
	D3DXMATRIX * GetMtxWorld(void) { return &m_mtxWorld; };				//マトリックスのポインタを取得
	D3DXVECTOR3 GetPos(void) { return m_pos; };							//位置の取得

private:
	D3DXMATRIX 		m_mtxWorld;		//ワールドマトリックスのポインタ
	D3DXVECTOR3		m_pos;			//位置情報
	D3DXVECTOR3		m_size;			//大きさ
	D3DXCOLOR		m_color;		//色
	int				m_nNumEffect;	//エフェクトのナンバー
	int				m_nPriority;	//優先順位の取得
	bool			m_bRemake;		//再設定するかどうか
};
#endif
