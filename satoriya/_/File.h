#ifndef	FILE_H
#define	FILE_H

//----------------------------------------------------------
// �t�@�C���n���h���̃��b�p�[�N���X
#define	NULL	0
class File {
public:
	File();
	~File();

	enum OpenFlag { READ, WRITE };
	bool	Open( const char* iFileName, OpenFlag iOpenFlag=READ );
	void	Close();
	
	bool	Read( void* oBuffer, unsigned long iLength );
	bool	Write( const void* iBuffer, unsigned long iLength );

	enum MoveMethod { BEGIN, CURRENT, END };
	bool	SetPosition( long iPosition, MoveMethod iMoveMethod=BEGIN );
	bool	GetPosition( unsigned long* oPosition );
	bool	GetSize( unsigned long* oSizeLow, unsigned long* oSizeHigh=NULL );

private:
	void*	mHandle;	// typedef void* HANDLE;
};

#include	<windows.h>

//----------------------------------------------------------
// �t�@�C������iShell�`API���g�p�j

BOOL	Move( LPCTSTR iTo, LPCTSTR iFrom );
BOOL	Copy( LPCTSTR iTo, LPCTSTR iFrom );
BOOL	Delete( LPCTSTR iFileName );
BOOL	Rename( LPCTSTR iTo, LPCTSTR iFrom );


// �t�@�C���̍ŏI�X�V�������擾
bool	GetLastWriteTime(LPCSTR iFileName, SYSTEMTIME& oSystemTime);
//	�t�@�C���̍X�V�������r�B
//	�Ԓl�����Ȃ�ΑO�ҁA���Ȃ�Ό�҂̂ق����V�����t�@�C���B
int		CompareTime(LPCSTR iLeft, LPCSTR iRight);
//	SYSTEMTIME���r�B
//	�Ԓl�����Ȃ�ΑO�ҁA���Ȃ�Ό�҂̂ق����V�����t�@�C���B
int	CompareTime(const SYSTEMTIME& stL, const SYSTEMTIME& stR);

// �t�@�C���̑傫�����擾
BOOL	GetFileSize( LPCSTR szFileName, DWORD* pdwSize );

// �t�@�C���E�t�H���_�̑��ݗL����₢���킹
bool	isExist(const char* iPath);
bool	isExistFile(const char* iPath);
bool	isExistFolder(const char* iPath);

//----------------------------------------------------------
// �t�@�C�����E�p�X

// �o�X������t�H���_���Ɗg���q���폜����B
void	ToOnlyFileName( LPSTR buf );

// �p�X������̊g���q��ύX����
void	SetExtention( LPTSTR szPath, LPCTSTR szNewExtention );
// �p�X������̊g���q�����擾����
LPCTSTR	GetExtention( LPCTSTR szPath );
LPTSTR	GetExtention( LPTSTR szPath );
// �g���q�𔻒肷��
bool	isExtention(LPCTSTR iPath, LPCTSTR iExtention);

// �p�X������̃t�@�C�����������擾����B
// �����񒆂� \ �L�����Ȃ��ꍇ�́A�^����ꂽ����������̂܂ܕԂ��B
LPCTSTR	GetFileName( LPCTSTR szPath );
LPTSTR	GetFileName( LPTSTR szPath );


// �R�����_�C�A���O���g�p���A�t�@�C���������[�U�[�ɑI��������B
bool	GetOpenFileName(
	LPTSTR	oFileName,	// MAX_PATH�����K�v�B
	HWND	iParentWindow=NULL,
	LPCTSTR	iFormat="�S�Ẵt�@�C�� (*.*)\0*.*\0",
	LPCTSTR iTitle="�t�@�C�����J��" );
bool	GetSaveFileName(
	LPTSTR	oFileName,	// MAX_PATH�����K�v�B
	HWND	iParentWindow=NULL,
	LPCTSTR	iFormat="�S�Ẵt�@�C�� (*.*)\0*.*\0",
	LPCTSTR	iTitle="�t�@�C����ۑ�����" );

//----------------------------------------------------------
// �t�@�C���E�t�H���_�\��

// �t�H���_���쐬�B�����^�����Ȃ�true�B
bool	MakeFolder(const char* iFolderName);
// �����K�w�ɓn��t�H���_���쐬�B
bool	MakeFolder_Nest(const char* iFolderName);


#endif	// FILE_H
