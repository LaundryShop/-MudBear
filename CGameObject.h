#ifndef CGAME_OBJECT_H
#define CGAME_OBJECT_H

#include "Global.h"

/**************************************************
*	�ްѵ�޼ު�ĸ׽.
**/
class CGameObject
{
public:
	CGameObject();
	//virtual ���z�֐�.
	virtual ~CGameObject();

	void SetPosition(const D3DXVECTOR3& Pos) {	m_Pos = Pos;	}	//�ʒu����ݒ�.
	void SetRotation(const D3DXVECTOR3& Rot) {	m_Rot = Rot; 	}	//��]����ݒ�.
	void SetScale(const float Scale){	m_fScale = Scale;		}	//�g�k����ݒ�.
	D3DXVECTOR3 GetPosition() { return m_Pos; }		//�ʒu�����擾.
	D3DXVECTOR3 GetRotation() { return m_Rot; }		//��]�����擾.
	float	GetScale() {	return m_fScale;	  }		//�g�k�����擾.

protected:
	D3DXVECTOR3	m_Pos;		//�ʒu���.
	D3DXVECTOR3	m_Rot;		//��]���.
	float	m_fScale;	//�g�k���.
};

#endif//#ifndef CGAME_OBJECT_H