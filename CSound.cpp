#include "CSound.h"

//�ݽ�׸�.
clsSound::clsSound()
{
	ZeroMemory( this, sizeof( clsSound ) );

}

//�޽�׸�.
clsSound::~clsSound()
{
}

//�����ݒ�֐�.
void clsSound::SetInitParam( char *sAlias, HWND hWnd )
{
	//����޳����ق�ݒ�.
	m_hWnd = hWnd;

	//��ر���ݒ�.
	strcpy_s( m_sAlias, sizeof( m_sAlias ), sAlias );
}

//����̧�ق��J��.
bool clsSound::Open( LPCTSTR sFName, char* sAlias, HWND hWnd )
{
	//�����ݒ�.
	SetInitParam( sAlias, hWnd );

	//�����.
	char cmd[STR_BUFF_MAX] = "open ";

	strcat_s( cmd, sizeof( cmd ), sFName );		//̧�ٖ���A��.
	strcat_s( cmd, sizeof( cmd ), " alias " );
	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//��ر���A��.

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//����̧�ق����.
bool clsSound::Close()
{
	//�����.
	char cmd[STR_BUFF_MAX] = "close ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );		//��ر���A��.

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//�Đ�.
bool clsSound::Play( bool bNotify )
{
	//�����.
	char cmd[STR_BUFF_MAX] = "play ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//��ر���A��.
	if( bNotify == true ){
		strcat_s( cmd, sizeof( cmd ), " notify" );
		//notify:�������I���������� hWnd �� MM_MCINOTIFY ү���ނ��߽Ă���.
	}

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//��~.
bool clsSound::Stop()
{
	//�����.
	char cmd[STR_BUFF_MAX] = "stop ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//��ر���A��.

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//�Ȃ̏�Ԃ��擾����֐�.
// ����Ԃ��擾����ꍇ�́A�Đ����Ɂunotify�v��ݒ肵�A
//   ����޳����ق�ү���ނ𑗂��Ă����K�v������.
bool clsSound::GetStatus( char *sStatus )
{
	//�����.
	char cmd[STR_BUFF_MAX] = "status ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//��ر���A��.
	strcat_s( cmd, sizeof( cmd ), " mode" );

	// STR_BUFF_MAX�́AsStatus�̕�����.
	if( mciSendString( cmd, sStatus, STR_BUFF_MAX, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//���̒�~���m�F����֐�.
bool clsSound::IsStopped()
{
	char sStatus[STR_BUFF_MAX] = "";

	//��Ԃ��擾.
	if( GetStatus( sStatus ) ){
		//�������r.
		if( strcmp( sStatus, "stopped" ) == 0 ){
			return true;
		}
	}
	return false;
}

//���̍Đ������m�F����֐�.
bool clsSound::IsPlaying()
{
	char sStatus[STR_BUFF_MAX] = "";

	//��Ԃ��擾.
	if( GetStatus( sStatus ) ){
		//�������r.
		if( strcmp( sStatus, "playing" ) == 0 ){
			return true;
		}
	}
	return false;
}

//�Đ��ʒu���ŏ��ɖ߂��֐�.
bool clsSound::SeekToStart()
{
	//�����.
	char cmd[STR_BUFF_MAX] = "seek ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//��ر���A��.
	strcat_s( cmd, sizeof( cmd ), " to start" );

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}