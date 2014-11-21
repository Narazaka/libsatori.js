#ifdef _MSC_VER 

	// VC�p�̊e��ݒ�

	// �e���v���[�g�����������̌x����}��
	#pragma warning( disable : 4786 ) //�u�f�o�b�O�����ł̎��ʎq�؎̂āv
	#pragma warning( disable : 4503 ) //�u�������ꂽ���O�̒��������E���z���܂����B���O�͐؂�̂Ă��܂��B�v

	// for�X�R�[�v��ANSI����������
	#ifndef for
	#define for if(0);else for
	#endif	// for

#endif	// _MSC_VER

#include	<iostream>
#include	<exception>
#include	<stdexcept>
#include	<string>
#include	<vector>
#include	<map>
#include	<set>
#include	<fstream>
#include	<sstream>
#include	<cstdarg>
#include	<cstdio> // for sprintf
using namespace std;

typedef unsigned char byte;
typedef vector<string>	strvec;
typedef pair<string, string>	strpair;

//----------------------------------------------------
// �����ł����g����O�I�u�W�F�N�g
class util_error : public runtime_error 
{
public:
	util_error(const string& iWhat) : runtime_error(iWhat) {}
};

//----------------------------------------------------
// �O���[�o���֐�

// ������͐����ō\������Ă��邩�H�i�}�C�i�X���j
bool	aredigits(const string& s);

// ����̈ꕶ�����Ō�ɏo������ʒu��Ԃ�
const char*	find_final_char(const char* str, char c);
inline char* find_final_char(char* str, char c) { return const_cast<char*>(find_final_char(static_cast<const char*>(str), c)); }

// �g���q��ύX�������̂�Ԃ�
string	set_extension(const string& str, const char* new_ext);
inline string	set_extension(const string& str, const string& new_ext) { return set_extension(str, new_ext.c_str()); }

// ������u��
bool	replace_first(string& str, const string& before, const string& after);
int	replace(string& str, const string& before, const string& after);

// ������̕���
inline vector<string> split(const string& i_dlmt, string i_target)
{
	vector<string> v;
	string::size_type found_pos;
	while ( (found_pos = i_target.find(i_dlmt)) != string::npos )
	{
		v.push_back(i_target.substr(0, found_pos) );
		i_target = i_target.substr( found_pos + i_dlmt.size() );
	}
	v.push_back(i_target);
	return v;
}

// printf�݊��ŕ�����𐶐����Astring�^�ŕԂ��B
	// unix�ł�_vsnprintf���g���Ȃ����߁A
	// ���̎�̊֐��ł̓o�b�t�@��(�����ł͌Œ��8192byte)���z����\��������܂��B
	// �������̃��[�U���͂ȂǁA�������s��̕�����͓n���Ȃ��ł��������B
string	stringf(const char* iFormat, ...);

// �󔒕������H
inline bool is_space(int c) { return c==' '||c=='\t'; }

// �P��\���������H
inline bool is_word_element(int c) { return isdigit(c)||isalpha(c)||c=='-'||c=='_'; }

// ������string�ɕϊ�
inline string itos(int n) { char buf[256]; sprintf(buf,"%d",n); return buf; }

// string�𐮐��ɕϊ�
// inline int stoi_internal(const string& s) { return atoi(s.c_str()); }

// �w�蕶����́A�w��̔���֐���true��Ԃ������݂̂ō\������Ă��邩�H
template<typename Function>
bool are_elements(const string& s, Function f) 
{
	for ( const char* p=s.c_str() ; *p!='\0' ; ++p )
	{
		if ( !f(*p) )
		{
			return	false;
		}
	}
	return	true;
}

//-------------------------------------------------------------------

// multimap�ɃC���^�t�F�[�X��ǉ��������́B
template<typename Key, typename Value>
class my_multimap : public multimap<Key, Value>
{
public:
	// �L�[���w�肵�Ēl���擾�B�v�f�������ꍇ�͗�O�𓊂���B
	Value get(const Key& iKey) const 
	{
		const_iterator i = lower_bound(iKey);
		if ( i==end() )
			throw util_error("strmap::get(\"" + iKey + "\"): key was not found.");
		return i->second;
	}
	
	// �L�[���w�肵�Ēl���Q�ƁB�L�[��������΃L�[�l�y�A��}��
	Value& operator[](const Key& iKey) 
	{
		multimap<Key, Value>::iterator i = lower_bound(iKey); // ::�ȍ���������parse error�ƌ�����B
		if ( i==end() )
		{
			i = insert( pair<Key, Value>(iKey, Value()) );
		}
		return i->second;
	}
	
