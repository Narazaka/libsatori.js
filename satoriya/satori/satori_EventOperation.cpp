#include	"satori.h"
#include <algorithm>

#include "random.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

inline bool	is_empty_script(const string& script) {
	for ( const char* p = script.c_str() ; *p!='\0' ; ) {
		if ( p[0]=='\\' ) {
			if ( p[1]=='0' || p[1]=='1' ) {
				p+=2;
				continue;
			}
			else if ( p[1]=='p' && p[2]=='[' ) {
				p+=3;
				while ( *p != ']' )
					if ( *p == '\0' )
						return	false;
					else
						++p;
				continue;
			}
		}
		return	false;
	}
	return	true;
}

int	Satori::EventOperation(string iEvent, map<string,string> &oResponse)
{
	// �g�[�N�擪�Ɂu���v������Ɛݒ肳���B�������""�̂܂܁B
	mCommunicateFor="";
	// �X�N���v�g������
	string	script="";

	// �V�X�e�����~���������E���Ă���
	if ( iEvent=="OnSecondChange" || iEvent=="OnMinuteChange" ) {

#ifndef POSIX
		if ( characters_hwnd.empty() && updateGhostsInfo() && !ghosts_info.empty() ) {
			sender << "��FMO����hwnd���擾���܂����B" << endl;

			strmap &ghost = ghosts_info[0];
			strmap::iterator it = ghost.find("hwnd");
			if ( it != ghost.end() ) {
				characters_hwnd[0] = (HWND)(stoi(it->second));
			}
			it = ghost.find("kerohwnd");
			if ( it != ghost.end() ) {
				characters_hwnd[1] = (HWND)(stoi(it->second));
			}
		}

		if ( is_single_monitor ) {
			//sender << "���V���O�����j�^�ł��B���؂�̓Ǝ�������s���܂���B" << endl;
		}
		else if ( ! mIsMateria ) {
			//sender << "��Materia�ȊO�ł͌��؂�̔���������n�ɔC���܂��B" << endl;
		}
		else if ( characters_hwnd.empty() ) {
			//sender << "���}���`���j�^�ł����Ahwnd���擾�ł��Ă��Ȃ����߁A���؂�̓Ǝ�������s���܂���B" << endl;
		}
		else {
		//	sender << "�����؂ꔻ�菈��" << endl;
			RECT	rc;
			::GetWindowRect(characters_hwnd[0], &rc);
			int	center = (rc.left + rc.right)/2;
			mRequestMap["Reference1"] = mReferences[1] =
				( center >= max_screen_rect.left && center <= max_screen_rect.right ) ? "0" : "1";
			/*sender << "�V�F���̍��[: " << rc.left << endl;
			sender << "�V�F���̉E�[: " << rc.right << endl;
			sender << "�V�F���̒���: " << center << endl;
			sender << "�f�X�N�g�b�v�̍��[: " << max_screen_rect.left << endl;
			sender << "�f�X�N�g�b�v�̉E�[: " << max_screen_rect.right << endl;
			sender << "�����؂ꔻ�茋��: " << mReferences[1] << endl;*/
		}
#endif

		mikire_flag = stoi(mReferences[1])!=0;
		kasanari_flag = stoi(mReferences[2])!=0;
		can_talk_flag = ( mReferences[3] != "0" );
		if ( iEvent[2]=='S' )
			++second_from_last_talk;
	}
	else if ( iEvent=="OnSurfaceChange" ) {
		cur_surface[0]=stoi(mReferences[0]);
		cur_surface[1]=stoi(mReferences[1]);
	} else if ( iEvent=="OnUpdateReady" ) {
		mReferences[0] = itos(stoi(mReferences[0])+1);
	}

	if ( (iEvent=="OnBoot" || iEvent=="OnGhostChanged") && !is_empty_script(on_loaded_script) ) {
		script = on_loaded_script;
		on_loaded_script = "";
	}
	else if ( iEvent=="OnClose" || iEvent=="OnGhostChanging" ) {
		string	on_unloading_script = GetSentence("OnSatoriClose");
		diet_script(on_unloading_script);
		if ( !is_empty_script(on_unloading_script) )
			script = on_unloading_script;
	}

	if ( !script.empty() ) {
	}
	else if ( iEvent=="OnAnchorSelect" && find(anchors.begin(),anchors.end(),mReferences[0])!=anchors.end() ) {
		// OnAnchorSelect�������Ƃ��Aref0��dicAnchor�̕����̏ꍇ��
		// �C�x���g����`����Ă���ꍇ�ł��V�X�e������D�悷��B
		script=GetSentence(mReferences[0]);
	}
	else if ( FindEventTalk(iEvent) ) {	// ���̍ہA�݊��C�x���g�ւ̒u���������ɍs����
		// ��`����Ă���Ȃ炻���D�悷��B
		script=GetSentence(iEvent);

		// ������ȉ��́A�C�x���g����`����Ă��Ȃ��ꍇ�̃f�t�H���g�����ł���B
	}
	else if ( iEvent=="OnMouseDoubleClick" )
	{
		static strvec v;
		if ( v.empty() )
		{
			v.push_back("���i�q�R�j�i�q�S�j����");
			v.push_back("�i�j");
		}
		script = SentenceToSakuraScriptExec(v);
	}
	else if ( iEvent=="OnMouseMove" ) {
		nade_valid_time = nade_valid_time_initializer; // �ȂŃZ�b�V�����̗L���������X�V
		int&	cur_nede_count = nade_count[ mReferences[4] ];
		if ( ++cur_nede_count >= nade_sensitivity ) {
#ifdef POSIX
		        int ret = 0;
#else
		        LRESULT	ret = 0;
			if ( !insert_nade_talk_at_other_talk && updateGhostsInfo() ) {
				string	hwnd_str = (ghosts_info[0])["hwnd"];
				HWND	hwnd = (HWND)(stoi(hwnd_str));
				if ( hwnd!=NULL ) {
					UINT	WM_SAKURAAPI = RegisterWindowMessage("Sakura");
					ret = ::SendMessage(hwnd, WM_SAKURAAPI, 140, 0);
				}
			}
#endif
			
			if ( ret == 0 ) {
				string	str = mReferences[3]+mReferences[4]+"�Ȃł��";
				if ( talks.is_exist(str) )
					script=GetSentence(str);
				sender << "Talk: " << script << endl;
			}
			nade_count.clear();
		}
	}
	else if ( iEvent=="OnChoiceSelect" ) {
		script=GetSentence(mReferences[0]);
	}
	else if ( iEvent=="OnWindowStateRestore"
		|| iEvent=="OnShellChanged"
		|| iEvent=="OnSurfaceRestore"
		|| iEvent=="�N��" ) {	// �u�N���v�́uOnBoot�v�uOnGhostChanged�v���烊�_�C���N�g�����
		script=surface_restore_string();
	}
	else if ( iEvent=="�I��" ) {
		script = "\\-";
	}
	else if ( iEvent=="OnMouseWheel" ) {
		koro_valid_time = 3;
		koro_count[ mReferences[4] ] += 1;
		if ( koro_count[ mReferences[4] ] >= 2 ) {
			string	str = mReferences[3]+mReferences[4]+"���낱��";
			if ( talks.is_exist(str) ) {
				script=GetSentence(str);
				koro_count.clear();
			}
		}
	}
	else if ( iEvent=="OnRecommendsiteChoice" )
	{
		if ( mReferences[3] != "" && mReferences[4] != "" ) {
			if ( mReferences[3] == "portal" ) {
				if ( ! GetRecommendsiteSentence("sakura.portalsites", script) ) {
					script = "";
				}
			}
			else {
				bool found = false;
				if ( mReferences[4] == "0" ) {
					if ( GetRecommendsiteSentence("sakura.recommendsites", script) ) {
						found = true;
					}
				}
				else if ( mReferences[4] == "0" ) {
					if ( GetRecommendsiteSentence("kero.recommendsites", script) ) {
						found = true;
					}
				}

				if ( ! found ) {
					string req = string("char") + mReferences[4] + ".recommendsites";
					if ( ! GetRecommendsiteSentence(req.c_str(), script) ) {
						script = "";
					}
				}
			}
		}
		else {
			if ( GetRecommendsiteSentence("sakura.recommendsites", script) ) {
				NULL;
			}
			else if ( GetRecommendsiteSentence("kero.recommendsites", script) ) {
				NULL;
			}
			else if ( GetRecommendsiteSentence("sakura.portalsites", script) ) {
				NULL;
			}
			else {
				script="";
			}
		}
	}
	else if ( iEvent=="OnCommunicate" ) {	// �b���|����ꂽ

		// ��Sender	�iif,�i�q�O�j==User,���[�U,�i�q�O�j�j
		if ( mReferences[0]=="user" )
			mReferences[0]="���[�U";

		// �X�N���v�g�ɘb�Җ���t��
		if ( !TalkSearch(mReferences[0]+"�u"+mReferences[1]+"�v", script, false) )
			script=GetSentence("COMMUNICATE�Y���Ȃ�");

		if ( mCommunicateFor==""  ) {	// �蓮�ł��؂�
			sender << "���XCOMMUNICATE�A�����ɑ��s�w�����������Ƃɂ��ł��؂�" << endl;
			mCommunicateLog.clear();
		}
		else if ( mCommunicateLog.find(script) != mCommunicateLog.end() ) {
			sender << "���XCOMMUNICATE�A���������[�v�ɂ��ł��؂�" << endl;
			script="";	// ��������Ȃ�
			mCommunicateLog.clear();
			mCommunicateFor = "";
		}
		else  if ( mCommunicateLog.find(mReferences[1]) != mCommunicateLog.end() ) {
			sender << "���XCOMMUNICATE�A���葤���[�v�ɂ��ł��؂�" << endl;
			mCommunicateLog.clear();
			mCommunicateFor = "";
		} 
		else {	// ���s
			sender << "���XCOMMUNICATE�A���s" << endl;
			mCommunicateLog.insert(mReferences[1]);
			mCommunicateLog.insert(script);
		}
	}

	if ( iEvent=="OnSecondChange" ) {

		// ���݂���^�C�}�̃f�B�N�������g
		for (strintmap::iterator i=timer.begin();i!=timer.end();++i)
			variables[i->first + "�^�C�}"] = int2zen( --(i->second) );

		// �����Z�[�u
		if ( mAutoSaveInterval > 0 ) {
			if ( --mAutoSaveCurrentCount <= 0 ) {
				if ( is_dic_loaded ) {
					this->Save(false);
				}
				mAutoSaveCurrentCount = mAutoSaveInterval;
			}
		}
	}

	diet_script(script);

	bool is_rnd_talk = false;

	if ( is_empty_script(script) && can_talk_flag && iEvent=="OnSecondChange" ) {

		// �^�C�}�\�񔭘b
		for (strintmap::iterator i=timer.begin();i!=timer.end();++i) {
			if ( i->second < 1 ) {
				//GetSentence���s���timer�̃C�e���[�^�͏�ԕω����Ă��邩������Ȃ��̂�
				//��������ۑ����Ă���
				string  timer_name = i->first;
				string	var_name = timer_name + "�^�C�}";

				sender << var_name << "�������B" << endl;

				reset_speaked_status();
				script=GetSentence(timer_name);
				
				timer.erase(timer_name);
				variables.erase(var_name);

				if ( !is_empty_script(script) ) {
					diet_script(script);
				}
				is_rnd_talk = true;
				break;
			}
		}

		// �������b
		if ( is_empty_script(script) && (mikire_flag==false || is_call_ontalk_at_mikire==true)) {
			if ( nade_valid_time>0 )
				if ( --nade_valid_time == 0 )
					nade_count.clear();
			if ( koro_valid_time>0 )
				if ( --koro_valid_time == 0 )
					koro_count.clear();
			if ( talk_interval>0 && --talk_interval_count<0 ) {
				string	iEvent="OnTalk";
				FindEventTalk(iEvent);

				reset_speaked_status();
				script=GetSentence(iEvent);

				diet_script(script);
				is_rnd_talk = true;
			}
		}
	}

	if ( is_empty_script(script) )
		return	204;	// ����Ȃ�

	// script�ւ̕t�^����
	if ( is_speaked_anybody() || is_rnd_talk ) {
		script = append_at_talk_start + surface_restore_string() + script + append_at_talk_end;

		// ����J�E���g������
		int	dist = int(talk_interval*(talk_interval_random/100.0));
		talk_interval_count = ( dist==0 ) ? talk_interval : 
			(talk_interval-dist)+(random(dist*2));
	}
	script += ( iEvent=="OnClose" || iEvent=="�I��" ) ? "\\-" : "\\e";


	if ( !mCommunicateFor.empty() ) { // �b�������̗L��
		sender << "���XCOMMUNICATE�A�u" << mCommunicateFor << "�v�֘b���|��" << endl;
		oResponse["Reference0"] = mCommunicateFor;
		oResponse["To"] = mCommunicateFor;
		if ( iEvent!="OnCommunicate" )
		{
			mCommunicateLog.clear(); // ����̂݁B���s���ɂ͂����ŃN���A�͂��Ȃ��B
		}
		//mCommunicateLog.insert(script);
	}
	oResponse["Value"]=script;

	// �P�g�[�N���ł̂ݗL���ȏd��������N���A
	words.handle_talk_end();
	talks.handle_talk_end();

	// �o���[���ʒu���L���Ȃ�ݒ�
	if ( validBalloonOffset[0] && validBalloonOffset[1] )
		oResponse["BalloonOffset"]=string()+BalloonOffset[0]+byte1_dlmt+BalloonOffset[1];

	return	200;
}



//		string	iEvent="OnTalk";
//		FindEventTalk(iEvent);
//		script = append_at_talk_start + surface_restore_string() + GetSentence(iEvent) + append_at_talk_end + "\\e";



// Communicate�`�������B�Y������Ȃ炻�̃X�N���v�g���擾�A�Y���Ȃ��Ȃ�false�B
bool	Satori::TalkSearch(const string& iSentence, string& oScript, bool iAndMode)
{
	const Talk* talk = talks.communicate_search(iSentence, iAndMode);
	if ( talk == NULL )
	{
		return false;
	}

	oScript = SentenceToSakuraScriptExec(*talk);
	sender << oScript << endl;
	return	true;
}



