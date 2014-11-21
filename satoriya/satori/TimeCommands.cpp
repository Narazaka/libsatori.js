#include	"../_/stltool.h"
#include	<windows.h>	
typedef	unsigned __int64	uint64;


string	TimeCommands(
	const string& iCommand,
	const deque<string>& iArguments)
{/*
	if ( iCommand=="tm" ) {

		SYSTEMTIME	st = {2002,1,2,1,0,0,0,0};
		if ( iArguments.size()>=7 ) { st.wMilliseconds=stoi_internal(iArguments[6]); }
		if ( iArguments.size()>=6 ) { st.wSecond=stoi_internal(iArguments[5]); }
		if ( iArguments.size()>=5 ) { st.wMinute=stoi_internal(iArguments[4]); }
		if ( iArguments.size()>=4 ) { st.wHour=stoi_internal(iArguments[3]); }
		if ( iArguments.size()>=3 ) { st.wDay=stoi_internal(iArguments[2]); }
		if ( iArguments.size()>=2 ) { st.wMonth=stoi_internal(iArguments[1]); }
		if ( iArguments.size()>=1 ) { st.wYear=stoi_internal(iArguments[0]) + ( iArguments[0].size() <= 2 ) ? 2000 : 0; }

		TimePos	tp(st);

		char	buf[256];
		sprintf(buf, "%s_%04d_%02d_%02d_%02d_%02d_%02d_%04d", STF_HEADER,
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		return	buf;
	}
	else if ( iCommand=="tm_span" ) {
		if ( iArguments.size()!=2 )
			return	"Error: �Q�̈������K�v�ł��B";
		if ( !isSTF(iArguments[0]) )
			return	"Error: ���ӂ�time�l���s���ł��B";
		if ( !isSTF(iArguments[1]) )
			return	"Error: �E�ӂ�time�l���s���ł��B";
		uint64	from = STFtoUINT64(iArguments[0]);
		uint64	to = STFtoUINT64(iArguments[1]);
		//return	SYSTEMTIMEtoSTF(UINT64toSYSTEMTIME_daycount(ui64));
		int	span_second = (to-from)/10000/1000;
		return	itos(span_second);
	}
	else {
		return	string("Error: '")+iCommand+"'�Ƃ����֐��͒�`����Ă��܂���B";
	}*/
	return	"";
}



#if	0
#define	STF_HEADER	"STF"	// STF_YY_MM_DD_HH_mm_SS_mill
#define	STSF_HEADER	"STSF"	// STSF_DDDDDDDDDD_HH_mm_SS_mill	���t��10�P�^�B


static	bool	isSTF(const string& stf) {
	strvec	vec;
	if ( split(stf, "_", vec)!=8 )
		return	false;
	if ( vec[0]!=STF_HEADER )
		return	false;
	return	true;
}


inline int DayOfWeek( int Year, int Month, int Day ) {
	if ( Month <= 2 ) {
		Year  = Year  - 1;
		Month = Month + 12;
	}
	return ((Year+(Year/4)-(Year/100)+(Year/400)+(((13*Month)+8)/5)+Day)%7);
}//DayOfWeek

static	SYSTEMTIME	STFtoSYSTEMTIME(const string& stf) {
	SYSTEMTIME	st;
	::ZeroMemory(&st, sizeof(st));

	strvec	vec;
	if ( split(stf, "_", vec)!=8 )
		return	st;
	if ( vec[0]!=STF_HEADER )
		return	st;

	st.wYear = stoi_internal(vec[1]);
	st.wMonth = stoi_internal(vec[2]);
	st.wDay = stoi_internal(vec[3]);
	st.wDayOfWeek = DayOfWeek(st.wYear, st.wMonth, st.wDay);
	st.wHour = stoi_internal(vec[4]);
	st.wMinute = stoi_internal(vec[5]);
	st.wSecond = stoi_internal(vec[6]);
	st.wMilliseconds = 0;
	return	st;
}

