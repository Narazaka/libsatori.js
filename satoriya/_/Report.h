/*------------------------------------------------------------
	Report	�G���[�񍐃N���X

	�g����

	void	function() 
	{
		Report	rp("process name");	

		rp="process step 1";
		if ( �`�` )
			return;		// �G���[�񍐂���܂��B

		rp="process step 2";
		if ( �`�` )
			return;		// �G���[�񍐂���܂��B

		rp="";	// ����I���B�G���[�񍐂���܂���B
	}



------------------------------------------------------------*/
#ifndef	REPORT_H
#define	REPORT_H

#define		WIN32_LEAN_AND_MEAN
#include	<windows.h>

#ifndef	LINE_MAX
#define	LINE_MAX	1024
#endif

typedef long HRESULT;	//	for return value of 'DirectX'

class	Report {

	// �R�s�[�E����̋֎~
	Report(Report&);
	Report&	operator=(Report&);

	// ���b�Z�[�W�{�b�N�X�̐e�E�B���h�E
	HWND	m_hWnd;

	// ���b�Z�[�W�o�b�t�@
	TCHAR	m_szProcess[LINE_MAX];
	TCHAR	m_szStep[LINE_MAX];
	TCHAR	m_szMessage[LINE_MAX];
	int		m_iMessagePos;	// szMessage���̏������݈ʒu

	// Report�N���X�̃I�u�W�F�N�g���݌�	
	static	int	m_nInstance;
	// ���b�Z�[�W�{�b�N�X�\�����̐e�E�B���h�E
	static	HWND	m_hMainWindow;
	// ��������nInstance�l�B�K�w�[�x�������B
			int	m_nNest;

	// m_nNest*2�̋󔒂�u���āA�e�f�o�C�X�ɏo��
	void	Output( LPCSTR );

public:

	// �񍐂��鏈����/���b�Z�[�W�{�b�N�X�̐e�E�B���h�E
	Report( LPCSTR szProcess, HWND hWnd=NULL );

	// ���̎��_�� m_szStep �ɓ��e������΃G���[�����B
	~Report();

	// �����̒i�K
	Report&	operator=( LPCSTR szStep );
	void	finish() { (*this)=""; }

	// �G���[�ڍׂ̋L�q�A�܂��̓��O�B
	Report&	operator<<( LPCSTR str );
	Report&	operator<<( int iValue );
	Report&	operator<<( DWORD dwValue );

	// �G���[���e�� m_szMessage �Ɋi�[�B
	Report&	DD( HRESULT hr );
	Report&	DI( HRESULT hr );
	Report&	API();	// use GetLastError()
};

// �\�[�X�t�@�C�����̈ʒu���o�͂���}�N��
// usage : rp << here;
#define	here	__FILE__ << " (" << __LINE__ << ")\n"

#endif	//	REPORT_H
