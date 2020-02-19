#ifndef CPUMP_H
#define CPUMP_H
#include "CMeshObject.h"

const float PUMP_SPD = 0.05f;		//空気いれ速度.
const float MAXGAUGE = 1.0f;		//ゲージバー最大数.
const float MINIMUMGAUGE = 0.01f;	//ゲージバー最小数.

class CPump
	:public CMeshObject
{
public:
	CPump();
	~CPump();
	//初期化.
	void Init();

	//移動.
	void Move(bool m_Moveflag);

	//チャージ(ゲージを回復する).
	void Charge(D3DXVECTOR3 m_GunPos);

	//リロードを設定.
	void SetReload() {
		m_ReloadFlag = true;
	}
	//水鉄砲ゲージを設定する.
	void SetGauge(float m_Gauge) {
		m_GaugeBarCnt = m_Gauge;
	}
	//水鉄砲ゲージが減少していく.
	float DecrementGauge() {
		return m_GaugeBarCnt -= 0.01f;
	}
	//水鉄砲ゲージを取得.
	float GetGauge() {
		return m_GaugeBarCnt;
	}
private:
	//情報を更新.
	void Update() final override;
	//モデルデータの情報を管理.
	void MeshCreate() final override;

	bool	m_ReloadFlag;	//リロード.
	bool	m_ReturnFlag;	//ポンプが手前に引いて戻る処理で使用するフラグ.
	float	m_GaugeBarCnt;	 //ゲージを減らしたり増やしたりする.
};
#endif //#ifndef CPUMP_H