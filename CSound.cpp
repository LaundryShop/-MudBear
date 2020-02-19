#include "CSound.h"

//ｺﾝｽﾄﾗｸﾀ.
clsSound::clsSound()
{
	ZeroMemory( this, sizeof( clsSound ) );

}

//ﾃﾞｽﾄﾗｸﾀ.
clsSound::~clsSound()
{
}

//初期設定関数.
void clsSound::SetInitParam( char *sAlias, HWND hWnd )
{
	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙを設定.
	m_hWnd = hWnd;

	//ｴｲﾘｱｽを設定.
	strcpy_s( m_sAlias, sizeof( m_sAlias ), sAlias );
}

//音声ﾌｧｲﾙを開く.
bool clsSound::Open( LPCTSTR sFName, char* sAlias, HWND hWnd )
{
	//初期設定.
	SetInitParam( sAlias, hWnd );

	//ｺﾏﾝﾄﾞ.
	char cmd[STR_BUFF_MAX] = "open ";

	strcat_s( cmd, sizeof( cmd ), sFName );		//ﾌｧｲﾙ名を連結.
	strcat_s( cmd, sizeof( cmd ), " alias " );
	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//ｴｲﾘｱｽを連結.

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//音声ﾌｧｲﾙを閉じる.
bool clsSound::Close()
{
	//ｺﾏﾝﾄﾞ.
	char cmd[STR_BUFF_MAX] = "close ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );		//ｴｲﾘｱｽを連結.

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//再生.
bool clsSound::Play( bool bNotify )
{
	//ｺﾏﾝﾄﾞ.
	char cmd[STR_BUFF_MAX] = "play ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//ｴｲﾘｱｽを連結.
	if( bNotify == true ){
		strcat_s( cmd, sizeof( cmd ), " notify" );
		//notify:処理が終了した時に hWnd に MM_MCINOTIFY ﾒｯｾｰｼﾞをﾎﾟｽﾄする.
	}

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//停止.
bool clsSound::Stop()
{
	//ｺﾏﾝﾄﾞ.
	char cmd[STR_BUFF_MAX] = "stop ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//ｴｲﾘｱｽを連結.

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//曲の状態を取得する関数.
// ※状態を取得する場合は、再生時に「notify」を設定し、
//   ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙにﾒｯｾｰｼﾞを送っておく必要がある.
bool clsSound::GetStatus( char *sStatus )
{
	//ｺﾏﾝﾄﾞ.
	char cmd[STR_BUFF_MAX] = "status ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//ｴｲﾘｱｽを連結.
	strcat_s( cmd, sizeof( cmd ), " mode" );

	// STR_BUFF_MAXは、sStatusの文字列数.
	if( mciSendString( cmd, sStatus, STR_BUFF_MAX, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//音の停止を確認する関数.
bool clsSound::IsStopped()
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態を取得.
	if( GetStatus( sStatus ) ){
		//文字列比較.
		if( strcmp( sStatus, "stopped" ) == 0 ){
			return true;
		}
	}
	return false;
}

//音の再生中を確認する関数.
bool clsSound::IsPlaying()
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態を取得.
	if( GetStatus( sStatus ) ){
		//文字列比較.
		if( strcmp( sStatus, "playing" ) == 0 ){
			return true;
		}
	}
	return false;
}

//再生位置を最初に戻す関数.
bool clsSound::SeekToStart()
{
	//ｺﾏﾝﾄﾞ.
	char cmd[STR_BUFF_MAX] = "seek ";

	strcat_s( cmd, sizeof( cmd ), m_sAlias );	//ｴｲﾘｱｽを連結.
	strcat_s( cmd, sizeof( cmd ), " to start" );

	if( mciSendString( cmd, NULL, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}