#ifdef _MSC_VER 

	// �}���`���j�^�֘A
	//#define WINVER 0x0500
	#include	<windows.h>
	#include	<multimon.h>
	#define SM_CXVIRTUALSCREEN      78
	#define SM_CYVIRTUALSCREEN      79

#endif	// _MSC_VER


#include	"satori.h"

#include	<fstream>
#include	<cassert>
#include      <locale.h>

#include	"../_/Utilities.h"
#include     "../_/random.h"

#include "posix_utils.h"

#ifdef POSIX
#include <unistd.h>
#endif

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////


//---------------------------------------------------------------------------
#ifndef POSIX
BOOL CALLBACK MonitorEnumFunc(HMONITOR hMonitor,HDC hdc,LPRECT rect,LPARAM lParam) {

    MONITORINFOEX MonitorInfoEx;
    MonitorInfoEx.cbSize=sizeof(MonitorInfoEx);

	static BOOL (WINAPI* pGetMonitorInfo)(HMONITOR,LPMONITORINFO) = NULL;
	if ( ! pGetMonitorInfo ) {
		(FARPROC&)pGetMonitorInfo = ::GetProcAddress(::GetModuleHandle("user32.dll"), "GetMonitorInfoA");
	}

	if ( pGetMonitorInfo==NULL )
		return	FALSE;
	if ( !(*pGetMonitorInfo)(hMonitor,&MonitorInfoEx) ) {
		sender << "'GetMonitorInfo' was failed." << endl;
        return FALSE;
    }


	RECT* pRect = ((RECT*)lParam);

	if ( MonitorInfoEx.dwFlags & MONITORINFOF_PRIMARY ) {
		pRect[1] = *rect;
		sender << "���j�^: " << MonitorInfoEx.szDevice << " / (" << 
			rect->left << "," << rect->top << "," << rect->right << "," << rect->bottom << ") / primary" << endl;
	}
	else {
		sender << "���j�^: " << MonitorInfoEx.szDevice << " / (" << 
			rect->left << "," << rect->top << "," << rect->right << "," << rect->bottom << ") / extra" << endl;
	}

	RECT&	max_screen_rect = pRect[0];
	if ( rect->left < max_screen_rect.left )
		max_screen_rect.left = rect->left;
	if ( rect->top < max_screen_rect.top )
		max_screen_rect.top = rect->top;
	if ( rect->right > max_screen_rect.right )
		max_screen_rect.right = rect->right;
	if ( rect->bottom > max_screen_rect.bottom )
		max_screen_rect.bottom = rect->bottom;

    return TRUE;
}
#endif

//---------------------------------------------------------------------------

#ifdef	_DEBUG
#ifdef _WINDOWS

class DummyDbgSetFlagClass {
public:
	DummyDbgSetFlagClass(void) {
		int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
		tmpFlag |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;
		tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;
		_CrtSetDbgFlag( tmpFlag );
	}
};
DummyDbgSetFlagClass dummy;

#endif // _WINDOWS
#endif // _DEBUG


string	Satori::getversionlist(const string& iBaseFolder)
{
	return "SHIORI/3.0\1SAORI/1.0";
}


