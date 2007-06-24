/*---------------------------------------------------------------------------

	�w���X�i���Ƃ�j�x

---------------------------------------------------------------------------*/
#ifndef SATORI_H
#define SATORI_H


//---------------------------------------------------------------------------
// �ėp�̃c�[����
#include	"../_/stltool.h"
#include	"../_/simple_stack.h"

// �ꂵ�΂ւ̑��M
#include	"../_/Sender.h"

// SAORI�֘A
#include	"shiori_plugin.h"

// SakuraDLL�Ƃ��ČĂяo����邽�߂̍\��
#include	"SakuraDLLHost.h"

//---------------------------------------------------------------------------
// �������i�[����\��

typedef string Word;
typedef strvec Talk;

#include "Families.h"
class AllWords : public Families<Word> {};
class AllTalks : public Families<Talk> {};

//---------------------------------------------------------------------------
#ifdef	_DEBUG
	#define	DBG(a)	a
#else
	#define	DBG(a)	NULL
#endif	//	_DEBUG

//---------------------------------------------------------------------------
// �萔�I��

static const char	ret_dlmt[3] = { 13, 10, 0 };
static const char	byte1_dlmt[2] = { 1, 0 };

// �o�[�W����������Ƃ�
extern const char* gSatoriName;
extern const char* gSatoriNameW;
extern const char* gSatoriCraftman;
extern const char* gSatoriCraftmanW;
extern const char* gSatoriVersion;
extern const char* gShioriVersion;
extern const char* gSaoriVersion;

static const int RESPONSE_HISTORY_SIZE=64;

enum SurfaceRestoreMode {
	SR_NONE = 0,
	SR_NORMAL,
	SR_FORCE,
};

//---------------------------------------------------------------------------

class escaper
{
	// �G�X�P�[�v�㕶���B
	// �u�������@�\�������Ȃ��J�b�R�́A��U����+id+���p�󔒂ɒu��������B
	// �����R�[�h���sjis�̈ꕶ���Ƃ��Ĉ����邪�A�Y�����镶���͖����B�_���ȈāB
	static const char sm_escape_sjis_code[3];

	//map<string, int> m_str2id;
	vector<string> m_id2str;

public:
	// ������������󂯎��A�����o�Ɋi�[���A�u�G�X�P�[�v���ꂽ������v��Ԃ��B
	string insert(const string& i_str);
	// �Ώە����񒆂Ɋ܂܂��u�G�X�P�[�v���ꂽ������v�����ɖ߂��B
	void unescape(string& io_str);
	// �����o���N���A
	void clear();
};

//---------------------------------------------------------------------------
class Satori : public Evalcator, public SakuraDLLHost
{

private:
	string	mBaseFolder;	// satori.dll�̑��݂���t�H���_
	string	mExeFolder;		// �{��.exe�̑��݂���t�H���_

	// ���N�G�X�g���e
	string	mStatusLine;	// ���N�G�X�g�̈�s��
	strmap	mRequestMap;	// : �Z�p���[�g����Ă�key:value
	string	mRequestID;		// SHIORI/3.0 ID
	string	mRequestCommand;	// GET, NOTIFY, ...
	string	mRequestType;		// SHIORI / SAORI / MAKOTO
	string	mRequestVersion;	// 1.0, 2.x, 3.0
	bool	mIsMateria;	// �܂Ă肠�͓��ꏈ�����v��
	strvec	mReferences;
	strvec  mKakkoCallResults;
	enum { SAORI, SHIORI2, SHIORI3, MAKOTO2, UNKNOWN } mRequestMode;
	// �i�[���ꂽ�����o����Response���쐬�B�Ԓl�̓X�e�[�^�X�R�[�h�B
	int		CreateResponse(strmap& oResponse);
	// SHIORI/3.0 ID��On�Ŏn�܂��Ă��ꍇ�ACreateResponse����Ă΂��
	int		EventOperation(string iEvent, map<string,string> &oResponse);

