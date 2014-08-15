#ifdef POSIX
#  include      "Utilities.h"
#  include      <iostream>
#  include      <stdlib.h>
#  include      <sys/types.h>
#  include      <sys/stat.h>
#  include      <dlfcn.h>
#else
#  include	<windows.h>
#  include	<mbctype.h>
#endif
#include	<assert.h>
#include	"../_/stltool.h"
#include	"../_/Sender.h"
#include	"shiori_plugin.h"
#include	"console_application.h"
#include	<sstream>
using namespace std;

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////


#ifdef POSIX
// dll�T�[�`�p�X�֘A�B
// POSIX��ł́A�������̏ꏊ��DLL�Ɠ����̃��C�u������u������SAORI�ɑΉ�����B
// DLL�Ɠ����Ƃ͉]���Ă��A����̓V���{���b�N�����N�ł���ׂ��ŁA�Ⴆ�Ύ��̂悤�ɂł���B
// % pwd
// /home/foo/.saori
// % ls -l
// -rwxr-xr-x x foo bar xxxxx 1 1 00:00 libssu.so
// lrwxr-xr-x x foo bar xxxxx 1 1 00:00 ssu.dll -> libssu.so
//
// �p�X�͊��ϐ� SAORI_FALLBACK_PATH ����擾����B����̓R������؂�̐�΃p�X�ł���B
static vector<string> posix_dll_search_path;
static bool posix_dll_search_path_is_ready = false;
static string posix_search_fallback_dll(const string& dllfile) {
    // dllfile�͒T�������t�@�C��DLL���B�p�X��؂蕶����/�B
    // ��փ��C�u���������t����΂��̐�΃p�X���A
    // ���t�����Ȃ���΋󕶎����Ԃ��B
    
    if (!posix_dll_search_path_is_ready) {
	// SAORI_FALLBACK_PATH������B
	char* cstr_path = getenv("SAORI_FALLBACK_PATH");
	if (cstr_path != NULL) {
	    split(cstr_path, ":", posix_dll_search_path);
	}
	posix_dll_search_path_is_ready = true;
    }

    string::size_type pos_slash = dllfile.rfind('/');
    string fname(
	dllfile.begin() + (pos_slash == string::npos ? 0 : pos_slash),
	dllfile.end());

    for (vector<string>::const_iterator ite = posix_dll_search_path.begin();
	 ite != posix_dll_search_path.end(); ite++ ) {
	string fpath = *ite + '/' + fname;
	struct stat sb;
	if (stat(fpath.c_str(), &sb) == 0) {
	    // ��փ��C�u���������݂���悤���B����ȏ�̃`�F�b�N�͏ȗ��B
	    return fpath;
	}
    }
    return string();
}
#endif

bool ShioriPlugins::load(const string& iBaseFolder)
{
	mBaseFolder = iBaseFolder;

	// mBaseFolder���X���b�V���ŏI����Ă��Ȃ���΁A�t����B
	if ( false == mBaseFolder.empty() && mBaseFolder[mBaseFolder.length() - 1] != DIR_CHAR)
	{
	    mBaseFolder += DIR_CHAR;
	}
	return true;
}

