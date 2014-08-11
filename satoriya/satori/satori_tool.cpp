#include	"satori.h"
#ifdef POSIX
#  include      "Utilities.h"
#else
#  include	<mbctype.h>	// for _ismbblead,_ismbbtrail
#endif


#include	<fstream>
#include	<cassert>
#include <algorithm>

#ifdef POSIX
#  include <iostream>
#include <sys/stat.h>
#endif

#include "random.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////


#ifndef POSIX
// �t�@�C���̍ŏI�X�V�������擾
bool	GetLastWriteTime(LPCSTR iFileName, SYSTEMTIME& oSystemTime) {
	HANDLE	theFile = ::CreateFile( iFileName, 
		GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( theFile==INVALID_HANDLE_VALUE )
		return	false;
	
	BY_HANDLE_FILE_INFORMATION	theInfo;
	::GetFileInformationByHandle(theFile, &theInfo);
	::CloseHandle(theFile);
	
	FILETIME	FileTime;
	::FileTimeToLocalFileTime(&(theInfo.ftLastWriteTime), &FileTime);
	::FileTimeToSystemTime(&FileTime, &oSystemTime);
	return	true;
}
#endif

//----------------------------------------------------------------------
//	�t�@�C�������݂��邩�`�F�b�N
//----------------------------------------------------------------------
#ifdef POSIX
bool FileExist(const string& f)
{
    struct stat s;
    return ::stat(f.c_str(), &s) != 0;
}
#else
bool FileExist(const string& f)
{
	return ::GetFileAttributes(f.c_str()) != 0xFFFFFFFFU;
}
#endif

#if 0
//----------------------------------------------------------------------
//	�t�@�C���̍X�V�������r�B
//	�Ԓl�����Ȃ�ΑO�ҁA���Ȃ�Ό�҂̂ق����V�����t�@�C���B
//----------------------------------------------------------------------
#ifdef POSIX
#include <sys/types.h>
#include <sys/stat.h>
int CompareTime(const string& file1, const string& file2) {
    // file1�̕����V�������1�A�����Ȃ�0�A�Â����-1�B
    struct stat s1, s2;
    int r1 = ::stat(file1.c_str(), &s1);
    int r2 = ::stat(file2.c_str(), &s2);
    if (r1 == 0) {
		if (r2 != 0) {
			return 1;
		}
    }
    else {
		if (r2 == 0) {
			return -1;
		}
		else {
			return 0;
		}
    }
    if (s1.st_mtime > s2.st_mtime) {
		return 1;
    }
    else if (s1.st_mtime < s2.st_mtime) {
		return -1;
    }
    else {
		return 0;
    }
}
#else
int	CompareTime(LPCSTR szL, LPCSTR szR) {
	assert(szL!=NULL && szR!=NULL);
	
	SYSTEMTIME	stL, stR;
	BOOL		fexistL, fexistR;
	
	// �X�V���t�𓾂�B
	fexistL = GetLastWriteTime(szL, stL);
	fexistR	= GetLastWriteTime(szR, stR);
	// ���݂��Ȃ��t�@�C���́u�Â��v�ƌ��Ȃ��B
	if ( fexistL ) {
		if ( !fexistR)
			return	1;
	} else {
		if ( fexistR )
			return	-1;
		else
			return	0;	// �ǂ���������Ⴕ�˂�
	}
	
	// �ŏI�X�V���t���r
	if ( stL.wYear > stR.wYear )	return	1;
	else if ( stL.wYear < stR.wYear )	return	-1;
	if ( stL.wMonth > stR.wMonth )	return	1;
	else if ( stL.wMonth < stR.wMonth )	return	-1;
	if ( stL.wDay > stR.wDay )	return	1;
	else if ( stL.wDay < stR.wDay )	return	-1;
	if ( stL.wHour > stR.wHour )	return	1;
	else if ( stL.wHour < stR.wHour )	return	-1;
	if ( stL.wMinute > stR.wMinute )	return	1;
	else if ( stL.wMinute < stR.wMinute )	return	-1;
	if ( stL.wSecond > stR.wSecond )	return	1;
	else if ( stL.wSecond < stR.wSecond )	return	-1;
	if ( stL.wMilliseconds > stR.wMilliseconds )	return	1;
	else if ( stL.wMilliseconds < stR.wMilliseconds )	return	-1;
	// ��������̊��S��v
	return	0;
}
#endif
#endif

string	Satori::GetWord(const string& name) {
	return "����";
}

void Satori::surface_restore_string_addfunc(string &str,map<int, int>::const_iterator &i)
{
	if ( i->first >= 2 ) {
		if ( ! mIsMateria ) {
			str += string() + "\\p[" + itos(i->first) + "]\\s[" + itos(i->second) + "]";
		}
	}
	else {
		str += string() + "\\" + itos(i->first) + "\\s[" + itos(i->second) + "]";
	}
}

string	Satori::surface_restore_string()
{
	enum SurfaceRestoreMode srestore = surface_restore_at_talk_onetime;
	if ( srestore == SR_INVALID ) {
		srestore = surface_restore_at_talk;
	}

	// ���������K�v�Ȃ��A�̏ꍇ
	// invalid��r�͂����̕ی��i���肦�Ȃ��j
	if ( srestore == SR_NONE || srestore == SR_INVALID ) {	
		return	"\\1";
	}

	string	str="";
	
	if ( srestore == SR_FORCE ) {	
		for ( map<int, int>::const_iterator i=default_surface.begin() ; i!=default_surface.end() ; ++i ) {
			if ( surface_changed_before_speak.size() ) {
				map<int,bool>::const_iterator found = surface_changed_before_speak.find(i->first);
				if ( found == surface_changed_before_speak.end() || found->second ) {
					surface_restore_string_addfunc(str,i);
				}
			}
			else {
				surface_restore_string_addfunc(str,i);
			}
		}
	}
	else {
		for ( map<int, int>::const_iterator i=default_surface.begin() ; i!=default_surface.end() ; ++i ) {
			if ( surface_changed_before_speak.find(i->first) == surface_changed_before_speak.end() ) {
				surface_restore_string_addfunc(str,i);
			}
		}
	}
		
	surface_changed_before_speak.clear();
	return	str;
}


// ���閼�O�ɂ��w�肳���u�S�ẮvURL�y�ѕt�я��A�̃��X�g
bool	Satori::GetURLList(const string& name, string& result)
{
	list<const Talk*> tg;
	talks.select_all(name,*this,tg);
	
	const string sep_1 = "\1";
	const string sep_2 = "\2";
	
	for ( list<const Talk*>::iterator it = tg.begin() ; it != tg.end() ; ++it )
	{
		const Talk& vec = **it;
		if ( vec.size() < 1 )
			continue;
		string	menu = UnKakko(vec[0].c_str());
		string	url = (vec.size()<2) ? ("") : (UnKakko(vec[1].c_str()));
		string	banner = (vec.size()<3) ? ("") : (UnKakko(vec[2].c_str()));
		
		int	len = menu.size()+url.size()+banner.size()+3;
		result.reserve(result.size() + len + 1);
		
		result += menu;
		result += sep_1;
		result += url;
		result += sep_1;
		result += banner;
		result += sep_2;
	}
	return	true;
}

// ���閼�O�ɂ��w�肳���URL���̎w��T�C�g�̃X�N���v�g���擾
bool	Satori::GetRecommendsiteSentence(const string& name, string& result)
{
	list<const Talk*> tg;
	talks.select_all(name,*this,tg);
	
	for ( list<const Talk*>::iterator it = tg.begin() ; it != tg.end() ; ++it )
	{
		const Talk& t = **it;
		if ( t.size() >= 4 && t[1]==mReferences[1] )
		{
			result = SentenceToSakuraScriptExec( Talk(t.begin()+3, t.end()) );
			return	true;
		}
	}
	return	false;
}

strmap*	Satori::find_ghost_info(string name) {
	vector<strmap>::iterator i=ghosts_info.begin();
	for ( ; i!=ghosts_info.end() ; ++i )
		if ( (*i)["name"] == name )
			return	&(*i);
		return	NULL;
}


bool Satori::calc_argument(const string &iExpression, int &oResult, bool for_non_talk)
{
	string exp = UnKakko(iExpression.c_str(), true, for_non_talk);
	if ( !calc(exp) ){
		return false;
	}
	oResult = zen2int(exp);
	return true;
}

string	Satori::special_call(
	const string& iCallName,
	const strvec& iArgv,
	bool for_calc,
	bool for_non_talk,
	bool iIsSecure)
{
	if ( iCallName == "when" ) {
		int result;
		if ( iArgv.size() < 2 || 3 < iArgv.size() ) {
			return "�����̌�������������܂���B";
		}
		if ( !calc_argument(iArgv[0], result, for_non_talk) ) return "' �����v�Z�s\x94\x5c�ł��B";
		if ( result != 0 ) {
			return	UnKakko(iArgv[1].c_str(), for_calc, for_non_talk);	// �^
		}
		else if ( iArgv.size()==3 ) {
			return	UnKakko(iArgv[2].c_str(), for_calc, for_non_talk);	// �U
		}
		else {
			return	"";	// �U��else�Ȃ�
		}
	}

	if ( iCallName == "times" ) {
		int count;
		int max;
		int body;
		string ret="";
		char buf[21]; //64bit
		mLoopCounters.push("0");
		try{
			if ( iArgv.size() == 2 ){
				if ( !calc_argument(iArgv[0], max, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				count = 0;
				sprintf(buf, "%d", count);
				mLoopCounters.top() = buf;
				body = 1;
			}
			else if( iArgv.size() == 3 ){
				if ( !calc_argument(iArgv[1], count, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				sprintf(buf, "%d", count);
				mLoopCounters.top() = buf;
				if ( !calc_argument(iArgv[0], max, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				max += count;
				body = 2;
			}
			else{
				throw("�����̌�������������܂���B");
			}
			for(int i=count; i<max; i++){
				sprintf(buf, "%d", i);
				mLoopCounters.top() = buf;
				ret += UnKakko(iArgv[body].c_str(), for_calc, for_non_talk);
			}
		}
		catch( const char *str ){
			ret = str;
		}
		mLoopCounters.pop();
		return ret;
	}

	if ( iCallName == "while" ) {
		int count;
		int result;
		int expression;
		int body;
		string ret="";
		char buf[21]; //64bit
		mLoopCounters.push("0");
		try {
			if ( iArgv.size() == 2 ){
				expression = 0;
				count = 0;
				body = 1;
			}
			else if( iArgv.size() == 3 ){
				expression = 0;
				if ( !calc_argument(iArgv[1], count, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				body = 2;
			}
			else{
				throw("�����̌�������������܂���B");
			}
			for(int i=count; i<INT_MAX; i++){
				sprintf(buf, "%d", i);
				mLoopCounters.top() = buf;
				if ( !calc_argument(iArgv[expression], result, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				if ( result == 0 ) {
					break;
				}
				ret += UnKakko(iArgv[body].c_str(), for_calc, for_non_talk);
			}
		}
		catch(const char * str){
			ret = str;
		}
		mLoopCounters.pop();
		return ret;
	}

	if ( iCallName == "for" ) {
		int start;
		int end;
		int step;
		int body;
		char buf[21]; //64bit
		string ret="";
		mLoopCounters.push("0");
		try{
			if ( iArgv.size() == 3 ){
				if ( !calc_argument(iArgv[0], start, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				sprintf(buf, "%d", start);
				mLoopCounters.top() = buf;
				if ( !calc_argument(iArgv[1], end, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				step = 1;
				body = 2;
			}
			else if ( iArgv.size() == 4 ) {
				if ( !calc_argument(iArgv[0], start, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				sprintf(buf, "%d", start);
				mLoopCounters.top() = buf;
				if ( !calc_argument(iArgv[1], end, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				if ( !calc_argument(iArgv[2], step, for_non_talk) ) throw("' �����v�Z�s\x94\x5c�ł��B");
				body = 3;
			}
			else {
				throw("�����̌�������������܂���B");
			}
			if ( step == 0 ) {
				throw("for�̑�����0���w�肳��܂����B");
			}
			step = abs(step);
			if ( start <= end ) {
				for(int i=start; i<=end; i+=step) {
					sprintf(buf, "%d", i);
					mLoopCounters.top() = buf;
					ret += UnKakko(iArgv[body].c_str(), for_calc, for_non_talk);
				}
			}
			else {
				for(int i=start; end<=i; i-=step) {
					sprintf(buf, "%d", i);
					mLoopCounters.top() = buf;
					ret += UnKakko(iArgv[body].c_str(), for_calc, for_non_talk);
				}
			}
		}
		catch(const char *str){
			ret = str;
		}
		mLoopCounters.pop();
		return ret;
	}

	assert(0);
	return "";
}



// ���͂̒��� �i ���������ꍇ�Ap�� �i �̎��̈ʒu�܂Ői�߂�ꂽ��ł��ꂪ���s�����B
// p�͂��̓����� �j �̎��̈ʒu�܂Ői�߂���B
// �Ԓl�̓J�b�R�̉��ߌ��ʁB
string	Satori::KakkoSection(const char*& p,bool for_calc,bool for_non_talk)
{
	string	thePluginName = "";
	string  theDelimiter = "";
	bool specialFlag = false;
	const char *pp=0;
	strvec	theArguments;
	string	kakko_str;

	if ( for_calc ) {
		for_non_talk = true;
	}
	for ( set<string>::iterator it = special_commands.begin(); it != special_commands.end(); ++it) {
		if ( strncmp(it->c_str(), p, it->size()) == 0 ) {
			pp = p + it->size();
			string c = get_a_chr(pp);
			//�������Ȃ��ꍇ�̓X�y�V�����t�H�[���ɂ���K�v�͂Ȃ��B
			if ( mDelimiters.find(c) != mDelimiters.end() ){
				specialFlag = true;
				theDelimiter = c;
				thePluginName = it->c_str();
				break;
			}
		}
	}

	if( specialFlag ) {
		assert(pp);
		int level = 0;
		const char *p_start = pp;
		while( true ){
			if ( *pp == '\0' ){
				return string("�i"); // ���J�b�R����������
			}
			string c = get_a_chr(pp);
			if ( c == "�i" ) {
				level++;
			}
			if ( c == "�j" ) {
				level--;
			}
			if ( level < 0 ) {
				theArguments.push_back( string(p_start, pp-p_start-2) );
				break;
			}
			if ( level == 0 ) {
				if ( c == theDelimiter ) {
					theArguments.push_back( string(p_start, pp-p_start-c.size()) );
					p_start = (char *)pp;
				}
			}
		}
		p = pp;
		return special_call(thePluginName, theArguments, for_calc, for_non_talk, secure_flag);
	}
	else {
		while (true) {
			if ( p[0] == '\0' )
				return	string("�i") + kakko_str;	// ���J�b�R����������
			
			string c = get_a_chr(p);
			if ( c=="�j" )
				break;
			else if ( c=="�i" ) {
				kakko_str += KakkoSection(p,false,for_non_talk); //�����̊��ʂ�0�ɒu���������Ȃ�
			}
			else
				kakko_str += c;
		}	
		string	result;
		if ( Call(kakko_str, result, for_calc, for_non_talk) )
			return	result;
		if ( for_calc )
			return	string("�O");
		else
			return	string("�i") + kakko_str + "�j";
	}
}

string	Satori::UnKakko(const char* p,bool for_calc,bool for_non_talk)
{
	assert(p!=NULL);
	string	result;
	while ( p[0] != '\0' ) {
		string c=get_a_chr(p);
		result += (c=="�i") ? KakkoSection(p,for_calc,for_non_talk) : c;
	}
	return	result;
}

void	Satori::erase_var(const string& key)
{
	if ( key == "�X�R�[�v�؂芷����" ) {
		append_at_scope_change = "";
	}
	else if ( key == "������X�N���v�g�ɂ��X�R�[�v�؂芷����" ) {
		append_at_scope_change_with_sakura_script = "";
	}
	else if ( key == "�X�N���v�g�̈�ԓ�" ) {
		header_script = "";
	}
	else if ( key == "�g�[�N�J�n��" ) {
		append_at_talk_start = "";
	}
	else if ( key == "�g�[�N�I����" ) {
		append_at_talk_end = "";
	}
	else if ( key == "�I�����J�n��" ) {
		append_at_choice_start = "";
	}
	else if ( key == "�I�����I����" ) {
		append_at_choice_end = "";
	}
	else {
		int ref;
		char firstChar;

		if ( IsArrayValue(key,ref,firstChar) ) {
			if ( firstChar=='R' ) {
				// Event�ʒm���̈����擾
				if (ref>=0 && ref<mReferences.size()) {
					mReferences[ref] = "";
					if ( ref == (mReferences.size()-1) ) {
						mReferences.pop_back();
					}
				}
			}
			else if ( firstChar=='H' ) {
				// �ߋ��̒u�������������Q��
				if ( kakko_replace_history.empty() ) { return; }

				strvec&	khr = kakko_replace_history.top();

				ref -= 1; //�������܂��łȂ��Ɨ����ɂȂ��I

				if ( ref>=0 && ref < khr.size() ) {
					khr[ref] = "";
					if ( ref == (khr.size()-1) ) {
						khr.pop_back();
					}
				}
			}
			else if ( firstChar=='A' ) {
				if ( mCallStack.empty() ) { return; }

				// call�ɂ��Ăяo���̈������Q��S
				strvec&	v = mCallStack.top();
				if ( ref >= 0 && ref < v.size() ) {
					v[ref] = "";
					if ( ref == (v.size()-1) ) {
						v.pop_back();
					}
				}
			}
			else if ( firstChar=='S' ) {
				// SAORI�ȂǃR�[�����̌��ʏ���
				if (ref>=0 && ref<mKakkoCallResults.size()) {
					mKakkoCallResults[ref] = "";
					if ( ref == (mKakkoCallResults.size()-1) ) {
						mKakkoCallResults.pop_back();
					}
				}
			}
			//else if ( firstChar=='C' ) {
			//}
		}
		else {
			variables.erase(key);
		}
	}
}

bool	Satori::system_variable_operation(string key, string value, string* result)
{
	// map�ɂ��悤��B
	
	if ( key == "����Ԋu" ) {
		talk_interval = zen2int(value);
		if ( talk_interval<3 ) talk_interval=0; // 3�����͒���Ȃ�
		
		// ����J�E���g������
		int	dist = static_cast<int>(talk_interval*(talk_interval_random/100.0));
		talk_interval_count = ( dist==0 ) ? talk_interval : (talk_interval-dist)+(random(dist*2));
		
		return true;
	}
	
	if ( key == "����Ԋu�덷" ) {
		talk_interval_random = zen2int(value);
		if ( talk_interval_random>100 ) talk_interval_random=100;
		if ( talk_interval_random<0 ) talk_interval_random=0;
		
		// ����J�E���g������
		int	dist = int(talk_interval*(talk_interval_random/100.0));
		talk_interval_count = ( dist==0 ) ? talk_interval : 
		(talk_interval-dist)+(random(dist*2));
		
		return true;
	}
	
	if ( key =="���؂�ĂĂ�����" ) {
		is_call_ontalk_at_mikire= (value=="�L��");
		return true;
	}

	if ( key == "����͒���Ȃ�" ) {
		return_empty=(value=="�L��");
		return true;
	}
	
	if ( key == "�X�N���v�g�̈�ԓ�" ) {
		header_script = value;
		return true;
	}

	if ( key == "�Ăяo���񐔐���" ) {
		m_nest_limit = zen2int(value);
		if ( m_nest_limit < 0 ) { m_nest_limit = 0; }
		return true;
	}

	if ( key == "�W�����v�񐔐���" ) {
		m_jump_limit = zen2int(value);
		if ( m_jump_limit < 0 ) { m_jump_limit = 0; }
		return true;
	}
	
	if ( key == "�X�R�[�v�؂芷����" ) {
		append_at_scope_change = zen2han(value);
		return true;
	}

	if ( key == "������X�N���v�g�ɂ��X�R�[�v�؂芷����" ) {
		append_at_scope_change_with_sakura_script = zen2han(value);
		return true;
	}

	if ( key == "�g�[�N�J�n��" ) {
		append_at_talk_start = zen2han(value);
		return true;
	}

	if ( key == "�g�[�N�I����" ) {
		append_at_talk_end = zen2han(value);
		return true;
	}

	if ( key == "�I�����J�n��" ) {
		append_at_choice_start = zen2han(value);
		return true;
	}
	
	if ( key == "�I�����I����" ) {
		append_at_choice_end = zen2han(value);
		return true;
	}

	if ( key == "��b���T�[�t�F�X�߂�" || key == "��b���T�[�t�B�X�߂�" ) {
		if ( value == "�L��" ) {
			surface_restore_at_talk = SR_NORMAL;
		}
		else if ( value == "����" ) {
			surface_restore_at_talk = SR_FORCE;
		}
		else {
			surface_restore_at_talk = SR_NONE;
		}
		return true;
	}

	if ( key == "����͉�b���T�[�t�F�X�߂�" || key == "����͉�b���T�[�t�B�X�߂�" ) {
		if ( value == "�L��" ) {
			surface_restore_at_talk_onetime = SR_NORMAL;
		}
		else if ( value == "����" ) {
			surface_restore_at_talk_onetime = SR_FORCE;
		}
		else {
			surface_restore_at_talk_onetime = SR_NONE;
		}
		return true;
	}
	
	if ( key == "�����A���J�[" ) {
		if ( value == "�L��" ) {
			auto_anchor_enable = true;
			auto_anchor_enable_onetime = auto_anchor_enable;
		}
		else {
			auto_anchor_enable = false;
			auto_anchor_enable_onetime = auto_anchor_enable;
		}
		return true;
	}

	if ( key == "����͎����A���J�[" ) {
		if ( value == "�L��" ) {
			auto_anchor_enable_onetime = true;
		}
		else {
			auto_anchor_enable_onetime = false;
		}
		return true;
	}
	
	if ( compare_head(key,  "�T�[�t�F�X���Z�l") && aredigits(key.c_str() + const_strlen("�T�[�t�F�X���Z�l")) ) {
		int n = zen2int(key.c_str() + const_strlen("�T�[�t�F�X���Z�l"));
		surface_add_value[n]= zen2int(value);
		
		variables[string()+"�f�t�H���g�T�[�t�F�X"+itos(n)] = value;
		next_default_surface[n] = zen2int(value);
		if ( !is_speaked_anybody() )
			default_surface[n]=next_default_surface[n];
		return true;
	}

	if ( compare_head(key,  "�f�t�H���g�T�[�t�F�X") && aredigits(key.c_str() + const_strlen("�f�t�H���g�T�[�t�F�X")) ) {
		int n = zen2int(key.c_str() + const_strlen("�f�t�H���g�T�[�t�F�X"));
		next_default_surface[n]= zen2int(value);
		if ( !is_speaked_anybody() )
			default_surface[n]=next_default_surface[n];
		return true;
	}

	if ( compare_head(key,  "BalloonOffset") && aredigits(key.c_str() + const_strlen("BalloonOffset")) ) {
		int n = stoi(key.c_str() + const_strlen("BalloonOffset"));
		BalloonOffset[n] = value;
		validBalloonOffset[n] = true;
		return true;
	}

	if ( key == "�g�[�N���̂Ȃł�ꔽ��") {
		insert_nade_talk_at_other_talk= (value=="�L��");
		return true;
	}
	
	if ( key == "�Ȃł�ꎝ���b��") {
		nade_valid_time_initializer = zen2int(value);
		return true;
	}

	if ( key == "�Ȃł�ꔽ����") {
		nade_sensitivity = zen2int(value);
		return true;
	}

	if ( key == "�f�o�b�O" ) {
		fDebugMode = (value=="�L��");
		return true;
	}

	if ( key == "Log" ) {
		Sender::validate(value=="�L��");
		return true;
	}

	if ( key == "RequestLog" ) {
		fRequestLog = (value=="�L��");
		return true;
	}

	if ( key == "OperationLog" ) {
		fOperationLog = (value=="�L��");
		return true;
	}
	
	if ( key == "ResponseLog" ) {
		fResponseLog = (value=="�L��");
		return true;
	}
	
	if ( key == "�����}���E�F�C�g�̔{��" || key == "�����}���E�G�C�g�̔{��" ) {
		rate_of_auto_insert_wait= zen2int(value);
		rate_of_auto_insert_wait = min(1000, max(0, rate_of_auto_insert_wait));
		variables["�����}���E�F�C�g�̔{��"] = int2zen(rate_of_auto_insert_wait);
		return true;
	}
	
	if ( key == "�����}���E�F�C�g�^�C�v" || key == "�����}���E�G�C�g�^�C�v"  ) {
		if ( value == "���" ) {
			type_of_auto_insert_wait = 2;
			variables["�����}���E�F�C�g�^�C�v"] = "���";
		}
		else if ( value == "����" ) {
			type_of_auto_insert_wait = 0;
			variables["�����}���E�F�C�g�^�C�v"] = "����";
		}
		else /* if ( value == "���X" ) */ {
			type_of_auto_insert_wait = 1;
			variables["�����}���E�F�C�g�^�C�v"] = "���X";
		}
		return true;
	}
	
	if ( key == "�����t�H���_" ) {
		strvec	words;
		split(value, ",",dic_folder);
		reload_flag=true;
		return true;
	}
	
	if ( key == "�Z�[�u�f�[�^�Í���" ) {
		fEncodeSavedata = (value=="�L��");
		return true;
	}
	
	if ( compare_head(key,"�P��Q�u") && compare_tail(key,"�v�̏d�����") ) {
		variables.erase(key);
		words.setOC( string(key.c_str()+8, key.length()-8-12), value );
		return true;
	}

	if ( compare_head(key,"���u") && compare_tail(key,"�v�̏d�����") ) {
		variables.erase(key);
		talks.setOC( string(key.c_str()+4, key.length()-4-12), value );
		return true;
	}
	
	if ( key == "���̃g�[�N" ) {
		variables.erase(key);
		int	count=1;
		while ( reserved_talk.find(count) != reserved_talk.end() )
			++count;
		reserved_talk[count] = value;
		sender << "����̃����_���g�[�N���u" << value << "�v�ɗ\\x96\xf1����܂����B" << endl;
		return true;
	}
	
	if ( compare_head(key,"������") && compare_tail(key,"��ڂ̃g�[�N") ) {
		variables.erase(key);
		int	count = zen2int( string(key.c_str()+6, key.length()-6-12) );
		if ( count<=0 ) {
			sender << "�g�[�N�\\x96\xf1�A�ݒ�l���w���ł��B" << endl;
		}
		else {
			while ( reserved_talk.find(count) != reserved_talk.end() )
				++count;
			reserved_talk[count] = value;
			sender << count << "���̃����_���g�[�N���u" << value << "�v�ɗ\\x96\xf1����܂����B" << endl;
		}
		return true;
	}

	if ( key=="�g�[�N�\\x96\xf1�̃L�����Z��" ) {
		if ( value=="��" )
			reserved_talk.clear();
		else
			for (map<int, string>::iterator it=reserved_talk.begin(); it!=reserved_talk.end() ; )
				if ( value == it->second )
					reserved_talk.erase(it++);
				else
					++it;
		return true;
	}

	if ( key == "SAORI�����̌v�Z" ) {
		if (value=="�L��")
			mSaoriArgumentCalcMode = SACM_ON;
		else if (value=="����")
			mSaoriArgumentCalcMode = SACM_OFF;
		else
			mSaoriArgumentCalcMode = SACM_AUTO;
		return true;
	}
	
	if ( key == "���������[�h" && value=="���s") {
		variables.erase(key);
		reload_flag=true;
		return true;
	}
	
	if ( key == "�ꂵ�Α��M") {
		variables.erase(key);
		Sender::reinit(value=="�L��");
		return true;
	}
	
	if ( key == "�蓮�Z�[�u" && value=="���s") {
		variables.erase(key);
		if ( is_dic_loaded ) {
			this->Save();
		}
		return true;
	}
	
	if ( key == "�����Z�[�u�Ԋu" ) {
		mAutoSaveInterval = zen2int(value);
		mAutoSaveCurrentCount = mAutoSaveInterval;
		if ( mAutoSaveInterval > 0 )
			sender << ""  << itos(mAutoSaveInterval) << "�b�Ԋu�Ŏ����Z�[�u���s���܂��B" << endl;
		else
			sender << "�����Z�[�u�͍s���܂���B" << endl;
		return true;
	}
	
	if ( key == "�S�^�C�}����" && value=="���s") {
		variables.erase(key);
		for (strintmap::const_iterator i=timer.begin();i!=timer.end();++i)
			variables.erase(i->first + "�^�C�}");
		timer.clear();
		return true;
	}
	
	if ( key == "����邱��" ) {
		variables.erase(key);
		teach_genre=value;
		if ( result != NULL )
			*result += "\\![open,teachbox]";
		return true;
	}
	
	if ( key.size()>6 && compare_tail(key, "�^�C�}") ) {
		string	name(key.c_str(), strlen(key.c_str())-6);
		/*if ( sentences.find(name) == sentences.end() ) {
		result = string("���@�^�C�}�I�����̃W�����v�� ��")+name+" ������܂���@��";
		// �Z�[�u�f�[�^���A�����l��
		}
		else */{
		int sec = zen2int(value);
		if ( sec < 1 ) {
			variables.erase(key);
			if ( timer.find(name)!=timer.end() ) {
				timer.erase(name);
				sender << "�^�C�}�u"  << name << "�v�̗\\x96\xf1���L�����Z������܂����B" << endl;
			} else
				sender << "�^�C�}�u"  << name << "�v�͌�����\\x96\xf1����Ă��܂���B" << endl;
		} else {
			timer[name] = sec;
			sender << "�^�C�}�u"  << name << "�v��" << sec << "�b��ɗ\\x96\xf1����܂����B" << endl;
		}
		}
		return true;
	}

	if ( key == "������؂�ǉ�" && value.size()>0 ) {
		variables.erase(key);
		mDelimiters.insert(value);
		return true;
	}
	
	if ( key == "������؂�폜" && value.size()>0 ) {
		variables.erase(key);
		mDelimiters.erase(value);
		return true;
	}
	
	if ( compare_head(key, "Value") && aredigits(key.c_str() + 5) )
	{
		variables.erase(key);
		if(value!=""){
			mResponseMap[string()+"Reference"+key.substr(5)] = value;
		}else{
			mResponseMap.erase(string()+"Reference"+key.substr(5));
		}
		return true;
	}

	return	false;
}


bool	Satori::calculate(const string& iExpression, string& oResult) {
	
	oResult = UnKakko(iExpression.c_str(),true);
	
	bool r = calc(oResult);
	if ( !r ) {
#ifdef POSIX
		errsender <<
			"error on Satori::calculate" << std::endl <<
			"Error in expression: " << iExpression << std::endl;
#else
		// ����������ƒ��ۉ����c�c
		errsender << string() + "�����v�Z�s�\�ł��B\n" + iExpression << std::endl;
#endif
	}
	return	r;
}



