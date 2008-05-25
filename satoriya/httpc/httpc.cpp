#pragma warning( disable : 4786 ) //�u�f�o�b�O�����ł̎��ʎq�؎̂āv
#pragma warning( disable : 4503 ) //�u�������ꂽ���O�̒��������E���z���܂����B���O�͐؂�̂Ă��܂��B�v

//����������������������������������������������������������������������������������
#include	<string>
#include	<deque>
using namespace std;


class	Saori {
public:
	bool	load(const string& iBaseFolder);
	bool	unload();
	string	request(deque<string>& iArguments, deque<string>& oValues);

	static Saori&	getInstance() {
		static Saori	theInstance;
		return	theInstance;
	}
private:
	Saori() {}
};

//����������������������������������������������������������������������������������
#include	<windows.h>
#include	<mbctype.h>	// for _ismbblead()
#include	<string>
#include	"../_/stltool.h"
using namespace std;

static const string ARGUMENT="Argument";
static const string VALUE="Value";

#define SAORI_VERSION "httpc/7a"

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


extern "C" __declspec(dllexport) BOOL __cdecl load(HGLOBAL iData,long iLength) {
	string theBaseFolder((char*)iData, iLength);
	::GlobalFree(iData);
	return	Saori::getInstance().load(theBaseFolder);
}

extern "C" __declspec(dllexport) BOOL __cdecl unload(void) {
	return	Saori::getInstance().unload(); 
}


extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL iData,long *ioLength) {

	// �O���[�o�����������󂯂Ƃ�
	string theRequest((char*)iData, *ioLength);
	::GlobalFree(iData);


	// ���N�G�X�g������𕪉�

	deque<string>	theArguments;
	
	const char* p = theRequest.c_str();
	string	first_line = getToken(p, '\x0a');	// ��s��
	
	if ( compare_head(first_line, "GET Version") ) {
		char *pResult = "SAORI/1.0 200 OK\x0d\x0aResult: " SAORI_VERSION "\x0d\x0a\x0d\x0a";
		
		*ioLength = strlen(pResult);
		HGLOBAL theData = ::GlobalAlloc(GMEM_FIXED, *ioLength + 1);
		::CopyMemory(theData, pResult, *ioLength);
		((BYTE*)theData)[*ioLength] = 0; // �[���I�[����Ȃ��Ƃ܂Ƃ��ɉ��߂��Ȃ�SHIORI�p
		
		return theData;
	}

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
			if ( n>=0 && n<65536 ) {
				if ( theArguments.size() <= n )
					theArguments.resize(n+1);
				theArguments[n]=value;
			}
		}
	} while (true);

	// �v���O�C�������s
	deque<string>	theValues;
	string	theResult = Saori::getInstance().request(theArguments, theValues);
	theArguments.clear();	// �����g��Ȃ�


	// ���X�|���X��������쐬
	string	theResponce = 
			"SAORI/1.0 200 OK\x0d\x0a"
			"Charset: Shift_JIS\x0d\x0a";
	theResponce += string("Result: ") + theResult + "\x0d\x0a";
	int	idx=0;
	for ( deque<string>::const_iterator i=theValues.begin() ; i!=theValues.end() ; ++i,++idx )
		theResponce += VALUE+itos(idx)+": "+ *i + "\x0d\x0a";
	theResponce += "\x0d\x0a";


	// �O���[�o���������œn��
	*ioLength = theResponce.size();
	HGLOBAL theData = ::GlobalAlloc(GMEM_FIXED, *ioLength + 1);
	::CopyMemory(theData, theResponce.c_str(), *ioLength);
	((BYTE*)theData)[*ioLength] = 0; // �[���I�[����Ȃ��Ƃ܂Ƃ��ɉ��߂��Ȃ�SHIORI�p

	return	theData;
}



//����������������������������������������������������������������������������������
/*
httpc
�vCharset, Thread
*/
#include	<windows.h>
#include	<assert.h>
#include	<mbctype.h>	// for _ismbblead,_ismbbtrail
#include	"../_/stltool.h"
#include	"../_/charset.h"
#include	"../_/Thread.h"


static	string	remote_folder="";
static	set<string>	opt;

string	erase_tag(const string& i) {
	string	o;
	const char* p=i.c_str();
	bool	valid=true;
	while ( *p != '\0' )
		if ( valid )
			if (*p=='<')
				valid=false;
			else
				o+= *p++;
		else
			if (*p++=='>')
				valid=true;
			else
				NULL;
	return	o;
}

struct Tag {
	string	name;
	strmap	prop;
};

