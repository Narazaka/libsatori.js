
#include	<windows.h>
#include	"../_/stltool.h"
#include	"../_/utilities.h"
#include	"../_/File.h"
#include	"../_/FileLister.h"
#include	"../_/FTP.h"
#include	"../_/MD5.h"

#include	<map>
#include	<list>
#include	<string>
#include	<vector>
#include	<fstream>
using namespace std;


void	error(const string& str);

// ���O����t�@�C����
extern	set<string>		deny_filename_set;

// �Ώۃt�@�C���ꗗ
extern	list<string>	files;
// files���t�H���_�ʂɕ����i�[
extern	map<string, set<string> >	files_on_dir;	// dirname : filenames

// ���ݒ�
extern	strmap	conf;
// ��ƃt�H���_
extern	string	base_folder;

static const char byte_value_1[2] = {1,0};
static const char byte_value_2[2] = {2,0};



#include	"Sender.h"


