#include	"Satori.h"
#include	<iostream>
using namespace std;

#include <windows.h>

class Ghost {
	Satori	mSatori;
public:
	Ghost() {
		char buf[MAX_PATH+1];
		const int len = ::GetCurrentDirectory(MAX_PATH, buf);
		strcpy(buf+len, "\\");
		if ( !mSatori.load(buf) )
			throw ;
	}
	~Ghost() {
		mSatori.unload();
	}
	string	request(string iCommandLine)
	{
		return ((SakuraDLLHost*)(&mSatori))->request( string() +
			"GET SHIORI/3.0\r\n"
			"ID: " + iEvent + "\r\n"
			"Sender: embryo\r\n"
			"SecurityLevel: local\r\n"
			"\r\n"
		);
	}

};


#include	<crtdbg.h>
int main() {
	
#ifdef	_DEBUG
	//���������[�N�̌��o(_DEBUG����`����Ă����ꍇ�̂ݗL���ł��B)
	int tmpDbgFlag;
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	//tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpDbgFlag |= _CRTDBG_ALLOC_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);

	//_CrtSetBreakAlloc(45109);
#endif



	Ghost* g = new Ghost;

	cout << g->request("OnBoot") << endl;
	cout << g->request("OnDoubleClick") << endl;
	//cout << ghost.request("OnNadeCopy") << endl;

	delete g;

	return	0;
}


/*
class A
{
	string m;
public:
	A(string i) : m(i) { cout << "A(" << m << ")" << endl; }
	~A() { cout << "~A(" << m << ")" << endl; }
};

A foo(A i_a)
{
	A the_a("���[�J���ϐ�");
	return A("�߂�l");
}


	/*{
		foo(A("����"));
		return 0;
	}*/

	/*
	{
		Families<string> fs;
		fs.add_element("���O", "���g", "������");
	}*/

	//for ( int i=0 ; i<10 ; ++i )


	//cout << ghost.request("get") << endl;
	//cout << ghost.request("����") << endl;
/*	cout << typeid(Ghost).name() << endl;
	
	cout << ghost.request("����") << ghost.request("����") << ghost.request("����") << ghost.request("����") << endl;
	cout << ghost.request("����") << ghost.request("����") << ghost.request("����") << ghost.request("����") << endl;
	cout << ghost.request("����") << ghost.request("����") << ghost.request("����") << ghost.request("����") << endl;
	cout << ghost.request("����") << ghost.request("����") << ghost.request("����") << ghost.request("����") << endl;
	cout << ghost.request("����") << ghost.request("����") << ghost.request("����") << ghost.request("����") << endl;
*/
 

