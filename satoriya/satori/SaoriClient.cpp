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

	int idx=0;
	for ( vector<string>::const_iterator i=i_argument.begin() ; i!=i_argument.end() ; ++i,++idx )
	{
		data.push_back( strpair(string("Argument")+itos(idx), *i) );
	}
	data.push_back( strpair("SecurityLevel", (i_is_secure ? "Local" : "External") ) );

	//---------------------
	// ���N�G�X�g���s

	strpairvec r_data;
	int return_code = this->SakuraDLLClient::request("EXECUTE", data, r_data);

	//---------------------
	// �ԓ������

	string result;

	for ( strpairvec::const_iterator i = r_data.begin() ; i != r_data.end() ; ++i )
	{
		const string& key = i->first;
		const string& value = i->second;

		if ( compare_head(key, "Value") && isdigit(key[strlen("Value")]) )
		{
			const int	pos = atoi(key.c_str() + strlen("Value"));
			if ( pos<0 || pos>65536 )
			{
				continue;
			}

			if ( o_value.size() <= pos )
			{
				o_value.resize(pos+1);
			}
			o_value[pos] = value;
		}
		else if ( key=="Result" )
		{
			o_result = value;
		}
	}

	return return_code;
}
