#ifndef CGRASS_GROUND_H
#define CGRASS_GROUND_H
#include "CMeshObject.h"

const int	GRASS_GROUND_MAX = 8;	//���̒n�ʂ̍ő吔.
const int   GRASS_GROUND_MIDDLE = GRASS_GROUND_MAX / 2;	//���̒n�ʂ̒���.

class CGrassGround
	:public CMeshObject
{
public:
	CGrassGround();
	~CGrassGround();

	//���̒n�ʂ̕\���ʒu���擾.
	D3DXVECTOR3 GetGrassGroundPos(int i) {
		return m_GrassGround[i];
	}

private:
	//�����X�V.
	void Update() final override;
	//���f���f�[�^�̏����Ǘ�.
	void MeshCreate() final override;

	D3DXVECTOR3	m_GrassGround[GRASS_GROUND_MAX];
};
#endif //#ifndef CGRASS_GROUND_H
