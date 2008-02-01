#include	"satori.h"

#include	<fstream>
#include	<cassert>
#ifdef POSIX
#  include      "Utilities.h"
#else
#  include	<mbctype.h>
#endif

#include "random.h"

//////////DEBUG/////////////////////////
#ifdef _WINDOWS
#ifdef _DEBUG
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif
////////////////////////////////////////

void	diet_script(string& ioScript) {
	//replace(ioScript, "\\h", "\\0");
	//replace(ioScript, "\\u", "\\1");
	erase(ioScript, "\\_w[0]");
	int	count;
	do {
		count=0;
		count += replace(ioScript, "\\1\\0", "\\0");
		count += replace(ioScript, "\\0\\0", "\\0");
		count += replace(ioScript, "\\0\\1", "\\1");
		count += replace(ioScript, "\\1\\1", "\\1");
		count += replace(ioScript, "\\n\\e", "\\e");
		count += replace(ioScript, "\\n[half]\\e", "\\e");
		count += replace(ioScript, "\\n\\-", "\\-");
		count += replace(ioScript, "\\n[half]\\-", "\\-");
		count += replace(ioScript, "\\e\\-", "\\-");
		count += replace(ioScript, "\\e\\e", "\\e");
	} while (count>0);

	while ( compare_tail(ioScript, "\\n") )
		ioScript.erase(ioScript.size()-2,2);
		//ioScript.assign(ioScript.substr(0, ioScript.size()-2));
}

bool	Satori::FindEventTalk(string& ioevent) {

	static	strmap	replace_map;
	static	bool	isinit=false;
	if ( !isinit ) {
		replace_map["OnBoot"]="�N��";
		replace_map["OnClose"]="�I��";
		replace_map["OnFirstBoot"]="����";
		replace_map["OnGhostChanged"]="���̃S�[�X�g����ύX";
		replace_map["OnGhostChanging"]="���̃S�[�X�g�֕ύX";
		//replace_map["OnMouseDoubleClick"]="OnTalk";
		replace_map["����"]="OnBoot";
		replace_map["���̃S�[�X�g����ύX"]="OnBoot";
		replace_map["���̃S�[�X�g�֕ύX"]="OnClose";
		replace_map["OnVanishSelecting"]="���Ŏw��";
		replace_map["OnVanishCancel"]="���œP��";
		replace_map["OnVanishSelected"]="���Ō���";
		replace_map["OnVanishButtonHold"]="���Œ��f";
		replace_map["OnTalk"]="";
		//replace_map[""]="";
		isinit = true;
	}

	while (true) {
		if ( talks.is_exist(ioevent) )
			return	true;	// �C�x���g�����݁A����Ɍ���
		if ( replace_map.find(ioevent) == replace_map.end() )
			return	false;	// �u�������Ώۂ���������
		sender << "event replaced " << ioevent <<  " �� " <<  replace_map[ioevent] << endl;
		ioevent = replace_map[ioevent];
	}
}




string	Satori::GetSentence(const string& name)
{
	string script, sentence=name;

	/*++m_nest_count;

	if ( m_nest_limit > 0 && m_nest_count > m_nest_limit ) {
		sender << "�Ăяo���񐔒��߁F" << name << endl;
		--m_nest_count;
		return string("�i" + name + "�j");
	}*/

	// �g�[�N��������X�N���v�g�ɕϊ�
	const Talk *pTalk = GetSentenceInternal(sentence);
	if ( pTalk ) {
		Sender::nest_object smo(2); 
		script = SentenceToSakuraScriptExec(*pTalk);
		sender << "return: " << script << "" << endl;
	}
	return	script;
}



// �����}���E�F�C�g
#define	character_wait_clear	\
	if ( mRequestID == "OnHeadlinesense.OnFind" ) { characters = 0;	} \
	else if( characters > 0 ) { \
		result += string("\\_w[") + itos( int(characters*basewait*rate_of_auto_insert_wait/100) ) + "]"; characters=0; \
	} else NULL



