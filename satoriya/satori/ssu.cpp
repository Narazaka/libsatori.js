//------------------------------------------------
//
//	���X�������[�e�B���e�B���C�u�����@ssu.dll
//
#include	"SaoriHost.h"
#include    "random.h"
#include	<map>
#include	<algorithm>
#include	<time.h>

class ssu : public SaoriHost {
public:
	ssu() {
		randomize(time(NULL));
	}
	virtual SRV		request(deque<string>& iArguments, deque<string>& oValues);
};
SakuraDLLHost* SakuraDLLHost::m_dll = new ssu;


static SRV	call_ssu(string iCommand, deque<string>& iArguments, deque<string>& oValues)
{
	// ���O�Ɩ��߂��֘A�t����map
	typedef SRV (*Command)(deque<string>&, deque<string>&);
	static map<string, Command>	theMap;
	if ( theMap.empty() )
	{ 
		// ���񏀔�
		#define	d(iName)	\
			SRV	_##iName(deque<string>&, deque<string>&); \
			theMap[ #iName ] = _##iName
		// ���߈ꗗ�̐錾�Ɗ֘A�t���B
		d(calc);			d(calc_float);		d(if);				d(unless);
		d(nswitch);			d(switch);			d(iflist);			d(substr);
		d(split);			d(join);			d(replace);			d(replace_first);	d(erase);
		d(erase_first);		d(count);			d(compare);			d(compare_head);
		d(compare_tail);	d(length);			d(is_empty);		d(is_digit);
		d(is_alpha);		d(zen2han);			d(han2zen);			d(hira2kata);
		d(kata2hira);		d(sprintf);			d(reverse);			d(at);
		d(choice);
		d(lsimg);			d(mkdir);
		#undef	d
	}

	// ���߂̑��݂��m�F
	map<string, Command>::iterator i = theMap.find(iCommand);
	if ( i==theMap.end() )
		return SRV(400, string()+"Error: '"+iCommand+"'�Ƃ������O�̖��߂͒�`����Ă��܂���B");

	// ���ۂɌĂ�
	return	i->second(iArguments, oValues);
}

SRV	ssu::request(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	SRV(400, "���߂��w�肳��Ă��܂���");

	// �ŏ��̈����͖��ߖ��Ƃ��Ĉ���
	string	theCommand = iArguments.front();
	iArguments.pop_front();
	return	call_ssu(theCommand, iArguments, oValues);
}


// �����������

#ifdef POSIX
#  include      "../_/Utilities.h"
#  include <sys/stat.h>
#  include <sys/types.h>
#else
#  include	<windows.h>
#  include	<mbctype.h>
#endif
#include	"../_/stltool.h"

/* �u�\�v��2�o�C�g�ڂ́u\�v�ł���̂ŁA�G�X�P�[�v����K�v������B */
//static const char	zen[] = 
//	"�@�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y����������������������������������������������������"
//	"�O�P�Q�R�S�T�U�V�W�X�I�h���������f�i�j���`�b�e�o�{���p�����H�Q�[�O�����u�G�F�v�A�B�E���~�|�C�D�m�n"
//	"�A�C�E�G�I�J�L�N�P�R�T�V�X�Z\x83\x5c�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z�}�~�����������������������������@�B�D�F�H�������b�J�K�A�B";
//static const char	han[] = 
//	" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
//	"0123456789!\"#$%&'()=~|`{+*}<>?_-^\\@[;:],.�/*-,.[]"
//	"�������������������������������������������ܦݧ���������ߤ�";
static const char	kata[] = "�A�C�E�G�I�J�L�N�P�R�T�V�X�Z\x83\x5c�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z�}�~���������������������������������@�B�D�F�H���������b�K�M�O�Q�S�U�W�Y�[�]�_�a�d�f�h�o�r�u�x�{�p�s�v�y�|";
static const char	hira[] = "�����������������������������������ĂƂȂɂʂ˂̂͂Ђӂւق܂݂ނ߂���������������񂟂��������������������������������������Âłǂ΂тԂׂڂς҂Ղ؂�";

//���p�S�p�ϊ��e�[�u��
static const char	zen_alpha[] = "�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y����������������������������������������������������";
static const char	han_alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

static const char	zen_digit[] = "�O�P�Q�R�S�T�U�V�W�X";
static const char	han_digit[] = "0123456789";

static const char   zen_symbol[] = "�@�I�h���������f�i�j���`�b�e�o�{���p�����H�Q�[�O�����u�G�F�v�A�B�E���~�|�C�D�m�n";
static const char   han_symbol[] = " !\"#$%&'()=~|`{+*}<>?_-^\\@[;:],.�/*-,.[]";

static const char   zen_kana_1[] = "�A�C�E�G�I�J�L�N�P�R�T�V�X�Z\x83\x5c�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z�}�~�����������������������������@�B�D�F�H�������b�A�B";
static const char   han_kana_1[] = "�������������������������������������������ܦݧ����������";

static const char   zen_kana_2[] = "�K�M�O�Q�S�U�W�Y�[�]�_�a�d�f�h�o�r�u�x�{�p�s�v�y�|";
static const char   han_kana_2[] = "�޷޸޹޺޻޼޽޾޿�������������������������������";

extern	bool calc(string& ioString);
extern	bool calc_float(string& ioString);

static string zen2han_internal(string &str,unsigned int flag = 0xffffU);

#include	<sstream>

// ���p/�S�p�𓯓��Ɉ�������ŕ�������Ԃ�
int	sjis_strlen(const char* p) {
	int	n=0;
	for (int i=0 ; p[i] != '\0' ; i += _ismbblead(p[i]) ? 2 : 1 )
		++n;
	return	n;
}

// ���p/�S�p�𓯓��Ɉ��������n�����ړ��A���ߎ���NULL
const char*	sjis_at(const char* p, int n) {
	for (int i=0 ; i<n ; ++i) {
		if ( *p == '\0' )
			return	NULL;
		p += _ismbblead(*p) ? 2 : 1;
	}
	return	p;
}

bool	printf_format(const char*& p, deque<string>& iArguments, stringstream& os)
{
	assert(*p=='%');
	if ( iArguments.empty() )
		return	false;	// �u�������Ώۂ�����

	++p;
	string	str = zen2han_internal(iArguments.front());
	iArguments.pop_front();

	// �t���O�w��ǂݍ���
	bool isSharp=false;

	while (true) {
		if ( *p == '-' ) { os << left; ++p; }
		else if ( *p == '+' ) { os << showpos; ++p; }
		else if ( *p == '0' ) { os.fill('0'); os<<internal; ++p; }
		else if ( *p == ' ' ) { os.fill(' '); os<<internal; ++p; }
		else if ( *p == '#' ) { isSharp = true; ++p; }
		else break;
	}

	// ���w��ǂݍ���
	int	width=0;
	bool	isReadWidth = false;
	if ( *p=='*' ) {
		isReadWidth = true;
		++p;
	} else {
		while ( *p>='0' && *p<='9' ) {
			width = width*10 + (*p - '0');
			++p;
			os.width(width);
		}
	}

	// ���x�w��ǂݍ���
	int	precision = 0;
	if ( *p == '.' ) {
		++p;
		while ( *p>='0' && *p<='9' ) {
			precision = precision*10 + (*p - '0');
			++p;
		}
		os.precision(precision);
	}

	// �t�H�[�}�b�g�ݒ� - #�t���O
	if ( isSharp ) {
		switch (*p) {
			case 'o':
			case 'x':
			case 'X':
				os << showbase;
				break;
			case 'e':
			case 'E':
			case 'f':
			case 'g':
			case 'G':
				os << showpoint;
				break;
		}
	}
	

	// �ϊ������ɉ����đ}��
	switch (*p) {
	case 's':
	case 'S':
		{
			os << str;
			break;
		}
	case 'c':
	case 'C':
		{
			os << (char)zen2int(str);
			break;
		}
	case 'd':
		{
			os << zen2int(str);
			break;
		}
	case 'i':
		{
			os << oct << zen2int(str);
			break;
		}
	case 'o':
		{
			os << oct << strtoul(zen2han(str).c_str(),NULL,10);
			break;
		}
	case 'u':
		{
			os << strtoul(zen2han(str).c_str(),NULL,10);
			break;
		}
	case 'x':
		{
			os << hex << nouppercase << strtoul(zen2han(str).c_str(),NULL,10);
			break;
		}
	case 'X':
		{
			os << hex << uppercase << strtoul(zen2han(str).c_str(),NULL,10);
			break;
		}
	case 'e':
		{
			os << scientific << nouppercase << strtod(zen2han(str).c_str(),NULL);
			break;
		}
	case 'E':
		{
			os << scientific << uppercase << strtod(zen2han(str).c_str(),NULL);
			break;
		}
	case 'g':
		{
			os << scientific << fixed << nouppercase << strtod(zen2han(str).c_str(),NULL);
			break;
		}
	case 'G':
		{
			os << scientific << fixed << uppercase << strtod(zen2han(str).c_str(),NULL);
			break;
		}
	case 'f':
		{
			os << fixed << strtod(zen2han(str).c_str(),NULL);
			break;
		}
	case 'n': break;
	case 'p': break;
	default: return false;
	}
	++p;
	return	true;
}

string	sprintf(deque<string>& iArguments) {
	stringstream s;
	string	str = iArguments.front();
	iArguments.pop_front();
	const char* p = str.c_str();
	while ( *p!='\0' ) {
		if ( *p=='%' ) {
			stringstream sf;
			if ( printf_format(p, iArguments, sf) ) {
				s << sf.str();
				continue;
			}
		}
		if ( _ismbblead(*p) ) {
			s << *p++; s << *p++;
		} else {
			s << *p++;
		}
	}
	return	s.str();
}


SRV _calc(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");
	string	exp = iArguments[0];
	if ( !calc(exp) )
		return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B"); // �u�\�v��2�o�C�g�ڂ́u\�v
	return	exp;
}

SRV _calc_float(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");
	string	exp = iArguments[0];
	if ( !calc_float(exp) )
	    return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B");
	return	exp;
}

SRV _if(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 || iArguments.size()>3 )
		return	SRV(400, "�����̌�������������܂���B");
	string	exp = iArguments[0];
	if ( !calc(exp) )
		return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B");
	if ( exp!="0" )
		return	iArguments[1];	// �^
	else
		if ( iArguments.size()==3 )
			return	iArguments[2];	// �U
		else
			return	SRV(204);	// �U��else�Ȃ�
}

