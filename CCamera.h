#ifndef CCAMERA_H
#define CCAMERA_H
#include "Global.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	D3DXVECTOR3 vPos;	//�ʒu.
	D3DXVECTOR3 vLook;	//�����ʒu.
	float		fYaw;	//Y����].
	D3DXMATRIX	mRot;	//��]�s��.
private:

};
#endif //#ifndef CCAMERA_H

