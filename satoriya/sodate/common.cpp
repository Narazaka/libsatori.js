#include	"common.h"



// ���O����t�@�C����
set<string>		deny_filename_set;
// �Ώۃt�@�C���ꗗ
list<string>	files;

map<string, set<string> >	files_on_dir;	// dirname : filenames

// ���ݒ�
strmap	conf;
// ��ƃt�H���_
string	base_folder;


void	error(const string& str) {
	::MessageBox(NULL, str.c_str(), "error - sodate", MB_OK);
}