string Satori::SentenceToSakuraScriptExec(const Talk& vec)
{
	string jump_to;
	string result;
	ptrdiff_t ip = 0;
	const Talk* pVec = &vec;
	bool comAndMode = mRequestID!="OnCommunicate";

	//���s��������
	string allresult = "\\1";
	speaker = 1;	// �{�̂� 0 ���ɂイ�� 1

	question_num=0;	// �I�����ԍ�
	characters = 0;	// ����������

	int jumpcount = 0;

	//return�֎~
	kakko_replace_history.push(strvec()); // �J�b�R�̑O���Q�Ɨp

	while ( TRUE ) {
		if ( speaker != 1 ) {
			allresult += "\\1";
			speaker = 1;	// �{�̂� 0 ���ɂイ�� 1
		}

		result = "";
		jump_to = "";

		kakko_replace_history.top().clear();
		int resp = SentenceToSakuraScriptInternal(*pVec,result,jump_to,ip);

		allresult += result;

		//resp == 0�őS���s�I��
		if ( ! resp ) {
			break;
		}

		//����ȊO�͂ǂ����ɃW�����v������
		if ( resp == 1 ) {
			string jump = jump_to;
			const Talk* pTR = GetSentenceInternal(jump);
			if ( ! pTR ) {
				sender << "��" << jump_to << " not found." << endl;
			}
			else {
				pVec = pTR;
				ip = 0;
			}
		}
		else if ( resp == 2 ) {
			string jump = jump_to;
			const Talk* pTR = talks.communicate_search(jump, comAndMode);
			if ( ! pTR ) {
				sender << "��" << jump_to << " not found." << endl;
			}
			else {
				pVec = pTR;
				ip = 0;
			}
		}
		++jumpcount;

		if ( m_jump_limit > 0 && jumpcount >= m_jump_limit ) {
			sender << "�W�����v�񐔒���" << endl;
			break;
		}
	}

	kakko_replace_history.pop(1);
	//return�֎~�I��

	return allresult;
}

