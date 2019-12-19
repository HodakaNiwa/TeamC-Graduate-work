//=============================================================================
//
// �e���g���[���� [Territory.h]
// Author : �������� Sato_Asumi
//
//=============================================================================
#ifndef _TERRITORY_H_
#define _TERRITORY_H_

#include "main.h"
#include "sceneX.h"
#include "loadText.h"
#include "Player.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TERRITORY_PRIORITY (3)
#define HIGHT_DIVISION	(2)		//�c�̕�����
#define WIDTH_DIVISION	(2)		//���̕�����
#define MAX_EREAPOS		((HIGHT_DIVISION + 1) * (WIDTH_DIVISION + 1))	//�G���A���W��

//=============================================================================
// �O���錾
//=============================================================================
class CPlayer;
class CLoadEffect;
class CRing;

//=============================================================================
// X�t�@�C���N���X
//=============================================================================
class CTerritory : public CSceneX
{
public:
	typedef enum
	{
		STATE_GET,
		STATE_NONE,
	}STATE;		

	CTerritory();
	CTerritory(int nPriority, OBJTYPE objtype);
	~CTerritory();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTerritory * Create(const D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nTexNum, int ModelNum, int nPoint);
	
	static void LoadTex(CLoadTextTerritory::TEX_INFO TexInfo);
	static void UnloadTex(void);
	static void LoadModel(CLoadTextTerritory::OBJ_INFO ObjInfo);
	static void UnloadModel(void);
	static LPD3DXMESH *GetMesh(void) { return m_pMesh; }
	static LPDIRECT3DTEXTURE9 *GetTexture(void) { return m_ppTexture; }
	static int GetCountObj(void) { return m_nCountObj; }
	static int GetMaxObj(void) { return m_nMaxObj; }
	static int GetMaxTex(void) { return m_nMaxTex; }
	static int GetMaxModel(void) { return m_nMaxModel; }
	static void Save(HWND hWnd);
	static void SetCountObj(int nCntObj) { m_nCountObj = nCntObj; }
	static void SetAddMaxObj(void) { m_nMaxObj++; }
	static void SetDownMaxObj(void) { m_nMaxObj--; }
	static void SetErea(void);
	static CTerritory * RequestPointer(int nCountObj);
	static int RequestErea(D3DXVECTOR3 Pos);
	static void ResetColorTerritory(void);	
	void SetTexNum(int nTex) { m_nTex = nTex; }
	void SetModelType(int ModelType) { m_nModelType = ModelType; }
	int GetTex(void) { return m_nTex; }
	int GetModelType(void) { return m_nModelType; }
	int GetPoint(void) { return m_nPoint; }
	void SetPoint(int nPoint) { m_nPoint = nPoint; }
	void SetColor(D3DXCOLOR col) { m_Color = col; }
	void SetPlayerNumber(int nNumPlayer);
	int GetPlayerNumber(void) { return m_nNumPlayer; }
	void CreateGetEffect(void);
	int GetErea(void) { return m_nErea; }
	D3DXCOLOR GetColor(void) { return m_Color; }
	void SetState(STATE state) { m_state = state; }	

protected:
	void SetDiffuseColor(D3DXCOLOR Color);
	//�ϐ�
	int  m_nTex;			//�e�N�X�`���ԍ�
	int  m_nModelType;		//���f���ԍ�
	int  m_nPoint;			//�|�C���g
	int  m_nNumPlayer;		//�v���C���[�ԍ�
	int  m_nOldNumPlayer;	//�O��̃v���C���[�ԍ�
	int  m_nErea;			//�G���A�̔ԍ�
	float m_fLength;		//����
	bool m_bGetTerritory;	//�e���g���[�̎擾���
	D3DXCOLOR m_Color;		//�F
	D3DXCOLOR m_OldColor;	//�O��̐F
	CRing **m_ppRing;		//����̃����O

private:
	//�֐��錾
	void CreateCollider(void);
	void CreateRing(void);
	void ReleaseRing(void);
	void UpdateGetRing(void);	
	void CreateShadow(void);

	//�ϐ��錾
	static LPDIRECT3DTEXTURE9 *  m_ppTexture;			// �e�N�X�`���ւ̃|�C���^
	static LPD3DXMESH *			 m_pMesh;				//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER *		 m_pBuffMat;			//�}�e���A�����ւ̃|�C���^
	static DWORD *				 m_pNumMat;				//�}�e���A�����̐�
	static D3DXVECTOR3 *		 m_pVtxMin;				//���f���ŏ��l
	static D3DXVECTOR3 *		 m_pVtxMax;				//���f���ő�l
	static int m_nMaxTex;								//�e�N�X�`���̍ő吔
	static int m_nMaxModel;								//���f���̍ő吔
	static int m_nCountObj;								//�I�u�W�F�N�g�̃J�E���^�[
	static int m_nMaxObj;								//�I�u�W�F�N�g�̍ő吔
	static D3DXCOLOR m_DefaultColor;					//�f�t�H���g�̐F
	static D3DXVECTOR2 m_pEreaPos[MAX_EREAPOS];			//�G���A�̍��W��
	CLoadEffect * m_pLoadEffect;
	STATE m_state;										//�����O�̏�Ԑݒ�
	int m_nCountTime;									//���Ԃ̉��Z�@�@�@�@
	int m_nRingColIndx;									//�����O�̐F�̃C���f�b�N�X�@
	bool * m_pbChangeColRing;							//�����O�̐F�ύX
};
#endif