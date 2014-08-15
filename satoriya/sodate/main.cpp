#include	"common.h"
#include	"makeZip.h"



int main( int argc, char *argv[ ], char *envp[ ] )
{
	// ���s�t�@�C���̂���t�H���_�����擾
	TCHAR	szPath[MAX_PATH]="";
	::GetModuleFileName(NULL, szPath, MAX_PATH);
	char*	p = FindFinalChar(szPath, '\\');
	if ( *p == NULL )
		return	1;
	*p='\0';
	base_folder = szPath;
	GetSender().sender() << "[sodate]" << endl;
	GetSender().sender() << "�f�B�X�N��̑Ώۃt�H���_�� " << base_folder << "�ł��B" << endl;

	::SetCurrentDirectory(base_folder.c_str());

	// �ݒu�ꏊ�̊m�F
	if ( !isExistFolder( (base_folder+"\\ghost\\master").c_str() ) ) {
		error("ghost/master��������܂���B�ݒu�ʒu���m�F���Ă��������B");
		return	false;
	}
	if ( !isExistFolder( (base_folder+"\\shell\\master").c_str() ) ) {
		error("shell/master��������܂���B�ݒu�ʒu���m�F���Ă��������B");
		return	false;
	}

	// �ݒ�t�@�C���̓ǂݍ���
	GetSender().sender() << "�ݒ�t�@�C�� sodate.dat ��ǂݍ��݂܂��B" << endl;
	if ( !strmap_from_file(conf, base_folder+"\\sodate.dat", byte_value_1) ) {
		error("��� sodate_setup.exe �����s���Ă��������B");
		return	false;
	}

	// �Í��ۑ����ꂽ�p�X���[�h���f�R�[�h
	string	str = decode(conf["password"]);
	int	len = str.size()/2;
	byte*	buf = new byte[len+1];
	buf[len]='\0';
	string_to_binary(str, buf);
	xor_filter(buf, len, 186);
	conf["password"] = decode( (char*)buf );

	// �Â�updates2.dau���폜
	::DeleteFile( (base_folder+"\\updates2.dau").c_str() );
	::DeleteFile( (base_folder+"\\ghost\\master\\updates2.dau").c_str() );

	// �Ώۃt�@�C���I��
	cout << "�Ώۃt�@�C����I�蒆" << endl;
	strvec	allow_files_vec, deny_files_vec;
	stringset	deny_files_set;
	split(conf["allow_files"], byte_value_2, allow_files_vec);
	split(conf["deny_files"], byte_value_2, deny_files_vec);

	// ���O�t�@�C�����i���݂���΁jset��
	for (strvec::iterator i=deny_files_vec.begin() ; i!=deny_files_vec.end() ; ++i) {
		string	full_path = base_folder+"\\"+*i;
		replace(full_path, "/", "\\");
		string	folder_name = get_folder_name(full_path);

		WIN32_FIND_DATA	fd;
		HANDLE	h = ::FindFirstFile(full_path.c_str(), &fd);
		if ( h == INVALID_HANDLE_VALUE )
			continue;
		
		do {
			if ( fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
				continue;
			deny_files_set.insert(folder_name+"\\"+fd.cFileName);
		} while ( ::FindNextFile(h, &fd) );
		::FindClose(h);
	}

	// �Ώۃt�@�C�����A���O�ΏۂɈ�v���Ȃ����Ƃ��m�F����files��
	for (strvec::iterator i=allow_files_vec.begin() ; i!=allow_files_vec.end() ; ++i) {

		string	full_path = base_folder+"\\"+*i;
		replace(full_path, "/", "\\");
		string	folder_name = get_folder_name(full_path);

		WIN32_FIND_DATA	fd;
		HANDLE	h = ::FindFirstFile(full_path.c_str(), &fd);
		if ( h == INVALID_HANDLE_VALUE ) {
			GetSender().sender() << *i << "�ɊY������t�@�C��������܂���B" << endl;
			continue;
		}
		
		int	n=0;
		do {
			if ( fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
				continue;
			string	filename=folder_name+"\\"+fd.cFileName;
			if ( deny_files_set.find(filename) == deny_files_set.end() ) {
				files.push_back(filename);
				++n;
			}
		} while ( ::FindNextFile(h, &fd) );
		::FindClose(h);

		if ( n==0 ) {
			GetSender().sender() << *i << "�ɊY������t�@�C��������܂���B" << endl;
			continue;
		}
	}

	if ( files.size()==0 ) {
		error("�ΏۂƂȂ�t�@�C�������݂��܂���B");
		return	1;
	}
	else {

		// �{�Ƃ�updates2.dau�ɍ\�������킹�邽�߂̃��X�g�̃\�[�g

		list<string>	OLD = files;	// ���������
		files.clear();
		list<string>&	NEW = files;	// ����ۂ̂ق�

		// �܂��͒P�Ȃ�A���t�@�x�b�g�\�[�g
		files.sort();		

		// �܂���shell�t�H���_�̒��g���ڍs
		list<string>::iterator	i;;
		for (i=OLD.begin(); i!=OLD.end() ;) {
			if ( compare_head(get_folder_name(*i), base_folder+"\\shell\\") ) {
				NEW.push_back(*i);
				i=OLD.erase(i);
			} else
				++i;
		}

		// ������ghost�t�H���_�̒��g���ڍs
		for (i=OLD.begin(); i!=OLD.end() ;) {
			if ( compare_head(get_folder_name(*i), base_folder+"\\ghost\\") ) {
				NEW.insert(NEW.begin(), *i);	// ���]���Ȃ��悤��
				i=OLD.erase(i);
			} else
				++i;
		}

		// ghost, shell,���[�g�ȊO�̃t�H���_�̒��g���ڍs�i����̂��H�j
		for (i=OLD.begin(); i!=OLD.end() ;) {
			if ( !compare_head(get_folder_name(*i), base_folder) ) {
				NEW.push_back(*i);
				i=OLD.erase(i);
			} else
				++i;
		}

		// �c��B���[�g�t�H���_�̒��g���ڍs
		NEW.insert(NEW.end(), OLD.begin(), OLD.end());
		OLD.clear();
	}



	GetSender().sender() << "updates2.dau���쐬��" << endl;
	bool	makeUpdates2(string base_folder, const list<string>& files);
	if ( !makeUpdates2(szPath, files) ) {
		error("updates2.dau���쐬�ł��܂���ł����B");
		return	1;
	}
	GetSender().sender() << "updates2.dau���쐬����" << endl;
	// �������̂��R�s�[
	::CopyFile( (base_folder+"\\updates2.dau").c_str(), (base_folder+"\\ghost\\master\\updates2.dau").c_str(), FALSE );


	if ( conf["is_create_archive"]!="0") {
		GetSender().sender() << "zip/nar�A�[�J�C�u���쐬��" << endl;

		if ( !is_exist_file(base_folder+"\\install.txt") ) {
			error("zip/nar�A�[�J�C�u���쐬���邽�߂ɂ́A�S�[�X�g�̃��[�g�t�H���_��install.txt���K�v�ł��B");
			return	false;
		}

		SYSTEMTIME	st;
		char	buf[10];
		::GetLocalTime(&st);
		sprintf(buf, "%02d", st.wYear%100);
		replace(conf["archive_filename"], "%year", buf);
		sprintf(buf, "%04d", st.wYear);
		replace(conf["archive_filename"], "%year4", buf);
		sprintf(buf, "%02d", st.wMonth);
		replace(conf["archive_filename"], "%month", buf);
		sprintf(buf, "%02d", st.wDay);
		replace(conf["archive_filename"], "%day", buf);
		sprintf(buf, "%02d", st.wHour);
		replace(conf["archive_filename"], "%hour", buf);
		sprintf(buf, "%02d", st.wMinute);
		replace(conf["archive_filename"], "%minute", buf);
		sprintf(buf, "%02d", st.wSecond);
		replace(conf["archive_filename"], "%second", buf);

		if ( (conf["archive_local_folder"])[ conf["archive_local_folder"].size()-1 ] != '\\' )
			conf["archive_local_folder"] += "\\";

		string	archive_filename = conf["archive_local_folder"]+conf["archive_filename"];

		// ���ɂ���Ȃ�폜���Ă���
		::DeleteFile(archive_filename.c_str());

		/*
		string	cmd = "zip.exe";
		string	opt = "-r " + archive_filename;
		for (list<string>::const_iterator i=files.begin(); i!=files.end() ; ++i ) {
			string	filename(*i);
			opt += string(" ") + (filename.c_str()+base_folder.size()+1);
		}
		opt += " ghost\\master\\updates2.dau";
		if ( opt.find(" install.txt")==string::npos )
			opt += " install.txt";	// nar���ɂ͕K�{�Ȃ̂ŁB
		opt += " > nul";
		*/
		//system((cmd+" "+opt).c_str());
		//::ShellExecute(NULL, NULL, (cmd+" "+opt).c_str(), NULL, NULL, SW_SHOW);
		//if ( !is_exist_file(archive_filename) ) {

		list<string>	zip_files;
		bool	exist_install_txt=false;
		for (list<string>::const_iterator i=files.begin(); i!=files.end() ; ++i ) {
			zip_files.push_back(i->c_str()+base_folder.size()+1);
			if ( zip_files.back()=="install.txt" )
				exist_install_txt = true;
		}
		zip_files.push_back("ghost\\master\\updates2.dau");
		if ( !exist_install_txt )
			zip_files.push_back("install.txt");

		if ( !makeZip(archive_filename, zip_files) ) {
			error("�A�[�J�C�u�t�@�C��'"+archive_filename+"'���쐬�ł��܂���ł����B");
		}
		else {
			GetSender().sender() << "zip/nar�A�[�J�C�u '" << archive_filename << "' ���쐬����" << endl;
		}
	}

	files.push_back(base_folder+"\\updates2.dau");
	// �t�H���_�ʂɕ����i�[ as global
	//		map<string, set<string> >	files_on_dir;	// dirname : filenames
	{
		for (list<string>::iterator i=files.begin(); i!=files.end() ; ++i) {
			string	rel_path = (i->c_str()+base_folder.size()+1);
			files_on_dir[ get_folder_name(rel_path) ].insert( get_file_name(rel_path) );
		}
	}

	bool	uploadFiles(const string&, list<string>&);
	if ( conf["is_upload"]!="0")
		uploadFiles(szPath, files);

	// ���[�g��updates2.dau�̓A�b�v���[�h������ɍ폜�B
	::DeleteFile( (base_folder+"\\updates2.dau").c_str() );

	return	0;
}


// files�ɂ� ���̂܂܃I�[�v���\�ȃt���p�X�������Ă��邱�ƁB
// updates2.dau�ɂ�base_folder���������������΃p�X���i�[�����B
bool	makeUpdates2(string base_folder, const list<string>& files) {
	ofstream	o((base_folder+"\\updates2.dau").c_str());
	if ( !o.is_open() )
		return	false;

	for (list<string>::const_iterator i=files.begin(); i!=files.end() ; ++i ) {
		
		File	file;
		if ( !file.Open( i->c_str(), File::READ) )
			continue;
		DWORD	size;
		if ( !file.GetSize(&size) )
			continue;
		char*	p = new char[size];
		if ( p==NULL )
			continue;
		if ( !file.Read(p, size) ) {
			delete [] p;
			continue;
		}

		MyMD5	md5calc;
		char	md5[33];
		md5calc.MD5_String(p, size, md5);
		delete [] p;
		file.Close();

		string	filename(*i);
		replace(filename, "\\", "/");
		char	sep[2] = { 1, 0 };
		o << (filename.c_str()+base_folder.size()+1) << sep << md5 << sep << endl;
		if ( conf["morelog"]!="0" )
			GetSender().sender() << (filename.c_str()+base_folder.size()+1) << " " << md5 << endl;
	}
	o.close();
	
	return	true;
}




