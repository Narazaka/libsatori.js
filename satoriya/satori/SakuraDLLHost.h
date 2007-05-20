#include "SakuraCS.h"

// �udll�Ƃ��ČĂ΂�鑤�v�����ۂɎg�p����N���X�B
// ������p�������N���X���`���Am_dll�ɗB��̎��̂�������B
class SakuraDLLHost : public SakuraCS
{
protected:
	static SakuraDLLHost* m_dll;
public:
	static SakuraDLLHost* I() { return m_dll; }

	SakuraDLLHost() : SakuraCS() {}
	virtual ~SakuraDLLHost() {}

	virtual bool	load(const string& i_base_folder) { return true; }
	virtual bool	unload() { return true; }

	// �f�̃��N�G�X�g��������󂯎��A�f�̃��X�|���X�������Ԃ��B
	// �����Ł����ĂԁB
	virtual string request(const string& i_request_string);
	
	// ���N�G�X�g�����s�B
	// �p�����ăI�[�o�[���C�h���Ă��������B
	// �߂�l�̓��^�[���R�[�h�B��蓾��l��200,204,400,500�B
	virtual int	request(
		const string& i_protocol,
		const string& i_protocol_version,
		const string& i_command,
		const strpairvec& i_data,
		
		string& o_protocol,
		string& o_protocol_version,
		strpairvec& o_data)=0;
};
