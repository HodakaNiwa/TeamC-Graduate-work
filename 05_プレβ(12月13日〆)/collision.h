//*****************************************************************************
//
//     �R���W�����̏���[collision.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "meshSphere.h"

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CScene;

//*****************************************************************************
//    �R���C�_�[�N���X�̒�`
//*****************************************************************************
class CCollider
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �R���C�_�[�̎��
	//------------------------
	typedef enum
	{
		TYPE_NONE = -1,            // ����
		TYPE_SQUARE2D,             // ��`(2D)
		TYPE_CIRCLE2D,             // �~�`(2D)
		TYPE_MESH,                 // ���b�V��
		TYPE_BOX,                  // ��
		TYPE_SPHERE,               // ��
		TYPE_CYLINDER,             // ��
		TYPE_SPHERE_PLAYERATTACK,  // �v���C���[�̍U����
		TYPE_SPHERE_ENEMYATTACK,   // �G�̍U����
		TYPE_MAX
	}TYPE;

	CCollider(TYPE type = TYPE_NONE, CScene *pParent = NULL);
	virtual ~CCollider();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);
	static void DeathCheck(void);
	static void SwicthDraw(void);
	static bool GetDrawAll(void);
	static CCollider *GetTop(int nType);

	void SetTransformToDevice(const LPDIRECT3DDEVICE9 pDevice);
	void CalcNotScaleTransform(void);
	void SetParent(CScene *pParent);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetRot(const D3DXVECTOR3 rot);
	void SetScale(const D3DXVECTOR3 scale);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetParentMtxWorld(D3DXMATRIX *pParentMtxWorld);
	void SetDisp(const bool bDisp);

	CScene *GetParent(void);
	CCollider *GetNext(void);
	CCollider *GetPrev(void);
	TYPE GetType(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetScale(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXMATRIX *GetParentMtxWorld(void);
	bool GetDisp(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void Release(void);

private:   // ���̃N���X�������A�N�Z�X�\
	static bool      m_bDrawAll;           // �S�ẴR���C�_�[��`�悷�邩�ǂ���
	static CCollider *m_apTop[TYPE_MAX];   // �擪�̃R���C�_�[�N���X�ւ̃|�C���^
	static CCollider *m_apCur[TYPE_MAX];   // ���݂̍Ō���|�C���^�ۑ��p

	CScene      *m_pParent;          // �R���C�_�[���������Ă���I�u�W�F�N�g�ւ̃|�C���^
	CCollider   *m_pNext;            // ���̃R���C�_�[�ւ̃|�C���^
	CCollider   *m_pPrev;            // �O�̃R���C�_�[�ւ̃|�C���^
	TYPE        m_Type;              // �R���C�_�[�̎��
	bool        m_bDeath;            // ���S�t���O
	D3DXVECTOR3 m_Pos;               // ���݂̈ʒu(�������̓��[�J���ʒu)
	D3DXVECTOR3 m_PosOld;            // �O��̈ʒu
	D3DXVECTOR3 m_Rot;               // ����
	D3DXVECTOR3 m_Scale;             // �傫��
	D3DXMATRIX  m_MtxWorld;          // ���[���h�}�g���b�N�X
	D3DXMATRIX  *m_pParentMtxWorld;  // �e�̃��[���h�}�g���b�N�X�ւ̃|�C���^
	bool        m_bDisp;             // �`�悷�邩�ǂ���
};

//*****************************************************************************
//    ��`(2D)�R���C�_�[�N���X�̒�`
//*****************************************************************************
class CSquareCollider2D : public CCollider
{
public:    // �N�ł��A�N�Z�X�\
	CSquareCollider2D(TYPE type = TYPE_SQUARE2D, CScene *pParent = NULL);
	~CSquareCollider2D();

	static CSquareCollider2D *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, CScene *pParent = NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);

	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);

	float GetWidth(void);
	float GetHeight(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fWidth;    // ��`�̕�
	float m_fHeight;   // ��`�̍���
};

//*****************************************************************************
//    �~�`(2D)�R���C�_�[�N���X�̒�`
//*****************************************************************************
class CCircleCollider2D : public CCollider
{
public:    // �N�ł��A�N�Z�X�\
	CCircleCollider2D(TYPE type = TYPE_CIRCLE2D, CScene *pParent = NULL);
	~CCircleCollider2D();

	static CCircleCollider2D *Create(D3DXVECTOR3 pos, float fRadius, CScene *pParent = NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fRadius);

	void SetRadius(const float fRadius);

	float GetRadius(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fRadius;    // �~�̔��a
};

//*****************************************************************************
//    �{�b�N�X�R���C�_�[�N���X�̒�`
//*****************************************************************************
class CBoxCollider : public CCollider
{
public:    // �N�ł��A�N�Z�X�\
	CBoxCollider(TYPE type = TYPE_BOX, CScene *pParent = NULL);
	~CBoxCollider();

	static CBoxCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fWidth, float fHeight, float fDepth, bool bReturnFlag);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, CScene *pScene, bool *pLand = NULL);

	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetDepth(const float fDepth);
	void SetReturnFlag(const bool bReturnFlag);

	float GetWidth(void);
	float GetHeight(void);
	float GetDepth(void);
	bool GetReturnFlag(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DCOLOR col;
	}BOX_VERTEX;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;      // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;      // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	float                   m_fWidth;        // ���̉���
	float                   m_fHeight;       // ���̍���
	float                   m_fDepth;        // ���̉��s
	bool                    m_bReturnFlag;   // ���������ۂɖ߂����ǂ���
};

