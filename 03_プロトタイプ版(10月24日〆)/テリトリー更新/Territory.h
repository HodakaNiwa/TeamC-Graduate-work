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
// �O���錾
//=============================================================================
class CPlayer;

//=============================================================================
// X�t�@�C���N���X
//=============================================================================
class CTerritory : public CSceneX
{
public:
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_START,	//�n�_
		STATE_PASS,		//�ʉߓ_
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
	static int GetCountObj(void) { return m_nCountObj; }
	static int GetMaxObj(void) { return m_nMaxObj; }
	static int GetMaxTex(void) { return m_nMaxTex; }
	static int GetMaxModel(void) { return m_nMaxModel; }
	static void Save(HWND hWnd);
	static void SetCountObj(int nCntObj) { m_nCountObj = nCntObj; }
	static void SetAddMaxObj(void) { m_nMaxObj++; }
	static void SetDownMaxObj(void) { m_nMaxObj--; }
	static CTerritory * RequestPointer(int nCountObj);
	void SetTexNum(int nTex) { m_nTex = nTex; }
	void SetModelType(int ModelType) { m_nModelType = ModelType; }
	int GetTex(void) { return m_nTex; }
	int GetModelType(void) { return m_nModelType; }
	int GetPoint(void) { return m_nPoint; }
	void SetPoint(int nPoint) { m_nPoint = nPoint; }

protected:
	//�֐�
	void UpdateNormal(CPlayer * pPlayer, int nCntPlayer);
	void UpdateStart(CPlayer * pPlayer, int nCntPlayer);
	void UpdatePass(CPlayer * pPlayer, int nCntPlayer);
	void SetColor(D3DXCOLOR Color);
	void ResetTerritory(CTerritory * pTop);
	bool ChackList(CTerritory * pTop, int * nCountTerritory);
	void ChackIn(D3DXVECTOR3 * pPos, int nMaxPos, int nNumPlayer);
	CTerritory * DestoroyTerritoryStart(void);
	CTerritory * DestoroyTerritory(void);

	//�ϐ�
	int  m_nTex;			//�e�N�X�`���ԍ�
	int  m_nModelType;		//���f���ԍ�
	int  m_nPoint;			//�|�C���g
	int  m_nNumPlayer;		//�v���C���[�ԍ�
	int  m_nOldNumPlayer;	//�O��̃v���C���[�ԍ�
	float m_fLength;		//����
	bool m_bGetTerritory;	//�e���g���[�̎擾���
	D3DXCOLOR m_Color;		//�F
	D3DXCOLOR m_OldColor;	//�O��̐F
	STATE m_State;			//���
	CTerritory * m_pPrev;	//�O�̃e���g���[�ւ̃|�C���^
	CTerritory * m_pNext;	//���̃e���g���[�ւ̃|�C���^

private:
	//�֐��錾
	void CreateCollider(void);

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
	static D3DXCOLOR m_CountryColor[CPlayer::TYPE_MAX];	//���̐F
};
#endif