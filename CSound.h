#include <Windows.h>

//ײ���ؓǍ�.
#pragma comment( lib, "winmm.lib" )

//�������ޯ̧.
#define STR_BUFF_MAX	256


/**********************************************************
*	����޸׽.
*		midi,mp3,wav�`����̧�ق̍Đ��E��~�����s��.
**/
class clsSound
{
public:
	clsSound();		//�ݽ�׸�.
	~clsSound();	//�޽�׸�.

	//����̧�ق��J��.
	bool Open( LPCTSTR sFName, char* sAlias, HWND hWnd );

	//����̧�ق����.
	bool Close();

	//�Đ�.
	bool Play( bool bNotify=false );

	//��~.
	bool Stop();

	//��Ԃ��擾����֐�.
	bool GetStatus( char *sStatus );

	//���̒�~���m�F����֐�.
	bool IsStopped();

	//���̍Đ������m�F����֐�.
	bool IsPlaying();

	//�Đ��ʒu���ŏ��ɖ߂��֐�.
	bool SeekToStart();

private:
	HWND	m_hWnd;	//����޳�����.
	char	m_sAlias[STR_BUFF_MAX];//��ر�.

	//�����ݒ�֐�.
	void SetInitParam( char *sAlias, HWND hWnd );
};
