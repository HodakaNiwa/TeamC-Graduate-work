//=============================================================================
//
// �I�u�W�F�N�g�̏��� [scene.h]
// Author : ���� �����@[ SATO ASUMI ]
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "renderer.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_LAYER	(8)

//=============================================================================
// �I�u�W�F�N�g�N���X
//=============================================================================
class CScene
{
public:
	//�I�u�W�F�N�g�̎��
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_SHADOW,
		OBJTYPE_FLOR,
		OBJTYPE_SKY,
		OBJTYPE_OBJE,
		OBJTYPE_BILLBORAD,
		OBJTYPE_BULLET,
		OBJTYPE_ORBIT,
		OBJTYPE_PARTICLE,
		OBJTYPE_EXPROTION,
		OBJTYPE_FADE,
		OBJTYPE_UI,
		OBJTYPE_BOTTUN,
		OBJTYPE_SCORE,
		OBJTYPE_DAMEGE,
		OBJTYPE_COMBO,
		OBJTYPE_WATER,
		OBJTYPE_GAUGE,
		OBJTYPE_STAR,
		OBJTYPE_BUTTERFLY,
		OBJTYPE_NUMBER2D,
		OBJTYPE_HART,
		OBJTYPE_SURPRISEMARK,
		OBJTYPE_ARROW,
		OBJTYPE_2DPOLYGON
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE ObjType = OBJTYPE_NONE);
	~CScene();

	//�����o�֐�
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Set(const D3DXVECTOR3 pos, const D3DXVECTOR3 size) = 0;

	static void ReleaseAll(void);									//�S�������̊J��
	static void ReleaseFade(void);									//�t�F�[�h�ȊO���J��
	static void UpdateAll(void);									//�S�čX�V����
	static void DrawAll(void);										//�S�ĕ\������
	static CScene * GetTop(int nLayer) { return m_pTop[nLayer]; };	//�擪�̃I�u�W�F�N�g�|�C���^�̎擾
	static CScene * GetCur(int nLayer) { return m_pCur[nLayer]; };	//���݁i�Ō���j�̃I�u�W�F�N�g�|�C���^�̎擾
	OBJTYPE GetObjType(void) { return m_ObjType; };					//�I�u�W�F�N�g�^�C�v�̎擾
	CScene * GetpNext(void) { return m_pNext; };					//���̃I�u�W�F�N�g�|�C���^�̎擾
	static void SetPause(bool bPause) { m_bPause = bPause; };		//�|�[�Y�̐ݒ�
	bool GetDraw(void) { return m_bDraw; }							//�`���Ԃ̎擾
		
protected:
	void Release(void);
	void SetDraw(bool bDraw) { m_bDraw = bDraw; };					//�`�悷�邩�ǂ���
	void Deth(int nCntLayer);


private:
	static CScene * m_pTop[MAX_LAYER];		//�擪�I�u�W�F�N�g�ւ̃|�C���^
	static CScene * m_pCur[MAX_LAYER];		//���݂̃I�u�W�F�N�g�ւ̃|�C���^�i�Ō���j
	CScene * m_pPrev;						//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene * m_pNext;						//���̃I�u�W�F�N�g�ւ̃|�C���^

	//�ÓI�����o�ϐ�
	static int		m_nNumAll;				//�z��̃J�E���^

	//�f�o�b�N�p�ϐ�
	static int		m_anNumOBJ[MAX_LAYER];	//�e���C���[�̃I�u�W�F�N�g��

	//�����o�ϐ�
	bool m_bDeth;			//���S�t���O
	bool m_bDraw;			//�`�悷�邩�ǂ���
	static bool m_bPause;	//�|�[�Y��Ԃɂ���
	static int m_nCountDraw;
	int m_nID;				//�g�pID

	D3DXVECTOR3	m_pos;		// �|���S���̈ʒu
	D3DXVECTOR3	m_Other;	// �|���S���̑Ίp��
	OBJTYPE m_ObjType;		//�I�u�W�F�N�g�̃^�C�v
};
#endif