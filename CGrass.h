#ifndef CGRASS_H
#define CGRASS_H
#include "CMeshObject.h"
#include "CExcelData.h"

const int GRASS_MAX = 12;			//草の最大数

class CGrass
	:public CMeshObject
{
public:
	CGrass();
	~CGrass();

	//地面の表示位置を取得.
	D3DXVECTOR3 GetGrassPos(int i) {
		return m_GrassPos[i];
	}
private:
	//情報を更新.
	void Update() final override;
	//モデルデータの情報を管理.
	void MeshCreate() final override;

	D3DXVECTOR3	m_GrassPos[GRASS_MAX];	//石の座標位置.

	CExcelData*		m_pExcelData;
};
#endif //#ifndef CGRASS_H