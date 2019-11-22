//=============================================================================
//
// デバッグウインドウ処理 [GUI.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _DEBUGWINDOE_H_
#define _DEBUGWINDOE_H_

#include "main.h"
#include "GUI.h"

//=============================================================================
// デバッグウインドウクラス
//=============================================================================
class CDebugWindow : public CGui
{
public:
	CDebugWindow();
	~CDebugWindow();
	void Init(void);
	void Uninit(void);
	void Update(void);
	static CDebugWindow * Create(void);
};
#endif