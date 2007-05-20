#include	"Parameter.h"
#include	<fstream>
#include	<strstream>
#include	<string>
using namespace std;


Parameter::Parameter() : mData() {
	clear(); 
}
Parameter::~Parameter() {
	mData.clear(); 
}
void	Parameter::clear() { 
	mData.clear(); 
	mData[""] = strmap();
	mSection = &(mData[""]);
}

// �t�@�C���ǂ�
bool	Parameter::loadFile(const char* iFileName) {

	ifstream	in(iFileName);
	if ( !in.is_open() )
		return	false;
	clear();

	while ( in.peek() != EOF ) {
		// �P�s�ǂݍ���
		strstream	line;
		int	c;
		while ( (c=in.get()) != '\n' && c!=EOF)
			line.put(c);
		line.put('\0');

		// �s�X�g���[�����Œ�A�e�s�ɑ΂�����
		processLine(line.str());
		// �s�X�g���[���̌Œ������
		line.rdbuf()->freeze(0);
	}
	in.close();

	// �����Z�N�V������I��ł���
	selectSection("");
	return	true;
}

// �f���~�^��΂�
static inline void skipDelimiter(const char*& p) {
	while ( *p==' ' || *p=='\t' || *p==',' )
		++p;
}
// �f���~�^��΂��A������ǂݍ��݁A�f���~�^��΂�
static inline string readString(const char*& p) {
	skipDelimiter(p);
	strstream	word;
	while ( *p!=' ' && *p!='\t' && *p!=',' && *p!='\0')
		word.put(*p++);
	word.put('\0');
	string	str = word.str();
	word.rdbuf()->freeze(0);
	skipDelimiter(p);
	return	str;
}

// �ǂݍ��񂾃t�@�C���̊e�s������
void	Parameter::processLine(char* p) {

	// �R�����g ; �ȍ~�͏���
	char*	comment_mark = strchr(p, ';');
	if ( comment_mark != NULL )
		*comment_mark = '\0';

	// ��s�`�F�b�N
	skipDelimiter(p);
	if ( *p=='\0' )
		return;	

	// �擪�̒P����擾
	string	theFront = readString(p);

	if ( *p=='\0' ) {
		// ��P�ꂾ���Ȃ�u�Z�N�V�������v
		map<string, strmap>::iterator i = mData.find(theFront);
		if ( i == mData.end() ) {
			mData[theFront] = strmap();
			i = mData.find(theFront);
		}
		mSection = &(i->second);
	}
	else {
		// �����̒P�ꂪ����΁A�擪�P����L�[�A�ȍ~��l�Ƃ���
		(*mSection)[theFront] = p;
	}
}

// �Z�N�V�����I���
bool	Parameter::selectSection(const char* iSection) {
	assert(iSection != NULL);
	map<string, strmap>::iterator i = mData.find(iSection);
	if ( i == mData.end() )
		return	false;	// �Z�N�V���������݂��Ȃ�

	mSection = &(i->second);
	return	true;
}


// �l���擾
#define	get_value_common	\
	assert(iKey != NULL);	\
	strmap::iterator i = mSection->find(iKey);	\
	if ( i==mSection->end() )	\
		return	false;	\
	else NULL \

#define	null_false	if ( *p=='\0' ) return false; else NULL

bool	Parameter::getValue(const char* iKey, int& oInt) {
	get_value_common;
	oInt = atoi(i->second.c_str());
	return	true;
}

bool	Parameter::getValue(const char* iKey, string& oString) {
	get_value_common;
	oString = i->second;
	return	true;
}

bool	Parameter::getValue(const char* iKey, CRect& oRect) {
	get_value_common;
	const char* p = i->second.c_str();
	oRect.l = atoi(readString(p).c_str()); null_false;
	oRect.t = atoi(readString(p).c_str()); null_false;
	oRect.r = atoi(readString(p).c_str()); null_false;
	oRect.b = atoi(readString(p).c_str());
	return	true;
}

bool	Parameter::getValue(const char* iKey, CSize& oSize) {
	get_value_common;
	const char* p = i->second.c_str();
	oSize.w = atoi(readString(p).c_str()); null_false;
	oSize.h = atoi(readString(p).c_str());
	return	true;
}

bool	Parameter::getValue(const char* iKey, CPoint& oPoint) {
	get_value_common;
	const char* p = i->second.c_str();
	oPoint.x = atoi(readString(p).c_str()); null_false;
	oPoint.y = atoi(readString(p).c_str());
	return	true;
}

bool	Parameter::getValue(const char* iKey, Color24& oColor) {
	get_value_common;
	const char* p = i->second.c_str();
	oColor.R(atoi(readString(p).c_str())); null_false;
	oColor.G(atoi(readString(p).c_str())); null_false;
	oColor.B(atoi(readString(p).c_str()));
	return	true;
}
