#ifndef	SYNC_H
#define	SYNC_H

#include	<windows.h>
#include	<assert.h>

//-------------------------------------------
// �C�x���g����
class	Event {
	HANDLE	mEvent;

	Event(const Event&);
	operator=(const Event&);
public:
	Event( BOOL iInitialState=FALSE, BOOL iManualReset=TRUE ) {
		mEvent = ::CreateEvent(NULL, iInitialState, iManualReset, NULL);
	}
	~Event() { ::CloseHandle(mEvent); }

	void	Set() {	::SetEvent(mEvent); }
	void	Reset() { ::ResetEvent(mEvent); }
	BOOL	isSet() { return ( ::WaitForSingleObject(mEvent, 0)==WAIT_OBJECT_0 ); }
	BOOL	WaitSet(DWORD iTimeOut=INFINITE) { return ( ::WaitForSingleObject(mEvent, iTimeOut)==WAIT_OBJECT_0 ); }

	operator HANDLE() { return mEvent; }
};

//-------------------------------------------
// �Q�̓����I�u�W�F�N�g�̕Е����V�O�i����ԂɂȂ�����Ԃ�
inline bool WaitFor2(HANDLE iObj1, HANDLE iObj2, DWORD iTimeOut=INFINITE) {
	HANDLE	theHandles[2] = { iObj1, iObj2 };
	return (::WaitForMultipleObjects(2, theHandles, TRUE, iTimeOut) != WAIT_TIMEOUT);
}

inline bool WaitFor1of2(HANDLE iObj1, HANDLE iObj2, DWORD iTimeOut=INFINITE) {
	HANDLE	theHandles[2] = { iObj1, iObj2 };
	return (::WaitForMultipleObjects(2, theHandles, FALSE, iTimeOut) != WAIT_TIMEOUT);
}

//-------------------------------------------
// �N���e�B�J���Z�N�V��������

#include	"CriticalSection.h"


//-------------------------------------------
// �񓯊����T�|�[�g�����t���O

class async_flag {
	volatile bool mFlag;
public:
	async_flag(bool iFlag=false) : mFlag(iFlag) {}
	~async_flag() {}
	void	set(bool iFlag=true) { mFlag=iFlag; }
	void	reset() { mFlag=false; }
	bool	get() const { return mFlag; }
	operator bool() const { return mFlag; }
	operator !() const  { return !mFlag; }
	operator =(bool iFlag) { mFlag=iFlag; return *this; }
};


#endif	//	SYNC_H

