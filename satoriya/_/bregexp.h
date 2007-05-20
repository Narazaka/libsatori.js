#pragma warning( disable : 4786 ) //�u�f�o�b�O�����ł̎��ʎq�؎̂āv
#pragma warning( disable : 4503 ) //�u�������ꂽ���O�̒��������E���z���܂����B���O�͐؂�̂Ă��܂��B�v
#ifndef for
#define for if(0);else for
#endif	// for

#include <string>
#include <map>
#include	<vector>
#include	<windows.h>
using namespace std;

typedef struct bregexp {
  const char *outp;    // BSubst �u���f�[�^�̐擪�|�C���^
  const char *outendp; // BSubst �u���f�[�^�̍ŏI�|�C���^+1
  const int  splitctr; // BSplit �z��
  const char **splitp; // BSplit �f�[�^�|�C���^
  int  rsv1;           // ���U�[�u�@���R�Ɏg�p�\ 
  char *parap;         // �p�^�[���f�[�^�|�C���^
  char *paraendp;      // �p�^�[���f�[�^�|�C���^+1
  char *transtblp;     // BTrans �ϊ��e�[�u���|�C���^
  char **startp;       // �}�b�`�����f�[�^�̐擪�|�C���^
  char **endp;         // �}�b�`�����f�[�^�̍ŏI�|�C���^+1
  int nparens;         // �p�^�[���̒���() �̐��B $1,$2, �𒲂ׂ�Ƃ��Ɏg�p
} BREGEXP;

typedef const char* PCSTR;


extern int (*BMatch)(PCSTR, PCSTR, PCSTR, BREGEXP**, char*);
extern int (*BSubst)(PCSTR, PCSTR, PCSTR, BREGEXP**, char*);
extern int (*BTrans)(PCSTR, PCSTR, PCSTR, BREGEXP**, char*);
extern int (*BSplit)(PCSTR, PCSTR, PCSTR, int, BREGEXP**, char*);
extern void (*BRegfree)(BREGEXP*);
extern char *(*BRegexpVersion)(void);



class BRegExpCache {
private:
	map<string, BREGEXP*> mCache;

public:
	~BRegExpCache() {
		for ( map<string, BREGEXP*>::iterator i=mCache.begin() ; i!=mCache.end() ; ++i )
			if ( i->second != NULL )
				BRegfree(i->second);
	}

	BREGEXP*	get(const char *szKind, const char *szReg) {

		// �L���b�V��������
		map<string, BREGEXP*>::iterator i = mCache.find(szReg);
		if ( i != mCache.end() )
			return	i->second; // ���������B������g���B

		// �V�K�ɃR���p�C��
		BREGEXP*	pBRegExp = NULL;
		char msg[80], p[] = " ";
		if (stricmp(szKind, "match") == 0)
			BMatch(szReg, p, p + 1, &pBRegExp, msg);
		else if (stricmp(szKind, "replace") == 0)
			BSubst(szReg, p, p + 1, &pBRegExp, msg);
		else
			BSplit(szReg, p, p + 1, 0, &pBRegExp, msg);

		if ( pBRegExp != NULL && pBRegExp->parap != NULL )
			mCache[string(pBRegExp->parap, pBRegExp->paraendp)] = pBRegExp;	// ���ʂ��L���b�V���ɒǉ�

		return	pBRegExp;
	}
};




class	BRegExp {
public:
	// bregexp.dll�����[�h/�A�����[�h����
	static bool load(string iPath="");
	static bool isloaded() { return mHandle != NULL; }
	static void unload();

	int // ��v�L��0/1�Bg�I�v�V�����t���Ȃ��v���B
	match(
		const string& iTarget,			// �Ώە�����
		const string& iRE,				// ���K�\���B/abc/ �Ƃ�
		//vector<string>*	oResult=NULL);	// [0]�͈�v�����S�́A[1]�ȍ~�͊e�J�b�R�B
		vector< pair<string, int> >* oResult=NULL);	// [0]�͈�v�����S�́A[1]�ȍ~�͊e�J�b�R�Bsecond��iTarget���̊J�n�ʒu

	int // ��v�L��0/1�Bg�I�v�V�����t���Ȃ��v���B
	replace(
		const string& iTarget,			// �Ώە�����
		const string& iRE,				// ���K�\�� s/abc/������/ �Ƃ�
		vector<string>*	oResult = NULL,	// [0]�͈�v�����S�́A[1]�ȍ~�͊e�J�b�R�B
		string*	oReplaced = NULL);		// �u���㕶����B

	int // �������B��v�����1�����B
	split(
		const string& iTarget,	 // �Ώە�����
		const string& iRE,		 // ���K�\�� /,/ �Ƃ� /\t/ �Ƃ�
		vector<string>*	oResult);// ��������

private:
	static HINSTANCE mHandle;
	static BRegExpCache mBRegExpCache;

// ������艺�͗v�čl�c�c

	string strLastString;
	string strLastRegExp;
	BREGEXP *pLastRegExp;
	int nLastPos;
	bool bLastSplit;
	bool bLastReplace;
	bool bCountMatch;

	void	ready(const string& iTarget, const string& iRE, const string& iFunction);
public:
	BRegExp() : pLastRegExp(NULL) {}

};