//*****************************************************************************
//    �����蔻������p�̃��b�V���X�t�B�A�N���X�̒�`
//*****************************************************************************
class CColDispSphere : public CMeshSphere
{
public:    // �N�ł��A�N�Z�X�\
	CColDispSphere(int nPriority = 3, OBJTYPE objType = OBJTYPE_COLDISPSPHERE);
	~CColDispSphere();

	static CColDispSphere *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Disp(void);
	void SetVtxBuffValue(void);
	void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) {};

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �X�t�B�A�R���C�_�[�N���X�̒�`
//*****************************************************************************
class CSphereCollider : public CCollider
{
public:    // �N�ł��A�N�Z�X�\
	CSphereCollider(TYPE type = TYPE_SPHERE, CScene *pParent = NULL);
	~CSphereCollider();

	static CSphereCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, bool bReturnFlag);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, float fColRadius, CScene *pScene);

	void SetRadius(const float fRadius);
	void SetReturnFlag(const bool bReturnFlag);
	void SetColDispSphere(CColDispSphere *pColDispSphere);

	float GetRadius(void);
	bool GetReturnFlag(void);
	CColDispSphere *GetColDispSphere(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	virtual void MakeColDispSphere(void);

private:   // ���̃N���X�������A�N�Z�X�\
	CColDispSphere *m_pColDispSphere;   // �����蔻������p���b�V���X�t�B�A�N���X�ւ̃|�C���^
	float          m_fRadius;           // ���̔��a
	bool           m_bReturnFlag;       // ���������ۂɖ߂����ǂ���
};

//*****************************************************************************
//    �V�����_�[�R���C�_�[�N���X�̒�`
//*****************************************************************************
class CCylinderCollider : public CCollider
{
public:    // �N�ł��A�N�Z�X�\
	CCylinderCollider(TYPE type = TYPE_CYLINDER, CScene *pParent = NULL);
	~CCylinderCollider();

	static CCylinderCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, float fHeight, bool bReturnFlag, CScene *pParent = NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand = NULL);

	void SetRadius(const float fRadius);
	void SetHeight(const float fHeight);
	void SetReturnFlag(const bool bReturnFlag);

	float GetRadius(void);
	float GetHeight(void);
	bool GetReturnFlag(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	bool LandCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand = NULL);
	bool RangeCollision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fColRadius, float fHeight, CScene *pScene, bool *pLand = NULL);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DCOLOR col;
	}CYLINDER_VERTEX;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;    // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;    // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	float           m_fRadius;             // ���̔��a
	float           m_fHeight;             // ���̍���
	bool            m_bReturnFlag;         // ���������ۂɖ߂����ǂ���
};

//*****************************************************************************
//    ���b�V���R���C�_�[�N���X�̒�`
//*****************************************************************************
class CMeshCollider : public CCollider
{
public:    // �N�ł��A�N�Z�X�\
	CMeshCollider(TYPE type = TYPE_MESH, CScene *pParent = NULL);
	~CMeshCollider();

