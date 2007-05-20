#ifndef	FILELISTER_H
#define	FILELISTER_H

#include	<windows.h>

// �w��p�X�̃t�@�C����񋓂���B
class	FileLister {
public:
	FileLister();
	~FileLister();

	// �񋓃p�X���w��B���C���h�J�[�h�g�p�B
	BOOL	Open( LPCSTR iFindPath );
	// ���̃t�@�C����񋓁B
	BOOL	Next();

	// ���݂̃t�@�C�������擾�B
	LPCSTR	Name();
	// ���݂̃t�@�C���̓t�H���_���H
	BOOL	isFolder();
	// ���݂̃t�@�C������ . �݂̂ō\������Ă���H
	BOOL	isDots();
	// �w��̊g���q���H
	BOOL	isExt( LPCSTR iExtention );
	// ���݂̃t�@�C���̏ڍ׃f�[�^���擾�B
	const WIN32_FIND_DATA*	Data();

	// �񋓂̏I���������s���B
	void	Close();

public:
	TCHAR	mPath[MAX_PATH];	// �񋓑Ώۃp�X
	HANDLE	mHandle;			// �񋓃n���h��
	WIN32_FIND_DATA	mData;	// �񋓂����t�@�C���̏��
};


#include	<string>
#include	<list>
using namespace std;

/*
// �t�@�C���������X�g�A�b�v
bool	ListFile(string iFindPath, const vector<WIN32_FIND_DATA>& oVector );
// �t�@�C���������X�g�A�b�v
bool	ListFile(string iFindPath, const vector<string>& oVector );
// �t�H���_�������X�g�A�b�v�AiDotsFolder�� .�y��..���܂߂邩�ۂ�
bool	ListFolder(string iFindPath, const vector<WIN32_FIND_DATA>& oVector, bool iDotsFolder=false );
*/

inline bool list_files(string i_folder, list<WIN32_FIND_DATA>& o_files)
{
	FileLister lister;
	if ( lister.Open((i_folder+"\\*.*").c_str()) == FALSE )
		return false;

	while ( lister.Next() ) 
	{
		o_files.push_back( *(lister.Data()) );
	} 
	
	return true;
}

inline list<WIN32_FIND_DATA> list_files(string i_folder)
{
	list<WIN32_FIND_DATA> data;
	list_files(i_folder, data);
	return data;
}



/*
void	do_folder(const string& iFolder, strvec& oVector) {

	vector<WIN32_FIND_DATA>	datas;

	FileLister	lister;
	if ( !lister.Open((iFolder+"\\*.*").c_str()) )
		return;
	while ( lister.Next() )
		datas.push_back( *lister.Data() );
	lister.Close();

	for (vector<WIN32_FIND_DATA>::iterator i=datas.begin(); i!=datas.end() ; ++i ) {
		if ( strcmp(i->cFileName, ".")==0 )
			continue;
		if ( strcmp(i->cFileName, "..")==0 )
			continue;
		string	path = iFolder + DIR_CHAR + i->cFileName;
		if ( i->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
			do_folder(path, oVector);
		else
			oVector.push_back(path);
	}
}
*/

#endif	//	FILELISTER_H
