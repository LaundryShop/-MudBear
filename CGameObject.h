#ifndef CGAME_OBJECT_H
#define CGAME_OBJECT_H

#include "Global.h"

/**************************************************
*	╧ч╟я╣лч╪ч╙╦д╦в╫.
**/
class CGameObject
{
public:
	CGameObject();
	//virtual ┴╪▒z┼ж░■.
	virtual ~CGameObject();

	void SetPosition(const D3DXVECTOR3& Pos) {	m_Pos = Pos;	}	//┬й▓u▐Н∙Я┌П░щ▓Х.
	void SetRotation(const D3DXVECTOR3& Rot) {	m_Rot = Rot; 	}	//┴Я⌠]▐Н∙Я┌П░щ▓Х.
	void SetScale(const float Scale){	m_fScale = Scale;		}	//┼g▐k▐Н∙Я┌П░щ▓Х.
	D3DXVECTOR3 GetPosition() { return m_Pos; }		//┬й▓u▐Н∙Я┌П▌Ф⌠╬.
	D3DXVECTOR3 GetRotation() { return m_Rot; }		//┴Я⌠]▐Н∙Я┌П▌Ф⌠╬.
	float	GetScale() {	return m_fScale;	  }		//┼g▐k▐Н∙Я┌П▌Ф⌠╬.

protected:
	D3DXVECTOR3	m_Pos;		//┬й▓u▐Н∙Я.
	D3DXVECTOR3	m_Rot;		//┴Я⌠]▐Н∙Я.
	float	m_fScale;	//┼g▐k▐Н∙Я.
};

#endif//#ifndef CGAME_OBJECT_H