bool ShioriPlugins::load_a_plugin(const string& iPluginLine)
{
	strvec	vec;
	split(iPluginLine, ",", vec);	// �J���}��؂�ŕ���
	if ( vec.size()<2 || vec[0].size()==0 ) {	// �Ăяo�����Ƒ��΃p�X���K�{
		GetSender().errsender() << iPluginLine + ": �ݒ�t�@�C���̏���������������܂���B" << std::endl;
		return	false;
	}
	if ( mCallData.find(vec[0]) != mCallData.end() ) {
		GetSender().errsender() << vec[0] + ": �����Ăяo������������`����Ă��܂��B" << std::endl;
		return	false;
	}

	// �t�H���_��؂�����ɉ����ĕ��ɓ���
	string filename = unify_dir_char(vec[1]);
	string fullpath = unify_dir_char(mBaseFolder + filename);

	if ( mDllData.find(fullpath) != mDllData.end() ) 
	{
		// ���ɓo�^�ς݂Ȃ�Q�Ƃ𑝂₷
		mDllData[fullpath].mRefCount += 1;
	}
	else 
	{
		// ���o�^�Ȃ�ǂݍ���

#ifndef POSIX
		// �l�b�g���[�N�X�V���ASAORI.dll���㏑���ł���dl2�Ƃ��ĕۑ��������Ɏb��Ώ�
		if ( compare_tail(fullpath, ".dll") )
		{
			string	dl2 = fullpath;
			dl2[dl2.size()-1]='2';
			FILE*	fp = fopen(dl2.c_str(), "rb");
			if ( fp != NULL )
			{
				fclose(fp);
				
				// .dll��������.dl2��.dll�Ƀ��l�[������B
				::DeleteFile(fullpath.c_str());
				::MoveFile(dl2.c_str(), fullpath.c_str());
			}
		}
#endif
		
		// �t�@�C���̑��݂��m�F
		FILE*	fp = fopen(fullpath.c_str(), "rb");
		if ( fp == NULL )
		{
#ifdef POSIX
            GetSender().errsender() << fullpath + ": failed to open" << std::endl;
#else
			GetSender().errsender() << fullpath + ": �v���O�C�������݂��܂���B" << std::endl;
#endif
			return	false;
		}
		fclose(fp);

		// �t�@�C�����E�t�H���_���E�g���q�𕪗�
		const char*	lastyen = NULL;
		const char*	lastdot = NULL;
		const char*	p = fullpath.c_str();
		while (*p != '\0') {
			if (*p == DIR_CHAR)
				lastyen=p;
			else if (*p == '.')
				lastdot=p;
			p += _ismbblead(*p) ? 2 : 1; 
		}
		if ( lastyen==NULL || lastdot==NULL )
		{
			return false;
		}

		string  dll_full_path(fullpath);
		string	foldername(fullpath.c_str(), lastyen - fullpath.c_str());
		string	filename(lastyen+1, strlen(lastyen)-strlen(lastdot)-1);
		string	extention(lastdot+1);

#ifdef POSIX
		// ���ϐ� SAORI_FALLBACK_ALWAYS ����`����Ă��āA����
		// ��ł�"0"�ł��Ȃ���΁A����dll�t�@�C�����J���Ă݂鎖��
		// ���߂�����Ȃ��B�����łȂ���΁A������dlopen���Ă݂�B
		char* env_fallback_always = getenv("SAORI_FALLBACK_ALWAYS");
		bool fallback_always = false;
		if (env_fallback_always != NULL) {
		    string str_fallback_always(env_fallback_always);
		    if (str_fallback_always.length() > 0 &&
			str_fallback_always != "0") {
			fallback_always = true;
		    }
		}
		bool do_fallback = true;
		if (!fallback_always) {
		    void* handle = dlopen(fullpath.c_str(), RTLD_LAZY);
		    if (handle != NULL) {
			// load, unload, request����o���Ă݂�B
			void* sym_load = dlsym(handle, "load");
			void* sym_unload = dlsym(handle, "unload");
			void* sym_request = dlsym(handle, "request");
			if (sym_load != NULL && sym_unload != NULL && sym_request != NULL) {
			    // �Ȃ�Ɛ���ɓǂ߂Ă��܂����B�����ړI�ō���������낤���B
			    do_fallback = false;
			}
		    }
		    dlclose(handle);
		}
		if (do_fallback) {
		    // ��փ��C�u������T���B
		    string fallback_lib = posix_search_fallback_dll(filename+"."+extention);
		    if (fallback_lib.length() == 0) {
			// �����B
			char* cstr_path = getenv("SAORI_FALLBACK_PATH");
			string fallback_path =
			    (cstr_path == NULL ?
			     "(environment variable `SAORI_FALLBACK_PATH' is empty)" : cstr_path);
			
			GetSender().errsender() <<
			    fullpath+": This is not usable in this platform.\n"+
			    "Fallback library `"+filename+"."+extention+"' doesn't exist: "+fallback_path) << std::endl;
			
			mDllData.erase(fullpath);

			return false;
		    }
		    else {
			cerr << "SAORI: using " << fallback_lib << " instead of " << fullpath << endl;
		    }

		    // �Q�ƃJ�E���g�Ɏg�����߁Afullpath�͏������Ȃ��B
		    // dll_full_path�̂݁B
		    dll_full_path = fallback_lib;
		}
#endif

		// POSIX���ł́A���C�u��������*.dll�ȊO�������B
#ifdef POSIX
		if ( 1 )
#else
		if ( compare_tail(fullpath, ".dll") )
#endif
		{
			// �v���O�C��DLL�����[�h
			mDllData[fullpath].mRefCount=1;
			extern const char* gSatoriName;
			if ( !(mDllData[fullpath].mSaoriClient.load(gSatoriName, "Shift_JIS", foldername+DIR_CHAR, dll_full_path)) )
			{
				mDllData.erase(fullpath);
				return	false;
			}

			// �o�[�W�����m�F
			string ver = mDllData[fullpath].mSaoriClient.get_version("Local");
			if ( ver != "SAORI/1.0" )
			{
				GetSender().errsender() << fullpath + ": SAORI/1.0��dll�ł͂���܂���BGET Version�̖߂�l�����Ή��̂��̂ł����B(" + ver + ")" << std::endl;
				mDllData.erase(fullpath);
				return	false;
			}
		}
	}

	// �Ăяo����map�ɓo�^
	mCallData[vec[0]].mDllPath = fullpath;