bool	Satori::load(const string& iBaseFolder)
{
	Sender::initialize();

	setlocale(LC_ALL, "Japanese");

	mBaseFolder = iBaseFolder;
	sender << "��SATORI::Load on " << mBaseFolder << "" << endl;

#if POSIX
	// �u/�v�ŏI����Ă��Ȃ���Εt����B
	if (mBaseFolder[mBaseFolder.size() - 1] != '/') {
	    mBaseFolder += '/';
	}
#else
	// �u\�v�ŏI����Ă��Ȃ���Εt����B
	if (mBaseFolder[mBaseFolder.size() - 1] != '\\') {
	    mBaseFolder += '\\';
	}
#endif


#ifdef	_MSC_VER
	// �{�̂̂���t�H���_���T�[�`
	{
		TCHAR	buf[MAX_PATH+1];
		::GetModuleFileName(NULL, buf, MAX_PATH);
		char*	p = FindFinalChar(buf, DIR_CHAR);
		if ( p==NULL )
			mExeFolder = "";
		else {
			*(++p) = '\0';
			mExeFolder = buf;
		}
	}
	sender << "�{�̂̏���: " << mExeFolder << "" << endl;
#endif // _MSC_VER

	// �����o������
	InitMembers();

#ifdef	_MSC_VER
	// �V�X�e���̐ݒ��ǂ�ł���
    OSVERSIONINFO	ovi;
    ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&ovi);
	string	os;
	if ( ovi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ) {
		if ( ovi.dwMinorVersion == 0 ) { mOSType=WIN95; os="Windows 95"; }
		else if ( ovi.dwMinorVersion == 10 ) { mOSType=WIN98; os="Windows 98"; }
		else if ( ovi.dwMinorVersion == 90 ) { mOSType=WINME; os="Windows Me"; }
		else { mOSType = UNDEFINED; os="undefined"; }
	} else {
		if ( ovi.dwMinorVersion == 0 ) {
			if ( ovi.dwMajorVersion == 4 ) { mOSType=WINNT; os="Windows NT"; }
			else if ( ovi.dwMajorVersion == 5 ) { mOSType=WIN2K; os="Windows 2000"; }
		}
		else { mOSType = WINXP; os="Windows XP or later"; }
	}
	sender << "�n�r���: " << os << endl;
	if ( mOSType==WIN95 ) {
		is_single_monitor = true;
	} else {
		BOOL (WINAPI* pEnumDisplayMonitors)(HDC,LPRECT,MONITORENUMPROC,LPARAM);
		(FARPROC&)pEnumDisplayMonitors = ::GetProcAddress(::GetModuleHandle("user32.dll"), "EnumDisplayMonitors");
		if ( pEnumDisplayMonitors==NULL ) {
			is_single_monitor = true;
		}
		else {
			RECT rectData[2];
			memset(rectData,0,sizeof(rectData));
			(*pEnumDisplayMonitors)(NULL,NULL,(MONITORENUMPROC)MonitorEnumFunc,(LPARAM)(rectData));

			max_screen_rect = rectData[0];
			desktop_rect = rectData[1];

			RECT*	rect;
			rect = &desktop_rect;
			sender << "�v���C�}���f�X�N�g�b�v: (" << 
				rect->left << "," << rect->top << "," << rect->right << "," << rect->bottom << ")" << endl;
			rect = &max_screen_rect;
			sender << "���z�f�X�N�g�b�v: (" << 
				rect->left << "," << rect->top << "," << rect->right << "," << rect->bottom << ")" << endl;
			is_single_monitor = ( ::EqualRect(&max_screen_rect, &desktop_rect)!=FALSE );
			sender << (is_single_monitor ? 
				"���j�^�͈�����Ɣ��f�A���؂ꔻ����Ăяo�����ɔC���܂��B" : 
				"�����̃��j�^���ڑ�����Ă���Ɣ��f�A���؂ꔻ��͗��X���s���܂��B") << endl;
		}
	}
