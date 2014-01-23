#include	"stltool.h"
#include	"simple_stack.h"
#ifdef POSIX
#  include      "Utilities.h"
#else
#  include	<mbctype.h>
#endif

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

// ���l�E�����񉉎Z���s���Č��ʂ�String�ɁB�Ԓlfalse�Ȃ玮���ρB
// �Q�����Z�q +,-,*,/,%,^,<,>,<=,>=,==,!=,&&,||,=~,!~
// �P�����Z�q +,-,!
// �퉉�Z�q �����l�A�J�b�R�A������

// ���l�͑S�ĉ��Z�\�A��������������Z�͈ȉ��̂݁B�������A���l�͕�����փL���X�g�����
// ������ != ������
// ������ == ������
// ������ + ������
// ������ - ������@����
// ������ < ������@������r
// ������ > ������@������r
// ������ <= ������@������r
// ������ >= ������@������r
// ������ * ���l�@
// ������ =~ ������
// ������ !~ ������

// �S�Ĕ��p�ł��邱�ƁB�󔒓��͔F�߂Ȃ�
extern bool calc(const char* iExpression, string& oResult,bool isStrict);
// ���p�S�p�X�y�[�X�ƃ^�u�L���̏����A�����E�L���̔��p���܂őS�����������
extern	bool calc(string& ioString,bool isStrict = false);


struct calc_element {
	string	str;
	int		priority;
	calc_element(string _str, int _priority) : str(_str), priority(_priority) {}
	calc_element() : str(), priority(0) {}
};



// �g�p�\���Z�q���H�@�����Ȃ璷���i1or2�j���A�����Ȃ��� 0 ��Ԃ��B
inline int check_operator(const char* p) {

	static const char*	oprs[] = { // �������̏��ɔ�r����́B
		"&&","||","==","!=","<=",">=","=~","!~","<",">","+","-","*","/","%"};
	static const int	num_oprs = sizeof(oprs)/sizeof(oprs[0]);

	for (int i=0 ; i<num_oprs ; ++i) {
		int len=strlen(oprs[i]);
		if ( strncmp(p, oprs[i], len) == 0 )
			return	len;
	}
	return	0;	// �ǂ̉��Z�q�ł��Ȃ�
}



inline bool my_isdigit(int c) {
	return	( c>='0' && c<='9' );
}

// ���l���H�@�����Ȃ璷�����A�����Ȃ��� 0 ��Ԃ��B
inline int check_number(const char* start_pos) {
	const char* p=start_pos;
	while ( my_isdigit(*p) )
		++p;
	int	len = p-start_pos;

	if ( len==0 )
		return	0;	// �ŏ�����Ⴄ
	if ( *p=='\0' || *p==')' )
		return	len;	// ���l�ō����I����Ă�Ȃ�true
	if ( check_operator(p)>0 )
		return	len;	// �������Z�q�ł��n�j
	return	0;	// �����łȂ��Ȃ�A����͕����񂾂낤�B
}