	// �߂�lmap
	strmap mResponseMap;

	// �u����������
	strmap	replace_before_dic;
	strmap	replace_after_dic;

	inimap	mCharacters;	// characters.ini�̓��e

	// ������̃G�X�P�[�v�Ə����߂�
	escaper m_escaper;

	// SAORI/�������߂��Ăяo���ۂ̈�����؂�ƂȂ镶����Q
	set<string>	mDelimiters;

	// �S�Ắ��g�[�N
	AllTalks	talks;
	// ����` ...�W�������������ꂽ�����̒P��B
	AllWords	words;


	// ���ϐ�
	strmap	variables;
	// �����A���J�[
	vector<string>	anchors;

	// �ϐ��̏����B�����Ɩ�肪����炵����H
	void	erase_var(const string& key);

	// ���I�ɓo�^���ꂽ�P��Bwords�ɂ����^����Bsatori_savedata.txt�ɕۑ�����̂��ړI�B
	map<string, vector<const Word*> >	mAppendedWords; // set�łȂ�vector�Ȃ̂́u�d������F����/�~���v�ɔ����āB

	// �߂����g�[�N�̗���
	deque<string>	mResponseHistory;

	// call����stack �������͍̂ċA�B
	simple_stack<strvec>	mCallStack;

	// �P��`�F�C��
	//map<string, set<string> >	mRelationalWord;	// <�P��, <����> > 
	//set<string>	mUsedRelation;	// <�g�p���ꂽ����> �P�g�[�N�Ń��Z�b�g�B�������Z�b�g���B

	// �I�𕪊�̋L�^    map<ID, pair<NUMBER, LABEL> >�@
	// �悭�l������A�I�����ڂƂ邾���Ȃ�S���s�v�������B�܂��S�̎擾�ł��Ă������͖�������
	map<string, pair<int, string> >	question_record;

	int	second_from_last_talk;	// �Ō�ɒ����Ă���̌o�ߎ���

	// �Ȃł��񐔁Bkey�͂Ȃł��ӏ��Avalue�͉񐔁B
	strintmap	nade_count;
	// �Ȃł��L������
	int	nade_valid_time;
		// SecondChange���Ɍ����A�R�b��move�����Ȃ���nade_count��S�N���A�B
		// nade_count��nade_sensitivity���z����Ƃ��̃C�x���g�������Anade_count��S�N���A�B
	bool	insert_nade_talk_at_other_talk;	// �����Ă�Œ��̂ȂŔ����L��
	int		nade_valid_time_initializer;	// �Ȃł�ꎝ���b���i�ȂŃZ�b�V�����̊����j
	int		nade_sensitivity;				// �Ȃł��䖝�񐔁i�����܂ł̉񐔁j


	strintmap	koro_count;	// ���낱���
	int	koro_valid_time;	// ���낱��L������

	// ����Ԋu�i�b�j�B�O�Ȃ�ق�B���ϐ�
	int	talk_interval;
	// ����Ԋu�덷�i�Ԋu�ɑ΂��道�j���ϐ�
	int	talk_interval_random;
	// ���񎩔�����܂ł̎��ԁB����ƃ��Z�b�g�B���ϐ�
	int	talk_interval_count;
	// �����}���E�F�C�g�̔{���B�ȗ���100�B
	int	rate_of_auto_insert_wait;
	// ���؂�ĂĂ�����iOnTalk���Ăяo���j���ǂ����t���O
	bool is_call_ontalk_at_mikire;

	// �t��������
	string	append_at_scope_change;
	string	append_at_scope_change_with_sakura_script;
	string	append_at_talk_start;
	string	append_at_talk_end;
	string	append_at_choice_start;
	string	append_at_choice_end;

	// ����ׂ�Ǘ��BSentenceToSakuraScript�̖��ċA�Ăяo�����ɏ������B
	int		speaker;		// �b��
	set<int>	speaked_speaker;		// �����ł��������H
	bool	is_speaked(int n) { return speaked_speaker.find(n) != speaked_speaker.end(); }
	bool	is_speaked_anybody() { return speaked_speaker.size()>0; }
	int		characters;
	int		question_num;