static string	tag_to_ss(Tag& tag) {
	static	string	href="";
	string&	nm = tag.name;
	strmap&	pp = tag.prop;

	if ( stricmp("/tr", nm.c_str())==0) return "\\n";
	if ( stricmp("/li", nm.c_str())==0) return "\\n";
	if ( stricmp("br", nm.c_str())==0) return "\\n";
	if ( stricmp("p", nm.c_str())==0) return "\\n";
	if ( stricmp("hr", nm.c_str())==0) return "\\n-----------------------------------------------\\n";
	if ( stricmp("a", nm.c_str())==0&& pp.find("href")!=pp.end() ) {

		string	href=pp["href"];
		/*if ( !compare_head(href, "http://") ) {
			if ( compare_head(href, "./") ) {
				href.assign(href.c_str()+1);
				href = remote_folder+href;
			}
			else if ( compare_head(href, "/" ) {
				href = remote_folder+href;
			}
			else if ( compare_head(href, "/" ) {
				href = remote_folder+href;
			}
		}*/

		return string() + "\\_a[" + href + "]";
	}
	if ( stricmp("/a", nm.c_str())==0) return "\\_a";
	//if ( stricmp("a", nm.c_str())==0&& pp.find("href")!=pp.end() ) return string() + "\\q[" + pp["href"] + "][�y";
	//if ( stricmp("/a", nm.c_str())==0) return "�z]";
	/*if ( stricmp("a", nm.c_str())==0&& pp.find("href")!=pp.end() ) {
		href = pp["href"];
		return	"\\q[";
	}
	if ( stricmp("/a", nm.c_str())==0&& href!="") {
		if ( !compare_head(href,"http://") && !compare_head(href,"ftp://") )
			href = remote_folder+href;
		string	ret = string()+","+href+"]";
		href="";
		return ret;
	}*/
	if ( stricmp("img", nm.c_str())==0&& pp.find("alt")!=pp.end() && pp["alt"].size()>0 ) return string()+"�k"+pp["alt"]+"�l";
	//if ( stricmp("img", nm.c_str())==0) return "�kimg�l";
	return	"";
}

static void	skip_delimiter(const char*& p) {
	while ( *p!='\0' )
		if (*p==' ' || *p=='\t' || *p=='\r' || *p=='\n')
			p += _ismbblead(*p) ? 2 : 1;
		else
			return;
}

static string	read_word(const char*& p) {
	string	o;
	skip_delimiter(p);
	if (*p=='"') {
		++p;
		while ( *p!='\0' && *p!='"' ) {
			if ( _ismbblead(*p) ) { o += *p++; o += *p++; } else { o += *p++; }
		}
		if (*p=='"' )
			++p;
	} else {
		while ( *p!='\0' && !(*p==' ' || *p=='\t' || *p=='\r' || *p=='\n' || *p=='=' || *p=='>') ) {
			if ( _ismbblead(*p) ) { o += *p++; o += *p++; } else { o += *p++; }
		}
	}
	skip_delimiter(p);
	return	o;
}

static Tag	analyze_tag(const string& str) {
	const char* p = str.c_str();
	Tag	tag;
	tag.name=read_word(p);
	while (*p!='\0') {
		string	key=read_word(p);
		if ( *p=='=' ) {
			string	value=read_word(++p);
			tag.prop[key]=value;
		} else
			tag.prop[key]="";
	}
	return	tag;
}


// <...>��ǂݍ���
static string	read_tag_area(const char*& p) {
	if ( *p!='<' )
		return	"";
	string	o;
	while (*++p!='>' && *p!='\0')
		o += *p;
	if ( *p=='>' )
		++p;
	return	o;
}

string	translate_tag(const string& i) {
	string	o;
	const char* p=i.c_str();
	while ( *p != '\0' ) {
		if (*p=='<') {
			string	tag_area = read_tag_area(p);
			Tag	tag = analyze_tag(tag_area);
			string	ss = tag_to_ss(tag);
			o += ss;
		} 
		else
			o += *p++;
	}
	//replace(o, "&nbsp;", "�@");
	replace(o, "&nbsp;", "");
	replace(o, "&lt;", "<");
	replace(o, "&gt;", ">");
	replace(o, "&amp;", "\\&");
	replace(o, "&copy;", "(c)");
	//while ( replace(o, "\n\n\n", "\n\n")>0 )
	while ( replace(o, "\\n\\n", "\\n")>0 )
		NULL;

	const int column_max=48, row_max=120;
	int	column=0, row=1;
	p=o.c_str();
	while ( *p!='\0' && row<row_max) {
		if ( p[0]=='\\' && p[1]=='n' ) {
			column=0;
			row++;
			p+=2;
		}
		else {
			if ( ++column >= column_max ) {
				column=0;
				row++;
			}
			++p;
		}
	}
	if ( *p!='\0' )
		o = o.substr(0, p-o.c_str());

	return	o;
}

