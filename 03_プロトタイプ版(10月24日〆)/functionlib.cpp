//*****************************************************************************
//
//     ���ʂŎg���֐��̏���[functionlib.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "library.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CFunctionLib::CFunctionLib()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CFunctionLib::~CFunctionLib()
{

}

//=============================================================================
//    �e�L�X�g�����񕪕������ǂݐi�߂鏈��
//=============================================================================
char *CFunctionLib::ReadLine(FILE *pFile, char *pDest)
{
	// �������ǂݐi�߂�
	fgets(pDest, 256, pFile);
	while (1)
	{// �L���ȍs��������܂Ń��[�v
		if (*pDest == *STRING_COMMENT)
		{// �擪�̕������L���ȕ�����ł͂Ȃ�
			fgets(pDest, 256, pFile);
		}
		else if (*pDest == *STRING_ENTER)
		{// �擪�̕��������s�R�[�h��������
			fgets(pDest, 256, pFile);
		}
		else if (*pDest == *STRING_SPACE)
		{// �擪�̕������󔒍s��������
			pDest = GetLineTop(pDest);
		}
		else if (*pDest == *STRING_TAB)
		{// �擪�̕�����Tab��������
			pDest = GetLineTop(pDest);
		}
		else
		{// �L���ȕ����񂪔������ꂽ
			break;
		}
	}

	return pDest;  // ������̃A�h���X��Ԃ�
}

//=============================================================================
//    �w��̕����񂪓���������ł��邩��r���鏈��
//=============================================================================
int CFunctionLib::Memcmp(char *pSrc, char *pComp)
{
	return memcmp(pSrc, pComp, strlen(pComp));  // 0�Ȃ瓯��������ł���Ɣ���
}

//=============================================================================
//    tab��X�y�[�X�������������ĕ������擪�Ɏ����Ă�������
//=============================================================================
char *CFunctionLib::GetLineTop(char *pSrc)
{
	while (1)
	{// �L���ȕ������o�Ă���܂ŌJ��Ԃ�
		if (*pSrc == *STRING_SPACE)
		{// �擪�̕������󔒍s��������
			pSrc++;
		}
		else if (*pSrc == *STRING_TAB)
		{// �擪�̕�����Tab��������
			pSrc += strlen(STRING_TAB);
		}
		else
		{// �Ⴄ�����񂪏o�Ă���
			break;
		}
	}

	return pSrc;
}

//=============================================================================
//    �ǂݎ���������╶�����ǂꂭ�炢���̂���Ԃ�����
//=============================================================================
int CFunctionLib::PopString(char *pSrc, char *pDest)
{
	int nCntWord = 0;   // �ǂݎ��Ɏז��ȕ�����
	char aStr[256];     // ������ۑ��p

	strcpy(aStr, pSrc); // �������ۑ����Ă���
	while (1)
	{// �����񂪐擪�ɗ���悤�ɕԂ�
		nCntWord++;   // �����������Z����
		pSrc++;
		if (*pSrc == *STRING_SPACE || *pSrc == *STRING_TAB || *pSrc == *STRING_ENTER)
		{// �󔒂��^�u���擪�ɂ���
			nCntWord++;                      // SPACE�̕����l�����ĕ��������Z
			break;
		}
	}
	return nCntWord;
}

//=============================================================================
//    �擪�̕����������ē��o�������鏈��
//=============================================================================
char *CFunctionLib::HeadPutout(char *pDest, char *pHead)
{
	pDest = GetLineTop(pDest);  // �����ȕ���������
	pDest += strlen(pHead);     // �f�[�^�������ɕs�v�ȕ����������
	return pDest;
}

//=============================================================================
//    ������𐮐��^�ɕϊ����ĕԂ�����
//=============================================================================
int CFunctionLib::ReadInt(char *pSrc, char *pHead)
{
	int nData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	nData = atoi(pSrc);
	return nData;
}

//=============================================================================
//    ������𕂓������_�^�ɕϊ����ĕԂ�����
//=============================================================================
float CFunctionLib::ReadFloat(char *pSrc, char *pHead)
{
	float fData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	fData = (float)atof(pSrc);
	return fData;
}

//=============================================================================
//    �������K�v�ȕ������������o���ĕԂ�����
//=============================================================================
char *CFunctionLib::ReadString(char *pSrc, char *pDest, char *pHead)
{
	int nWord = 0;
	// ������𔲂��o��
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	nWord = CFunctionLib::PopString(pSrc, pDest);
	strcpy(pDest, "\0");
	strcpy(pDest, pSrc);
	strcpy(&pDest[nWord - 1], "\0");

	return pDest;
}

