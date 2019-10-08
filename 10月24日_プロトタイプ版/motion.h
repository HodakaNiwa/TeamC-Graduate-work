//=============================================================================
//
// モーション処理 [motion.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "input.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CModel;

//=============================================================================
// 3Dポリゴンクラス
//=============================================================================
class CMotion
{
public:
	//キー要素
	typedef struct
	{
		float fPosX;	//X座標
		float fPosY;	//Y座標
		float fPosZ;	//Z座標
		float fRotX;	//X向き
		float fRotY;	//Y向き
		float fRotZ;	//Z向き
	}KEY;

	//キー情報
	typedef struct
	{
		int nFrame;	//フレーム数
		KEY * aKey;	//パーツ分のキー
	}KEY_INFO;

	//モーション情報
	typedef struct
	{
		bool m_bLoop;				//ループ有りかどうか
		int nNumKey;				//キーの総数
		KEY_INFO * m_pKeyInfo;		//キー情報
	}MOTION_INFO;

	CMotion();
	~CMotion();
	HRESULT Init(void);
	void Uninit(void);
	int Update(void);
	void Draw(void);
	static CMotion * Create(void);

	void SetMotion(MOTION_INFO * pMotionInfo) { m_pMotionInfo = pMotionInfo; };		//モーション情報を設定する
	void SetMaxMotion(int nMaxMotion) { m_nMaxMotion = nMaxMotion; };				//モーションの最大数をセットする
	MOTION_INFO * GetMotionInfo(void) { return m_pMotionInfo; };					//モーションデータの取得
	int GetKey(void) { return m_nKey; };											//現在のキーを取得
	int GetnCountMotion(void) { return m_nCountMotion; };							//現在のフレーム数の取得
	void SetModel(CModel ** pModel) { m_pModel = pModel; };							//モデル情報の取得
	void SetMaxModel(int nMaxModel) { m_nMaxModel = nMaxModel; };					//モデルの最大数取得
	D3DXVECTOR3 GetMove(void) { return m_move; };									//高さを返す
	void SetBlend(bool bBlend) { m_bBlend = bBlend; };								//モーションのブレンド状態の設定

	int GetNumMotion(void) { return m_nNumMotion; };								//現在のモーションを取得
	void SetNumMotion(int NumMotion);												//モーションの設定

private:
	KEY * SetKey(int nCntModel);
	void SetMotionDifference(KEY * pNextKey, KEY * pKey, int nCntModel);
	CModel **		m_pModel;			//パーツモデルのポインタ
	int				m_nMaxModel;		//読み込むモデルの最大数
	D3DXVECTOR3		m_move;				//移動
	D3DXVECTOR3		m_rot;				//向き
	int				m_nMaxMotion;		//モーションの最大数
	MOTION_INFO *	m_pMotionInfo;		//モーション情報
	int				m_nKey;				//現在のキーNO
	int				m_nCountMotion;		//モーションカウンター
	int				m_nNumMotion;		//現在何のモーションか
	int				m_nNextNumMotion;	//次のモーション
	bool			m_bBlend;			//次のモーションにつなげるかどうか
};
#endif