SRV _unless(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 || iArguments.size()>3 )
		return	SRV(400, "�����̌�������������܂���B");
	string	exp = iArguments[0];
	if ( !calc(exp) )
		return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B");
	if ( exp=="0" )
		return	iArguments[1];	// �U
	else
		if ( iArguments.size()==3 )
			return	iArguments[2];	// �^
		else
			return	SRV(204);	// �^��else�Ȃ�
}

SRV _nswitch(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 )
		return	SRV(400, "����������܂���B");

	int	n = zen2int(iArguments[0]);
	//iArguments.pop_front();
	//if ( iArguments.size()>n )
	if ( n>0 && iArguments.size()>n )
		return	SRV(200, iArguments[n]);
	else
		return	SRV(204);
}

SRV _switch(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 )
		return	SRV(400, "����������܂���B");

	const string	lhs = iArguments[0];
	const int max = iArguments.size();
	for (int i=1 ; i<max ; i+=2) {
		if ( i==max-1 ) // ��������̏ꍇ�A�Ō�̂P��else��
			return	SRV(200, iArguments[i]);
		string	exp = string("(") + lhs + ")==(" + iArguments[i] + ")";
		if ( !calc(exp) )
			return	SRV(400, string()+"switch��"+itos((i-1)/2+1)+"�ځA�� '"+exp+"' �͌v�Z�s\x94\x5c�ł����B");
		if ( exp!="0" )
			return	SRV(200, iArguments[i+1]);
	}
	return	SRV(204);
}

