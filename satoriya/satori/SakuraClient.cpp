#include "SakuraClient.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////


// ���N�G�X�g�𑗂�A���X�|���X���󂯎��B�߂�l�̓��^�[���R�[�h�B
int SakuraClient::request(
	const string& i_protocol,
	const string& i_protocol_version,
	const string& i_command,
	const strpairvec& i_data,
	
	string& o_protocol,
	string& o_protocol_version,
	strpairvec& o_data)
{
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ���N�G�X�g��������쐬

	string	request = i_command + " " + i_protocol + "/" + i_protocol_version + CRLF;
	for ( strpairvec::const_iterator it = i_data.begin() ; it != i_data.end() ; ++it )
	{
		request += it->first + ": " + it->second + CRLF;
	}
	request += CRLF;


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ���N�G�X�g���s

	string response = this->request(request);


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ���Ԏ������

	// ��s�ڂ�؂�o��
	o_protocol = cut_token(response, "/");
	o_protocol_version = cut_token(response, " ");
	int return_code = atoi( cut_token(response, " ").c_str() );
	string return_string = cut_token(response, CRLF);

	// �ȍ~�̃f�[�^�s��؂�o��
	while ( response.size() > 0 )
	{
		string value = cut_token(response, CRLF);
		string key = cut_token(value, ": ");
		o_data.push_back( strpair(key, value) );
	}
	
	return return_code;
}
