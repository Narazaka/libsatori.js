#include	"stltool.h"
#include	<sstream>
#include	<cassert>
#ifdef POSIX
#  include      "Utilities.h"
#else
#  include      "Utilities.h"
//#  include	<mbctype.h>	// for _ismbblead,_ismbbtrail
#endif

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

ostream& operator<<(ostream& o, const strvec& i) {
	for ( strvec::const_iterator p=i.begin() ; p!=i.end() ; ++p )
		o << *p << endl;
	return	o;
}

ostream& operator<<(ostream& o, const strmap& i) {
	for ( strmap::const_iterator p=i.begin() ; p!=i.end() ; ++p )
		o << p->first << "=" << p->second << endl;
	return	o;
}

ostream& operator<<(ostream& o, const strintmap& i) {
	for ( strintmap::const_iterator p=i.begin() ; p!=i.end() ; ++p )
		o << p->first << "=" << p->second << endl;
	return	o;
}
istream& operator>>(istream& i, strvec& o) {
	string	str;
	while ( getline(i, str) )
		o.push_back(str);
	return	i;
}
istream& operator>>(istream& i, strmap& o) {
	string	key, value;
	while ( getline(i, key, '=') && getline(i, value) )
		o[key] = value;
	return	i;
}
istream& operator>>(istream& i, strintmap& o) {
	string	key, value;
	while ( getline(i, key, '=') && getline(i, value) )
		o[key] = stoi(value);
	return	i;
}

// �X�g���[������A�f���~�^�܂ł�ǂݍ����string�Ɋi�[
bool	getline(istream& i, string& o, int delimtier) {
	if ( i.peek() == EOF )
		return	false;	// �t�@�C���̏I�[�ł����ȁB

	int	c;	// �ꕶ���ێ�
	stringstream	line;	// �s���i�[����X�g���[��
	while ( (c=i.get()) != delimtier && c!=EOF)
		line.put(c);
	o=line.str();
	return	true;
}

bool	getline(istream& i, int& o, int delimtier) {
	if ( i.peek() == EOF )
		return	false;	// �t�@�C���̏I�[�ł����ȁB

	o=0;
	int	c;	// �ꕶ���ێ�
	while ( (c=i.get()) != delimtier && c!=EOF) {
		if ( isdigit(c) ) {
			o *= 10;
			o += c-'0';
		}
	}
	return	true;
}

bool	aredigits(const char* p) {
	if ( *p=='-' )
		++p;
	if ( *p=='\0' )
		return false;
	while ( *p!='\0' )
		if ( !isdigit(*p++) )
			return	false;
	return	true;
}

bool	arealphabets(const char* p) {
	while (*p) {
		int	c = *p++;
		if ( c>='a' && c<='z' )
			continue;
		if ( c>='A' && c<='Z' )
			continue;
		if ( c==(-126) ) {	// sjis�ɂ�����`-�y,��-�����܂ނP�o�C�g��
			c = *p++;
			if ( c>=("�`")[1] && c<=("�y")[1] )
				continue;
			if ( c>=("��")[1] && c<=("��")[1] )
				continue;
		}
		return	false;
	}
	return	true;
}


bool	replace_first(string& str, const string& before, const string& after) {
	int	pos = str.find(before);
	if ( pos == string::npos )
		return	false;
	str.replace(pos, before.size(), after);
	return	true;
}

/*int	replace(string& str, const string& before, const string& after) {
	int	n=0;
	while (replace_first(str, before, after))
		++n;
	return	n;
}
*/

