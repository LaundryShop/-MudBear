#ifndef CBACKGROUNDMANAGER_H
#define CBACKGROUNDMANAGER_H
#include "CMeshObject.h"

const float BACKGROUND_MOVING_MAX = 86.0f;	//�ړ�����.
class CBackground
	:public CMeshObject
{
public:
	CBackground();
	~CBackground();

	void Init();			//������.
	void Move(bool m_flag);	//�v���C���[�ƈꏏ�Ɉړ�.
private:
	//�����X�V.
	//���g�k���]�A���W�Ȃǂ̒l�������ް��ɔ��f.
	void Update() final override;
	//���f���f�[�^�̏����Ǘ�.
	void MeshCreate() final override;
};
#endif //#ifndef CBACKGROUNDMANAGER_H
