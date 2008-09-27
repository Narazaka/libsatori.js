#include "SakuraClient.h"

#ifndef POSIX
#  include <windows.h> // HMODULE,BOOL,HGLOBAL�Ƃ�
#endif


// dll�z���_�[�Bdll�����[�h���ă��N�G�X�g�𑗂�
class SakuraDLLClient : public SakuraClient
{
public:
	SakuraDLLClient();
	virtual ~SakuraDLLClient();

	// �͂��߂��
	virtual bool load(
		const string& i_sender,
		const string& i_charset,
		const string& i_protocol,
		const string& i_protocol_version,
		const string& i_work_folder,	// ��ƃf�B���N�g���BDLL::load�̈����B�Ō�� \ �܂��� / ���K�v
		const string& i_dll_fullpath);	// DLL�̃t���p�X

	// ������
	virtual void unload();

	// �o�[�W�����擾�BGET Version����"SAORI/1.0" �݂����̂�Ԃ��B
	virtual string get_version(const string& i_security_level);

	// �f�̃��N�G�X�g������𑗂�A�f�̃��X�|���X��������󂯎��B
	virtual string request(const string& i_request_string);

	// ���N�G�X�g�𑗂�A���X�|���X���󂯎��B�߂�l�̓��^�[���R�[�h�B
	virtual int request(
		const string& i_command,
		const strpairvec& i_data,
		strpairvec& o_data);

private:
	string m_sender;
	string m_charset;
	string m_protocol;
	string m_protocol_version;

#ifdef POSIX
	void*   mModule;
	bool    (*mLoad)(char* h, long len);
	char*   (*mRequest)(char* h, long* len);
	bool    (*mUnload)();
#else
	HMODULE	mModule;
	BOOL	(*mLoad)(HGLOBAL h, long len);
	HGLOBAL	(*mRequest)(HGLOBAL h, long* len);
	BOOL	(*mUnload)();
#endif
};
