#ifndef CHIDETEX_H
#define CHIDETEX_H

#include "CSprite.h"
#include "MyMacro.h"



class CHideTex
{
public:

	CHideTex();
	~CHideTex();

	//�ǂݍ��ݏ����֐�.
	void Load(D3DXMATRIX mView, D3DXMATRIX mProj, D3DXVECTOR3 vCamera);	

	//�`�揈���֐�.
	void Render();		

	//�B���e�N�X�`�������֐��֐�.
	void HideTex_Disp(D3DXVECTOR3 Pos);		

	//�A�j���[�V�����J�E���g�֐�.
	void Move();											
	
	//��������֐�.
	void Release();													

	//�l���֐�.
	bool GetHideTexFlag() { return m_HideTexFlag; }
	D3DXVECTOR2 GetUVPos() { return m_UV; }

	//�u���֐�
	void Attach(CSprite* Sprite) { m_pHideTex = Sprite; }

	void Init();

private:
	D3DXMATRIX	m_mView;
	D3DXMATRIX	m_mProj;
	D3DXVECTOR3 m_vCamera;
	CSprite*	m_pHideTex;		//�B���e�N�X�`��.
	D3DXVECTOR3 m_InitPos;				//�ʒu.
	D3DXVECTOR3 m_Distance;
	D3DXVECTOR3	m_Pos;
	bool		m_HideTexFlag;	//�B���t���O.
	int						m_AnimCount;			//�J�E���^.
	D3DXVECTOR2				m_UV;					//�e�N�X�`��UV���W.




};



#endif	//CHIDETEX_H
