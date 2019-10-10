//*****************************************************************************
//
//     共通ライブラリの処理[library.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// 共通で使う文字の定義
#define STRING_TAB       "\t"                     // Tabの文字列
#define STRING_ENTER     "\n"                     // 改行コード
#define STRING_SPACE     " "                      // 空白の文字列
#define STRING_COMMENT   "#"                      // コメントが開始される文字
#define SCRIPT           "SCRIPT"                 // スクリプト読み込み開始の合図
#define END_SCRIPT       "END_SCRIPT"             // スクリプト読み込み終了の合図

//*****************************************************************************
//     共通関数クラスの定義
//*****************************************************************************
class CFunctionLib
{
public:    // 誰でもアクセス可能
	CFunctionLib();
	~CFunctionLib();

	static char *ReadLine(FILE *pFile, char *pDest);
	static int Memcmp(char *pSrc, char *pComp);
	static char *GetLineTop(char *pSrc);
	static int PopString(char *pSrc, char *pDest);
	static char *HeadPutout(char *pDest, char *pHead);
	static int ReadInt(char *pSrc, char *pHead);
	static float ReadFloat(char *pSrc, char *pHead);
	static char *ReadString(char *pSrc, char *pDest, char *pHead);
	static bool ReadBool(char *pSrc, char *pHead);
	static D3DXVECTOR3 ReadVector3(char *pSrc, char *pHead);
	static D3DXCOLOR ReadVector4(char *pSrc, char *pHead);
	static char *WriteInt(int nData, char *pDest);
	static char *WriteFloat(float fData, char *pDest);
	static char *WriteFloat3(float *pData, char *pDest);
	static float RotationVectorX(float fRot, float X, float Y);
	static float RotationVectorY(float fRot, float X, float Y);
	static float Vec2Cross(D3DXVECTOR2* Vec1, D3DXVECTOR2* Vec2);
	static float Vec2Length(float fValueX1, float fValueY1, float fValueX2, float fValueY2);
	static float Vec3Length(D3DXVECTOR3* Vec1, D3DXVECTOR3* Vec2);
	static D3DXVECTOR3* Vec3Direction(D3DXVECTOR3* Dest, D3DXVECTOR3* Start, D3DXVECTOR3* End);
	static float CalcAngleToDest(float fStartX, float fStartY, float fDestX, float fDestY);
	static D3DXVECTOR3 *CalcReflectVector3(D3DXVECTOR3 *Out, D3DXVECTOR3 *AdvanceVec, D3DXVECTOR3 *Normal);
	static D3DXVECTOR3* CalcRadianToDegree(D3DXVECTOR3* Dest);
	static D3DXVECTOR3* CalcDegreeToRadian(D3DXVECTOR3* Dest);
	static D3DXVECTOR3 *CheckRotationPi(D3DXVECTOR3 *Dest);
	static float CheckRotationOverPiX(float X);
	static float CheckRotationOverPiY(float Y);
	static float CheckRotationOverPiZ(float Z);
	static float CheckRotationOverMinusPiX(float X);
	static float CheckRotationOverMinusPiY(float Y);
	static float CheckRotationOverMinusPiZ(float Z);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};


//*****************************************************************************
//    ファイル保存クラスの定義
//*****************************************************************************
class CFileSaver
{
public:    // 誰でもアクセス可能
	CFileSaver(char *pSaveFileName);
	~CFileSaver();

	static CFileSaver *Create(char *pSaveFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Print(char *fmt, ...);

	void SetSavefileName(char *pFileName);
	char *GetSavefileName(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	FILE *m_pFile;               // ファイルポインタ
	char m_aSaveFileName[256];   // 保存するファイル名
};


//*****************************************************************************
//    ファイル読み込みクラスの定義
//*****************************************************************************
class CFileLoader
{
public:    // 誰でもアクセス可能
	CFileLoader(char *pLoadFileName);
	~CFileLoader();

	static CFileLoader *Create(char *pLoadFileName);

	HRESULT Init(void);
	void Uninit(void);
	char *GetString(char *pSrc);

	void SetLoadfileName(char *pFileName);
	char *GetLoadfileName(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	FILE *m_pFile;               // ファイルポインタ
	char m_aLoadFileName[256];   // 読み込むファイル名
};

#endif