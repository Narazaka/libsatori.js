#include	"satori.h"
#ifdef POSIX
#  include "Utilities.h"
#else
#  include	<mbctype.h>
#endif

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////


bool	Satori::Translate(string& ioScript) {

	if ( ioScript.empty() )
		return	false;

	const bool is_OnTranslate = (mRequestID=="OnTranslate");
	const bool is_AnchorEnable = is_OnTranslate ? false : mRequestID.compare(0,2,"On") == 0;

	// ������X�N���v�g�Ƃ���ȊO�𕪊����ď�����������
	vector<string>	vec;
	string	acum;
	bool	content=false;	// ���ɒ��g������̂�
	bool	is_first_question = true; // �I�𕪊�L�^�̏��������p�B
	int	last_speaker=0;
	const char* p = ioScript.c_str();
	while (*p) {
		string	c=get_a_chr(p);	// �S�p���p��킸�ꕶ���擾���Ap���ꕶ�������߂�
		
		if ( c=="\\" || c=="%" ) {
			if (*p=='\\'||*p=='%') {	// �G�X�P�[�v���ꂽ\, %
				acum += c + *p++;
				continue;
			}
			
			const char*	start=p;
			string	cmd="",opt="";
	
			while (!_ismbblead(*p) && (isalpha(*p)||isdigit(*p)||*p=='!'||*p=='*'||*p=='&'||*p=='?'||*p=='_'))
				++p;
			cmd.assign(start, p-start);
	
			if (*p=='[') {
				const char* opt_start = ++p;
				while (*p!=']') {
					if (p[0]=='\\' && p[1]==']')	// �G�X�P�[�v���ꂽ]
						++p;
					p += _ismbblead(*p) ? 2 : 1;
				}
				opt.assign(opt_start, p++ -opt_start);
			}
			
			// �I�𕪊򃉃x���ɑ΂�����ꏈ��
			if ( !is_OnTranslate && 
				cmd=="q" && opt!="" && count(opt, ",")>0 && 
				mRequestID!="OnHeadlinesense.OnFind") {

				// �I�𕪊򂪂���X�N���v�g�ł���΁A���̏���őI�𕪊�L�^���N���A
				if ( is_first_question ) {
					question_record.clear();	
					is_first_question = false;
				}

				// �I�𕪊���L�^
				{
					strvec	vec;
					split(opt, ",", vec);
					if ( vec.size()==1 )	// count��split��㩁B
						vec.push_back("");	// 
					string	label=vec[0], id=vec[1];

					if ( false == compare_head(id, "On") )
					{ 
						// On�Ŏn�܂���̂�OnChoiceSelect���o�R����Ȃ����߁A�ΏۊO�Ƃ���

						int	count = question_record.size()+1;
						question_record[id] = pair<int,string>(count, label);

						// ���x���h�c�ɏ����߂�
						opt = label+","+id+byte1_dlmt+label+byte1_dlmt+itos(count);
						for (int i=2;i<vec.size();++i)
							opt += string(",") + vec[i];
					}
				}
			}
			else if ( cmd=="0" || cmd=="h" ) { last_speaker=0; }
			else if ( cmd=="1" || cmd=="u" ) { last_speaker=1; }
			else if ( cmd=="p" && aredigits(opt) ) {
				last_speaker=stoi(opt);
				if ( mIsMateria || last_speaker<=1 ) {
					cmd = (opt=="0") ? "0" : "1";
					opt = "";
				}
			}
			else if ( cmd=="s" && !opt.empty() ) {
				last_talk_exiting_surface[last_speaker]=stoi(opt);
			}
			else if ( cmd.size()==2 && cmd[0]=='s' && isdigit(cmd[1]) ) {
				last_talk_exiting_surface[last_speaker]=cmd[1]-'0';
			}
						
			if ( !acum.empty() )
				vec.push_back(acum);

			if ( opt=="" )
				vec.push_back(c + cmd);
			else
				vec.push_back(c + cmd + "[" + opt + "]");
			acum="";

			static	set<string>	nc_cmd;	// �L���Ɛ����Ȃ�������X�N���v�g�Q
			static	bool	initialized=false;
			if (!initialized) {
				initialized=true;
				nc_cmd.insert("0"); nc_cmd.insert("1"); nc_cmd.insert("h"); nc_cmd.insert("u"); nc_cmd.insert("p");
				nc_cmd.insert("n"); nc_cmd.insert("w"); nc_cmd.insert("_w"); nc_cmd.insert("e");
			}
			if ( nc_cmd.find(cmd)==nc_cmd.end() )
				content = true;

		}
		else {
			content = true;
			acum += c;
		}
	}
	if ( !acum.empty() )
		vec.push_back(acum);

	if (!content)
		return	false;	// ���g�̖����X�N���v�g�i���s���Ă����Ȃ��Ă��ꏏ�j�Ɣ��f�B

	ioScript="";
	string repstr;

	for (vector<string>::iterator i=vec.begin() ; i!=vec.end() ; ++i) {
		if ( i->at(0)!='\\' && i->at(0)!='%' ) {
			// ������X�N���v�g�ȊO�̕��ւ̏���

			// �A���J�[�}��
			if ( is_AnchorEnable ) {
				string::size_type n = i->size();
				for ( string::size_type c=0 ; c<n ; ++c ) {
					for ( vector<string>::iterator j=anchors.begin() ; j!=anchors.end() ; ++j ) {
						if ( n - c >= j->size() ) {
							if ( i->compare(c,j->size(),*j) == 0 ) {
								repstr = "\\_a[";
								repstr += *j;
								repstr += "]";
								repstr += *j;
								repstr += "\\_a";

								i->replace(c,j->size(),repstr);
								c += repstr.size();
								n = i->size();
								break;
							}
						}
					}
					if ( c < n && _ismbblead(i->at(c)) ) {
						++c;
					}
				}
			}
		}
		ioScript += *i;
	}


	// ����u������������K�p
	if ( !is_OnTranslate )
		for ( strmap::iterator di=replace_after_dic.begin() ; di!=replace_after_dic.end() ; ++di )
			replace(ioScript, di->first, di->second);

	diet_script(ioScript);	// ���X�g�_�C�G�b�g

	// �G�X�P�[�v���Ă�����������߂�
	m_escaper.unescape(ioScript);

	return	true;
}

