#include	"CExcelData.h"

#include <iostream>
#include <fstream>	//入力はifstream、出力はofstreamに必要.
#include <string>	//std::getline関数を使うために必要.
#include <sstream>	//これは必要.
#include <vector>	//これは必要.
using namespace std;

CExcelData::CExcelData()
	: DataCnt(0)
{
}

CExcelData::~CExcelData()
{
}

void CExcelData::MeshObjectData(const char* text, D3DXVECTOR3 m_Pos[DATA_MAX])
{
	//ファイルを開く.
	fstream	filestream(text);
	if (!filestream.is_open()) {
		_ASSERT_EXPR(false, L"ファイル読み込み失敗");
		return;
	}

	bool	isNotSkip = false;
	//ファイルの読み込み.
	while (!filestream.eof())
	{
		string	buffer;
		filestream >> buffer;
		if (!isNotSkip) {
			isNotSkip = true;
			continue;	//一行目をスキップ.
		}
		//ファイルから読み込
		vector<string>	record;
		istringstream	streambuffer(buffer);
		string			token;
		enPARAM_DATA enGrassLineNo = enPARAM_DATA::enPOSITION_X;
		while (getline(streambuffer, token, ','))
		{
			record.push_back(token);
			switch (enGrassLineNo)
			{
			case CExcelData::enPARAM_DATA::enPOSITION_X:
				m_Pos[DataCnt].x = static_cast<float>(atof(token.c_str()));
				enGrassLineNo = enPARAM_DATA::enPOSITION_Y;
				break;
			case CExcelData::enPARAM_DATA::enPOSITION_Y:
				m_Pos[DataCnt].y = static_cast<float>(atof(token.c_str()));
				enGrassLineNo = enPARAM_DATA::enPOSITION_Z;
				break;
			case CExcelData::enPARAM_DATA::enPOSITION_Z:
				m_Pos[DataCnt].z = static_cast<float>(atof(token.c_str()));
				enGrassLineNo = enPARAM_DATA::NONE;
				break;
			case CExcelData::enPARAM_DATA::enMAX:
				break;
			default:
				break;
			}
		}
		if (enGrassLineNo == enPARAM_DATA::NONE) {
			DataCnt++;
		}
	}
}
