#ifndef CGRASS_H
#define CGRASS_H
#include "CMeshObject.h"
#include "CExcelData.h"

const int GRASS_MAX = 12;			//���̍ő吔

class CGrass
	:public CMeshObject
{
public:
	CGrass();
	~CGrass();

	//�n�ʂ̕\���ʒu���擾.
	D3DXVECTOR3 GetGrassPos(int i) {
		return m_GrassPos[i];
	}
private:
	//�����X�V.
	void Update() final override;
	//���f���f�[�^�̏����Ǘ�.
	void MeshCreate() final override;

	D3DXVECTOR3	m_GrassPos[GRASS_MAX];	//�΂̍��W�ʒu.

	CExcelData*		m_pExcelData;
};
#endif //#ifndef CGRASS_H