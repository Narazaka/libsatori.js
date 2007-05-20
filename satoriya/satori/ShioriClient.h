#include "SakuraDLLClient.h"

// SHIORI/3.0�N���C�A���g
class ShioriClient : public SakuraDLLClient
{
public:
	ShioriClient() {}
	virtual ~ShioriClient() {}

	bool load(
		const string& i_sender, // �N���C�A���g���BUSER_AGENT�݂����Ȃ��́B
		const string& i_charset, // �����R�[�h�Z�b�g�B�ʏ��Shift_JIS
		const string& i_work_folder, // �x��load�ɓn���t�H���_���i�����̂���t�H���_���j
		const string& i_dll_fullpath); // �x�̃t���p�X

	int request(
		const string& i_id, // OnBoot�Ƃ�
		const vector<string>& i_references, // Reference?
		bool i_is_secure, // SecurityLevel
		string& o_value, // �x���Ԃ���������X�N���v�g�╶���񃊃\�[�X
		vector<string>& o_references // �����߂�l�B�ʏ��OnCommunicate�ł����g��Ȃ�
		);
};