int	replace(string& str, const string& before, const string& after) {
	if ( str=="" || before=="" ) return 0;

	// �����񒷂̌v�Z�p
	const int		beforeLength = before.size();
	const int		afterLength = after.size();
	const int		textLength = str.size();
	const int		diffLength = afterLength - beforeLength;

	// �u�������Ώۂ��������邩���J�E���g���Ă���
	const char*	found=str.c_str();
	int	count=0;
	while ( (found=strstr_hz(found, before.c_str())) != NULL ) {
		found += beforeLength;
		++count;
	}
	if ( count==0 ) return 0;

	// ���ԃo�b�t�@���m��
	char*	buf = new char[textLength + diffLength*count + 1];

	// �u���������[�v
	const char*	pread = str.c_str();
	char*	pwrite = buf;
	found = str.c_str();
	while ( (found=strstr_hz(pread, before.c_str())) != NULL ) {
		// �Ώە����񒼑O�܂ł��R�s�[
		strncpy(pwrite, pread, found-pread);
		pwrite += found-pread;
		pread = found;

		// �u��������������R�s�[
		strcpy(pwrite, after.c_str());
		pwrite += afterLength;
		pread += beforeLength;
	}
	// �c����R�s�[
	strcpy(pwrite, pread);

	// �ߋ��̗̈������A�o�b�t�@�̎��̂���e�Ƃ���B
	str = buf;
	delete buf;
	return	count;
}

// ���������
bool	erase_first(string& str, const string& before) {
	int	pos = str.find(before);
	if ( pos == string::npos )
		return	false;
	str.erase(pos, before.size());
	return	true;
}
int	erase(string& str, const string& before) {
	if ( str=="" || before=="" ) return 0;

	// �����񒷂̌v�Z�p
	const int		beforeLength = before.size();
	const int		textLength = str.size();

	// �u�������Ώۂ��������邩���J�E���g���Ă���
	const char*	found=str.c_str();
	int	count=0;
	while ( (found=strstr_hz(found, before.c_str())) != NULL ) {
		found += beforeLength;
		++count;
	}
	if ( count==0 ) return 0;

	// ���ԃo�b�t�@���m��
	char*	buf = new char[textLength - beforeLength*count + 1];

	// �������[�v
	const char*	pread = str.c_str();
	char*	pwrite = buf;
	found = str.c_str();
	while ( (found=strstr_hz(pread, before.c_str())) != NULL ) {
		strncpy(pwrite, pread, found-pread);
		pwrite += found-pread;
		pread = found+beforeLength;
	}
	// �c����R�s�[
	strcpy(pwrite, pread);

	// �ߋ��̗̈������A�o�b�t�@�̎��̂���e�Ƃ���B
	str = buf;
	delete buf;
	return	count;
}

// �Ώی��̐��𐔂���
int	count(const string& str, const string& target) {
	const char*	found=str.c_str();
	int	count=0;
	while ( (found=strstr_hz(found, target.c_str())) != NULL ) {
		found += target.size();
		++count;
	}
	return	count;
}


// �t�@�C���̑��݂��m�F
bool	is_exist_file(const string& iFileName) {
	ifstream	in(iFileName.c_str());
	if ( !in.is_open() )
		return	false;
	in.close();
	return	true;
}

bool	strvec_from_file(
	strvec&	o,
	const string& iFileName)
{
	assert(!iFileName.empty());
	ifstream	in(iFileName.c_str());
	if ( !in.is_open() )
		return	false;
	while ( in.peek() != EOF ) {
		// �P�s�ǂݍ���
		stringstream	line;
		int	c;
		while ( (c=in.get()) != '\n' && c!=EOF) {
		    if (c != '\r') {
				line.put(c);
		    }
		}
		o.push_back( line.str() );
	}
	in.close();
	return	true;
}

bool	strvec_to_file(
	const strvec& vec,
	const string& iFileName)
{
	assert(!iFileName.empty());
	ofstream	out(iFileName.c_str());
	if ( !out.is_open() )
		return	false;
	strvec::const_iterator	it;
	for (it=vec.begin() ; it!=vec.end() ; ++it)
		out << *it << endl;
	out.close();
	return	true;
}


bool	strmap_from_file(strmap& o, const string& iFileName, const string& dlmt, const string& front_comment_mark)
{
	ifstream	in(iFileName.c_str());
	if ( !in.is_open() )
		return	false;
	while ( in.peek() != EOF )
	{
		// �P�s�ǂݍ���
		string line;
		int	c;
		while ( (c=in.get()) != '\n' && c!=EOF)
		{
		    if (c != '\r') {
				line += c;
		    }
		}

		if ( line.compare(0, front_comment_mark.size(), front_comment_mark)==0 )
		{
			continue;
		}

		string::size_type pos = line.find(dlmt);
		if ( pos == string::npos )
		{
			o[ line ] = string();
		}
		else
		{
			o[ line.substr(0, pos) ] = line.substr( pos + dlmt.size() );
		}
	}
	in.close();
	return	true;
}