static bool	make_array(const char*& p, std::vector<calc_element>& oData) {


	while (true) {

		// �퉉�Z�q�܂��͒P�����Z�q���擾
		int	len;

		if ( *p == '(' ) {
			oData.push_back( calc_element("(", 110) );
			if ( !make_array(++p, oData) )	// �J�b�R�����ċA����
				return	false;	// �G���[�̓g�b�v�܂œ`����
			if ( *p++ !=')' )
				return	false;
			oData.push_back( calc_element(")", 10) );
		}
		else if (*p=='!' || *p=='+' || *p=='-') {	// �P�����Z�q
			oData.push_back( calc_element(string(p++, 1), 90) );
			continue;
		}
		else if ( (len=check_number(p))!=0 ) {	// �퉉�Z�q�i���l�j
			oData.push_back( calc_element(string(p,len), 100) );
			p+=len;
		}
		else {	// �퉉�Z�q�i������j
			//return	false;	// ������L���ɂ���ƕ����񉉎Z�����Ȃ��B���킹�悤�Ƃ���ƃG���[

			// ���Z�qor�I���܂őS�Ă𕶎���ƌ��Ȃ�
			const char*	start=p;
			while (*p!='\0' && *p!=')') {
				if (*p=='!' || *p=='+' || *p=='-' )
					break;
				if ( check_operator(p) )
					break;
				p += _ismbblead(*p) ? 2 : 1;
			}
			if (p==start)
				return	false;	// �퉉�Z�q���K�v�Ȃ̂ɁA�Ȃ��B

			oData.push_back( calc_element(string(start,p-start), 100) );
		}

		// ���̏I���B�퉉�Z�q�̌�ł��邱�̏ꏊ�ł̂ݐ���E�o
		if ( *p=='\0' || *p==')' )
			return	true;

		// �Q�����Z�q���擾
		if ( (len=check_operator(p))==0 )
			return	false;	// �ǂ̉��Z�q�ł��Ȃ�
		string	str(p,len);
		p+=len;

		// ���Z�q�ɉ����ėD��x��ݒ�
		int	priority;
		if ( str=="^" ) { priority=80; }
		else if ( str=="=~" || str=="!~" ) { priority=75; } // �p�^�[���}�b�`
		else if ( str=="*" || str=="/" || str=="%" ) { priority=70; }
		else if ( str=="+" || str=="-" ) { priority=60; }
		else if ( str=="<" || str==">" || str=="<=" || str==">=" ) { priority=50; }
		else if ( str=="==" || str=="!=" ) { priority=45; }
		else if ( str=="&&" ) { priority=40; }
		else if ( str=="||" ) { priority=35; }
		else return	false;

		oData.push_back( calc_element(str, priority) );
	}
}

// �Q�����Z�i���l�̂ݗp�j
#define	a_op_b(op)	\
	else if ( el.str == #op ) {	\
		assert(stack.size()>=2); \
		if ( !aredigits(stack.from_top(0)) || !aredigits(stack.from_top(1)) ){ return false; }\
		int	result = stoi(stack.from_top(1)) op stoi(stack.from_top(0)); \
		stack.pop(2); stack.push(itos(result)); }

// �Q�����Z�istring�Ƃ��Ĉ��� != �� == �p�j
#define	even_a_op_b(op)	\
	else if ( el.str == #op ) {	\
		assert(stack.size()>=2); \
		int	result = stack.from_top(1) op stack.from_top(0); \
		stack.pop(2); stack.push(itos(result)); }

// �Q�����Z�istring�Ƃ��Ĉ��� != �� == �p�j
#define	length_a_op_b(op)	\
	else if ( el.str == #op ) {	\
		assert(stack.size()>=2); \
		if ( aredigits(stack.from_top(0)) && aredigits(stack.from_top(1)) ){\
			int	result = stoi(stack.from_top(1)) op stoi(stack.from_top(0)); \
			stack.pop(2); stack.push(itos(result)); \
		} else {\
			int	result = (stack.from_top(1)).size() op (stack.from_top(0)).size(); \
			stack.pop(2); stack.push(itos(result)); \
		} \
	}


