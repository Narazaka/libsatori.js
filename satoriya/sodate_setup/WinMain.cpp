#pragma comment(lib, "comctl32.lib")
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h> // for �R�����R���g���[�������� InitCommonControls()

// �^�u�ɓ\��t�����q�_�C�A���O�̃R�[���o�b�N�֐�
LRESULT CALLBACK TabSheetProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_INITDIALOG:
		return TRUE;	// TRUE�ɂ���Ƃ��Ƃ��~�\
	}
	return FALSE;	// ���Ƃ͑S���f�t�H���g�C��
}




#include	"../_/utilities.h"
#include	"../_/stltool.h"
#include	"../_/Dialog.h"
#include	"resource.h"

string	base_folder;	// ��ƃt�H���_��
strmap	conf;

static const char* conf_filename = "sodate.dat";
static const char byte_value_1[2] = {1,0};
static const char byte_value_2[2] = {2,0};
static const byte xor_magic = 186;


class UserDialog : public Dialog {
	bool	Save();
public:
	virtual	BOOL	OnInitDialog( HWND hwndFocus, LONG lInitParam );
	virtual	BOOL	OnCommand( WORD wNotifyCode, WORD wID, HWND hwndCrl );
};



BOOL	UserDialog::OnInitDialog( HWND hwndFocus, LONG lInitParam ) {

	// �R�����R���g���[���̏�����
	// �^�u�R���g���[�����g���ꍇ�͕K�{
	::InitCommonControls();
	{
		HWND tab = toHWND(IDC_TAB);
		TCITEM tc;
		tc.mask = TCIF_TEXT;
		tc.pszText = "�ݒ�";
		TabCtrl_InsertItem(tab, 0, &tc);
		//tc.mask = TCIF_TEXT;
		//tc.pszText = "���ӂ�";
		//TabCtrl_InsertItem(tab, 1, &tc);

		HWND sheet1 = 
			::CreateDialog(m_hInstance, (LPCTSTR) IDD_SHEET1, m_hDlg, (DLGPROC) TabSheetProc);

		RECT rc;
		::GetClientRect(tab, &rc);
		TabCtrl_AdjustRect(tab, FALSE, &rc);
		::MapWindowPoints(tab, m_hDlg, (POINT*)&rc, 2);
		::MoveWindow(sheet1, rc.left, rc.top,
			rc.right - rc.left, rc.bottom - rc.top, FALSE);
	
		// �f�t�H���g�ō����̃^�u��\��
		::ShowWindow(sheet1, SW_SHOW);
	}


	if ( strmap_from_file(conf, base_folder+"\\"+conf_filename, byte_value_1) )
	{
		// �ݒ�t�@�C���̓ǂݍ��݂ɐ���

		// \2��؂肾�������̂�\r\n�ɖ߂�
		replace( conf["allow_files"], byte_value_2, "\r\n");
		replace( conf["deny_files"], byte_value_2, "\r\n");

		// �p�X���[�h���f�R�[�h
		string	str = decode(conf["password"]);
		int	len = str.size()/2;
		byte*	buf = new byte[len+1];
		buf[len]='\0';
		string_to_binary(str, buf);
		xor_filter(buf, len, xor_magic);
		conf["password"] = decode( (char*)buf );
	}
	else
	{
		// �ݒ�t�@�C���̓ǂݍ��݂Ɏ��s�Aconf�Ƀf�t�H���g�l��ݒ肷��

		conf["is_upload"]="1";
		conf["host"]="127.0.0.1";
		conf["id"]="test";
		conf["password"]="1234";
		conf["ghostroot"]="homepage/myghost";
		conf["timediff"]="0";
		conf["yeardiff"]="0";
		conf["is_create_archive"]="1";
		//conf["archive_filename"]="myGhost%year%month%day_%hour%minute.nar";
		conf["archive_filename"]="myghost%year%month%day.nar";
		conf["archive_local_folder"]=base_folder;
		conf["allow_files"]=
			"readme.txt\r\n"
			"thumbnail.png\r\n"
			"ghost/master/*.txt\r\n"
			"ghost/master/*.dll\r\n"
			"ghost/master/*.ico\r\n"
			"shell/master/*.txt\r\n"
			"shell/master/*.png\r\n"
			"";
		conf["deny_files"]=
			"ghost/master/satori_savedata.txt\r\n"
			"ghost/master/satorite.txt\r\n"
			"ghost/master/nsconf.txt\r\n"
			"ghost/master/Dict-KEEPSs.txt\r\n"
			"ghost/master/Dict-Learned.txt\r\n"
			"ghost/master/misaka_vars.txt\r\n"
			"ghost/master/misaka_vars.txt\r\n"
			"ghost/master/aya_variable.cfg\r\n"
			"";
		conf["morelog"] = "0";
		conf["yeardiff"] = "0";
		conf["dog"] = "1";
		conf["is_passive"] = "0";
	}

	// conf�̓��e���_�C�A���O�ɐݒ�

	Check(IDC_IS_UPLOAD, conf["is_upload"]!="0");
	SetText(IDC_HOST, conf["host"]);
	SetText(IDC_ID, conf["id"]);
	SetText(IDC_PASSWORD, conf["password"]);
	SetText(IDC_GHOSTROOT, conf["ghostroot"]);
	SetText(IDC_TIMEDIFF, conf["timediff"]);
	Check(IDC_IS_PASSIVE, conf["is_passive"]=="1");
	Check(IDC_IS_CREATE_ARCHIVE, conf["is_create_archive"]!="0");
	SetText(IDC_ARCHIVE_FILENAME, conf["archive_filename"]);
	SetText(IDC_ARCHIVE_LOCAL_FOLDER, conf["archive_local_folder"]);
	SetText(IDC_ALLOW_FILES, conf["allow_files"]);
	SetText(IDC_DENY_FILES, conf["deny_files"]);


	BOOL	flag = isChecked(IDC_IS_UPLOAD) ? TRUE : FALSE;
	Enable(IDC_HOST, flag);
	Enable(IDC_ID, flag);
	Enable(IDC_PASSWORD, flag);
	Enable(IDC_GHOSTROOT, flag);
	Enable(IDC_TIMEDIFF, flag);
	Enable(IDC_IS_PASSIVE, flag);
	flag = isChecked(IDC_IS_CREATE_ARCHIVE) ? TRUE : FALSE;
	Enable(IDC_ARCHIVE_FILENAME, flag);
	Enable(IDC_ARCHIVE_LOCAL_FOLDER, flag);

	return	FALSE;
}

