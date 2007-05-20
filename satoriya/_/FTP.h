#ifdef _MSC_VER 
// �e���v���[�g�����������̌x����}��
#pragma warning( disable : 4786 ) //�u�f�o�b�O�����ł̎��ʎq�؎̂āv
#pragma warning( disable : 4503 ) //�u�������ꂽ���O�̒��������E���z���܂����B���O�͐؂�̂Ă��܂��B�v
// for�X�R�[�v��ANSI����������
#ifndef for
#define for if(0);else for
#endif	// for
#endif	// _MSC_VER

#include	<windows.h>
#include	<wininet.h>
#include	<stdio.h>

#include	<string>
#include	<iostream>
#include	<map>
#include	<cassert>
using namespace std;

class WinInetHandle
{
	HINTERNET h;
public:
	WinInetHandle() : h(NULL) {}
	~WinInetHandle() { close(); }

	bool open(string i_client_name)
	{
		if ( h == NULL )
		{
			h = ::InternetOpen(
				i_client_name.c_str(),
				INTERNET_OPEN_TYPE_PROXY,//INTERNET_OPEN_TYPE_DIRECT,
				NULL,
				NULL,
				0);
		}
		return (h != NULL);
	}

	void close()
	{
		if ( h != NULL )
		{
			::InternetCloseHandle(h);
		}
	}

	operator HINTERNET() { return h; }
};

class FTP
{
public:
	FTP(HINTERNET iInternet);
	~FTP();

	bool	open(const string& host, const string& id, const string& password, bool is_passive=false);
	void	close();
	
	bool	cd(const string& subdir);
	bool	pwd(string& o_dir);
	bool	mkdir(const string& subdir);
	bool	put(const string& local_filepath, const string& upload_filename, bool isBinary=true);
	bool	get(const string& local_filepath, const string& upload_filename, bool isBinary=true);
	
	// ����Ɍ���A����Z�b�V�������iopen���Ă���close����܂Łj�P�񂵂����s�ł��Ȃ�
	void	ls(map<string, WIN32_FIND_DATA>& oFileList);

private:
	HINTERNET	mInternet;
	bool mIsSelfOpenInternetHandle;
	HINTERNET	mFtpSession;
};