static bool	calc_polish(simple_stack<calc_element>& polish, string& oResult,bool isStrict) {
	simple_stack<string>	stack;
	for ( int n=0 ; n<polish.size()-1 ; n++ ) {
		calc_element&	el=polish[n];
		if ( el.priority==100 ) { // �퉉�Z�q
			stack.push(el.str);
		}
		else if ( el.priority==90 ) {	// �P�����Z�q
			assert(stack.size()>=1);
			if ( !aredigits(stack.top()) )
				return	false;
			if ( el.str=="!" ) stack.push( itos(!stoi(stack.pop())) );
			else if ( el.str=="+" ) NULL;
			else if ( el.str=="-" ) stack.push( itos(-stoi(stack.pop())) );
			else assert(0);
		}
		a_op_b(^)
		else if ( el.str == "*" ) {
			assert(stack.size()>=2);
			string	rhs=stack.pop(), lhs=stack.pop();
			if ( aredigits(lhs) && aredigits(rhs) ) {
				stack.push(itos( stoi(lhs)*stoi(rhs) )); 
			} else if ( aredigits(rhs) && ! isStrict ) {
				int	num = stoi(rhs);
				stack.push("");
				for (int i=0;i<num;++i)
					stack.top() += lhs;
			} else {
				return	false;
			}
		}
		a_op_b(/)
		a_op_b(%)
		else if ( el.str == "+" ) {
			assert(stack.size()>=2);
			string	rhs=stack.pop(), lhs=stack.pop();
			if ( aredigits(lhs) && aredigits(rhs) ) {
				stack.push(itos( stoi(lhs)+stoi(rhs) )); 
			} else if ( ! isStrict ) {
				stack.push(lhs+rhs); 
			} else {
				return false;
			}
		}
		else if ( el.str == "-" ) {
			assert(stack.size()>=2);
			string	rhs=stack.pop(), lhs=stack.pop();
			if ( aredigits(lhs) && aredigits(rhs) ) {
				stack.push(itos( stoi(lhs)-stoi(rhs) )); 
			} else if ( ! isStrict ) {
				erase(lhs, rhs);
				stack.push(lhs);
			} else {
				return false;
			}
		}
		else if ( el.str == "=~" || el.str == "!~" ) {
			// �p�^�[���}�b�`
			assert(stack.size()>=2);
			string	target=stack.pop(), re=stack.pop();

			stack.push("0");
		}
		length_a_op_b(<)
		length_a_op_b(>)
		length_a_op_b(<=)
		length_a_op_b(>=)
		even_a_op_b(==)
		even_a_op_b(!=)
		a_op_b(&&)
		a_op_b(||)
		else 
			assert(0);

	}
	assert(stack.size()==1);
	oResult = stack.pop();
	return	true;
}

bool calc(const char* iExpression, string& oResult,bool isStrict) {
	std::vector<calc_element>	org;
	if ( !make_array(iExpression, org) )
		return	false;
	if ( *iExpression!='\0' )
		return	false;	// �Ȃ񂩃S�~���c���Ă��H

	simple_stack<calc_element>	stack,polish;
	stack.push(calc_element("Guard", 0));	// �ԕ�

	std::vector<calc_element>::const_iterator i;
	for ( i=org.begin() ; i!=org.end() ; ++i ) {
		while ( i->priority <= stack.top().priority && stack.top().str != "(" )
			polish.push(stack.pop());
		if ( i->str != ")" ) stack.push(*i); else stack.pop();
	}

	// stack����c������o��
	while ( !stack.empty() )
		polish.push(stack.pop());

	// �v�Z
	return	calc_polish(polish, oResult,isStrict);
}


bool calc(string& ioString,bool isStrict)
{
	string iString = ioString;

	erase(iString, "�@");
	erase(iString, " ");
	erase(iString, "\t");

	// Ʈۂ͒P�̂ŉ��Z�q�ɂ͂������Ȃ��[
	replace(iString, "���`", "=~");
	replace(iString, "�I�`", "!~");

	replace(iString, "�{", "+");
	replace(iString, "�|", "-");
	replace(iString, "��", "*");
	replace(iString, "�~", "*");
	replace(iString, "�^", "/");
	replace(iString, "��", "/");
	replace(iString, "��", "%");
	replace(iString, "�O", "^");
	replace(iString, "��", "<");
	replace(iString, "��", ">");
	replace(iString, "��", "=");
	replace(iString, "�I", "!");
	replace(iString, "��", "&");
	replace(iString, "�b", "|");
	replace(iString, "�i", "(");
	replace(iString, "�j", ")");
	replace(iString, "�O", "0");
	replace(iString, "�P", "1");
	replace(iString, "�Q", "2");
	replace(iString, "�R", "3");
	replace(iString, "�S", "4");
	replace(iString, "�T", "5");
	replace(iString, "�U", "6");
	replace(iString, "�V", "7");
	replace(iString, "�W", "8");
	replace(iString, "�X", "9");

	string	theResult;
	if ( !calc(iString.c_str(), theResult, isStrict) ) {
		return	false;
	}

	//�S�p�E���p�Ƃ����ނ�݂ɕϊ����Ȃ��悤�ɋC������
	if ( theResult != iString ) {
		ioString = theResult;
	}
	return	true;
}