bool	strmap_to_file(const strmap& oMap, const string& iFileName, const string& dlmt)
{
	ofstream	out(iFileName.c_str());
	if ( !out.is_open() )
		return	false;
	strmap::const_iterator	it;
	for (it=oMap.begin() ; it!=oMap.end() ; ++it)
		out << it->first << dlmt << it->second << endl;
	out.close();
	return	true;
}

bool	string_from_file(string& o, const string& iFileName) {
	ifstream	in(iFileName.c_str());
	if ( !in.is_open() )
		return	false;
	/*in.seekg(0, ios::end);
	streampos	size = in.tellg();
	in.seekg(0, ios::beg);
	in.read(out, size);*/
	while (in.peek() != EOF)
		o+=in.get();
	in.close();
	return	true;
}

bool	string_to_file(const string& i, const string& iFileName) {
	ofstream	out(iFileName.c_str());
	if ( !out.is_open() )
		return	false;
	out.write(i.c_str(), i.size());
	out.close();
	return	true;
}


string	get_a_chr(const char*& p) {
	if ( *p=='\0' )
		return	"";
	char	buf[3];
	if ( p[0] == 0xff ) { //�����œ���ȕ\���Ƃ��Ă���
		buf[0]=*p++;
		buf[1]='\0';
	}
	else if ( _ismbblead(p[0]) ) {
		buf[0]=*p++;
		buf[1]=*p++;
		buf[2]='\0';
	}
	else {
		buf[0]=*p++;
		buf[1]='\0';
	}
	return	buf;
}



string	encode(const string& s) {
	const char*	p = s.c_str();
	int	len = s.size();
	string	ret;

	for ( int n=0 ; n<len/2 ; ++n ) {
		ret += p[n];
		ret += p[len-n-1];
	}
	if ( len&1 ) ret += p[len/2];

	return	ret;
}

string	decode(const string& s) {
	const char*	p = s.c_str();
	int	len = s.size();
	string	ret;

	for ( int n=0 ; n<len ; n+=2 ) ret += p[n];
	for ( int n=len-((len&1)?2:1) ; n>=0 ; n-=2 ) ret += p[n];

	return	ret;
}
/*

string	encode(const string& in) {
	int	len = in.size();
	char*	buf = new char[len+1];
	buf[len]='\0';
	
	const char*	i = in.c_str();
	char*	o = buf;

	for ( int n=0 ; n<len/2 ; ++n ) {
		*o++ += i[n];
		*o++ += i[len-n-1];
	}
	if ( len&1 )
		*o++ += i[len/2];

	string	ret = buf;
	delete [] buf;
	return	ret;
}

string	decode(const string& in) {
	int	len = in.size();
	char*	buf = new char[len+1];
	buf[len]='\0';
	
	const char*	i = in.c_str();
	char*	o = buf;

	for ( int n=0 ; n<len ; n+=2 )
		*o++ += i[n];
	for ( n=len-((len&1)?2:1) ; n>=0 ; n-=2 )
		*o++ += i[n];

	string	ret = buf;
	delete [] buf;
	return	ret;
}
*/

const char*	strstr_hz(const char* target, const char* find) {
	int	len=strlen(find);
	const char* p=target;
	while ( *p!='\0' ) {
		if ( strncmp(p, find, len)==0 )
			return	p;
		if ( _ismbblead(*p) )
			p+=2; 
		else
			++p;
	}
	return	NULL;
}

bool	compare_tail(const string& str, const string& tail) {
	const int diff = str.size()-tail.size();
	if ( diff < 0 )
		return	false;
	return	tail.compare(str.c_str()+diff)==0;
}




inline int charactor_to_binary(char c) {
	if ( c>='0' && c<='9' )
		return	c-'0';
	else if ( c>='a' && c<='f' )
		return	c-'a'+10;
	else if ( c>='A' && c<='F' )
		return	c-'A'+10;
	else {
		assert(0);
		return	0;
	}
}

