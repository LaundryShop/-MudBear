#ifndef CSTONE_H
#define CSTONE_H
#include "CMeshObject.h"
#include "CExcelData.h"

const int STONE_MAX = 8;	//�΂̍ő吔
class CStone
	:public CMeshObject
{
public:
	CStone();
	~CStone();

	//�n�ʂ̕\���ʒu���擾.
	D3DXVECTOR3 GetStonePos(int i) {
		return m_StonePos[i];
	}
private:
	//�����X�V.
	void Update() final override;
	//���f���f�[�^�̏����Ǘ�.
	void MeshCreate() final override;

	D3DXVECTOR3	m_StonePos[STONE_MAX];	//�΂̍��W�ʒu.

	CExcelData*		m_pExcelData;
};
#endif //#ifndef CSTONE_H