int Satori::SentenceToSakuraScriptInternal(const strvec &vec,string &result,string &jump_to,ptrdiff_t &ip)
{
	// �ċA�Ǘ�
	static	int nest_count=0;
	++nest_count;
	//DBG(sender << "enter SentenceToSakuraScriptInternal, nest-count: " << nest_count << ", vector_size: " << vec.size() << endl);

	if ( m_nest_limit > 0 && nest_count > m_nest_limit ) {
		sender << "�Ăяo���񐔒���" << endl;
		--nest_count;
		return 0;
	}

	static const int basewait=3;

	strvec::const_iterator it = vec.begin();
	std::advance(it,ip);

	for ( ; it != vec.end() ; ++it) {
		const char*	p = it->c_str();
		//DBG(sender << nest_count << " '" << p << "'" << endl);

		if ( it==vec.begin() && strncmp(p, "��", 2)==0 ) {
			p+=2;
			updateGhostsInfo();	// �S�[�X�g�����X�V

			if ( ghosts_info.size()>=2 ) {	// �������������ȊO�ɃS�[�X�g�͂���̂��B
				string	temp = p;
				vector<strmap>::iterator i=ghosts_info.begin(); ++i; // �����͔�΂�
				for ( ; i!=ghosts_info.end() ; ++i ) { 
					string	name = (*i)["name"];
					sender << "ghost: " << name <<endl;
					if ( compare_head(temp, name) ) {// ��������
						mCommunicateFor = name;
						p += mCommunicateFor.size();
						break;
					}
				}
				if ( i==ghosts_info.end() ) {	// ���肵�Ȃ������ꍇ
					// �����_��
					//int n = random(ghosts_info.size()-1))+1;
					//assert( n>=1 && n < ghosts_info.size());
					mCommunicateFor = (ghosts_info[1])["name"];

					// ������A�ׂŋN�����Ă���`�`�ɂȂ��
				}
			}
		}

		// �I����	\q?[id,string]
		if ( strncmp(p, "�Q", 2)==0 ) {
			if ( strlen(p)>1023 )
				continue;
			char	buf[1024];
			strncpy(buf, p+2, sizeof(buf) / sizeof(buf[0]));

			char*	choiced = buf;
			char*	id = (char*)strstr_hz(buf, "\t"); // �I�������x���ƃW�����v��̋�؂�

			result += append_at_choice_start;
			if ( id == NULL ) {
				string	str=UnKakko(choiced);
				//result += string("\\q")+itos(question_num++)+"["+str+"]["+str+"]";
				result += "\\q["+str+","+str+"]";
			} else {
				*id++='\0';
				while ( *id=='\t' ) ++id; // �I�������x���ƃW�����v��̋�؂�
				//result += string("\\q")+itos(question_num++)+"["+UnKakko(id)+"]["+UnKakko(choiced)+"]";
				result += "\\q["+UnKakko(choiced)+","+UnKakko(id)+"]";
			}
			result += append_at_choice_end;

			continue;
		}

		// ������Ɣ����ȑ��݈Ӌ`�B
		if ( strncmp(p, "\\s", 2)==0 ) {	
			if ( !is_speaked(speaker) ) {
				if ( surface_changed_before_speak.find(speaker) == surface_changed_before_speak.end() ) {
					surface_changed_before_speak.insert(map<int,bool>::value_type(speaker,is_speaked_anybody()) );
				}
			}
		}

		// �W�����v
		if ( strncmp(p, "��", 2)==0 || strncmp(p, "��", 2)==0 ) {
			strvec	words;
			split(p+2, "\t", words, 2); // �W�����v��ƃW�����v�����̋�؂�

			if ( words.size()>=2 ) {
				string	r;
				if ( !calculate(words[1], r) )
					break;
				if ( r=="0" || r=="�O" ) {
					sender << "���v�Z���ʂ��O���������߁A���s���܂��B" << endl;
					continue;
				}
			}

			if ( words.size() >= 1 ) {
				jump_to = UnKakko(words[0].c_str());
			}
			else {
				jump_to.erase();
			}

			if ( strncmp(p, "��", 2)==0 ) {
				ip = std::distance(vec.begin(),it) + 1;
				--nest_count;
				return 2;
			}
			else {
				ip = std::distance(vec.begin(),it) + 1;
				--nest_count;
				return 1;
			} 
		}

		// �ϐ���ݒ�
		if ( strncmp(p, "��", 2)==0 ) {
			const char* v;
			string	value;
			bool	do_calc=false;
			p+=2;
			if ( (v=strstr_hz(p, "\t"))!=NULL ) { // �ϐ����ƕϐ��ɐݒ肷����e�̋�؂�
				value = UnKakko(v+1);
			}
			else if ( (v=strstr_hz(p, "��"))!=NULL || (v=strstr_hz(p, "="))!=NULL ) {
				value = UnKakko(v+((*v=='=') ? 1 : 2));
				do_calc=true;
			}
			else {
				//v = p+strlen(p);
				//value="";
				result += "\\n���@���ɂ��ϐ�������ɂ́A�^�u�ɂ���؂肩�A���ɂ��v�Z�����K�v�ł��@��\\n\\n[half]'" + value +"'";
				break;
			}

			string	key(p, v-p);
			key = UnKakko(key.c_str());

			if ( key=="" ) {
				result += "��"; // �����̂܂ܕ\��
			}
			else if ( aredigits(zen2han(key)) ) {
				sender << "��" << key << "�@�����݂̂̕ϐ����͈����܂���." << endl;
				erase_var(key);	// ���ݖ���
			}
			else if ( value=="" ) {
				sender << "��" << key << "�^cleared." << endl;
				erase_var(key);	// ���ݖ���
				system_variable_operation(key, "", &result);//���ݖ����������̂��V�X�e���ϐ������I
			}
			else {
				bool isOverwritten;
				bool isSysValue;

				// "0"�͑������ɎQ�Ƃ��鎞�A�G���[��Ԃ��Ȃ��悤�ɁB
				string *pstr = GetValue(key,isSysValue,true,&isOverwritten,"0");

				if ( do_calc ) {
					if ( !calculate(value, value) )
						break;
					if ( aredigits(value) ) {
						value = int2zen(stoi(value));
					}
				}

				sender << "��" << key << "��" << value << "�^" << 
					(isOverwritten ? "written." : "overwritten.")<< endl;

				if ( pstr ) { *pstr = value; }
				system_variable_operation(key, value, &result);
			}
			continue;
		}

		// �ʏ폈��
		while ( p[0] != '\0' ) {
			string	c=get_a_chr(p);	// �S�p���p��킸�ꕶ���擾���Ap���ꕶ�������߂�

			if ( c=="�i" ) {	// �������擾�E�}��
				string	tmp = KakkoSection(p);
				result += tmp;
			}
			else if ( c=="\xff" ) {	//��������\��
				c=get_a_chr(p);
				if ( c=="\x01" ) { //0xff0x01���T�[�t�F�X�؂�ւ��@��ɔ��p���l��1��������
					c=get_a_chr(p);
					int speaker_tmp = atoi(c.c_str());
					if ( is_speaked(speaker) && speaker != speaker_tmp ) {
						result += append_at_scope_change;
						characters += 2;
					}
					speaker = speaker_tmp;
					character_wait_clear;
					if ( speaker == 0 ) {
						result += "\\0";
					}
					else if ( speaker == 1 ) {
						result += "\\1";
					}
					else {
						result += "\\p[" + c + "]";
					}
				}
			}
			else if ( c=="�F" ) {	// �X�R�[�v�؂�ւ� - �����͓�l��z��B
				if ( is_speaked(speaker) ) {
					result += append_at_scope_change;
					characters += 2;
				}
				speaker = (speaker==0) ? 1 : 0;
				character_wait_clear;
				result += (speaker ? "\\1" : "\\0");
			}
			else if ( c=="\\" || c=="%" ) {	// ������X�N���v�g�̉��߁A�Ƃ��������߂̃X�L�b�v�B

				if (*p=='\\'||*p=='%') {	// �G�X�P�[�v���ꂽ\, %
					result += c + *p++;
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
					opt=UnKakko(opt.c_str());
				}

				if ( cmd=="n" ) {
					// ���s
					character_wait_clear;
				}
				else if ( ( (cmd=="0" || cmd=="h") && speaker==1) || ( (cmd=="1" || cmd=="u") && speaker==0) ) {
					// �X�R�[�v�؂�ւ�
					if ( is_speaked(speaker) ) {
						result += append_at_scope_change_with_sakura_script;
						characters += 2;
					}
					speaker = stoi(cmd);
					character_wait_clear;
				}
				else if ( cmd=="p" && aredigits(opt) ) {
					// �X�R�[�v�؂�ւ�onSSP/CROW
					if ( is_speaked(speaker) ) {
						result += append_at_scope_change_with_sakura_script;
						characters += 2;
					}
					speaker = stoi(opt);
					character_wait_clear;
				}

				if ( opt!="" ) {
					//sender << "ss_cmd: " << c << "," << cmd << "," << opt << endl;
					result += c + cmd + "[" + opt + "]";
				} else {
					//sender << "ss_cmd: " << c << "," << cmd << endl;
					result += c + cmd;
				}


				//result += string(start, p-start);
			}
			else {	// �ʏ�̈ꕶ��
				speaked_speaker.insert(speaker);
				result += c;
				characters += c.size();
				if ( c=="�B" || c=="�A" )
					character_wait_clear;
			}

		}
		character_wait_clear;
		result += "\\n";
	}

	//DBG(sender << "leave SentenceToSakuraScriptInternal, nest-count: " << nest_count << endl);
	--nest_count;
	return 0;
}

