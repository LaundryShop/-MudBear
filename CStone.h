#ifndef CSTONE_H
#define CSTONE_H
#include "CMeshObject.h"
#include "CExcelData.h"

const int STONE_MAX = 8;	//石の最大数
class CStone
	:public CMeshObject
{
public:
	CStone();
	~CStone();

	//地面の表示位置を取得.
	D3DXVECTOR3 GetStonePos(int i) {
		return m_StonePos[i];
	}
private:
	//情報を更新.
	void Update() final override;
	//モデルデータの情報を管理.
	void MeshCreate() final override;

	D3DXVECTOR3	m_StonePos[STONE_MAX];	//石の座標位置.

	CExcelData*		m_pExcelData;
};
#endif //#ifndef CSTONE_H