static	string	SYSTEMTIMEtoSTF(const SYSTEMTIME& st) {
	char	buf[256];
	sprintf(buf, "%s_%04d_%02d_%02d_%02d_%02d_%02d_%04d", STF_HEADER,
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return	buf;
}

static	uint64	SYSTEMTIMEtoUINT64(const SYSTEMTIME& st) {
	FILETIME	ft;
	if ( !::SystemTimeToFileTime(&st, &ft) )
		return	0;
	uint64	ui64 = *( (uint64*)((void*)(&ft)) );
/*	uint64	ui64 = ft.dwHighDateTime;
	ui64 <<= 32;
	ui64 |= ft.dwLowDateTime;*/
	return	ui64;
}

static	SYSTEMTIME	UINT64toSYSTEMTIME(uint64 ui64) {
	FILETIME	ft;
	ft.dwLowDateTime = (DWORD)ui64;
	ft.dwHighDateTime = ui64>>32;
	SYSTEMTIME	st;
	::ZeroMemory(&st, sizeof(st));
	::FileTimeToSystemTime(&ft, &st);
	return	st;
}

static	SYSTEMTIME	UINT64toSYSTEMTIME_daycount(uint64 ui64) {
	ui64 /= 10000;	// �P�ʂ�100ns����1ms�ցB
	SYSTEMTIME	st;
	::ZeroMemory(&st, sizeof(st));
	st.wMilliseconds = ui64%1000;
	ui64/=1000;
	st.wSecond = ui64%60;
	ui64/=60;
	st.wMinute = ui64%60;
	ui64/=60;
	st.wHour = ui64%24;
	ui64/=24;
	st.wDay = ui64;
	return	st;
}

inline	uint64	STFtoUINT64(const string& stf) {
	return	SYSTEMTIMEtoUINT64( STFtoSYSTEMTIME(stf) );
}

inline	string	UINT64toSTF(uint64 ui64) {
	return	SYSTEMTIMEtoSTF( UINT64toSYSTEMTIME(ui64) );
}




	set(FILETIME&)
	FILETIME	FileTime() { return ft; }
	__int64
	SYSTEMTIME	


class TimePos {
	FILETIME	ft;
	
	string	to_string() {
		SYSTEMTIME	st = FileTimeToSystemTime(ft);
		buf
	}
	
	bool	from_string(const STF.cpp 2
string& iString) {
	}
};

struct	TimeSpan {
	FILETIME	ft;
	
	string	to_string();
	bool	from_string(const string& iString);
};

// �������쐬
TimePos		tm(int,int=1,int=1,int=0,int=0,int=0,int=0)
// ���Ԃ��쐬
TimeSpan	tmd(int,int=0,int=0,int=0,int=0)
TimeSpan	tmh(int,int=0,int=0,int=0)
TimeSpan	tmm(int,int=0,int=0)
TimeSpan	tms(int,int=0)
// �o��
string	tmf(const string&, const TimePos&);
string	tmf(const string&, const TimeSpan&);
// �Q�̓���������Ԃ��v�Z
TimeSpan	tm_span(const TimePos&, const TimePos&);
// �Q�̓����܂��͊��Ԃ��r�B�Ԓl�� -1, 0, 1�B
int	tm_comp(const TimeSpan&, const TimeSpan&);
int	tm_comp(const TimeSpan&, const TimeSpan&);
// �����Ɗ��Ԃ̉����Z
TimePos	tm_add(const TimePos&, const TimeSpan&);
TimePos	tm_sub(const TimePos&, const TimeSpan&);
// ���ԓ��m�̉����Z
TimeSpan	tm_add(const TimeSpan& iTimeSpan, const TimeSpan& iTimeSpan);
TimeSpan	tm_sub(const TimeSpan& iTimeSpan, const TimeSpan& iTimeSpan);
// ���ԂƐ����̏揜�Z
TimeSpan	tm_mul(const TimeSpan& iTimeSpan, int n);
TimeSpan	tm_div(const TimeSpan& iTimeSpan, int n);
// ����m��n���܂ł̊���
TimeSpan	tm_from
TimeSpan	tm_to



���N�̂W���Q�P��
�itm_span,�itm_now,&year�j,�j�j
	
�itmnow,%year�j
�itmprintf,�itmnow�j,%year�j
�itmscanf,�itmnow�j,%year�j



�����p�J�b�R�̓W�J	�L��
�����p�J�b�R�̓W�J	����

�i�i�i�j�j�j

#endif
