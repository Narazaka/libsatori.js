#include	"satori.h"	

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

int		Satori::CreateResponse(strmap& oResponse)
{
	// NOTIFY�ł���Βl��ۑ�
	/*strmap	mNotifiedMap;
	if ( mRequestCommand=="NOTIFY" )
		mNotifiedMap[mRequestID] = mRequestMap["Reference0"];*/
	if ( mRequestCommand=="NOTIFY" ) {
		if ( mRequestID=="hwnd" ) {
#ifndef POSIX
			strvec	vec;
			const int max = split(mReferences[0], byte1_dlmt, vec);
			if ( max > 0 ) {
				characters_hwnd.clear();
			}
			for (int n=0 ; n<max ; ++n) {
				characters_hwnd[n] = (HWND)(stoi(vec[n]));
				GetSender().sender() << "���X�� id:" << n << " ��hWnd���擾���܂����B" << endl;
			}
#endif
		}
		else if ( mRequestID=="capability" ) {
			bool isErrorHeader = false;
			for ( strvec::const_iterator it = mReferences.begin() ; it != mReferences.end(); ++it ) {
				if ( *it == "response.errorlevel" ) {
					isErrorHeader = true;
				}
			}
			GetSender().errsender().set_log_mode(isErrorHeader);
		}
	}

	string	result;

	//����ϊ���������
	reset_speaked_status();

	//���ۂ̌Ăяo���J�n
	if ( mRequestID == "OnDirectSaoriCall" ) {
		string	str;
		int	n=0;
		bool	isfirst = true;
		for (strvec::const_iterator i=mReferences.begin() ; i!=mReferences.end() ; ++i, ++n)
			if ( i->empty() && mRequestMap.find( string("Reference")+itos(n) )==mRequestMap.end() )
				break;
			else
				if ( isfirst ) {
					str = *i;
					isfirst = false;
				}
				else
					str += "," + *i;
		//while ( compare_tail(str, ",") )
		//	str.assign( str.substr(0, str.size()-2)+"]" );
		string	temp;
		if ( secure_flag ) {
			GetSender().sender() << "[DirectCall]" << endl;
			Call(str, temp);
			return	204;
		}
		else {
			GetSender().sender() << "local/Local�łȂ��̂ŏR��܂���: " << str << endl;
			return	204;
		}
	}
	else if ( compare_head(mRequestID, "On") ) {
		if ( words.is_exist(mRequestID) )
			return	Call(mRequestID, oResponse["Value"]) ? 200 : 204;
		return	EventOperation(mRequestID, oResponse);
	}
	else if ( mRequestID == "version" )
		result = gSatoriVersion;
	else if ( mRequestID == "craftman" )
		result = gSatoriCraftman;
	else if ( mRequestID == "craftmanw" )
		result = gSatoriCraftmanW;
	else if ( mRequestID == "name" )
		result = gSatoriName;
	else if ( mRequestID.find(".recommendsites") != string::npos || mRequestID=="sakura.portalsites" ) {
		if ( !GetURLList(mRequestID, result) )	// URL���X�g�̎擾
			return	204;
	} 
	else if ( !Call(mRequestID, result) ) {
		return	204;
	}
	else {
	}

	if ( result.empty() )
		return	204;

	oResponse["Value"] = result;
	return	200;
}
