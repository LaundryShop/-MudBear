#include "CMouse.h"

CMouse::CMouse()
{
}

CMouse::~CMouse()
{
}

void CMouse::SetMouse(HWND	m_hWnd)
{
	//�}�E�X�J�[�\���ʒu�擾.
	GetCursorPos(&m_MousePos);
	//�X�N���[�����W���N���C�A���g���W�ɕϊ�.
	ScreenToClient(m_hWnd, //�E�B���h�E�n���h��.
		&m_MousePos);//(out)POINT�\����.
	//�}�E�X���W���}�b�v�ʒu.
	//�}�E�X���W���N���C�A���g�̈�ɓ����Ă���.
	(0 <= m_MousePos.x) && (m_MousePos.x <= WND_W);
	(0 <= m_MousePos.y) && (m_MousePos.y <= WND_H);
	//---------------------------------------------------

	//---------------------------------------------------
	//	2019_7_19�X�V(�g�c).
	//�J�[�\���̒���(�^�񒆂ɂ���).
	m_CursorPos.x = m_MousePos.x - CURSOR_SIZE / 2 + 8;
	m_CursorPos.y = m_MousePos.y - CURSOR_SIZE / 2 + 22;
}

//��`�Ɠ_(�}�E�X�J�[�\��)�̔�����Ƃ�
bool CMouse::RecAndPos(
	float iDx, float iDy,	//��`�z�u���Ă鍶�[X | ��`�z�u���Ă�Y.
	float iDw, float iDh)	//��`�̕�,����.
{
	if ((m_MousePos.x >= iDx && m_MousePos.x <= iDx + iDw) &&
		(m_MousePos.y >= iDy && m_MousePos.y <= iDy + iDh))
	{
		return true;
	}
	return false;
}

//�_(�}�E�X�J�[�\��)�Ɖ~�̔�����Ƃ�.
bool CMouse::PointAndCircle(float iDx, float iDy, float m_Radius)
{
	float A = m_MousePos.x - (iDx - 10.0f);		//-10.0f�̓}�E�X�̈ʒu������Ă��邽�߂ɒ��߂��Ă���.
	float B = m_MousePos.y - (iDy - 40.0f);		//-40.0f�̓}�E�X�̈ʒu������Ă��邽�߂ɒ��߂��Ă���.
	float C = sqrtf(A * A + B * B);

	if (C <= m_Radius) {
		return true;
	}
	return false;
}

// �X�N���[�����W�����[���h���W�ɕϊ�.
D3DXVECTOR3 * CMouse::CalcScreenToWorld(
	D3DXVECTOR3 * Pos,
	float Sx, float Sy, float fZ,
	D3DXMATRIX * View, D3DXMATRIX * Prj)
{
	// �e�s��̋t�s����Z�o.
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = WND_W / 2.0f; VP._22 = -WND_H / 2.0f;
	VP._41 = WND_W / 2.0f; VP._42 = WND_H / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�.
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(Pos, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return Pos;
}
