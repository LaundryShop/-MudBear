#include "CCamera.h"

CCamera::CCamera()
{
	//��ׂ̐ݒ�.
	vPos = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	vLook = D3DXVECTOR3(0.0f, 1.0f, 6.0f);
}

CCamera::~CCamera()
{
}
