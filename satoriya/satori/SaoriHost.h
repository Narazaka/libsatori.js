#include "SakuraDLLHost.h"

#include	<string>
#include	<deque>
using namespace std;

// SAORI�߂�l
struct SRV {
	int	mReturnCode;
	string	mResultString;
	SRV(int iReturnCode, string iResulutString="") : mReturnCode(iReturnCode), mResultString(iResulutString) {}
	SRV(string iResulutString) : mReturnCode(200), mResultString(iResulutString) {}
	SRV(const char* iResulutString) : mReturnCode(200), mResultString(iResulutString) {}
};


// SAORI�x�[�X�N���X
class SaoriHost : public SakuraDLLHost
{
	virtual int	request(
		const string& i_protocol,
		const string& i_protocol_version,
		const string& i_command,
		const strpairvec& i_data,
		
		string& o_protocol,
		string& o_protocol_version,
		strpairvec& o_data);
public:
	SaoriHost() : SakuraDLLHost() {}
	virtual ~SaoriHost() {}

	virtual SRV	request(deque<string>& iArguments, deque<string>& oValues)=0;
		// iArguments�͂������܂��B
};
