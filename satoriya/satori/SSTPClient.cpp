#include "../_/stltool.h"
#include "SSTPClient.h"
#include "SakuraFMO.h"

// �C�x���g�𑗂�z�Ƃ�
// http://futaba.sakura.st/sstp.html#notify11


extern bool readSakuraFMO(map<string, strmap>& oSakuraFMOMap);

bool direct_sstp(
	const string& i_script,
	const string& i_client_name,
	HWND i_client_window)
{
	SakuraFMO	fmo;
	if ( !fmo.update() )
	{
		return	false;
	}

	string request = string() +
		"SEND SSTP/1.1" + CRLF +
		"Script: " + i_script + CRLF +
		"Sender: " + i_client_name + CRLF +
		"HWnd: " + itos((int)i_client_window) + CRLF +
		"Charset: Shift_JIS" + CRLF +
		CRLF;

	COPYDATASTRUCT cds;
	cds.dwData = 9801; // �ł����̂���
	cds.cbData = request.size();
	cds.lpData = (LPVOID)request.c_str();

	for( map<string, strmap>::iterator i=fmo.begin() ; i!=fmo.end() ; ++i )
	{
		HWND host_window = (HWND)atoi(i->second["hwnd"].c_str());
		if ( host_window == NULL )
		{
			continue;
		}

		::SendMessage(host_window, WM_COPYDATA, (WPARAM)i_client_window, (LPARAM)&cds);
	}
	return true;
}