bool	UserDialog::Save()
{
	conf["is_upload"] = isChecked(IDC_IS_UPLOAD) ? "1" : "0";
	GetText(IDC_HOST, conf["host"]);
	GetText(IDC_ID, conf["id"]);
	GetText(IDC_PASSWORD, conf["password"]);
	GetText(IDC_GHOSTROOT, conf["ghostroot"]);
	GetText(IDC_TIMEDIFF, conf["timediff"]);
	conf["is_passive"] = isChecked(IDC_IS_PASSIVE) ? "1" : "0";
	conf["is_create_archive"] = isChecked(IDC_IS_CREATE_ARCHIVE) ? "1" : "0";
	GetText(IDC_ARCHIVE_FILENAME, conf["archive_filename"]);
	GetText(IDC_ARCHIVE_LOCAL_FOLDER, conf["archive_local_folder"]);
	GetText(IDC_ALLOW_FILES, conf["allow_files"]);
	GetText(IDC_DENY_FILES, conf["deny_files"]);

	// �ۑ��p��\r\n��\2��؂�ɕϊ�
	replace( conf["allow_files"], "\r\n", byte_value_2);
	replace( conf["deny_files"], "\r\n", byte_value_2);

	string	str = encode(conf["password"]);
	int	len = str.size();
	byte*	buf = new byte[len];
	memcpy(buf, str.c_str(), len);
	xor_filter(buf, len, xor_magic);
	conf["password"] = encode( binary_to_string(buf, len) );
	delete [] buf;

	// �t�@�C���ɕۑ�
	if ( !strmap_to_file(conf, base_folder+"\\"+conf_filename, byte_value_1) ) 
	{
		::MessageBox(m_hDlg,
			(base_folder+"\\"+conf_filename+"\n\n�ݒ�t�@�C�����ۑ��ł��܂���ł����B").c_str()
			,"error", MB_OK);
		return	false;
	}
	return	true;
}