SRV _iflist(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 )
		return	SRV(400, "����������܂���B");

	const string	lhs = iArguments[0];
	const int max = iArguments.size();
	for (int i=1 ; i<max ; i+=2) {
		if ( i==max-1 ) // ��������̏ꍇ�A�Ō�̂P��else�����B�����܂ł����疳�����ł����Ԃ��B
			return	SRV(200, iArguments[i]);
		string	exp = lhs + iArguments[i];
		if ( !calc(exp) )
			return	SRV(400, string()+"iflist��"+itos((i-1)/2+1)+"�ځA�� '"+exp+"' �͌v�Z�s\x94\x5c�ł����B");
		if ( exp!="0" )
			return	SRV(200, iArguments[i+1]);
	}
	return	SRV(204);
}


SRV _substr(deque<string>& iArguments, deque<string>& oValues) {

	if ( iArguments.size()<1 )
		return	SRV(400, "����������܂���B");

	// �Ώە�����
	const char* p = iArguments[0].c_str();
	if ( iArguments.size()==1 )
		return	SRV(200, p); // �����P�Ȃ�S�̂�Ԃ�

	const int	len = sjis_strlen(p);

	// �n�_
	int	start = zen2int(iArguments[1]);
	if ( start < 0 )
		start = len + start;

	// �n�_����̃I�t�Z�b�g�l
	int offset = (iArguments.size()<=2) ? len : zen2int(iArguments[2]);
	if ( offset==0 || offset==INT_MIN ) // INT_MIN�̎��͕������]�������Ȃ��̂�0�����B
		return	SRV(204);
	if ( offset<0 ) {
		start += offset;
		offset = -offset;
	}
	assert(offset >= 0 );

	if ( start < 0 )
		start = 0;
	if ( start >= len )
		return	SRV(204);
	if ( start + offset >= len )
		offset = len - start;

	const char* const start_p = sjis_at(p, start);
	const char* const end_p = sjis_at(start_p, offset);
	return	SRV(200, string(start_p, end_p));
}