	map<int,bool> surface_changed_before_speak;	// ��b�O�ɃT�[�t�F�X���؂�ւ��w�����������H


	// �ߋ��̃J�b�R�u���������L���B�����i�g�H�j�Ŏg�p
	// SentenceToSakuraScript�̍ċA�ɓ�������B
	simple_stack<strvec>	kakko_replace_history;	

	// ��b���T�[�t�F�X�߂��E���ϐ�
	enum SurfaceRestoreMode	surface_restore_at_talk;
	map<int, int>	default_surface;
	map<int, int>	surface_add_value;
	map<int, int>	next_default_surface; // �r����def_surface��؂芷���Ă��A����request�ł͎g��Ȃ�
	string	surface_restore_string();

	// �Ԃ�l�}�~
	bool return_empty;

	// �X�N���v�g�w�b�_
	string header_script;

	// �����Ăяo���}�~
	int m_nest_limit;
	int m_nest_count;

	// �΂�[��ʒu
	map<int, bool>	validBalloonOffset;	// 1��ł��ݒ肳�ꂽ��L�� �[���Е��������ƈӖ����������B�ނ��B
	map<int, string>	BalloonOffset;

	// ���Ԍn���擾�p
#ifdef POSIX
	unsigned long tick_count_at_load, tick_count_total;
#else
	DWORD	tick_count_at_load, tick_count_total;
#endif
	
	// �T�[�t�F�X
	map<int, int>	cur_surface;
	map<int, int>	last_talk_exiting_surface;
	// ���b�X�V�����u��ԁv
	bool	mikire_flag,kasanari_flag,can_talk_flag;

	// COMMUNICATE����
#ifdef POSIX
	bool updateGhostsInfo() { return true; } // �������Ȃ��B
#else
	bool	updateGhostsInfo();	// FMO������擾
#endif
	vector<strmap>	ghosts_info;	// FMO�̓��e���̂܂܁B0�͎������g�A1�`�͏��Ԃǂ���B
	strmap*	find_ghost_info(string name);	// ghosts_info�������A����̃S�[�X�g�̏��𓾂�

	string	mCommunicateFor;	// �b�������ΏۃS�[�X�g�B���Őݒ肳��response��To������
	set<string>	mCommunicateLog;	// ��b���O�B�J��Ԃ����������ꍇ�͉�b�ł��؂�

	// �e�Z�N�V�����̃��O�f���L���B
	bool	fRequestLog, fOperationLog, fResponseLog;

	// �ϐ��ɂ�胊���[�h���w�����ꂽ��ON�B
	bool	reload_flag;
	// �����t�H���_
	strvec	dic_folder;

	// TEACH�����ϐ���
	string	teach_genre;

	// �^�C�}���F���b�܂ł̕b��
	strintmap	timer;

	// �g�[�N�̗\��
	map<int, string>	reserved_talk;

	// �u�Ǝ��C�x���g�ɂ��materia�C�x���g�̒u�������v�̂��߂̃X�N���v�g����
	string	on_loaded_script;
	string	on_unloading_script;

	// �x�v���O�C��
	ShioriPlugins	mShioriPlugins;
	string	inc_call(const string&, const strvec&, strvec&, bool is_secure);

	// ���S�H
	bool	secure_flag;

	// �܂Ƃ��Ɏ����ǂݍ��݂ł������ǂ���
	bool	is_dic_loaded;

	// ���O�̕\���I����
	string	last_choice_name;

	// �Z�[�u�f�[�^�ۑ����̈Í����L��
	bool	fEncodeSavedata;
	// ������������v�Z���邩
	enum { SACM_ON, SACM_OFF, SACM_AUTO } mSaoriArgumentCalcMode;

	// �����Z�[�u�Ԋu
	int	mAutoSaveInterval;
	int	mAutoSaveCurrentCount;

