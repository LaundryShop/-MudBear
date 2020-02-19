#include	"CExcelData.h"

#include <iostream>
#include <fstream>	//���͂�ifstream�A�o�͂�ofstream�ɕK�v.
#include <string>	//std::getline�֐����g�����߂ɕK�v.
#include <sstream>	//����͕K�v.
#include <vector>	//����͕K�v.
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
	//�t�@�C�����J��.
	fstream	filestream(text);
	if (!filestream.is_open()) {
		_ASSERT_EXPR(false, L"�t�@�C���ǂݍ��ݎ��s");
		return;
	}

	bool	isNotSkip = false;
	//�t�@�C���̓ǂݍ���.
	while (!filestream.eof())
	{
		string	buffer;
		filestream >> buffer;
		if (!isNotSkip) {
			isNotSkip = true;
			continue;	//��s�ڂ��X�L�b�v.
		}
		//�t�@�C������ǂݍ�
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