SRV _split(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 || iArguments.size()>3 )
		return	SRV(400, "�����̌�������������܂���B");

	strvec	vec;
	if ( iArguments.size()==1 ) {
		split(iArguments[0],vec);
	}
	else if ( iArguments.size()==2 ) {
		split(iArguments[0],iArguments[1],vec);
	}
	else {
		split(iArguments[0],iArguments[1],vec,zen2int(iArguments[2]));
	}

	for ( strvec::iterator i=vec.begin() ; i!=vec.end() ; ++i )
		oValues.push_back(*i);
	return	SRV(200, itos(vec.size()));
}

SRV _join(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	SRV(400, "�����̌�������������܂���B");

	string	r = iArguments[1];
	for (int n=2 ; n<iArguments.size() ; ++n)
		r += iArguments[0] + iArguments[n];
	return	r;
}

SRV _replace(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=3 )
		return	SRV(400, "�����̌�������������܂���B");
	replace(iArguments[0], iArguments[1], iArguments[2]);
	return	SRV(200, iArguments[0]);
}

SRV _replace_first(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=3 )
		return	SRV(400, "�����̌�������������܂���B");
	replace_first(iArguments[0], iArguments[1], iArguments[2]);
	return	iArguments[0];
}

SRV _erase(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	erase(iArguments[0], iArguments[1]);
	return	iArguments[0];
}

SRV _erase_first(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	erase_first(iArguments[0], iArguments[1]);
	return	iArguments[0];
}

SRV _count(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	return	itos( count(iArguments[0], iArguments[1]) );
}

SRV _compare(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	return	(strcmp(iArguments[0].c_str(), iArguments[1].c_str())==0) ? "1" : "0";
}

SRV _compare_head(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	return	compare_head(iArguments[0], iArguments[1]) ? "1" : "0";
}

SRV _compare_tail(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	return	compare_tail(iArguments[0], iArguments[1]) ? "1" : "0";
}

SRV _length(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	"0";
	return	itos( sjis_strlen(iArguments[0].c_str()) );
}

SRV _is_empty(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	"1";
	if ( iArguments[0].empty() )
		return	"1";
	else
		return	"0";
}

SRV _is_digit(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 || iArguments[0].empty() )
		return	"0";
	int	i;
	const char* p = iArguments[0].c_str();
	int step = 1;

	static const char zen_pm[] = "�{�|";

	if ( p[0] == '-' || p[0] == '+' ) {
		p += 1;
	}
	else if ( (p[0] == zen_pm[0] && p[1] == zen_pm[1]) || (p[0] == zen_pm[2] && p[1] == zen_pm[3]) ) {
		p += 2;
	}

	if ( *p == 0 ) { return "0"; }

	for ( ; *p ; p += step ) {
		for ( i=0 ; i<20 ; i+=2) {
			if ( p[0]==zen_digit[i] && p[1]==zen_digit[i+1] ) {
				break;
			}
		}
		if ( i<20 ) {
			step = 2;
			continue;
		}

		for ( i=0 ; i<10 ; ++i) {
			if ( p[0]==han_digit[i] ) {
				break;
			}
		}
		if ( i<10 ) {
			step = 1;
			continue;
		}

		return	"0";
	}
	return	"1";
}