	// iKey�̑��݂��m�F
	bool exists(const Key& iKey) const 
	{
		return	find(iKey) != end();
	}
};

// �L�[���l��������ł���my_multimap�B�t�@�C��������e���\�z�ł���B
class strmap : public my_multimap<string, string>
{
public:
	// �w��t�@�C���̊e�s���f���~�^�ŕ������Amap������B
	void load(const string& iFileName, int iDelimiter='=', int iComment='#');
};


//-------------------------------------------------------------------
// syslog�֏o�͂���ostream

#ifdef _MSC_VER 
	// VC�R���p�C�����p�̃_�~�[
	#define LOG_INFO 0
#else
	#include	<syslog.h>
#endif	// _MSC_VER

class syslog_streambuf : public streambuf 
{
	string	m_line;	// ����̍s�Bendl������܂ŕێ��B
	string	m_last_line; // ���O�ɏo�͂����s�B
	int	m_level; // ���O�̕񍐃��x���B
public:
	syslog_streambuf() : streambuf(), m_level(LOG_INFO) {}
	virtual int overflow(int c=EOF);

	void	level(int i_level) { m_level=i_level; }
	string	last_line() { return m_last_line; }
};

class syslog_stream : public ostream 
{
	syslog_streambuf	m_buf;
public:
	syslog_stream() : ostream(&m_buf) {}
	void	level(int i_level) { m_buf.level(i_level); }
	string	last_line() { return m_buf.last_line(); }
};




//----------------------------------------------------
// �O���[�o���I�u�W�F�N�g

// �f�o�b�O�o�͗p�X�g���[��
extern	ostream&	cdbg;
// ���O�o�͗p�X�g���[���Bslog.level()�ŕ񍐃��x����ύX�ł���B
extern	syslog_stream	slog;

//-------------------------------------------------------------------
// ������/������

// BASE64�G���R�[�h�^�f�R�[�h
vector<byte> decodeBASE64(string i_encoded_str);
string encodeBASE64(const vector<byte>& i_data);

// RC4�G���R�[�h�i�f�R�[�h�ƃG���R�[�h�͓���̏����j
void encodeRC4(const byte* i_buf, byte* o_buf, int i_buflen, const byte* i_key, int i_keylen);
vector<byte> encodeRC4(const vector<byte>& i_buf, const vector<byte>& i_key);
inline string encodeRC4(string i_buf, const vector<byte>& i_key)
{
	vector<byte> the_buf = decodeBASE64(i_buf);
	vector<byte> the_r_buf = encodeRC4(the_buf, i_key);
	return	string( (char*) &the_r_buf.front() , the_r_buf.size() );
}


//-------------------------------------------------------------------
// �f�B���N�g�����X�e�B���O

// opendir, readdir, closedir�̃��b�p
#ifdef _MSC_VER
	
#else
	#include <sys/types.h>
	#include <dirent.h>
#endif // _MSC_VER

class directory
{
	DIR* m_dir;
public:
	directory() : m_dir(NULL) {}
	~directory() { close(); }

	bool	open(const string& i_dirname) {
		close();
		m_dir = opendir(i_dirname.c_str());
		return m_dir != NULL;
	}
	
	void	close() {
		if (m_dir != NULL) {
			closedir(m_dir);
			m_dir = NULL;
		}
	}
	
	struct dirent * read() {
		if (m_dir == NULL)
			return NULL;
		return readdir(m_dir);
	}
};

#include <sys/stat.h> // for stat

// �w��f�B���N�g���̃t�@�C�����ƃt�@�C�����̈ꗗ���擾����
inline bool get_files(const string& i_directory_name, map<string, struct stat>& o_files) 
{
	directory	dir;
	if ( !dir.open(i_directory_name) )
		return	false;

	struct dirent *p;
	while ((p = dir.read()) != NULL ) 
	{
		struct stat st;
		if ( stat((i_directory_name + DIR_CHAR + p->d_name).c_str(), &st) )
			continue;
		o_files[p->d_name] = st;
	}
	
	return	true;
}

//-------------------------------------------------------------------
// ���[�����M

void send_a_mail(
	const string& i_host,
	unsigned short i_port, // =25
	const string& i_from,
	const string& i_to,
	const string& i_subject,
	const vector<string>& i_message);



#endif	// PROV_BATCH_UTIL_H
