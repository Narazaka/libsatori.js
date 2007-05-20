#include "SaoriHost.h"
#include <stdlib.h>

int	SaoriHost::request(
	const string& i_protocol,
	const string& i_protocol_version,
	const string& i_command,
	const strpairvec& i_data,
	
	string& o_protocol,
	string& o_protocol_version,
	strpairvec& o_data)
{
	// �Ԃ��v���g�R���͍ŏ��Ɍ��߂Ă���
	o_protocol = "SAORI";
	o_protocol_version = "1.0";

	// SAORI�ł��邱�ƁB�o�[�W�����͍ŒႽ��1.0�𖞂����Ă���Ηǂ��̂Ŗ��Ȃ�
	if ( i_protocol != "SAORI" ) { return 400; }

	// GET Version�Ȃ炻�̂܂�200��Ԃ�
	if ( i_command == "GET Version" ) { return 200; }
	// ����ȊO��command�͕s��
	if ( i_command != "EXECUTE" ) { return 400; }

	// ������ Argument? �Ɋi�[
	deque<string> arguments;
	for ( strpairvec::const_iterator it=i_data.begin() ; it!=i_data.end() ; ++it)
	{
		if ( it->first.compare(0, 8, "Argument") == 0 )
		{
			const int n = atoi(it->first.c_str() + 8);
			if ( n>=0 && n<65536 )
			{
				if ( arguments.size() <= n )
				{
					arguments.resize(n+1);
				}
				arguments[n] = it->second;
			}
		}
	}

	// SAORI���s
	deque<string> values;
	SRV	srv = request(arguments, values);

	// �߂�l����́��i�[
	o_data.push_back( strpair("Charset", "Shift_JIS") );
	o_data.push_back( strpair("Result", srv.mResultString) );
	int n=0;
	for ( deque<string>::const_iterator it=values.begin() ; it!=values.end() ; ++it)
	{
		o_data.push_back( strpair(string("Value") + itos(n++), *it) );
	}

	return srv.mReturnCode;
}