void	string_to_binary(const string& iString, byte* oArray) {
	int	len = iString.size()/2;
	for ( int i=0 ; i<len ; ++i)
		oArray[i] = charactor_to_binary(iString[i*2])*16 + charactor_to_binary(iString[i*2+1]);
}


inline char binary_to_charactor(int b) {
	if ( b<10 )
		return	'0'+b;
	else if ( b<16 )
		return	'a'+(b-10);
	else {
		assert(0);
		return	0;
	}
}

string	binary_to_string(const byte* iArray, int iLength) {
	char*	buf = new char[iLength*2];
	for ( int i=0 ; i<iLength ; ++i) {
		buf[i*2] = binary_to_charactor(iArray[i]/16);
		buf[i*2+1] = binary_to_charactor(iArray[i]%16);
	}
	string	str(buf, iLength*2);
	delete [] buf;
	return	str;
}

void	xor_filter(byte* ioArray, int iLength, byte iXorValue) {
	for ( int i=0 ; i<iLength ; ++i)
		ioArray[i] ^= iXorValue;
}

const char*	find_final_char(const char* str, char c) {
	const char*	last=NULL, *p=str;
	for (;*p; p+=_ismbblead(*p)?2:1)
		if ( *p==c )
			last=p;
	return	last;
}

string	get_folder_name(const string& str) {
	char*	buf = new char[str.size()+1];
	strcpy(buf, str.c_str());
	char*	p = find_final_char(buf, DIR_CHAR);
	string	ret;
	if ( p==NULL )
		ret = "";
	else {
		*p='\0';
		ret = buf;
	}
	delete [] buf;
	return	ret;
}

string	get_file_name(const string& str) {
	const char*	p = find_final_char(str.c_str(), DIR_CHAR);
	return	( p==NULL ) ? str : p+1;
}

string	get_extention(const string& str) {
	const char*	p = find_final_char(str.c_str(), '.');
	return	( p==NULL ) ? "" : p+1;
}

string	set_extention(const string& str, const char* new_ext) {
	const char*	p = find_final_char(str.c_str(), '.');
	if ( p==NULL )
		if ( new_ext==NULL )
			return	str;
		else
			return	str+"."+new_ext;
	else
		if ( new_ext==NULL )
			return	string(str.c_str(), p-str.c_str());
		else
			return	string(str.c_str(), p-str.c_str()+1)+new_ext;
}

string	set_filename(const string& str, const char* new_filename) {
	const char*	p = find_final_char(str.c_str(), DIR_CHAR);
	if ( p==NULL )
		if ( new_filename==NULL )
			return	str;
		else
			return	str+DIR_CHAR+new_filename;
	else
		if ( new_filename==NULL )
			return	string(str.c_str(), p-str.c_str());
		else
			return	string(str.c_str(), p-str.c_str()+1)+new_filename;
}



// .ini�t�@�C����ǂݍ���
bool	inimap::load(const string& iFileName) {
	this->clear();

	// �t�@�C�����J��
	ifstream	in(iFileName.c_str());
	if ( !in.is_open() )
		return	false;
	// ���݂̃Z�N�V�����ւ̃C�e���[�^
	inimap::iterator	theSection = this->end();

	// �e�s�ɑ΂�����
	while ( in.peek() != EOF ) {

		string	str;	// ���̃��[�v�ň����s������

		{// �P�s�ǂݍ����str�Ɋi�[
			stringstream	line;
			int	c;
			while ( (c=in.get()) != '\n' && c!=EOF) {
			    if (c != '\r') {
					line.put(c);
			    }
			}
			str = line.str();
		}

		if ( str.empty() || str[0]==';' ) {
			// ��s�܂��̓R�����g�s
		}
		else if ( str.size()>=2 && str[0]=='[' ) {
			// �Z�N�V�������̐ݒ�s [SectionName]
			string::size_type	end_pos = str.find(']', 1);
			if ( end_pos == string::npos )
				return	false;	// ���J�b�R�̖�����J�b�R�𔭌��A�ُ�Ƃ݂Ȃ�
			string	section_name = str.substr(1, end_pos-1); // �Z�N�V�������擾
			pair<inimap::iterator, bool> result = 
				this->insert( inimap::value_type(section_name, strmap()) ); // map�ɑ}��
			theSection = result.first;	// ���݂̃Z�N�V�������w���C�e���[�^���擾
		}
		else {
			// �ʏ�̍s key=value
			string::size_type	eq_pos = str.find('=', 0);
			if ( eq_pos == string::npos )
				continue;	// []��=��������s�ł��Ȃ����ȍs

			// theSection�����ݒ�̂܂܂����ɗ����Ƃ��͖����̃Z�N�V������ݒ�
			if (theSection == this->end())
			{
				pair<inimap::iterator, bool> result = 
					this->insert( inimap::value_type("", strmap()) ); // map�ɑ}��
				theSection = result.first;	// ���݂̃Z�N�V�������w���C�e���[�^���擾
			}

			// map�ɑ}��
			string	key = str.substr(0, eq_pos);
			string	value = str.substr(eq_pos+1, string::npos);
			theSection->second[key] = value;
		}
	}
	return	true;
}

