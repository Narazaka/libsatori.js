	/*

		files.clear();	// ���̃��X�g���N���A


		list<string>	dir_names;
		for (map<string, set<string> >::iterator j=files_on_dir.begin(); j!=files_on_dir.end() ; ++j)
			dir_names.push_back(j->first);
		dir_names.sort();
		dir_names.erase(base_folder);	// ���[�g��



		for (j=files_on_dir.begin(); j!=files_on_dir.end() ; ++j) {

			set<string>&	a_folder_files = j->second();
			list<string>	temp;

			for (set<string>::iterator k=a_folder_files.begin() ; k!=a_folder_files.end() ; ++k) {


			}

				// �Ċi�[
		}
		*/



// �w��t�H���_�ȉ��̑S�t�@�C�����擾����
void	do_folder(string iBaseFolder, string iSubFolders, list<string>& oList) {

	vector<WIN32_FIND_DATA>	datas;

	FileLister	lister;
	if ( !lister.Open((iBaseFolder+"\\"+iSubFolders+"*.*").c_str()) )
		return;
	while ( lister.Next() )
		datas.push_back( *lister.Data() );
	lister.Close();

	for (vector<WIN32_FIND_DATA>::iterator i=datas.begin(); i!=datas.end() ; ++i ) {
		const char* theFileName = i->cFileName;
		if ( strcmp(theFileName, ".")==0 )
			continue;
		if ( strcmp(theFileName, "..")==0 )
			continue;
		if ( deny_filename_set.find( iSubFolders+theFileName ) != deny_filename_set.end() )
			continue;

		if ( i->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY )
			do_folder(iBaseFolder, iSubFolders + theFileName + "\\", oList);
		else
			oList.push_back(iSubFolders+theFileName);
	}
}



	// ���O�t�@�C��set�̍쐬
	strvec	deny;
	if ( strvec_from_file(deny, base_folder+"\\network_updater_deny.txt") ) {
		for (strvec::iterator i=deny.begin(); i!=deny.end() ; ++i ) {
			replace(*i, "/", "\\");
			deny_filename_set.insert(*i);
		}
	}


	// �����͍̂폜���Ƃ��i���Ƃő����Bfiles�ł̏d���������ړI�j
	::DeleteFile((base_folder+"\\updates2.dau").c_str());	

	// ���O���t�@�C���ꗗ���쐬
	do_folder(base_folder, "", files);

	// ����
	files.push_front("updates2.dau");



