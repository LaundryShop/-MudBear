#ifndef CGRASS_GROUND_H
#define CGRASS_GROUND_H
#include "CMeshObject.h"

const int	GRASS_GROUND_MAX = 8;	//草の地面の最大数.
const int   GRASS_GROUND_MIDDLE = GRASS_GROUND_MAX / 2;	//草の地面の中間.

class CGrassGround
	:public CMeshObject
{
public:
	CGrassGround();
	~CGrassGround();

	//草の地面の表示位置を取得.
	D3DXVECTOR3 GetGrassGroundPos(int i) {
		return m_GrassGround[i];
	}

private:
	//情報を更新.
	void Update() final override;
	//モデルデータの情報を管理.
	void MeshCreate() final override;

	D3DXVECTOR3	m_GrassGround[GRASS_GROUND_MAX];
};
#endif //#ifndef CGRASS_GROUND_H