#ifdef POSIX
	mCallData[vec[0]].mIsBasic = false; // �g���q�ł͔��f�ł��Ȃ��̂ŁA�Ƃ肠����Saori Basic�̃T�|�[�g�͖����c
#else
	mCallData[vec[0]].mIsBasic = !compare_tail(fullpath, ".dll");
#endif
	strvec::const_iterator j=vec.begin();
	for ( j+=2 ; j!=vec.end() ; ++j )
		mCallData[vec[0]].mPreDefinedArguments.push_back(*j);

	return	true;
}

void	ShioriPlugins::unload()
{
	for ( map<string, CallData>::iterator i=mCallData.begin() ; i!=mCallData.end() ; ++i ) {
		if ( i->second.mIsBasic )
			continue;
		DllData&	ddat = mDllData[i->second.mDllPath];
		if ( --ddat.mRefCount == 0 ) {
			ddat.mSaoriClient.unload();
			mDllData.erase(i->second.mDllPath);
		}
	}
	mCallData.clear();
}

string	ShioriPlugins::request(const string& iCallName, const strvec& iArguments, strvec& oResults, const string& iSecurityLevel) {

	if ( mCallData.find(iCallName) == mCallData.end() ) {
		GetSender().errsender() << iCallName + ": ���̌Ăяo�����͒�`����Ă��܂���B" << std::endl;
		return	"";
	}
	CallData&	theCallData = mCallData[iCallName];

	if ( theCallData.mIsBasic ) 
	{
		// SAORI-basic�̌Ăяo��

		// �����ׁ[��external�Ăяo���𔻕ʂ���\�͂������̂Łi���ϐ����œn���΂����H�j
		// ���̎��_�őS�Đ؂�܂��B-universal�̓��N�G�X�g����security level�w�b�_��n���ASAORI���Ŕ��ʂ��Ă��������܂��B
		if ( iSecurityLevel != "local" && iSecurityLevel != "Local" )
			return	""; 

		string	theCommandLine = theCallData.mDllPath;
		strvec::const_iterator i;
		for ( i=theCallData.mPreDefinedArguments.begin() ; i!=theCallData.mPreDefinedArguments.end() ; ++i )
			theCommandLine += " "+ *i;
		for ( i=iArguments.begin() ; i!=iArguments.end() ; ++i )
			theCommandLine += " "+ *i;

		string out;
		string r = call_console_application(
			theCommandLine,
			get_folder_name(theCallData.mDllPath).c_str(),
			out);
		if ( r != "" )
		{
			// �G���[
			GetSender().errsender() << iCallName + ": " + r << std::endl;
			return "";
		}
		else
		{
			// ����I��
			return out;
		}
	}
	else 
	{
		// SAORI-universal�̌Ăяo��

		//---------------------
		// ���N�G�X�g�쐬

		vector<string> req;
		req.insert(req.end(), theCallData.mPreDefinedArguments.begin(), theCallData.mPreDefinedArguments.end());
		req.insert(req.end(), iArguments.begin(), iArguments.end());

		//---------------------
		// ���N�G�X�g���s

		assert( mDllData.find(theCallData.mDllPath) != mDllData.end() );

		string result;
		int return_code = mDllData[ theCallData.mDllPath ].mSaoriClient.request(
			 req,
			 ( iSecurityLevel == "local" || iSecurityLevel == "Local" ),
			 result,
			 oResults);

		//---------------------
		// �ԓ��ɑΏ�

		switch (return_code)
		{
		case 200:
		case 204:
			break;
		case 400:
			GetSender().errsender() << theCallData.mDllPath + " - " + iCallName + " : 400 Bad Request / �Ăяo���̕s��" << std::endl;
			break;
		case 500:
			GetSender().errsender() << theCallData.mDllPath + " - " + iCallName + " : 500 Internal Server Error / saori���ł̃G���[" << std::endl;
			break;
		default:
			GetSender().errsender() << theCallData.mDllPath + " - " + iCallName + " : " + itos(return_code) + "? / ��`����Ă��Ȃ��X�e�[�^�X��Ԃ��܂����B" << std::endl;
			break;
		}

		return	result;
	}
}