#include	<wininet.h>

static bool	checkHTTP() {
	HINTERNET hInternet = InternetOpen(
		"httpc_for_saori",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);
	if ( hInternet==NULL )
		return	false;
	::InternetCloseHandle(hInternet);
	return	true;
}


static bool	getHTTP(const string& iURL, string& oResult) {

	/* WININET������ */
	HINTERNET hInternet = InternetOpen(
		"httpc_for_saori",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);
	if ( hInternet==NULL )
		return	false;

	/* remote_folder�̃I�[�v�� */
	HINTERNET hFile = InternetOpenUrl(
		hInternet,
		iURL.c_str(),
		NULL,
		0,
		INTERNET_FLAG_RELOAD,
		0);
	if ( hFile==NULL ) {
		InternetCloseHandle(hInternet);
		return	false;
	}

	/* �I�[�v������remote_folder����f�[�^��(8192�o�C�g����)�ǂݍ��� */
	vector<char>	theData;
	for(;;) {
		DWORD ReadSize=0;
		static char Buf[8192];
		BOOL bResult = InternetReadFile(
			hFile,
			Buf,
			8192,
			&ReadSize);

		/* �S�ēǂݍ��񂾂烋�[�v�𔲂��� */
		if(bResult && (ReadSize == 0))
			break;

		int cur = theData.size();
		theData.resize( cur + ReadSize );
		memcpy( theData.begin() + cur, Buf, ReadSize );
	}

	/* �㏈�� */
	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	// ������I�[�BUnicode�̏ꍇ���l����4byte�B
	for (int i=0 ; i<4 ; ++i)
		theData.push_back(0);
	
	oResult.assign(theData.begin(), theData.size());

	// �����R�[�h�̔��ʁE�ϊ�
	if ( opt.find("jis") != opt.end() )
		oResult = jis2sjis(oResult);
	else if ( opt.find("euc") != opt.end() )
		oResult = euc2sjis(oResult);
	else if ( opt.find("utf8") != opt.end() )
		oResult = UTF8toSJIS(oResult);
	/*else if ( opt.find("UTF-16LE") != opt.end() )
		NULL;
	else if ( opt.find("UTF-16BE") != opt.end() )
		NULL;*/
	else if ( opt.find("sjis") != opt.end() )
		NULL;
	else
		oResult = convertSomethingToSJIS(oResult.c_str());

	// ���s�R�[�h������
	erase(oResult, "\r");
	erase(oResult, "\n");

	// �^�O�̏���
	if ( opt.find("translate_tag") != opt.end() )
		oResult=translate_tag(oResult);
	else if ( opt.find("erase_tag") != opt.end() )
		oResult=erase_tag(oResult);

	return	true;
}


static bool	saveHTTP(const string& iURL, const string& iFileName) {

	/* WININET������ */
	HINTERNET hInternet = InternetOpen(
		"httpc_for_saori",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);
	if ( hInternet==NULL )
		return	false;

	/* remote_folder�̃I�[�v�� */
	HINTERNET hFile = InternetOpenUrl(
		hInternet,
		iURL.c_str(),
		NULL,
		0,
		INTERNET_FLAG_RELOAD,
		0);
	if ( hFile==NULL ) {
		InternetCloseHandle(hInternet);
		return	false;
	}

	// �������ރt�@�C�����J��
	FILE*	fp = fopen(iFileName.c_str(), "wb");
	if ( fp==NULL ) {
		InternetCloseHandle(hFile);
		InternetCloseHandle(hInternet);
		return	false;
	}

	/* �I�[�v������remote_folder����f�[�^��(8192�o�C�g����)�ǂݍ��� */
	for(;;) {
		DWORD ReadSize=0;
		static char Buf[8193];
		BOOL bResult = InternetReadFile(
			hFile,
			Buf,
			8192,
			&ReadSize);

		/* �S�ēǂݍ��񂾂烋�[�v�𔲂��� */
		if(bResult && (ReadSize == 0))
			break;

		fwrite(Buf, ReadSize, 1, fp);
	}

	/* �㏈�� */
	fclose(fp);
	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);
	return	true;
}


static	string	base_folder="";

bool	Saori::load(const string& iBaseFolder) {
	base_folder = iBaseFolder;
	return true;
}
bool	Saori::unload() {
	return true;
}


