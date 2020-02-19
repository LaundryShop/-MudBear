#ifndef CEXCELDATA_H
#define CEXCELDATA_H
#include "Global.h"

const int DATA_MAX = 10;
class CExcelData
{
public:
	CExcelData();
	~CExcelData();

	//���b�V���I�u�W�F�N�g���f�[�^�ǂݍ���.
	void	MeshObjectData(const char* text, D3DXVECTOR3 m_Pos[DATA_MAX]);
private:
	//�p�����[�^.
	enum class enPARAM_DATA
	{
		NONE = -1,

		enPOSITION_X,		//x���W
		enPOSITION_Y,		//y���W
		enPOSITION_Z,		//z���W

		enMAX
	};
	int		DataCnt;		//�f�[�^�J�E���g(Excel�f�[�^�ɕK�v).
};
#endif //#ifndef CEXCELDATA_H
