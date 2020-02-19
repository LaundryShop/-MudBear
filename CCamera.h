#ifndef CCAMERA_H
#define CCAMERA_H
#include "Global.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	D3DXVECTOR3 vPos;	//ˆÊ’u.
	D3DXVECTOR3 vLook;	//’‹ˆÊ’u.
	float		fYaw;	//Y²‰ñ“].
	D3DXMATRIX	mRot;	//‰ñ“]s—ñ.
private:

};
#endif //#ifndef CCAMERA_H