#undef	DBG
#define	DBG(a)	a

// �w�肳�ꂽ���O�ɊY�������₩�烉���_���ň�I�сA
// ������X�N���v�g�ɓW�J���ĕԂ��B
// �ċA���J�E���g���Ă�̂ŕs�p�ӂ�return�͌��ցB
const Talk* Satori::GetSentenceInternal(string& ioSentenceName)
{
	// �ċA�Ǘ�
	static	int nest_count=0;
	++nest_count;

	// �����_���g�[�N���\�񂳂�Ă����ꍇ�̓��ꏈ���B�������P��̃g�[�N�����łP�񂾂��B
	static	bool	reserved_talk_processed;
	if ( nest_count==1 )
		reserved_talk_processed=false;
	if ( !reserved_talk_processed && (ioSentenceName=="" || ioSentenceName=="OnTalk") )
	{
		reserved_talk_processed = true;

		string	reserved_talk_name;	// ����b���ׂ��g�[�N������΁A���̖��O�ɂȂ�
		if ( !reserved_talk.empty() ) {
			// �\��g�[�N�̓Y����1���f�N�������g
			map<int, string>::iterator	it = reserved_talk.begin();
			while ( it!=reserved_talk.end() )
			{
				reserved_talk[it->first-1] = it->second;
				reserved_talk.erase(it++);
			}

			// �擪�̃g�[�N���b���ׂ����ɂȂ��Ă���΁A���O���擾������A�L���[�A�E�g�B
			it=reserved_talk.begin();
			if ( it->first<=0 ) {
				reserved_talk_name = it->second;
				reserved_talk.erase(it);	// ����͍̂폜
			}
		}

		if ( ioSentenceName=="OnTalk" && talks.is_exist("OnTalk") ) {	
			// OnTalk�ł���A����`����Ă�ꍇ
			if ( reserved_talk_name.empty() ) {	// �b���ׂ��g�[�N�͂Ȃ��������_���g�[�N�ł���
				mRequestMap["Reference0"]=mReferences[0]="0";
				mRequestMap["Reference1"]=mReferences[1]="";
			}
			else {	// ����A�b���ׂ��\��g�[�N�����݂���
				mRequestMap["Reference0"]=mReferences[0]="1";
				mRequestMap["Reference1"]=mReferences[1]=reserved_talk_name;
			}
		}
		else {
			// �������AOnTalk�ł������Ƃ��Ă���`����ĂȂ��ꍇ
			if ( !reserved_talk_name.empty() )	// ����A�b���ׂ��\��g�[�N�����݂���
				ioSentenceName = reserved_talk_name;	// �C�x���g����\��g�[�N�Œu��������
		}
	}

	// map����w�薼�����g�[�N�Q������
	sender << "��" << ioSentenceName;
	const Talk* talk = talks.select(ioSentenceName, *this);
	if ( talk == NULL )
	{
		sender << " not matched." << endl; // �����Ɉ�v������̂��Ȃ������B
		--nest_count;
		return NULL;
	}
	sender << endl;
	--nest_count;
	return talk;
}