#endif // _MSC_VER

	// �u�������ǂݎ��
	strmap_from_file(replace_before_dic, mBaseFolder+"replace.txt", "\t");
	strmap_from_file(replace_after_dic, mBaseFolder+"replace_after.txt", "\t");

	// �L�����f�[�^�ǂݍ���
	mCharacters.load(mBaseFolder + "characters.ini");
	for ( inimap::const_iterator i=mCharacters.begin() ; i!=mCharacters.end() ; ++i ) {
		const strmap& m = i->second;
		strmap::const_iterator j;

		// �u�������ɒǉ�
		j = m.find("popular-name");
		if ( j != m.end() && j->second.size()>0 ) 
			replace_before_dic[j->second + "�F"] = string("\xff\x01") + zen2han(i->first); //0xff0x01�͂��Ƃŕϊ�
		j = m.find("initial-letter");
		if ( j != m.end() && j->second.size()>0 ) 
			replace_before_dic[j->second + "�F"] = string("\xff\x01") + zen2han(i->first); //0xff0x01�͂��Ƃŕϊ�

		j = m.find("base-surface");
		if ( j != m.end() && j->second.size()>0 )
			system_variable_operation( string("�T�[�t�F�X���Z�l") + i->first, j->second);
	}

	//for ( strmap::const_iterator j=replace_before_dic.begin() ; j!=replace_before_dic.end() ; ++j )
	//	cout << j->first << ": " << j->second << endl;

	// �����_�}�C�Y
	randomize();


	//------------------------------------------

	// �R���t�B�O�ǂݍ���
	LoadDictionary(mBaseFolder + "satori_conf.txt", false);

	// �ϐ����������s
	GetSentence("������");	

	// SAORI�ǂݍ���
	Family<Word>* f = words.get_family("SAORI");
	if ( f != NULL )
	{
		vector<const Word*> els;
		f->get_elements_pointers(els);

		mShioriPlugins->load(mBaseFolder);
		for ( vector<const Word*>::const_iterator i=els.begin(); i!=els.end() ; ++i)
		{
			if ( (*i)->size()>0 && !mShioriPlugins->load_a_plugin(**i) )
			{
				sender << "SAORI�ǂݍ��ݒ��ɃG���[������: " << **i << endl;
			}
		}
	}
	talks.clear();
	words.clear();

	//------------------------------------------

	// �Z�[�u�f�[�^�ǂݍ���
	bool oldConf = fEncodeSavedata;

	bool loadResult = LoadDictionary(mBaseFolder + "satori_savedata." + (fEncodeSavedata?"sat":"txt"), false);
	GetSentence("�Z�[�u�f�[�^");
	bool execResult = talks.get_family("�Z�[�u�f�[�^") != NULL;

	if ( ! loadResult || ! execResult ) {
		loadResult = LoadDictionary(mBaseFolder + "satori_savebackup." + (fEncodeSavedata?"sat":"txt"), false);
		GetSentence("�Z�[�u�f�[�^");
		execResult = talks.get_family("�Z�[�u�f�[�^") != NULL;
	}

	talks.clear();
	
	reload_flag = false;

	if ( variables.find("�S�[�X�g�N�����ԗ݌v�b") != variables.end() ) {
		sec_count_total = zen2int(variables["�S�[�X�g�N�����ԗ݌v�b"]);
	}
	else if ( variables.find("�S�[�X�g�N�����ԗ݌v�~���b") != variables.end() ) {
		sec_count_total = zen2int(variables["�S�[�X�g�N�����ԗ݌v�~���b"]) / 1000;
	}
	else {
		sec_count_total = zen2int(variables["�S�[�X�g�N�����ԗ݌v(ms)"]) / 1000;
	}
	variables["�N����"] = itos( zen2int(variables["�N����"])+1 );

	// �u�P��̒ǉ��v�œo�^���ꂽ�P����o���Ă���
	const map< string, Family<Word> >& m = words.compatible();
	for ( map< string, Family<Word> >::const_iterator it = m.begin() ; it != m.end() ; ++it )
	{
		vector<const Word*> v;
		it->second.get_elements_pointers(v);
		for ( vector<const Word*>::iterator itx = v.begin() ; itx < v.end() ; ++itx ) {
			mAppendedWords[it->first].push_back(**itx);
		}
	}

	//------------------------------------------

	// �w��t�H���_�̎�����ǂݍ���
	int loadcount = 0;
	strvec::iterator i = dic_folder.begin();
	if ( i==dic_folder.end() ) {
		loadcount += LoadDicFolder(mBaseFolder);	// ���[�g�t�H���_�̎���
	} else {
		for ( ; i!=dic_folder.end() ; ++i )
			loadcount += LoadDicFolder(mBaseFolder + *i + DIR_CHAR);	// �T�u�t�H���_�̎���
	}

	is_dic_loaded = loadcount != 0;

	//------------------------------------------

	secure_flag = true;

	system_variable_operation("�P��Q�u���v�̏d�����", "�L���A�g�[�N��");
	system_variable_operation("���u���v�̏d�����", "�L��");
	//system_variable_operation("�P��Q�u�G�߂̐H�ו��v�̏d�����", "�L���A�g�[�N��");

	GetSentence("OnSatoriLoad");
	on_loaded_script = GetSentence("OnSatoriBoot");
	diet_script(on_loaded_script);

	sender << "loaded." << endl;
	return	true;
}


//---------------------------------------------------------------------------
#define	ENCODE(x)	(fEncodeSavedata ? encode(encode(x)) : (x))