// .ini�t�@�C���֕ۑ�
bool	inimap::save(const string& iFileName) const {

	// �t�@�C�����J��
	ofstream	out(iFileName.c_str());
	if ( !out.is_open() )
		return	false;

	for (inimap::const_iterator i=this->begin() ; i!=this->end() ; ++i)
	{
		out << "[" << i->first << "]" << endl;	// [SectionName]���o��
		for (strmap::const_iterator j=i->second.begin() ; j!=i->second.end() ; ++j)
			out << j->first << "=" << j->second << endl;	// key=value���o��
	}

	return	true;
}


// .ini�t�@�C����ǂݍ���
bool	inivec::load(const string& iFileName) {
	this->clear();

	// �t�@�C�����J��
	ifstream	in(iFileName.c_str());
	if ( !in.is_open() )
		return	false;
	// ���݂̃Z�N�V�����ւ̃C�e���[�^
	iterator	theSection = this->end();

	// �e�s�ɑ΂�����
	while ( in.peek() != EOF ) {

		string	str;	// ���̃��[�v�ň����s������

		{// �P�s�ǂݍ����str�Ɋi�[
			int	c;
			while ( (c=in.get()) != '\n' && c!=EOF) {
			    if (c != '\r') {
					str += c;
			    }
			}
		}

		if ( str.empty() ) {
			// ��s
		}
		else if ( str.size()>=2 && str[0]=='[' ) {
			// �Z�N�V�������̐ݒ�s [SectionName]
			string::size_type	end_pos = str.find(']', 1);
			if ( end_pos == string::npos )
				return	false;	// ���J�b�R�̖�����J�b�R�𔭌��A�ُ�Ƃ݂Ȃ�
			string	section_name = str.substr(1, end_pos-1); // �Z�N�V�������擾
			
			this->push_back( value_type(section_name, strpairvec()) ); // vector�ɒǉ�
			--(theSection = this->end());	// ���݂̃Z�N�V�������w���C�e���[�^���擾
		}
		else {
			// �ʏ�̍s key=value
			string::size_type	eq_pos = str.find('=', 0);
			if ( eq_pos == string::npos )
				continue;	// []��=��������s�ł��Ȃ����ȍs

			// theSection�����ݒ�̂܂܂����ɗ����Ƃ��͖����̃Z�N�V������ݒ�
			if (theSection == this->end())
			{
				this->push_back( value_type("", strpairvec()) ); // vector�ɒǉ�
				--(theSection = this->end());	// ���݂̃Z�N�V�������w���C�e���[�^���擾
			}
	
			string	key = str.substr(0, eq_pos);
			string	value = str.substr(eq_pos+1, string::npos);
			theSection->second.push_back( strpair(key, value) );
		}
	}
	return	true;
}

// printf�݊��ŕ�����𐶐����Astring�^�ŕԂ��B
#include	<cstdarg>
string	stringf(const char* iFormat, ...) {
	static const int BUF_SIZE = 4096;
	char	buf[BUF_SIZE];
	va_list	argptr;
	va_start(argptr, iFormat);
#ifdef POSIX
#  define _vsnprintf vsnprintf
#endif
	_vsnprintf(buf, BUF_SIZE-1, iFormat, argptr);
	va_end(argptr);
	return	buf;
}