SRV _is_alpha(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 || iArguments[0].empty() )
		return	"0";
	return	arealphabets(iArguments[0]) ? "1" : "0";
}

SRV _zen2han(deque<string>& iArguments, deque<string>& oValues) {
	if ( ! iArguments.size() )
		return	SRV(400, "�����̌�������������܂���B");

	unsigned int flag = 0xffff;
	if ( iArguments.size() >= 2 ) {
		flag = 0;
		if ( iArguments[1].find("�A���t�@�x�b�g") != string::npos ) {
			flag |= 0x1;
		}
		if ( iArguments[1].find("����") != string::npos ) {
			flag |= 0x2;
		}
		if ( iArguments[1].find("�L��") != string::npos ) {
			flag |= 0x4;
		}
		if ( iArguments[1].find("�J�i") != string::npos ) {
			flag |= 0x8;
		}
	}
	return zen2han_internal(iArguments[0],flag);
}

string zen2han_internal(string &str,unsigned int flag)
{
	char	before[3]="�@", after[2]=" ";

	if ( flag & 0x1 ) { //�A���t�@�x�b�g
		for (int n=0 ; n<sizeof(han_alpha) ; ++n) {
			before[0]=zen_alpha[n*2];
			before[1]=zen_alpha[n*2+1];
			after[0]=han_alpha[n];
			replace(str, before, after);
		}
	}
	if ( flag & 0x2 ) { //����
		for (int n=0 ; n<sizeof(han_digit) ; ++n) {
			before[0]=zen_digit[n*2];
			before[1]=zen_digit[n*2+1];
			after[0]=han_digit[n];
			replace(str, before, after);
		}
	}
	if ( flag & 0x4 ) { //�L��
		for (int n=0 ; n<sizeof(han_symbol) ; ++n) {
			before[0]=zen_symbol[n*2];
			before[1]=zen_symbol[n*2+1];
			after[0]=han_symbol[n];
			replace(str, before, after);
		}
	}
	if ( flag & 0x8 ) { //�J�i
		int n;
		char after2[3] = "  ";
		for (n=0 ; n< (sizeof(han_kana_2)/2) ; ++n) {
			before[0]=zen_kana_2[n*2];
			before[1]=zen_kana_2[n*2+1];
			after2[0]=han_kana_2[n*2];
			after2[1]=han_kana_2[n*2+1];
			replace(str, before, after2);
		}
		for (n=0 ; n<sizeof(han_kana_1) ; ++n) {
			before[0]=zen_kana_1[n*2];
			before[1]=zen_kana_1[n*2+1];
			after[0]=han_kana_1[n];
			replace(str, before, after);
		}
	}
	return	str;
}

SRV _han2zen(deque<string>& iArguments, deque<string>& oValues) {
	if ( ! iArguments.size() )
		return	SRV(400, "�����̌�������������܂���B");

	unsigned int flag = 0xffff;
	if ( iArguments.size() >= 2 ) {
		flag = 0;
		if ( iArguments[1].find("�A���t�@�x�b�g") != string::npos ) {
			flag |= 0x1;
		}
		if ( iArguments[1].find("����") != string::npos ) {
			flag |= 0x2;
		}
		if ( iArguments[1].find("�L��") != string::npos ) {
			flag |= 0x4;
		}
		if ( iArguments[1].find("�J�i") != string::npos ) {
			flag |= 0x8;
		}
	}

	char	before[2]=" ", after[3]="  ";
	string&	str=iArguments[0];

	if ( flag & 0x1 ) { //�A���t�@�x�b�g
		for (int n=0 ; n<sizeof(han_alpha) ; ++n) {
			before[0]=han_alpha[n];
			after[0]=zen_alpha[n*2];
			after[1]=zen_alpha[n*2+1];
			replace(str, before, after);
		}
	}
	if ( flag & 0x2 ) { //����
		for (int n=0 ; n<sizeof(han_digit) ; ++n) {
			before[0]=han_digit[n];
			after[0]=zen_digit[n*2];
			after[1]=zen_digit[n*2+1];
			replace(str, before, after);
		}
	}
	if ( flag & 0x4 ) { //�L��
		for (int n=0 ; n<sizeof(han_symbol) ; ++n) {
			before[0]=han_symbol[n];
			after[0]=zen_symbol[n*2];
			after[1]=zen_symbol[n*2+1];
			replace(str, before, after);
		}
	}
	if ( flag & 0x8) { //�J�i
		int n;
		char before2[3] = "  ";
		for (n=0 ; n<(sizeof(han_kana_2)/2) ; ++n) {
			before2[0]=han_kana_2[n*2];
			before2[1]=han_kana_2[n*2+1];
			after[0]=zen_kana_2[n*2];
			after[1]=zen_kana_2[n*2+1];
			replace(str, before2, after);
		}
		for (n=0 ; n<sizeof(han_kana_1) ; ++n) {
			before[0]=han_kana_1[n];
			after[0]=zen_kana_1[n*2];
			after[1]=zen_kana_1[n*2+1];
			replace(str, before, after);
		}
	}

	return	str;
}

