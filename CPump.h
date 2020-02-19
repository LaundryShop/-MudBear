#ifndef CPUMP_H
#define CPUMP_H
#include "CMeshObject.h"

const float PUMP_SPD = 0.05f;		//��C���ꑬ�x.
const float MAXGAUGE = 1.0f;		//�Q�[�W�o�[�ő吔.
const float MINIMUMGAUGE = 0.01f;	//�Q�[�W�o�[�ŏ���.

class CPump
	:public CMeshObject
{
public:
	CPump();
	~CPump();
	//������.
	void Init();

	//�ړ�.
	void Move(bool m_Moveflag);

	//�`���[�W(�Q�[�W���񕜂���).
	void Charge(D3DXVECTOR3 m_GunPos);

	//�����[�h��ݒ�.
	void SetReload() {
		m_ReloadFlag = true;
	}
	//���S�C�Q�[�W��ݒ肷��.
	void SetGauge(float m_Gauge) {
		m_GaugeBarCnt = m_Gauge;
	}
	//���S�C�Q�[�W���������Ă���.
	float DecrementGauge() {
		return m_GaugeBarCnt -= 0.01f;
	}
	//���S�C�Q�[�W���擾.
	float GetGauge() {
		return m_GaugeBarCnt;
	}
private:
	//�����X�V.
	void Update() final override;
	//���f���f�[�^�̏����Ǘ�.
	void MeshCreate() final override;

	bool	m_ReloadFlag;	//�����[�h.
	bool	m_ReturnFlag;	//�|���v����O�Ɉ����Ė߂鏈���Ŏg�p����t���O.
	float	m_GaugeBarCnt;	 //�Q�[�W�����炵���葝�₵���肷��.
};
#endif //#ifndef CPUMP_H