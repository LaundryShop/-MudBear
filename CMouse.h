#ifndef CMOUSE_H
#define CMOUSE_H

#include "Global.h"
const float CURSOR_SIZE = 128.0f;	//�J�[�\���̉摜�T�C�Y.

class CMouse
{
public:
	CMouse();
	~CMouse();
	//�}�E�X���Z�b�g.
	void SetMouse(HWND	m_hWnd);
	
	//��`�Ɠ_(�}�E�X�J�[�\��)�̔�����Ƃ�.
	bool RecAndPos(
		float iDx, float iDy,
		float iDw, float iDh);

	//�_(�}�E�X�J�[�\��)�Ɖ~�̔�����Ƃ�.
	bool PointAndCircle(float iDx, float iDy, float m_Radius);

	// �g�c�ύX
	// �X�N���[�����W�����[���h���W�ɕϊ�.
	D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* Pos, float Sx,  // �X�N���[��X���W.
		float Sy,  // �X�N���[��Y���W.
		float fZ,// �ˉe��Ԃł�Z�l�i0�`1�j.
		D3DXMATRIX* View, D3DXMATRIX* Prj);

	//�J�[�\���̈ʒu���X���擾.
	float	GetCursorPosX() {
		return	m_CursorPos.x;
	}
	//�J�[�\���̈ʒu���Y���擾.
	float	GetCursorPosY() {
		return	m_CursorPos.y;
	}

	//�}�E�X�̈ʒu���X���擾.
	long	GetMousePosX() {
		return	m_MousePos.x;
	}
	//�}�E�X�̈ʒu���Y���擾.
	long	GetMousePosY() {
		return	m_MousePos.y;
	}
private:
	D3DXVECTOR3	m_CursorPos;	//�J�[�\���ʒu.
	POINT		m_MousePos;		//�}�E�X�̈ʒu���.
};
#endif //#ifndef CMOUSE_H

