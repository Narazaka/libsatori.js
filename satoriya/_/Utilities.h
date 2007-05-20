// C++ Utilities

#ifndef	UTILITIES_H
#define	UTILITIES_H

#include	<assert.h>
#include	<string.h>

#ifdef POSIX
inline bool _ismbblead(char c) {
    // �����AShift_JIS�Œ�ŗǂ��̂��낤�B
    unsigned char _c = c;
    return (_c >= 0x81 && _c <= 0x9f) || (_c >= 0xe0 && _c <= 0xfc);
}
inline bool _ismbbtrail(char c) {
    unsigned char _c = c;
    return (_c >= 0x40 && _c <= 0x7e) || (_c >= 0x80 && _c <= 0xec);
}
#else
#  include	<mbctype.h>	// for _ismbblead,_ismbbtrail
#endif

// �|�C���^�̎w���ʒu���P�����C���N�������g�B�S�p�E���p���Ή�
inline void	mbinc(const char*& p) {
	//p += _ismbblead(*p) ? 2 : 1; 
	if ( _ismbblead(p[0]) ) { 
		assert(_ismbbtrail(p[1]));
		p+=2;
	} 
	else
		p++;
}

// �����񒆂���w���1byte�������Ō�ɏo������ʒu��Ԃ��B
// ������Ȃ������ꍇ�� NULL ��Ԃ�
const char*	FindFinalChar(const char* iString, char iC);
inline char*	FindFinalChar(char* iString, char iC) {
	return	const_cast<char*>(FindFinalChar( static_cast<const char*>(iString), iC ));
}

// �t�@�C��������g���q���폜����
bool	CutExtention(char* iFileName);
// �g���q��ύX�܂��͒ǉ�����
void	SetExtention(char* iFileName, const char* iNewExtention);
// �t�@�C�����̊g���q�������擾����
char*	GetExtention(char* iFileName);

// iString�̈ʒu���甼�p�X�y�[�X�y�є��p�^�u���΂����ʒu��Ԃ�
const char*	SkipDelimiter(const char* iString);
inline char*	SkipDelimiter(char* iString) {
	return	const_cast<char*>(SkipDelimiter( static_cast<const char*>(iString)));
}


// str1��str2�͓��������H
inline bool CompareStr( const char* str1, const char* str2 ) {
	return	(::strcmp( str1, str2 ) == 0 );
}
// str����part�����݂��邩�H
inline bool FindStr( const char* str, const char* part ) {
	return	(::strstr( str, part ) != NULL );
}

/*
#ifndef	max
#define	max( a, b )	(((a)>(b)) ? (a) : (b) )
#endif
#ifndef	min
#define	min( a, b )	(((a)<(b)) ? (a) : (b) )
#endif
#define	bit_on( t, bit )	( (t) |= (1<<(bit)) )
#define	bit_off( t, bit )	( (t) &= ~(1<<(bit)) )
#define	get_bit( t, bit )	( (t) & (1<<(bit)) )
*/

// t �� a �ȏ� b �ȉ��Ɏ��߂�B
// t = max( a, min( b, t ) ) �Ɠ����B
#ifndef	area
#define	area( t, a, b )	if (t<(a)) t=(a); else if (t>(b)) t=(b); else
#endif

template<class T>
T	Min( T a, T b ) { return (a<b)?a:b; }
template<class T>
T	Max( T a, T b ) { return (a>b)?a:b; }
template<class T>
T	Abs( T a ) { return (a>=0)?a:-a; }
template<class T>
T	MinMax( T min, T t, T max ) { return t<min ? min : Min(max,target); }
template<class T>
void Swap( T* a, T* b ) { assert( a!=NULL && b!=NULL ); T temp; temp=*a; *a=*b; *b=temp; }


template<class T>
T&	Area( T& t, T a, T b ) { return t = Max( a, Min( b, t ) ); }

inline bool Compare( const char* a, const char *b ) {
	assert( a != NULL && b != NULL );
	return	( strcmp( a, b ) == 0 );
}