SRV _hira2kata(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");

	string&	str=iArguments[0];
	for (int i=0 ; str[i]!='\0' ; i+=_ismbblead(str[i])?2:1) {
		for (int j=0 ; j<sizeof(hira) ; j+=2) {
			if ( str[i]==hira[j] && str[i+1]==hira[j+1] ) {
				str[i]=kata[j];
				str[i+1]=kata[j+1];
			}
		}
	}
	return	iArguments[0];
}

SRV _kata2hira(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");

	string&	str=iArguments[0];
	for (int i=0 ; str[i]!='\0' ; i+=_ismbblead(str[i])?2:1) {
		for (int j=0 ; j<sizeof(hira) ; j+=2) {
			if ( str[i]==kata[j] && str[i+1]==kata[j+1] ) {
				str[i]=hira[j];
				str[i+1]=hira[j+1];
			}
		}
	}
	return	iArguments[0];
}

SRV _sprintf(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.empty() )
		return	SRV(400, "����������܂���B");
	return	sprintf(iArguments);
}

SRV _reverse(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.empty() )
		return	SRV(400, "����������܂���B");

	string	r;
	const char* p = iArguments[0].c_str();
	while (*p != '\0') {
		const int len = _ismbblead(*p)?2:1;
		r = string(p, len) + r;
		p += len;
	}

	return	r;
}

SRV _at(deque<string>& iArguments, deque<string>& oValues) {

	if ( iArguments.size()==2 ) {
		const char* p = sjis_at(iArguments.at(0).c_str(), zen2int(iArguments.at(1)));
		return	(p==NULL || *p=='\0') ? "" : string(p, _ismbblead(*p)?2:1);
	}
	//else if ( iArguments.size()==3 ) {
	//}
	else
		return	SRV(400, "����������������܂���B");
}



/*
if ( compare_head(theCommand, "tm") ) {
	string	TimeCommands(const string& iCommand, const deque<string>& iArguments);
	return	TimeCommands(theCommand, iArguments);
}
*/
SRV _choice(deque<string>& iArguments, deque<string>& oValues)
{
	if ( iArguments.size()==0 )
	{
		return	"";
	}
	return iArguments[ random(iArguments.size()) ];
}

SRV _lsimg(deque<string>& iArguments, deque<string>& oValues)
{
	if (iArguments.size() == 0)
		return "0";
#ifdef WIN32
	WIN32_FIND_DATA wfd;
	string d(iArguments[0]);
	if (!compare_tail(d, "\\")) d += '\\';
	d += '*';
	HANDLE h = FindFirstFile(d.c_str(), &wfd);
	if (h == INVALID_HANDLE_VALUE)
		return "0";
	do {
		string lo(wfd.cFileName);
		transform(lo.begin(), lo.end(), lo.begin(), tolower);
		if (compare_tail(lo, ".png") ||
			compare_tail(lo, ".jpg") ||
			compare_tail(lo, ".jpe") ||
			compare_tail(lo, ".jpeg") ||
			compare_tail(lo, ".bmp"))
		{
			oValues.push_back(wfd.cFileName);
		}
	} while (FindNextFile(h, &wfd));
	FindClose(h);
	char s[32];
	sprintf(s, "%d", oValues.size());
	return s;
#else
	// TODO ���ꂩ�Ȃ�Ƃ�����
#endif
}

#ifdef WIN32
static inline int mkdir(const char* path, int mode)
{
	return !CreateDirectory(path, NULL);
}
#endif
SRV _mkdir(deque<string>& iArguments, deque<string>& oValues)
{
	if (iArguments.size() == 0)
		return "";
	if (mkdir(iArguments[0].c_str(), 0777) == 0)
		return "1";
	else
		return "0";
}
