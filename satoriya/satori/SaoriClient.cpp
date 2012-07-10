#include "SaoriClient.h"
#include "../_/stltool.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

bool SaoriClient::load(
	const string& i_sender,
	const string& i_charset,
	const string& i_work_folder,
	const string& i_dll_fullpath)
{
	return this->SakuraDLLClient::load(i_sender, i_charset, "SAORI", "1.0", i_work_folder, i_dll_fullpath);
}

int SaoriClient::request(
	const vector<string>& i_argument,
	bool i_is_secure,
	string& o_result,
	vector<string>& o_value)
{
	//---------------------
	// ���N�G�X�g�쐬

	strpairvec data;

	data.push_back( strpair("Charset", "Shift_JIS" ) );
	data.push_back( strpair("Sender", "SATORI" ) );
	data.push_back( strpair("SecurityLevel", (i_is_secure ? "Local" : "External") ) );

	int idx=0;
	for ( vector<string>::const_iterator i=i_argument.begin() ; i!=i_argument.end() ; ++i,++idx )
	{
		data.push_back( strpair(string("Argument")+itos(idx), *i) );
	}

	//---------------------
	// ���N�G�X�g���s

	strpairvec r_data;
	int return_code = this->SakuraDLLClient::request("EXECUTE", data, r_data);

	//---------------------
	// �ԓ������
	// ���ӁIValue�w�b�_�����݂��Ȃ��Ƃ��́AS?�n�V�X�e���ϐ����������邽�߂Ƀf�[�^���㏑�����Ȃ����ƁI
	// ���S�[�X�g�̌݊������ɒ��Ӂio_value�j

	string result;
	int maxValueSize = -1;

	for ( strpairvec::const_iterator i = r_data.begin() ; i != r_data.end() ; ++i )
	{
		const string& key = i->first;
		const string& value = i->second;

		if ( compare_head(key, "Value") && isdigit(key[const_strlen("Value")]) )
		{
			const int	pos = atoi(key.c_str() + const_strlen("Value"));
			if ( pos<0 || pos>65536 )
			{
				continue;
			}

			if ( o_value.size() <= pos )
			{
				o_value.resize(pos+1);
			}
			o_value[pos] = value;
			if ( maxValueSize < pos ) {
				maxValueSize = pos;
			}
		}
		else if ( key=="Result" )
		{
			o_result = value;
		}
	}

	//Value�w�b�_���Ȃ������ꍇ�̂ݐ؂�l�߂�
	if ( maxValueSize >= 0 ) {
		o_value.resize(maxValueSize+1);
	}

	return return_code;
}
