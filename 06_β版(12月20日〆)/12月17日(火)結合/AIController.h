//@@@@@@@@@@@@@@@@@
//					AIRg[[ [AIController.h]
//				Author : Kobayashi_Sho-hei / ¬Ñ «º
//@@@@@@@@@@@@@@@@@
#ifndef _AICONTROLLER_H_
#define _AICONTROLLER_H_

#include "main.h"

class CAIController
{
public:

	//	---<<RXgN^EfXgN^>>---
	CAIController();
	~CAIController();

	//	---<<e>>---
	void AIManual01();
	void AIManual02();

	//	2_ÔÌ£ðßé
	static float dist(D3DXVECTOR3 target, D3DXVECTOR3 owner);	//	dist(_ , Gl~[)

private:
};
#endif