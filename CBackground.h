#ifndef CBACKGROUNDMANAGER_H
#define CBACKGROUNDMANAGER_H
#include "CMeshObject.h"

const float BACKGROUND_MOVING_MAX = 86.0f;	//移動距離.
class CBackground
	:public CMeshObject
{
public:
	CBackground();
	~CBackground();

	void Init();			//初期化.
	void Move(bool m_flag);	//プレイヤーと一緒に移動.
private:
	//情報を更新.
	//※拡縮や回転、座標などの値をﾓﾃﾞﾙﾃﾞｰﾀに反映.
	void Update() final override;
	//モデルデータの情報を管理.
	void MeshCreate() final override;
};
#endif //#ifndef CBACKGROUNDMANAGER_H
