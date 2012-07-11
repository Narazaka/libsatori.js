#include	"SaoriHost.h"
#include	<map>

class cn : public SaoriHost {
public:
	virtual SRV		request(deque<string>& iArguments, deque<string>& oValues);
};
SakuraDLLHost* SakuraDLLHost::m_dll = new cn;


static SRV	call_cn(string iCommand, deque<string>& iArguments, deque<string>& oValues)
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
		d(at);
		#undef	d
	}

	// ���߂̑��݂��m�F
	map<string, Command>::const_iterator i = theMap.find(iCommand);
	if ( i==theMap.end() ) {
		return SRV(400, string()+"Error: '"+iCommand+"'�Ƃ������O�̖��߂͒�`����Ă��܂���B");
	}

	// ���ۂɌĂ�
	return	i->second(iArguments, oValues);
}

SRV	cn::request(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	SRV(400, "���߂��w�肳��Ă��܂���");

	// �ŏ��̈����͖��ߖ��Ƃ��Ĉ���
	string	theCommand = iArguments.front();
	iArguments.pop_front();
	return	call_cn(theCommand, iArguments, oValues);
}


// �����������

#ifdef POSIX
#  include      "../_/Utilities.h"
#else
#  include	<windows.h>
#  include	<mbctype.h>
#endif
#include	"../_/stltool.h"

SRV _at(deque<string>& iArguments, deque<string>& oValues) {

	if ( iArguments.size()==2 ) {
	}
	else
		return	SRV(400, "����������������܂���B");

	return 200;
}
