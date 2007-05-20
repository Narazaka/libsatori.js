#include	"FileLister.h"
#include	<assert.h>

//------------------------------------------------------------------------
FileLister::FileLister() {
	::ZeroMemory( &mPath, sizeof(mPath) );
	mHandle = INVALID_HANDLE_VALUE;
	::ZeroMemory( &mData, sizeof(mData) );
};
//------------------------------------------------------------------------
FileLister::~FileLister() {
	Close();
}

//------------------------------------------------------------------------
// �񋓃p�X���w��B���C���h�J�[�h�g�p�B
BOOL	FileLister::Open( LPCSTR iFindPath ) {
	assert(iFindPath != NULL);
	Close();
	::lstrcpy( mPath, iFindPath );
	return	TRUE;
}

//------------------------------------------------------------------------
// ���̃t�@�C����񋓁B
BOOL	FileLister::Next() {

	if ( mPath[0] == '\0' )
		return	FALSE;

	if ( mHandle == INVALID_HANDLE_VALUE ) {
		mHandle = ::FindFirstFile( mPath, &mData );
		if ( mHandle == INVALID_HANDLE_VALUE )
			return	FALSE;
	}
	else {
		if ( !::FindNextFile( mHandle, &mData ) ) {
			mHandle = INVALID_HANDLE_VALUE;
			return	FALSE;
		}
	}
	return	TRUE;
}

//------------------------------------------------------------------------
// ���݂̃t�@�C�������擾�B
LPCSTR	FileLister::Name() {
	return	mData.cFileName;
}

//------------------------------------------------------------------------
// ���݂̃t�@�C���̏ڍ׃f�[�^���擾�B
const WIN32_FIND_DATA*	FileLister::Data() {
	return	&mData;
}

//------------------------------------------------------------------------
// ���݂̃t�@�C���̓t�H���_���H
BOOL	FileLister::isFolder() {
	return ( mData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY );
}

//------------------------------------------------------------------------
// ���݂̃t�@�C������ . �݂̂ō\������Ă���H
BOOL	FileLister::isDots() {
	char*	p = mData.cFileName;
	if (*p != '.') 
		return FALSE;
	for ( ; *p != '\0' ; p++ )
		if ( *p != '.' )
			return	FALSE;
	return	TRUE;
}

//------------------------------------------------------------------------
// �w��̊g���q���H
BOOL	FileLister::isExt( LPCSTR iExtention ) {
	assert(iExtention != NULL);

	int	len = strlen(mData.cFileName);
	char*	p = mData.cFileName;
	for ( int i=len-1 ; i>=0 ; i-- )
		if ( p[i] == '.' )
			break;

	if ( i>=0 )
		return ( stricmp( iExtention, p+i+1 ) == 0 );// �������Ƃ�
	else
		return ( iExtention[0] == '\0' );			// ���������Ƃ�
}

//------------------------------------------------------------------------
// �񋓂̏I���������s���B
void	FileLister::Close() {
	if ( mHandle != INVALID_HANDLE_VALUE ) {
		::FindClose(mHandle);
		mHandle=INVALID_HANDLE_VALUE;
	}
}

/*




		void	FindPath( LPCSTR iFindPath );


	// �����p�X���w�肷��B���C���h�J�[�h�g�p�B
	void	Open( LPCSTR );
	// ���������t�@�C���������擾����B
	LPCSTR	GetFoundFileName();
	// 
	void	Close( LPCSTR );
	
	 {
	}
	}
	void	Reset
	

};


	FileFinder	ff;
	ff.First("c:\*.*");
	do {
		GetFoundData().cFileName;
	} while ( ff.Next() );
	ff.Close();
	
	FileFinder ff("c:\\*.*");
	while ( ff.FindNext() != NULL ) {
		(LPCSTR)ff.Now()
	}

class	FileFinder {
	HANDLE			m_hFind;	// �����n���h��
	WIN32_FIND_DATA	m_fdFound;	// ���������t�@�C���̏��
public:
	FileFinder() {
		m_hFind = INVALID_HANDLE_VALUE;
		::ZeroMemory( &m_fdFound, sizeof(WIN32_FIND_DATA) );
	}
	~FileFinder() {
		Close();
	}

	BOOL	First( LPCSTR szPath ) {
		Close();
		m_hFind = ::FindFirstFile( szPath, &m_fdFound );
		if ( m_hFind == INVALID_HANDLE_VALUE )
			return	FALSE;
	}

	BOOL	Next() {
		return	::FindNextFile( m_hFind, &m_fdFound );
	}

	BOOL	Close( LPCSTR szPath ) {
		if ( m_hFind != INVALID_HANDLE_VALUE ) {
			::FindClose(m_hFind);
			m_hFind = INVALID_HANDLE_VALUE;
		}
	}

	const WIN32_FIND_DATA& GetFoundData() {
		return	&m_fdFound;
	}
};
*/