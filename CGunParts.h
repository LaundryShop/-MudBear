#ifndef CGUNPARTS_H
#define CGUNPARTS_H
#include "CMeshObject.h"

class CGunParts
	:public CMeshObject
{
public:
	CGunParts();
	~CGunParts();
	//������.
	void Init();

	//�ړ�.
	void Move(bool m_Moveflag);
private:
	//�����X�V.
	void Update() final override;
	//���f���f�[�^�̏����Ǘ�.
	void MeshCreate() final override;

};
#endif //#ifndef CGUNPARTS_H