//=============================================================================
//    �������Bool�^�ɕϊ����ĕԂ�����
//=============================================================================
bool CFunctionLib::ReadBool(char *pSrc, char *pHead)
{
	bool bBool = false;
	int nBool = CFunctionLib::ReadInt(pSrc, pHead);
	if (nBool == 1)
	{// ���[�v����ԍ�������
		bBool = true;
	}
	else
	{// ���[�v���Ȃ��ԍ�������
		bBool = false;
	}

	return bBool;
}

//=============================================================================
//    ��������R�����x�N�g���^�ɕϊ����ĕԂ�����
//=============================================================================
D3DXVECTOR3 CFunctionLib::ReadVector3(char *pSrc, char *pHead)
{
	int nWord = 0;
	char aStr[256];
	D3DXVECTOR3 Vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// X
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	Vector.x = (float)atof(pSrc);

	// Y
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Vector.y = (float)atof(pSrc);

	// Z
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Vector.z = (float)atof(pSrc);

	return Vector;
}

//=============================================================================
//    ��������S�����x�N�g���^�ɕϊ����ĕԂ�����
//=============================================================================
D3DXCOLOR CFunctionLib::ReadVector4(char *pSrc, char *pHead)
{
	int nWord = 0;
	char aStr[256];

	D3DXCOLOR Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	// r
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	Color.r = (float)atof(pSrc);

	// g
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.g = (float)atof(pSrc);

	// b
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.b = (float)atof(pSrc);

	// a
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.a = (float)atof(pSrc);

	return Color;
}

//=============================================================================
//    int�^�̃f�[�^����char�^�̃f�[�^��Ԃ�����
//=============================================================================
char *CFunctionLib::WriteInt(int nData, char *pDest)
{
	sprintf(pDest, "%d", nData);
	return pDest;
}

//=============================================================================
//    float�^�̃f�[�^����char�^�̃f�[�^��Ԃ�����
//=============================================================================
char *CFunctionLib::WriteFloat(float fData, char *pDest)
{
	sprintf(pDest, "%.1f", fData);
	return pDest;
}

//=============================================================================
//    float3�^�̃f�[�^����char�^�̃f�[�^��Ԃ�����
//=============================================================================
char *CFunctionLib::WriteFloat3(float *pData, char *pDest)
{
	sprintf(pDest, "%.1f %.1f %.1f", pData[0], pData[1], pData[2]);
	return pDest;
}

//=============================================================================
//    �x�N�g������]�����鏈��(X����)
//=============================================================================
float CFunctionLib::RotationVectorX(float fRot, float X, float Y)
{
	return cosf(fRot) * X - sinf(fRot) * Y;
}

//=============================================================================
//    �x�N�g������]�����鏈��(Y����)
//=============================================================================
float CFunctionLib::RotationVectorY(float fRot, float X, float Y)
{
	return sinf(fRot) * X + cosf(fRot) * Y;
}

//=============================================================================
//    2�̃x�N�g���̊O�ς��o��
//=============================================================================
float CFunctionLib::Vec2Cross(D3DXVECTOR2* Vec1, D3DXVECTOR2* Vec2)
{
	return Vec1->x * Vec2->y - Vec1->y * Vec2->x;
}

//=============================================================================
//    �������o��
//=============================================================================
float CFunctionLib::Vec2Length(float fValueX1, float fValueY1, float fValueX2, float fValueY2)
{
	return sqrtf((fValueX1 - fValueX2) * (fValueX1 - fValueX2) +
		(fValueY1 - fValueY2) * (fValueY1 - fValueY2));
}

//=============================================================================
//    2�̃x�N�g���̋������o��
//=============================================================================
float CFunctionLib::Vec3Length(D3DXVECTOR3* Vec1, D3DXVECTOR3* Vec2)
{
	return sqrtf((Vec1->x - Vec2->x) * (Vec1->x - Vec2->x) +
		(Vec1->y - Vec2->y) * (Vec1->y - Vec2->y) +
		(Vec1->z - Vec2->z) * (Vec1->z - Vec2->z));
}

//=============================================================================
//    �n�_���猩���ړI�n�܂ł̕������o��
//=============================================================================
D3DXVECTOR3* CFunctionLib::Vec3Direction(D3DXVECTOR3* Dest, D3DXVECTOR3* Start, D3DXVECTOR3* End)
{
	Dest->x = End->x - Start->x;
	Dest->y = End->y - Start->y;
	Dest->z = End->z - Start->z;
	return Dest;
}

