#ifndef CGROUNDMANAGER_H
#define CGROUNDMANAGER_H
#include "CMeshObject.h"

const int	GROUND_MAX = 6;		//�n�ʂ̍ő吔.
class CGround
	:public CMeshObject
{
public:
	CGround();
	~CGround();

	//�n�ʂ̕\���ʒu���擾.
	D3DXVECTOR3 GetGroundPos(int i) {
		return m_GroundPos[i];
	}
private:
	//�����X�V.
	void Update() final override;
	//���f���f�[�^�̏����Ǘ�.
	void MeshCreate() final override;

	D3DXVECTOR3	m_GroundPos[GROUND_MAX];	//�n�ʂ̍��W�ʒu.
};
#endif //#ifndef CGROUNDMANAGER_H