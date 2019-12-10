//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//					AIコントローラー処理 [AIController.h]
//				Author : Kobayashi_Sho-hei / 小林 将兵
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
#ifndef _AICONTROLLER_H_
#define _AICONTROLLER_H_

#include "main.h"

class CAIController
{
public:

	//	---<<コンストラクタ・デストラクタ>>---
	CAIController();
	~CAIController();

	//	---<<各処理>>---
	void AIManual01();
	void AIManual02();

	//	2点間の距離を求める
	static float dist(D3DXVECTOR3 target, D3DXVECTOR3 owner);	//	dist(拠点 , エネミー)

private:
};
#endif