#ifndef	THREAD_H
#define	THREAD_H

#include	<windows.h>
#include	<assert.h>

class	Thread {
private:
	HANDLE	mThread;	// �X���b�h�n���h��
	DWORD	mID;		// �X���b�h��ID
	
	// �X���b�h�̃��C���֐��i�������z�j
	virtual	DWORD	ThreadMain()=0;
	// ���X���b�h�֐��i_this->ThreadMain���Ăяo�������j
	static	DWORD WINAPI ThreadProc(LPVOID iThis) { return reinterpret_cast<Thread*>(iThis)->ThreadMain(); }

protected:
	LPVOID	mParam;	// �X���b�h�N�����̈���

public:
	Thread() : mThread(NULL), mID(0), mParam(NULL) {}
	virtual	~Thread() {}

	// �쐬
	virtual	bool	create(void* iParam=NULL, bool iSuspendedCreate=false);
	// �n���h�������
	virtual	void	close();

	// �D��x�ݒ�
	virtual	BOOL	setPriority(int nPriority) { return ::SetThreadPriority(mThread, nPriority); }
	// �������f
	virtual	BOOL	terminate(DWORD dwExitCode=-1) { return ::TerminateThread(mThread, dwExitCode); }
	// �I���҂�
	virtual	DWORD	waitEnd(DWORD dwTimeOut=INFINITE) { return ::WaitForSingleObject(mThread, dwTimeOut); };
	// �I�����m�F
	virtual	DWORD	isEnd() { return ( ::WaitForSingleObject(mThread, 0 ) == WAIT_OBJECT_0 );}
};

#endif	//	THREAD_H