#ifdef POSIX
#  include <time.h>
#endif
bool	Satori::Save(bool isOnUnload) {

	// �����o�ϐ��𗢁X�ϐ���
	for (map<int, string>::iterator it=reserved_talk.begin(); it!=reserved_talk.end() ; ++it)
		variables[string("������")+itos(it->first)+"��ڂ̃g�[�N"] = it->second;

	// �N�����ԗ݌v��ݒ�
	variables["�S�[�X�g�N�����ԗ݌v�b"] =
	    itos(posix_get_current_sec() - sec_count_at_load + sec_count_total);
	// (�݊��p)
	variables["�S�[�X�g�N�����ԗ݌v�~���b"] =
	    itos((posix_get_current_sec() - sec_count_at_load + sec_count_total)*1000);
	variables["�S�[�X�g�N�����ԗ݌v(ms)"] =
	    itos((posix_get_current_sec() - sec_count_at_load + sec_count_total)*1000);

	if ( isOnUnload ) {
		secure_flag = true;
		(void)GetSentence("OnSatoriUnload");
	}

	string	theFullPath = mBaseFolder + "satori_savedata.tmp";

	ofstream	out(theFullPath.c_str());
	bool	temp = Sender::is_validated();
	Sender::validate();
	sender << "saving " << theFullPath << "... " ;
	Sender::validate(temp);
	if ( !out.is_open() )
	{
		sender << "failed." << endl;
		return	false;
	}

	string	line = "���Z�[�u�f�[�^";
	string  data;

	out << ENCODE(line) << endl;
	for (strmap::const_iterator it=variables.begin() ; it!=variables.end() ; ++it) {
		string	str = zen2han(it->first);
		if ( str[0]=='S' && aredigits(str.c_str()+1) ) {
			continue;
		}
		if ( str == "����͒���Ȃ�" || str == "����͉�b���T�[�t�F�X�߂�" || str == "����͉�b���T�[�t�B�X�߂�" || str == "����͎����A���J�[" ) {
			continue;
		}

		data = it->second;
		
		replace(data,"��","�Ӄ�");
		replace(data,"�i","�Ӂi");
		replace(data,"�j","�Ӂj");
		m_escaper.unescape_for_dic(data);

		string	line = string("��")+it->first+"\t"+data; // �ϐ���ۑ�
		out << ENCODE(line) << endl;
	}

	for ( map<string, vector<Word> >::const_iterator i=mAppendedWords.begin() ; i!=mAppendedWords.end() ; ++i )
	{
		if ( ! i->second.empty() ) {
			out << endl << ENCODE( string("��") + i->first ) << endl;
			for ( vector<Word>::const_iterator j=i->second.begin() ; j!=i->second.end() ; ++j )
			{
				out << ENCODE( *j ) << endl;
			}
		}
	}

	out.flush();
	out.close();

	sender << "ok." << endl;

	//�o�b�N�A�b�v
	string	realFullPath = mBaseFolder + "satori_savedata." + (fEncodeSavedata?"sat":"txt");
	string	realFullPathBackup = mBaseFolder + "satori_savebackup." + (fEncodeSavedata?"sat":"txt");
#ifdef POSIX
	unlink(realFullPathBackup.c_str());
	rename(realFullPath.c_str(),realFullPathBackup.c_str());
	rename(theFullPath.c_str(),realFullPath.c_str());
#else
	::DeleteFile(realFullPathBackup.c_str());
	::MoveFile(realFullPath.c_str(),realFullPathBackup.c_str());
	::MoveFile(theFullPath.c_str(),realFullPath.c_str());
#endif

	//����Ȃ��ق�������
	string	delFullPath = mBaseFolder + "satori_savedata." + (fEncodeSavedata?"txt":"sat");
	string	delFullPathBackup = mBaseFolder + "satori_savebackup." + (fEncodeSavedata?"txt":"sat");
#ifdef POSIX
	unlink(delFullPath.c_str());
	unlink(delFullPathBackup.c_str());
#else
	::DeleteFile(delFullPath.c_str());
	::DeleteFile(delFullPathBackup.c_str());
#endif

	return	true;
}

//---------------------------------------------------------------------------
bool	Satori::unload() {

	// �t�@�C���ɕۑ�
	if ( is_dic_loaded ) {
		this->Save(true);
	}
	is_dic_loaded = false;

	// �v���O�C�����
	mShioriPlugins->unload();

	sender << "��SATORI::Unload ---------------------" << endl;
	return	true;
}