BOOL	UserDialog::OnCommand( WORD wNotifyCode, WORD wID, HWND hwndCrl ) {
	switch (wID) {

	case	IDC_IS_UPLOAD:
		{
			BOOL	flag = isChecked(IDC_IS_UPLOAD) ? TRUE : FALSE;
			Enable(IDC_HOST, flag);
			Enable(IDC_ID, flag);
			Enable(IDC_PASSWORD, flag);
			Enable(IDC_GHOSTROOT, flag);
			Enable(IDC_TIMEDIFF, flag);
			Enable(IDC_IS_PASSIVE, flag);
		}
		break;

	case	IDC_IS_CREATE_ARCHIVE:
		{
			BOOL	flag = isChecked(IDC_IS_CREATE_ARCHIVE) ? TRUE : FALSE;
			Enable(IDC_ARCHIVE_FILENAME, flag);
			Enable(IDC_ARCHIVE_LOCAL_FOLDER, flag);
		}
		break;

	case	IDC_ARCHIVE_HELP:
		::MessageBox(m_hDlg,
			"�t�@�C�����̐ݒ�ɂ́A�ȉ��̂悤�ȃ^�O���g���܂��i���݂̓����ɒu���������܂��j�B\n"
			"\n"
			"%year\t�N\n"
			"%year4\t�N�i�S���j\n"
			"%month\t��\n"
			"%day\t��\n"
			"%hour\t��\n"
			"%minute\t��\n"
			"%second\t�b\n"
			"\n"
			"�g���q��nar�܂���zip���w�肵�Ă��������i���̂Q�̌`���́A���O���Ⴄ�����Œ��g�͓����ł��j�B\n"
			,"nar/zip�A�[�J�C�u�ɂ���", MB_OK);
		break;

	case	IDC_FTP_HELP:
		::MessageBox(m_hDlg,
			"�z�X�g���AID�A�p�X���[�h�́A���i�g���Ă���FTP�N���C�A���g�Ɠ����ݒ�ɂ��Ă��������B\n"
			"\n"
			"�u�S�[�X�g�̃��[�g�f�B���N�g���v�ɂ́AFTP�T�[�o�ɐڑ��������̃f�B���N�g������A�S�[�X�g�̍X�V�f�[�^��u���f�B���N�g���ւ̑��΃p�X���w�肵�Ă��������B\n"
			"\n"
			"�T�[�o�ɂ���Ă͎������w�肷��Ƃ��������B���Ԃ�B\n"
			"�t�@�C�A�E�H�[�����ɂ���ꍇ�Ȃǂ́uPASV���[�h�v�Ƀ`�F�b�N�������Ƃ��������B���Ԃ�B\n"
			,"FTP�A�b�v���[�h�ɂ���", MB_OK);
		break;

	case	IDC_DOG:
		conf["morelog"] = ( IDYES==::MessageBox(m_hDlg, "�o�߂��ׂ����\�����܂����H", "�ڂ����ݒ�", 
			MB_YESNO|((conf["morelog"]!="0")?0:MB_DEFBUTTON2)) ) ? "1" : "0";
		conf["yeardiff"] = ( IDYES==::MessageBox(m_hDlg, "�����[�g�������P�N�x��Ă��肵�܂����H", "�ڂ����ݒ�", 
			MB_YESNO|((conf["yeardiff"]!="0")?0:MB_DEFBUTTON2)) ) ? "1" : "0";
		conf["dog"] = ( IDYES==::MessageBox(m_hDlg, "���͍D���ł����H", "�ڂ����ݒ�", 
			MB_YESNO|((conf["dog"]!="0")?0:MB_DEFBUTTON2)) ) ? "1" : "0";
		break;


	case	IDC_TARGET_HELP:
		::MessageBox(m_hDlg,
			"�S�[�X�g�̃��[�g�t�H���_����̑��΃p�X���A���s�ŋ�؂��Ďw�肵�Ă��������B\n"
			"���C���h�J�[�h�i*.txt�A*.*�Ȃǁj���g���܂��B\n"
			"\n"
			"��i�Ŏw�肵���t�@�C���������A�b�v���[�h����܂��B\n"
			"��i�Ŏw�肵�������Ɋ܂܂�Ă��Ă��A���i�Ŏw�肳�ꂽ�ꍇ�͏��O����܂��B\n"
			"\n"
			"�Ȃ��Aupdates2.dau�͂����ł͎w�肵�Ȃ��ł��������B\n"
			"sodate.exe�������I�ɏ������܂��B\n"
			,"�Ώۃt�@�C���ɂ���", MB_OK);
		break;

	case IDC_ABOUT:
		if ( IDYES==::MessageBox(m_hDlg,
			"sodate\n"
			"\n"
			"���\t�����l�€\n"
			"�A����\thttp://www.geocities.co.jp/SiliconValley-Cupertino/8536/\n"
			"\n"
			"\n"
			"���ӌ�������΂��񂹂��������B\n"
			"���A��L�y�[�W���J���܂����H\n"
			,"about", MB_YESNO|MB_DEFBUTTON2 )) {

			::ShellExecute(NULL, NULL, "http://www.geocities.co.jp/SiliconValley-Cupertino/8536/", NULL, NULL, SW_SHOW);
		}
		break;

	case IDOK:
		if ( Save() )
			End(0);
		break;

	case IDCANCEL:
		End(0);
		break;

	default:
		return	FALSE;
	}
	return	TRUE;
}





int WINAPI
WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInst,
	LPSTR lpszCmdParam,
	int iShowCmd) 
{
	// ��ƃt�H���_�����쐬
	char	theBaseFolder[MAX_PATH];	// ��ƃt�H���_
	::GetModuleFileName( NULL, theBaseFolder, MAX_PATH );
	char*	p = FindFinalChar(theBaseFolder, '\\');
	*(++p)='\0';	// �t�@�C�������폜
	base_folder = theBaseFolder;

	UserDialog*	dlg = new UserDialog;
	int	ret = dlg->Run(hInstance, IDD_DIALOG, NULL);
	delete dlg;
	dlg=NULL;
	return	ret;
}