// �����𔽓]
template<class T>
void	ReverseSign( T& ioT ) { ioT = -ioT; }

// iBase�����_�Ƃ�����Βl�𔽓]�������̂�Ԃ�
template<class T>
T	Reverse(T iBase, T iPos) { return (-(iPos-iBase)) + iBase; }


#define	PAI_HALF	(3.1416/2)
#define	PAI		(3.1416)
#define	PAI2	(3.1416*2)
#define	toRadian(degree)	(( (degree)*PAI2 )/360.0)
#define	toDegree(radian)	(int)(((radian)*360.0)/PAI2)

typedef	unsigned char	uchar;
typedef	unsigned short	ushort;
typedef	unsigned long	ulong;
typedef	unsigned int	uint;
typedef	signed char		int8;
typedef	signed short	int16;
typedef	signed long		int32;
#ifdef POSIX
typedef long long       int64;
#else
typedef	signed __int64	int64;
#endif
typedef	uchar	byte;
typedef	ushort	word;
typedef	ulong	dword;

// ��P�����̎w��bit��on/off���A�Q�Ƃ�Ԃ��B���Z�q�I�ɗp����B
template<class T>
T& BitOn( T& t, int bit ) { assert( bit>=0 && bit<sizeof(T)*8 ); return t |= (1<<bit); }
template<class T>
T& BitOff( T& t, int bit ) { assert( bit>=0 && bit<sizeof(T)*8 ); return t &= ~(1<<(bit)); }
template<class T>
T& SetBit( T& t, int bit, int value ) { assert( bit>=0 && bit<sizeof(T)*8 ); return value ? BitOn(t,bit) : BitOff(t,bit); }
// ��P�����̎w��bit��Ԃ��B�l�͕ύX���Ȃ��B
template<class T>
int GetBit( T t, int bit ) { assert( bit>=0 && bit<sizeof(T)*8 ); return ( t & (1<<(bit)) ) != 0 ; }

// ���������o�C�g�ɃA���C������ۂ̑���Ȃ��o�C�g����Ԃ��B
inline int AlignPad( int i, int n ) { assert(n!=0); return ( i%n == 0 ) ? 0 : (n-i%n); }
// ���������o�C�g�ɃA���C�������l��Ԃ�
inline int Align4( int i ) { return (i+3)&0xfffffffc; }
inline int Align( int i, int n ) { assert(n!=0); return ( i%n == 0 ) ? i : i+(n-i%n); }

// �\���̂�0�Ŗ��߂�B
template<class T>
inline	void	ClearStruct( T* pt ) { assert(pt!=NULL); memset( pt, 0, sizeof(pt) ); }
// ������0�Ŗ��߂�B�s���S�łȂ��錾���ꂽ�z�񂪑ΏہB
#define	ClearMemory(pt)	memset( pt, 0, sizeof(pt) )

// �A���t�@�u�����h�I�Ȍv�Z
template<class T, class U>
T	RateProcess( T a, T b, U b_rate, U max ) {
	assert( max != 0 );
	return	( a*(max-b_rate) + b*b_rate )/max;
}

// �����ύX����
template<class T>
T	ChangeBase( T value, T old_base, T new_base ) {
	assert( old_base != 0 );
	return	(value*new_base/old_base);
}

// �������琮�����^���������擾
template<class T>
int	GetIntegral( T d ) { return (int)d; }
template<class T>
T	GetDecimal( T d ) { return d-(int)d; }

#define	unless(b)	if (!(b))

#define	once(f)	static int __first__=true; if (__first__) { __first__=false; f; } else NULL

inline	bool	isHexDigit(int c) { return ( (c>='0'&&c<='9') || (c>='A'&&c<='F') || (c>='a'&&c<='f') ); }
inline	int	Hex2Num(int c) {
	assert(isHexDigit(c));
	if ( c>='0'&&c<='9' )
		return	c-'0';
	if ( c>='A'&&c<='F' )
		return	c-'A'+10;
	return	c-'a'+10;
}

// ��悳��
template<class T>
T	pow2(const T& t) { return t*t; }


#endif // UTILITIES_H