	// �������
	int	numWord, numParentheres, numSurfaceChange,
		numDictionary, numDictionarySize;

	// �V�X�e�����n ----------

	enum { UNDEFINED, WIN95, WIN98, WINME, WINNT, WIN2K, WINXP } mOSType;

#ifndef POSIX
	// �}���`���j�^
	bool	is_single_monitor;	// �ŏ�ʁB���ꂪfalse�Ȃ�Έȉ����g���Ă͂����Ȃ�
	RECT	desktop_rect;
	RECT	max_screen_rect;
	map<int, HWND>	characters_hwnd;
#endif

	// �����o�֐�

	void	InitMembers();

	int	 LoadDicFolder(const string& path);
	bool LoadDictionary(const string& filename,bool warnFileName = true);

	string	GetWord(const string& name);

	string	KakkoSection(const char*& p,bool for_calc = false);
	string	UnKakko(const char* p,bool for_calc = false);

	bool	GetURLList(const string& name, string& result);
	bool	GetRecommendsiteSentence(const string& name, string& result);

	// �w��C�x���g�����ꍇ�A�C�x���g���̒u������������ioevent��Ԃ��B����ł������Ȃ�false
	bool	FindEventTalk(string& ioevent);

	// Communicate�`�������B�Y���Ȃ��Ȃ�false�Band_mode��true�Ȃ�S�P���v�ȊO�͖����Ƃ���
	bool	TalkSearch(const string& iSentence, string& oScript, bool iAndMode);

	// �V�X�e���ϐ��ݒ莞�̓���
	bool	system_variable_operation(string key, string value, string* result=NULL);

	// �����B�Ԓl�͑��s�̗L���B���s����SentenceName��GetSentence�B
	bool	GetSentence(string& ioSentenceName, string& oResultScript);

	// ����]�����A���ʂ̐^�U�l��Ԃ�
	bool evalcate_to_bool(const Condition& i_cond);

	// �����ɓn���ꂽ���̂������̖��O�ł���Ƃ��A�u�������Ώۂ�����Βu��������B
	bool	CallReal(const string& word, string& result);

	string* GetValue(const string &key,bool iIsExpand = false,bool *oIsExpanded = NULL);

	void surface_restore_string_addfunc(string &str,map<int, int>::const_iterator &i);

public:

	Satori() {}
	virtual ~Satori() {}

	// SHIORI/3.0�C���^�t�F�[�X
	virtual bool load(const string& i_base_folder);
	virtual bool unload();
	virtual int	request(
		const string& i_protocol,
		const string& i_protocol_version,
		const string& i_command,
		const strpairvec& i_data,
		
		string& o_protocol,
		string& o_protocol_version,
		strpairvec& o_data);

	// �ϐ����̃f�[�^���t�@�C���ɕۑ�
	bool	Save(bool isOnUnload=false);	

	// strvec���炳����X�N���v�g�𐶐�����
	string	SentenceToSakuraScript(const strvec& vec);
	// strvec�Ƀv���v���Z�X���|������A������X�N���v�g�𐶐�����B���Ƃ�ėp
	string	SentenceToSakuraScript_with_PreProcess(const strvec& vec);
	// �w�肳�ꂽ���O�́������擾����
	string	GetSentence(const string& name);
	// �����ɓn���ꂽ���̂������̖��O�ł���Ƃ��A�u�������Ώۂ�����Βu��������B
	bool	Call(const string& word, string& result);
	// ���X���x���ł̌v�Z���s���B�߂�l�͐��ہB
	bool calculate(const string& iExpression, string& oResult);

	// �ŏI�u�����������B�u����̃X�N���v�g�����g�������i���s���Ă����Ȃ��Ă��ꏏ�j�Ɣ��f������false��Ԃ��B
	bool	Translate(string& script);

};

//---------------------------------------------------------------------------
string	int2zen(int i);
string	zen2han(string str);
bool	calc(string&);
void	diet_script(string&);




//---------------------------------------------------------------------------
#endif


