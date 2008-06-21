#pragma warning( disable : 4786 ) //�u�f�o�b�O�����ł̎��ʎq�؎̂āv
#pragma warning( disable : 4503 ) //�u�������ꂽ���O�̒��������E���z���܂����B���O�͐؂�̂Ă��܂��B�v

#include	"Saori.h"
#include	<windows.h>
#include	<mbctype.h>	// for _ismbblead()
#include	<string>
#include	"../_/stltool.h"

static const string ARGUMENT="Argument";
static const string VALUE="Value";


// c �܂��͕�����̏I���܂� p ��i�߂�B
// �i�񂾏ꏊ�܂ł̕������Ԃ��B
string getToken(const char*& p, char c) {
	const char* start=p;
	while (*p!='\0' && *p!=c)
		p += _ismbblead(*p) ? 2 : 1; 
	if (*p!='\0')
		return	string(start, (p++)-start);
	else
		return	string(start, p-start);
}



static Saori	gSaori;


extern "C" __declspec(dllexport) BOOL __cdecl load(HGLOBAL iData,long iLength) {
	string theBaseFolder((char*)iData, iLength);
	::GlobalFree(iData);
	return	gSaori.load(theBaseFolder);
}

extern "C" __declspec(dllexport) BOOL __cdecl unload(void) {
	return	gSaori.unload(); 
}


extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL iData,long *ioLength) {

	// �O���[�o�����������󂯂Ƃ�
	string theRequest((char*)iData, *ioLength);
	::GlobalFree(iData);


	// ���N�G�X�g������𕪉�

	int	theStatusCode = 500;
	string	theResult;
	deque<string>	theValues;

	const char* p = theRequest.c_str();
	string	first_line = getToken(p, '\x0a');	// ��s��
	string	protocol;
	/*if ( find(first_line, "MAKOTO") )
		protocol = "MAKOTO/1.0";
	else*/
		protocol = "SAORI/1.0";

	if ( compare_head(first_line, "GET Version") )
	{
		theStatusCode = 200;
	}
	else if ( compare_head(first_line, "EXECUTE") ) 
	{
		deque<string>	theArguments;

		do {
			string	key = getToken(p, ':');
			if ( key.empty() )
				break;
			while (*p==' ') ++p;
			string	value = getToken(p, '\x0a');
			if ( value.empty() )
				break;
			if(value[value.size()-1]=='\x0d')
				value.erase(value.size()-1);	// 0x0d����菜��
			if ( value.empty() )
				break;


			if ( key.substr(0, ARGUMENT.size()) == ARGUMENT ) {
				int	n = atoi( key.c_str() + ARGUMENT.size() );
				if ( n>=0 && n<100 ) {
					if ( theArguments.size() <= n )
						theArguments.resize(n+1);
					theArguments[n]=value;
				}
			}
			else if ( key=="String" ) {
				if ( theArguments.empty() )
					theArguments.resize(1);
				theArguments[0]=value;
			}

		} while (true);

		// �v���O�C�������s
		theStatusCode = gSaori.request(theArguments, theResult, theValues);
	}
	else
	{
		theStatusCode = 400;
	}


	// �X�e�[�^�X���C�����쐬
	string	theResponce = protocol+" ";
	switch(theStatusCode) {
	case 200: theResponce += "200 OK"; break;
	case 204: theResponce += "204 No Content"; break;
	case 400: theResponce += "400 Bad Request"; break;
	default:  theResponce += "500 Internal Server Error"; break;
	}
	theResponce += "\x0d\x0a""Charset: Shift_JIS\x0d\x0a";

	if ( theStatusCode == 200 ) {
		// �Ԓl��t�^
		theResponce += string("Result: ") + theResult + "\x0d\x0a";
		int	idx=0;
		for ( deque<string>::const_iterator i=theValues.begin() ; i!=theValues.end() ; ++i,++idx )
			theResponce += VALUE+itos(idx)+": "+ *i + "\x0d\x0a";
	}
	theResponce += "\x0d\x0a";


	// �O���[�o���������œn��
	*ioLength = theResponce.size();
	HGLOBAL theData = ::GlobalAlloc(GMEM_FIXED, *ioLength);
	::CopyMemory(theData, theResponce.c_str(), *ioLength);
	
	/*sender << "obu ������������������������������������" << endl
		<< theRequest
		<< "�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|" << endl
		<< theResponce
		<< "������������������������������������ obu" << endl;*/

	return	theData;
}

