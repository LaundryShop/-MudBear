#ifndef CEXCELDATA_H
#define CEXCELDATA_H
#include "Global.h"

const int DATA_MAX = 10;
class CExcelData
{
public:
	CExcelData();
	~CExcelData();

	//メッシュオブジェクトをデータ読み込み.
	void	MeshObjectData(const char* text, D3DXVECTOR3 m_Pos[DATA_MAX]);
private:
	//パラメータ.
	enum class enPARAM_DATA
	{
		NONE = -1,

		enPOSITION_X,		//x座標
		enPOSITION_Y,		//y座標
		enPOSITION_Z,		//z座標

		enMAX
	};
	int		DataCnt;		//データカウント(Excelデータに必要).
};
#endif //#ifndef CEXCELDATA_H