	static CMeshCollider *Create(float fWidth, float fDepth, int nXBlock, int nZBlock, int nNumVertex, LPDIRECT3DVERTEXBUFFER9 pVtxBuffParent, D3DXMATRIX *pMtxParent, bool bReturnFlag, CScene *pParent = NULL);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetWorldVertexPos(void);

	void SetVerPos(D3DXVECTOR3 *pVerPos);
	void SetWidth(const float fWidth);
	void SetDepth(const float fDepth);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetVtxBuffParent(const LPDIRECT3DVERTEXBUFFER9 pVtxBuffParent);
	void SetReturnFlag(const bool bReturnFlag);

	D3DXVECTOR3 *GetVerPos(void);
	float GetWidth(void);
	float GetDepth(void);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetNumVertex(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuffParent(void);
	bool GetReturnFlag(void);

	float GetPolyHeight(D3DXVECTOR3 pos, bool *pLand = NULL, D3DXVECTOR3 *pPolyNormal = NULL, D3DXVECTOR3 *pVerPos = NULL);
	bool CheckPenetration(D3DXVECTOR3 pos, int *pXBlock = NULL, int *pYBlock = NULL, int *pLeftVerIndex = NULL);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3             *m_pVerPos;           // ���_���W�ւ̃|�C���^
	float                   m_fWidth;             // 1�u���b�N���̕�
	float                   m_fDepth;             // 1�u���b�N���̉��s
	int                     m_nXBlock;            // ���̕�����
	int                     m_nZBlock;            // ���s�̕�����
	int                     m_nNumVertex;         // ���_��
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffParent;     // ���_�o�b�t�@�ւ̃|�C���^
	bool                    m_bReturnFlag;        // �����������ɉ����߂����ǂ���
};


//*****************************************************************************
//    �v���C���[�̍U���p�X�t�B�A�R���C�_�[�N���X�̒�`
//****************************************************************************
class CPlayerAttackSphereCollider : public CSphereCollider
{
public:    // �N�ł��A�N�Z�X�\
	CPlayerAttackSphereCollider(TYPE type = TYPE_SPHERE_PLAYERATTACK, CScene *pParent = NULL);
	~CPlayerAttackSphereCollider();

	static CPlayerAttackSphereCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, int nLife, int nPower);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPower(const int nPower);
	void SetLife(const int nLife);

	int GetPower(void);
	int GetLife(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void MakeColDispSphere(void);

private:   // ���̃N���X�������A�N�Z�X�\
	int m_nPower;  // �U����
	int m_nLife;   // ����
};

//*****************************************************************************
//    �G�̍U���p�X�t�B�A�R���C�_�[�N���X�̒�`
//****************************************************************************
class CEnemyAttackSphereCollider : public CSphereCollider
{
public:    // �N�ł��A�N�Z�X�\
	CEnemyAttackSphereCollider(TYPE type = TYPE_SPHERE_ENEMYATTACK, CScene *pParent = NULL);
	~CEnemyAttackSphereCollider();

	static CEnemyAttackSphereCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 scale, float fRadius, int nLife, int nPower);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPower(const int nPower);
	void SetLife(const int nLife);

	int GetPower(void);
	int GetLife(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void MakeColDispSphere(void);

private:   // ���̃N���X�������A�N�Z�X�\
	int m_nPower;  // �U����
	int m_nLife;   // ����
};

//*****************************************************************************
//    �R���C�_�[�Ǌ��N���X�̒�`
//*****************************************************************************
class CColliderManager
{
public:    // �N�ł��A�N�Z�X�\
	CColliderManager();
	~CColliderManager();

	static CColliderManager *Create(int nNumCollider = 1);

	HRESULT Init(int nNumCollider = 1);
	void Uninit(void);

	void SetNumCollider(const int nNumCollider);
	void SetCollider(CCollider *pCollider, int nIdx);

	int GetNumCollider(void);
	CCollider **GetCollider(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void MakeMemoryCollider(void);
	void ReleaseCollider(void);

	int       m_nNumCollider; // �R���C�_�[�̐�
	CCollider **m_pCollider;  // �R���C�_�[�N���X�ւ̃|�C���^
};

#endif