//=============================================================================
//    �n�_���猩���ړI�n�ւ̊p�x���v�Z����
//=============================================================================
float CFunctionLib::CalcAngleToDest(float fStartX, float fStartY, float fDestX, float fDestY)
{
	return atan2f(fDestX - fStartX, fDestY - fStartY);
}

//=============================================================================
//    �~�������Z����p�x���Z�Ɍv�Z���鏈��
//=============================================================================
D3DXVECTOR3* CFunctionLib::CalcRadianToDegree(D3DXVECTOR3* Dest)
{
	Dest->x = D3DXToDegree(Dest->x);
	Dest->y = D3DXToDegree(Dest->y);
	Dest->z = D3DXToDegree(Dest->z);
	return Dest;
}

//=============================================================================
//    �p�x���Z����~�������Z�Ɍv�Z���鏈��
//=============================================================================
D3DXVECTOR3* CFunctionLib::CalcDegreeToRadian(D3DXVECTOR3* Dest)
{
	Dest->x = D3DXToRadian(Dest->x);
	Dest->y = D3DXToRadian(Dest->y);
	Dest->z = D3DXToRadian(Dest->z);
	return Dest;
}

//=============================================================================
//    ���˃x�N�g�����Z�o���鏈��
//=============================================================================
D3DXVECTOR3 *CFunctionLib::CalcReflectVector3(D3DXVECTOR3 *Out, D3DXVECTOR3 *AdvanceVec, D3DXVECTOR3 *Normal)
{
	D3DXVECTOR3 Normal_n;
	D3DXVec3Normalize(&Normal_n, Normal);
	*Out = *AdvanceVec - (2.0f * D3DXVec3Dot(AdvanceVec, &Normal_n) * Normal_n);
	return Out;
}

//=============================================================================
//    �������~�����𒴂��Ă��Ȃ����`�F�b�N����
//=============================================================================
D3DXVECTOR3 *CFunctionLib::CheckRotationPi(D3DXVECTOR3 *Dest)
{
	// X����
	Dest->x = CheckRotationOverPiX(Dest->x);
	Dest->x = CheckRotationOverMinusPiX(Dest->x);

	// Y����
	Dest->y = CheckRotationOverPiY(Dest->y);
	Dest->y = CheckRotationOverMinusPiY(Dest->y);

	// Z����
	Dest->z = CheckRotationOverPiZ(Dest->z);
	Dest->z = CheckRotationOverMinusPiZ(Dest->z);

	return Dest;
}

//=============================================================================
//    �������~�����𒴂��Ă��Ȃ����`�F�b�N����(X����)
//=============================================================================
float CFunctionLib::CheckRotationOverPiX(float X)
{
	if (X > D3DX_PI)
	{
		X -= D3DX_PI * 2.0f;
	}

	return X;
}

//=============================================================================
//    �������~�����𒴂��Ă��Ȃ����`�F�b�N����(Y����)
//=============================================================================
float CFunctionLib::CheckRotationOverPiY(float Y)
{
	if (Y > D3DX_PI)
	{
		Y -= D3DX_PI * 2.0f;
	}

	return Y;
}

//=============================================================================
//    �������~�����𒴂��Ă��Ȃ����`�F�b�N����(Z����)
//=============================================================================
float CFunctionLib::CheckRotationOverPiZ(float Z)
{
	if (Z > D3DX_PI)
	{
		Z -= D3DX_PI * 2.0f;
	}

	return Z;
}

//=============================================================================
//    �������~�����𒴂��Ă��Ȃ����`�F�b�N����(X����, �}�C�i�XVer)
//=============================================================================
float CFunctionLib::CheckRotationOverMinusPiX(float X)
{
	if (X < -D3DX_PI)
	{
		X += D3DX_PI * 2.0f;
	}

	return X;
}

//=============================================================================
//    �������~�����𒴂��Ă��Ȃ����`�F�b�N����(Y����, �}�C�i�XVer)
//=============================================================================
float CFunctionLib::CheckRotationOverMinusPiY(float Y)
{
	if (Y < -D3DX_PI)
	{
		Y += D3DX_PI * 2.0f;
	}

	return Y;
}

//=============================================================================
//    �������~�����𒴂��Ă��Ȃ����`�F�b�N����(Z����, �}�C�i�XVer)
//=============================================================================
float CFunctionLib::CheckRotationOverMinusPiZ(float Z)
{
	if (Z < -D3DX_PI)
	{
		Z += D3DX_PI * 2.0f;
	}

	return Z;
}