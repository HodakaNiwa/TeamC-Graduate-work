//=============================================================================
//
// テキストの読み込み処理 [LoadText.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _LOADTEXT_H_
#define _LOADTEXT_H_

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "scene2D.h"
#include "sceneOrbit.h"

//=============================================================================
// テキスト読み込みクラス
//=============================================================================
class CLoadText
{
public:
	//テクスチャの情報
	typedef struct
	{
		int		nMaxTex;			//テクスチャの最大数
		char ** ppFileName;			//ファイルを格納する
	}TEX_INFO;

	CLoadText();
	~CLoadText();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	static CLoadText * Create(void);
	TEX_INFO GetTexInfo(void) { return m_TexInfo; };									//テクスチャ情報
	void SetLoadTextFileName(char * pFileName) { m_pLoadTextFileName = pFileName; };	//読み込むテキストファイル目の保管

protected:
	char * ReadLine(FILE * pFile, char * pDest);
	char * AddChar(char * pStrCur, char * aStrCur, int nAddChar);
	int PopString(char * pStrCur, char * aStrCur);
	void AddNull(char * pLine);
	char * m_pStrCur;					//文字列の先頭へのポインタ
	char  m_aLine[256];				//文字列読み込み用（一行分）
	char  m_aStrCur[256];				//文字列抜き出し用
	TEX_INFO m_TexInfo;				//テクスチャ情報
	char * m_pLoadTextFileName;		//テキストファイル名

};

//=============================================================================
// ステージオブジェクト情報読み込みクラス
//=============================================================================
class CLoadTextObject : public CLoadText
{
public:
	typedef struct
	{
		int nMaxModel;				//モデルのの最大数
		char ** ppModelFileName;	//モデルのファイル名格納
	}OBJ_INFO;

	CLoadTextObject();
	~CLoadTextObject();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextObject * Create(char * pFileName);
	OBJ_INFO GetObjInfo(void) { return m_ObjInfo; };

private:
	OBJ_INFO m_ObjInfo;
};

//=============================================================================
// テリトリー情報読み込みクラス
//=============================================================================
class CLoadTextTerritory : public CLoadText
{
public:
	typedef struct
	{
		int nMaxModel;				//モデルのの最大数
		char ** ppModelFileName;	//モデルのファイル名格納
	}OBJ_INFO;

	CLoadTextTerritory();
	~CLoadTextTerritory();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextTerritory * Create(char * pFileName);
	OBJ_INFO GetObjInfo(void) { return m_ObjInfo; };

private:
	OBJ_INFO m_ObjInfo;
};

//=============================================================================
// テクスチャの読み込み情報
//=============================================================================
class CLoadTextTexture : public CLoadText
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 size;
		int nTex;
		CScene2D::TYPE type;
	}POLYGON_INFO;

	CLoadTextTexture();
	~CLoadTextTexture();
	HRESULT Init(void);
	void Uninit(void);
	static CLoadTextTexture * Create(char * pFileName);
	POLYGON_INFO GetPolygonInfo(int nCntPolygon) { return m_pPolygonInfo[nCntPolygon]; }
	int GetMaxPolygon(void) { return m_nMaxPolygon; }

private:
	POLYGON_INFO * m_pPolygonInfo;
	int m_nMaxPolygon;
	int m_nCountPoly;
};

//=============================================================================
// 軌跡の読み込みクラス
//=============================================================================
class CLoadTextOrbit : public CLoadText
{
public:
	CLoadTextOrbit();
	~CLoadTextOrbit();
	HRESULT Init(void);
	void Uninit(void);
	CSceneOrbit::ORBIT_INFO * GetOrbit(void) { return m_pOrbit; };	//軌跡情報の取得
	static CLoadTextOrbit * Create(void);

private:
	CSceneOrbit::ORBIT_INFO * m_pOrbit;	//軌跡ポインタ
};
#endif