#include "SakuraDLLClient.h"
#include "../_/Sender.h"
#include <assert.h>
#ifdef POSIX
#  include <dlfcn.h>
#  include <string.h>
#  include <stdlib.h>
#  define FALSE 0
#endif

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

SakuraDLLClient::SakuraDLLClient()
{
	mModule = NULL;
	mLoad = NULL;
	mRequest = NULL;
	mUnload = NULL;
}

SakuraDLLClient::~SakuraDLLClient()
{
	unload();
}

string	SakuraDLLClient::request(const string& iRequestString)
{
	if ( mRequest==NULL )
	{
		GetSender().errsender() << "SakuraDLLClient::request: ���[�h���Ă��Ȃ����C�u������request���悤�Ƃ��܂����B" << endl;
		return	"";
	}

	long len = iRequestString.length();

#ifdef POSIX
	char* h = static_cast<char*>(malloc(len + 1));
#else
	HGLOBAL h = ::GlobalAlloc(GMEM_FIXED, len + 1);
#endif

	memcpy(h, iRequestString.c_str(), len + 1); //ZeroTerm�܂�
	h = mRequest(h, &len);

	if ( h ) {
	#ifdef POSIX
		string theResponse(static_cast<char*>(h), len);
		free(h);
	#else
		void *pLock = ::GlobalLock(h);
		if ( pLock ) {
			string theResponse(static_cast<char*>(pLock), len);
			::GlobalFree(h);
			return theResponse;
		}
		else { //�o�O�΍� - GlobalAlloc�Ŋm�ۂ��ĂȂ��|�C���^����
			return string(static_cast<char*>(h), len);
		}
	#endif

	}
	else {
		return string("");
	}
}

// �o�[�W�����擾�BGET Version����"SAORI/1.0" �݂����̂�Ԃ��B
string SakuraDLLClient::get_version(const string& i_security_level)
{
	strpairvec data;
	data.push_back( strpair("Sender", m_sender) );
	data.push_back( strpair("Charset", m_charset) );
	data.push_back( strpair("SecurityLevel", i_security_level) );

	string r_protocol, r_protocol_version;
	strpairvec r_data;
	this->SakuraClient::request(
		m_protocol,
		m_protocol_version,
		"GET Version",
		data,
		r_protocol,
		r_protocol_version,
		r_data);

	return r_protocol + "/" + r_protocol_version;
}

// ���N�G�X�g�𑗂�A���X�|���X���󂯎��B�߂�l�̓��^�[���R�[�h�B
int SakuraDLLClient::request(
	const string& i_command,
	const strpairvec& i_data,
	strpairvec& o_data)
{
	string r_protocol, r_protocol_version;
	int return_code = this->SakuraClient::request(
		m_protocol,
		m_protocol_version,
		i_command,
		i_data,
		r_protocol,
		r_protocol_version,
		o_data);
	return return_code;
}

void	SakuraDLLClient::unload()
{
#ifdef POSIX
    if (mModule != NULL) {
	    dlclose(mModule);
	}
#else
	if ( mUnload != NULL )
		mUnload();
	if ( mModule != NULL )
		::FreeLibrary(mModule);
#endif
	mModule = NULL;
	mLoad = NULL;
	mRequest = NULL;
	mUnload = NULL;
}



bool	SakuraDLLClient::load(
	const string& i_sender,
	const string& i_charset,
	const string& i_protocol,
	const string& i_protocol_version,
	const string& i_work_folder,
	const string& i_dll_fullpath)
{
	unload();

	m_sender = i_sender;
	m_charset = i_charset;
	m_protocol = i_protocol;
	m_protocol_version = i_protocol_version;
	string work_folder = unify_dir_char(i_work_folder);
	string dll_fullpath = unify_dir_char(i_dll_fullpath);

	GetSender().sender() << "SakuraDLLClient::load '" << dll_fullpath << "' ...";
#ifdef POSIX
	mModule = dlopen(dll_fullpath.c_str(), RTLD_LAZY);
	if (mModule == NULL) {
	    GetSender().sender() << "failed." << endl;
	    GetSender().errsender() << dlerror() << endl;
	    return false;
	}
#else
	mModule = ::LoadLibraryEx(dll_fullpath.c_str(),NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
	if ( mModule==NULL ) {
		GetSender().sender() << "failed." << endl;
		GetSender().errsender() << dll_fullpath + ": LoadLibrary�Ŏ��s�B" << endl;
		return	false;
	}
#endif

#ifdef POSIX
	mLoad = (bool (*)(char*, long))dlsym(mModule, "load");
	mRequest = (char* (*)(char*, long*))dlsym(mModule, "request");
	mUnload = (bool (*)())dlsym(mModule, "unload");
#else
	mLoad = (BOOL (*)(HGLOBAL, long))::GetProcAddress(mModule, "load");
	if ( ! mLoad ) {
		mLoad = (BOOL (*)(HGLOBAL, long))::GetProcAddress(mModule, "_load");
	}

	mRequest = (HGLOBAL (*)(HGLOBAL, long*))::GetProcAddress(mModule, "request");
	if ( ! mRequest ) {
		mRequest = (HGLOBAL (*)(HGLOBAL, long*))::GetProcAddress(mModule, "_request");
	}

	mUnload = (BOOL (*)())::GetProcAddress(mModule, "unload");
	if ( ! mUnload ) {
		mUnload = (BOOL (*)())::GetProcAddress(mModule, "_unload");
	}

#endif
	if ( mRequest==NULL )
	{
		GetSender().sender() << "failed." << endl;
		unload();
		GetSender().errsender() << dll_fullpath + ": request���G�N�X�|�[�g����Ă��܂���B" << endl;
		return	false;
	}
	if ( mLoad!=NULL )
	{
		long len = work_folder.length();
#ifdef POSIX
		char* h = static_cast<char*>(malloc(len + 1));
#else
		HGLOBAL h = ::GlobalAlloc(GMEM_FIXED, len + 1);
#endif
		memcpy(h, work_folder.c_str(), len + 1); //ZeroTerm�܂�
		if ( mLoad(h, len) == FALSE )
		{
			GetSender().sender() << "failed." << endl;
			unload();
			GetSender().errsender() << dll_fullpath + ": load()��FALSE��Ԃ��܂����B" << endl;
			return	false;
		}
	}

	GetSender().sender() << "succeed." << endl;
	return	true;
}

