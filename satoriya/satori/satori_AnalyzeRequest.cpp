#include	"satori.h"

#ifdef POSIX
/* POSIX�ł�stricmp�͒�`����Ă��Ȃ��B�����strcasecmp���g����B */
#  define stricmp strcasecmp
#  include <string.h>
#endif

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

int	Satori::request(
	const string& i_protocol,
	const string& i_protocol_version,
	const string& i_command,
	const strpairvec& i_data,
	
	string& o_protocol,
	string& o_protocol_version,
	strpairvec& o_data)
{
	//-------------------------------------------------
	// ���N�G�X�g�P�ʂ̃N���X�����o��������

	mRequestMap.clear();
	mRequestID = "";
	mReferences.clear();
	mReferences.reserve(8); // �ŏ��l

	// �������N���X�����o�ɐݒ�
	mRequestCommand = i_command;
	mRequestType = i_protocol;
	mRequestVersion = i_protocol_version;
	mStatusLine = i_command + " " + i_protocol + "/" + i_protocol_version;

	// �Ԃ��v���g�R���̃f�t�H���g�l
	o_protocol = "SHIORI";
	o_protocol_version = "3.0";

	// ���邲�Ƃɏ���������ϐ�
	return_empty = false;
	surface_restore_at_talk_onetime = SR_INVALID;
	is_quick_section = false;

	// �X�N���v�g�w�b�_
	header_script = "";

	// �v���g�R���𔻕�
	if ( i_protocol=="SAORI" && i_protocol_version[0]>='1' )
	{
		o_protocol = i_protocol;
		o_protocol_version = "1.0";
		mRequestMode = SAORI;
	}
	else if ( i_protocol=="SHIORI" && i_protocol_version[0]>='3' )
	{
		mRequestMode = SHIORI3;
	}
	else if ( i_protocol=="SHIORI" && i_protocol_version[0]=='2' )
	{
		mRequestMode = SHIORI2;
			// 2.x�ɂ��o�[�W�����Ƃ��Ă�3.0��Ԃ�
	}
	else if ( i_protocol=="MAKOTO" && i_protocol_version[0]>='2' )
	{
		o_protocol = i_protocol;
		o_protocol_version = "2.0";
		mRequestMode = MAKOTO2;
	}
	else if ( i_protocol=="UNKNOWN" )
	{
		mRequestMode = UNKNOWN;
	}
	else
	{
		// ���Ή��̃v���g�R���������B
		return	400;
	}

	// �f�[�^����mRequestMap�Ɋi�[�B
	// SHOIRI/3.0�ȊO�̃v���g�R���̏ꍇ�ASHOIRI/3.0�ɕϊ����s���B
	for ( strpairvec::const_iterator it = i_data.begin() ; it != i_data.end() ; ++it )
	{
		string key = it->first;
		const string& value = it->second;

		switch ( mRequestMode ) {
		case SAORI:
			if ( compare_head(key, "Argument") ) {
				int	n = stoi(key.c_str()+8);
				if ( n==0 )
					key = "ID";
				else
					key = "Reference" + itos(n-1);
			}
			break;
		case SHIORI2:	// �������͂Ă��Ɓ[
			if ( key=="Event" )
				key="ID";
			break;
		case MAKOTO2:
			if ( key=="String" )
				key="Reference0";
			break;
		default:
			break;
		}

		mRequestMap[key] = value;
		if ( compare_head(key, "Reference") ) {
			int	n = stoi(key.c_str()+9);
			if ( n>=0 && n<65536 ) {
				if ( n>=mReferences.size() )
					mReferences.resize(n+1);
				mReferences[n]=value;
			}
		}
	}

	if ( mRequestMode == MAKOTO2 )
	{
		mRequestMap["ID"] = "OnMakoto";
	}

	mRequestID = mRequestMap["ID"];
	mIsMateria = ( mRequestMap["Sender"] == "embryo" );


	//-------------------------------------------------
	// ���N�G�X�g������

	if ( mRequestCommand=="GET Version" )
	{
		if ( mRequestMode == SHIORI2 )
		{
			o_data.push_back( strpair("ID", gSatoriName) );
			o_data.push_back( strpair("Craftman", gSatoriCraftman) );
			o_data.push_back( strpair("Version", gSatoriVersion) );
		}
		return 200;
	}


	// �I�𕪊�L�^��ϐ��ɑ���Bref0�����ɖ߂��B
	if ( compare_head(mRequestID, "OnChoice") ) // OnChoiceSelect/OnChoiceEnter�̗���
	{
		strvec	vec;
		int	ref_no = ( mRequestID=="OnChoiceEnter" )?1:0;
		string&	info = mRequestMap[string("Reference")+itos(ref_no)];
		if ( split(info, byte1_dlmt, vec)==3 ) // \1��؂�̂R������ł���Ȃ��
		{
			info=mReferences[ref_no]=variables["�I���h�c"]=vec[0];
			variables["�I�����x��"]=vec[1];
			variables["�I��ԍ�"]=vec[2];
		}
	}

	// ���O�ɂ��ĐF�X
	bool	logmode = !( mRequestID=="OnSurfaceChange" || mRequestID=="OnSecondChange"
		|| mRequestID=="OnMouseMove" || mRequestID=="OnTranslate" 
		/*|| compare_tail(mRequestID, "caption")*/ || compare_tail(mRequestID, "visible")
		|| compare_head(mRequestID, "menu.") || mRequestID.find(".color.")!=string::npos );

	if(fRequestLog && logmode)
	{
		sender << "--- Request ---" << endl << mStatusLine << endl; // << iRequest << endl;
		for(strmap::iterator i=mRequestMap.begin() ; i!=mRequestMap.end() ; ++i)
			if ( !i->first.empty() && !i->second.empty()
				&& i->first!="SecurityLevel" 
				&& i->first!="Sender" 
				&& i->first!="Charset" )
				sender << i->first << ": " << i->second << endl;
	}

	// �����゠�H
	strmap::const_iterator it = mRequestMap.find("SecurityLevel");
	secure_flag = ( it!=mRequestMap.end() && stricmp(it->second.c_str(), "local")==0 );

	// ���C������
	Sender::validate( fOperationLog && logmode );
	sender << "--- Operation ---" << endl;

	int status_code;
	if ( mRequestID=="ShioriEcho" ) {
		// ShioriEcho����
		if ( fDebugMode && secure_flag ) {
			string result = SentenceToSakuraScriptExec_with_PreProcess(mReferences);
			if ( result.length() ) {
				static const char* const dangerous_tag[] = {"\\![updatebymyself]",
					"\\![vanishbymyself]",
					"\\![enter,passivemode]",
					"\\![enter,inductionmode]",
					"\\![leave,passivemode]",
					"\\![leave,inductionmode]",
					"\\![lock,repaint]",
					"\\![unlock,repaint]",
					"\\![biff]",
					"\\![open,browser",
					"\\![open,mailer",
					"\\![raise",
					"\\j["};

				std::string replace_to;
				for ( int i = 0 ; i < (sizeof(dangerous_tag)/sizeof(dangerous_tag[0])) ; ++i ) {
					replace_to = "���m";
					replace_to += dangerous_tag[i]+2; //\���k�L
					replace(result,dangerous_tag[i],replace_to);
				}

				//Translate(result); - Translate�͌�ł�����
				mResponseMap["Value"] = result;
				status_code = 200;
			}
			else {
				status_code = 204;
			}
		}
		else {
			if ( fDebugMode ) {
				sender << "local/Local�łȂ��̂ŏR��܂���: ShioriEcho" << endl;
				status_code = 403;
			}
			else {
				static const std::string dbgmsg = "�f�o�b�O���[�h�������ł��B�g�p���邽�߂ɂ́��f�o�b�O���L���ɂ��Ă��������B: ShioriEcho";
				sender << dbgmsg << endl;

				mResponseMap["Value"] = "\\0" + dbgmsg + "\\e";
				status_code = 200;
			}
		}
	}
	else {
		status_code = CreateResponse(mResponseMap);
	}

	// �㏈���P
	default_surface = next_default_surface;

	//--------------------------------------------------------------------

	// Value�ɑ΂���ŏI����
	if ( status_code==200 ) {	// && compare_head(mRequestID, "On")
		strmap::iterator i = mResponseMap.find("Value");
		if ( i!=mResponseMap.end() ) {
			if ( return_empty ) {
				status_code = 204;
				mResponseMap.erase(i);
			}
			else {
				if ( !Translate(i->second) ) {
					status_code = 204;
					mResponseMap.erase(i);
				} 
				else {
					second_from_last_talk = 0;
					if ( compare_head(mRequestID, "On") ) {
						mResponseHistory.push_front(i->second);
						if ( mResponseHistory.size() >= RESPONSE_HISTORY_SIZE )
							mResponseHistory.pop_back();
					}
				}
			}
		}
	}

	sender << "status code : " << itos(status_code) << endl;

	//--------------------------------------------------------------------

	for(strmap::const_iterator i=mResponseMap.begin() ; i!=mResponseMap.end() ; ++i)
	{
		string	key=i->first, value=i->second;

		switch ( mRequestMode ) {
		case SAORI:
			if ( key=="Value" ) {
				key = "Result";
				value = header_script + value;
			}
			else if ( compare_head(key, "Reference") ) {
				key = string() + "Value" + (key.c_str()+9);
			}
			break;
		case SHIORI2:
			if ( key=="Value" ) {
				key = "Sentence";
				value = header_script + value;
			}
			break;
		case MAKOTO2:
			if ( key=="Value" ) {
				key = "String";
				value = header_script + value;
			}
			break;
		default:
			if ( key=="Value" ) {
				value = header_script + value;
			}
			break;
		}
		o_data.push_back( strpair(key, value) );
	}

	if ( errsender.get_log_mode() ) {
		const std::vector<string> &errlog = errsender.get_log();

		std::string errmsg;
		std::string errlevel;

		for ( std::vector<string>::const_iterator itr = errlog.begin() ; itr != errlog.end(); ++itr ) {
			errmsg += "SATORI - ";
			errmsg += mRequestID;
			errmsg += " > ";
			errmsg += *itr;
			errmsg += "\1";
			errlevel += "critical\1";
		}

		if ( errmsg.length() ) {
			errmsg.erase(errmsg.end()-1,errmsg.end());
			errlevel.erase(errlevel.end()-1,errlevel.end());

			o_data.push_back( strpair("ErrorLevel",errlevel) );
			o_data.push_back( strpair("ErrorDescription",errmsg) );
		}
		errsender.clear_log();
	}

	Sender::validate();
	if(fResponseLog && logmode)
	{
		sender << "--- Response ---" << endl << mResponseMap << endl;
	}
	mResponseMap.clear();

	//--------------------------------------------------------------------

	// �����[�h����
	if ( reload_flag )
	{
		reload_flag = false;
		string	tmp = mBaseFolder;
		sender << "����reloading." << endl;
		unload();
		load(tmp);
		sender << "����reloaded." << endl;
	}

	return	status_code;
}
