#include <iostream>
#include <string>
#include <ctime>
#include <windows.h>
using namespace std;

#pragma warning( disable : 4786 ) //�u�f�o�b�O�����ł̎��ʎq�؎̂āv
#pragma warning( disable : 4503 ) //�u�������ꂽ���O�̒��������E���z���܂����B���O�͐؂�̂Ă��܂��B�v

#include "get_browser_info.h"

int main()
{
	CBrowserInfo *pB = new CBrowserInfo;

	while (true) {
		vector< str_pair >  url;
		if ( !pB->GetMulti(url) ) {
			cout << "(Error)" << endl;
		}
		else {
			for ( int i = 0 ; i < url.size() ; ++i ) {
				cout << url[i].first << " | " << url[i].second << endl;
			}
		}
		cout << "---------------" << endl;
		Sleep(2000);
	}

	delete pB;

	return 0;
}