/*
	httpc.dll

�@SAORI�K�i��http�N���C�A���g�B
�@Web��̊e��f�[�^��HTTP�Ŏ擾����B

�@�����̎w��ɂ��A�S�ʂ�̓��������B


�E�����Ȃ�

  �C���^�[�l�b�g�ɐڑ��ł��邩�ǂ����̊m�F�B
  �ڑ��ł���Ȃ�1�A�ł��Ȃ��Ȃ�0��Ԃ��B


�E�����P��

  ��P������remote_folder���w��A�擾�������̂����̂܂ܕԂ��B


�E�����Q��

  ��P������remote_folder���w��A
�@�擾�������̂��Q�����̃t�@�C�����ŕۑ��B
�@����������1�A���s������0��Ԃ��B


�E�����R�@

  ��P������remote_folder���w��A
  �擾�������̂���A��Q�E�R�����ň͂܂��͈͂�Ԃ��B

  �Ⴆ�΁A
  ��P�������@http://www.yahoo.co.jp/
  ��Q�������@href="
  ��R�������@"

  �������Ƃ���ƁA���t�[�̃g�b�v�y�[�W����A��ԍŏ��ɂ��郊���N��remote_folder���擾�����B
  �܂��A�����Ԓl����Q�ڈȍ~�̃����N���擾�ł���B


*/
#include	"dsstp.h"
class HttpcThread : public Thread {
	string	mID;
	deque<string>	mArguments;
	virtual	DWORD	ThreadMain() {
		deque<string>	refs;
		string	result = Saori::getInstance().request(mArguments, refs);	// ref3�`
		refs.push_front(result);	// ref2
		refs.push_front("-");	// ref1
		refs.push_front(mID);	// ref0
		sendDirectSSTP_for_NOTIFY("httpc", "OnHttpcNotify", refs);
		delete this;
		return	0;
	}
public:
	HttpcThread(const string& iID, const deque<string>& iArguments) : 
	  Thread(), mID(iID), mArguments(iArguments) {}
};
//map<string, HttpcThread*>	gThreads;

string	Saori::request(deque<string>& iArguments, deque<string>& oValues) {

	if ( iArguments.size()>0 && iArguments[0]=="bg" ) {
		iArguments.pop_front();
		if ( iArguments.size()<1 )
			return	"bg����Ȃ�ID���w�肵�Ă��������܂���ƁB";
		string	id = iArguments[0];
		iArguments.pop_front();
		(new HttpcThread(id, iArguments))->create();
		return	"";
	}


	opt.clear();
	while (true) {
		if ( iArguments.empty() )
			break;

		static const char*	exist_options[] = 
			{"euc", "jis", "sjis", "utf8", "erase_tag", "translate_tag"};
		static const int	max = sizeof(exist_options)/sizeof(exist_options[0]);
		int i=0;
		for ( ; i<max ; ++i ) {
			if ( iArguments.front()==exist_options[i] ) {
				opt.insert( iArguments.front() );
				iArguments.pop_front();
			}
		}
		if ( i==max )
			break;

	}


/*	if ( iArguments.size()>0 ) {
		remote_folder=iArguments.front();
		const char* slash=FindFinalChar(remote_folder, '/');
		const char* period=FindFinalChar(remote_folder, '.');


		if ( compare_tail(remote_folder, "/") ) {
			const char* p=FindFinalChar(remote_folder, '/');
			remote_folder.assign(remote_folder.c_str(), p-remote_folder.c_str();
		}
	} else
		remote_folder="";*/

	string	data;

	if ( iArguments.size()==0 )
		return	checkHTTP() ? "1" : "0";
	else if ( iArguments.size()==1 )
		return	getHTTP(iArguments[0], data) ? data : "";
	else if ( iArguments.size()==2 )
		return	saveHTTP(iArguments[0], base_folder+iArguments[1]) ? "1" : "0";
	else if ( iArguments.size()==3 ) {

		if ( !getHTTP(iArguments[0], data) )
			return	"";

		int	find_count=0;
		const string&	start = iArguments[1];
		const string&	end = iArguments[2];
		string	ret_str="";
		while(true) {
			int	n = data.find(start);
			if ( n==string::npos )
				break;

			data.erase(0, n+start.size());
			n = data.find(end);
			if ( n==string::npos )
				break;

			string	value( data.c_str(), n );	// ���e�擾
			if ( find_count++ == 0 )
				ret_str = value;
			else
				oValues.push_back(value);

			data.erase(0, n+end.size());
		}
		return	ret_str;
	}

	return	string();
}
