#ifndef SAKURACLIENT_H
#define SAKURACLIENT_H

#include "SakuraCS.h"

// SHIORI, SAORI, MAKOTO, SSTP�Ȃǂ̃N���C�A���g�B
// ���N�G�X�g�𑗂鑤�̏������z�x�[�X�N���X�B
class SakuraClient : public SakuraCS
{
public:
	SakuraClient() {}
	virtual ~SakuraClient() {}
	
	// �f�̃��N�G�X�g������𑗂�A�f�̃��X�|���X��������󂯂Ƃ�B
	// �����͒ʐM��i�ɂ��B
	virtual string request(const string& i_request_string)=0;

	// ���N�G�X�g�𑗂�A���X�|���X���󂯎��B�߂�l�̓��^�[���R�[�h�B
	virtual int request(
		const string& i_protocol,
		const string& i_protocol_version,
		const string& i_command,
		const strpairvec& i_data,
		
		string& o_protocol,
		string& o_protocol_version,
		strpairvec& o_data);
};


#endif // SAKURACLIENT_H
