#ifndef CGROUNDMANAGER_H
#define CGROUNDMANAGER_H
#include "CMeshObject.h"

const int	GROUND_MAX = 6;		//地面の最大数.
class CGround
	:public CMeshObject
{
public:
	CGround();
	~CGround();

	//地面の表示位置を取得.
	D3DXVECTOR3 GetGroundPos(int i) {
		return m_GroundPos[i];
	}
private:
	//情報を更新.
	void Update() final override;
	//モデルデータの情報を管理.
	void MeshCreate() final override;

	D3DXVECTOR3	m_GroundPos[GROUND_MAX];	//地面の座標位置.
};
#endif //#ifndef CGROUNDMANAGER_H