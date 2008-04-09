#include	"common.h"

#include	<iomanip>



// file-scope
static HINTERNET hInternet;


static bool	upload_a_dir(const string& base_folder, const string& dir_name, set<string>& dir_files) {

	string	local_folder = base_folder+"\\"+dir_name;
	string	remote_folder = conf["ghostroot"];
	replace(remote_folder, "/", "\\");
	if ( remote_folder[ remote_folder.size()-1 ] != '\\' )
		remote_folder += "\\";
	remote_folder += dir_name;

	// ftp�ڑ�
	FTP	ftp(hInternet);
	if ( !ftp.open(conf["host"], conf["id"], conf["password"], conf["is_passive"]!="0") ) {
		error("FTP�T�[�o�ւ̐ڑ����m���ł��܂���ł����B�ݒ���������Ă��������B");
		return	false;
	}

	// �t�H���_������
	strvec	remote_folders;
	split(remote_folder, "\\", remote_folders);

	// �ڕW�̃t�H���_�֍~��t�H���_�쐬
	if(1){	
		for ( strvec::iterator i=remote_folders.begin() ; i!=remote_folders.end() ; ++i ) {
			if ( !ftp.cd(*i) ) {
				if ( !ftp.mkdir(*i) ) {
					error("FTP�T�[�o���Ńt�H���_�쐬�Ɏ��s���܂����B");
					return	false;
				}
				else if ( !ftp.cd(*i) ) {
					error("FTP�T�[�o���Ńt�H���_�ړ��Ɏ��s�H�H");
					return	false;
				}
			}
		}
	}

	
	// �����[�g�̃t�@�C�������擾
	string pwdtxt;
	ftp.pwd(pwdtxt);
	sender << pwdtxt << "���̃t�@�C�������擾��...";
	map<string, WIN32_FIND_DATA>	remoteFilesData;
	ftp.ls(remoteFilesData);
	sender << "����" << endl << local_folder << "���̃t�@�C���Ƃ̍X�V���ԏƍ����J�n�B" << endl;

	// �A�b�v���[�h
	for (set<string>::iterator i=dir_files.begin(); i!=dir_files.end() ; ++i) {

		string	local_file_path = local_folder+"\\"+*i;
		bool	isUpload;

		if ( remoteFilesData.find(*i)==remoteFilesData.end() ) {
			isUpload = true;	// �T�[�o��ɑ��݂��Ȃ�
		}
		else {
			SYSTEMTIME	stLocal, stRemote;
			GetLastWriteTime(local_file_path.c_str(), stLocal);
			WIN32_FIND_DATA	fd = remoteFilesData[*i];
			FILETIME	ft=fd.ftLastWriteTime, ftLocal;
			//FileTimeToLocalFileTime(&ft, &ftLocal);
			ftLocal = ft;
			FileTimeToSystemTime( &ftLocal, &stRemote);

			if ( conf.find("yeardiff")!=conf.end() )
				stRemote.wYear += stoi( conf["yeardiff"] );
			if ( conf.find("timediff")!=conf.end() ) {
				int	hour = stRemote.wHour;
				hour += stoi( conf["timediff"] );
				if ( hour>=24 ) {
					stRemote.wDay++;
					hour -= 24;
				}
				else if ( hour<0 ) {
					stRemote.wDay--;
					hour += 24;
				}
				stRemote.wHour = hour;
			}

			if ( conf["morelog"]!="0" ) {
				
				sender << "Local  " 
					<< stLocal.wYear << "/"
					 << setfill('0') << setw(2)<< stLocal.wMonth << "/"
					  << setfill('0') << setw(2)<< stLocal.wDay << " "
					   << setfill('0') << setw(2)<< stLocal.wHour << ":"
						<< setfill('0') << setw(2)<< stLocal.wMinute << ":"
						 << setfill('0') << setw(2)<< stLocal.wSecond << endl;
				sender << "Remote " 
					<< stRemote.wYear << "/"
					 << setfill('0') << setw(2)<< stRemote.wMonth << "/"
					  << setfill('0') << setw(2)<< stRemote.wDay << " "
					   << setfill('0') << setw(2)<< stRemote.wHour << ":"
						<< setfill('0') << setw(2)<< stRemote.wMinute << ":"
						 << setfill('0') << setw(2)<< stRemote.wSecond << endl;
			}


			isUpload = CompareTime(stLocal, stRemote)>0;	// Local���V������΁B
		}

		if ( isUpload ) {
			sender << "�@" << *i << "���A�b�v���[�h��...";
			if ( !ftp.put(local_file_path, *i) ) {
				sender << "���s" << endl;
				error(local_file_path+"�̃A�b�v���[�h�Ɏ��s");
				return	false;
			}
			sender << "����" << endl;
		}
		else {
			sender << "�@" << *i << "�͍X�V����Ă��܂���B" << endl;
		}
	}

	/*sender << "ftp-dir: " << ftp.getCurrentDirectry() << endl;
	sender << local_folder << dir_name << endl;
	sender << remote_folder << dir_name << endl;*/

	return	true;
}


bool	uploadFiles(const string& base_folder, list<string>& files) {

	/* WININET�̏����� */
	hInternet = ::InternetOpen(
		"network_updater",
		INTERNET_OPEN_TYPE_PRECONFIG,//INTERNET_OPEN_TYPE_DIRECT,
		NULL,
		NULL,
		0);
	if ( hInternet==NULL ) {
		error("�C���^�[�l�b�g�ւ̐ڑ��ł��܂���ł���");
		return	false;
	}


	// �t�H���_�P�ʂŃA�b�v���[�h�����s
	for (map<string, set<string> >::iterator j=files_on_dir.begin(); j!=files_on_dir.end() ; ++j) {
		if ( !upload_a_dir(base_folder, j->first, j->second) ) {
			::InternetCloseHandle(hInternet);
			return	false;
		}
	}

	
	::InternetCloseHandle(hInternet);
	return	true;
}


