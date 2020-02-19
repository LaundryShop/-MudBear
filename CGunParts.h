#ifndef CGUNPARTS_H
#define CGUNPARTS_H
#include "CMeshObject.h"

class CGunParts
	:public CMeshObject
{
public:
	CGunParts();
	~CGunParts();
	//初期化.
	void Init();

	//移動.
	void Move(bool m_Moveflag);
private:
	//情報を更新.
	void Update() final override;
	//モデルデータの情報を管理.
	void MeshCreate() final override;

};
#endif //#ifndef CGUNPARTS_H

