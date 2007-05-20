/*
	���̂Ƃ���ueuc,jis��sjis�ɕϊ�����v���Ƃ����ł��Ȃ��B
	�������g���͗e�ՁB

	���ʁE�ϊ��ɂ͂�������Q�Ƃ����B
	http://www.mars.dti.ne.jp/~torao/program/appendix/japanese.html

	���ʃ��[�`���̋���
	
		�e�L�X�g���Ɂ@charset=??? �܂��� encoding=??? ������΍̗p�i�����������ꍇ�͍ŏ��̂��̂��g���j
		???�̕�����Ƃ��Ă� iso-2022-jp, shift_jis, x-sjis, x-euc-jp, euc-jp, UTF-16BE, UTF-16LE, UTF-8 ��F���B

		����̏����́AUTF��BOM, charset, encoding, jis�G�X�P�[�v, euc/sjis����ł���B
		Unicode�͓��e�ɂ�锻�ʂ͍s��Ȃ��B

		charset�������ꍇ�ł��Ajis�͊m���Ȕ��肪�ł���B
		euc��sjis�́A�݂��Ɏg���Ă��Ȃ��R�[�h�����݂���Ίm���Ȕ��肪�ł��邪�A
		���S�ɏd�Ȃ��������̕��������g���Ă��Ȃ������ꍇ�A���ʕs�\�Ƃ��� CS_NULL ��Ԃ��B


	sjis2jis�͂�������B
	�\�[�X�͖w�ǂ��̂܂�܁B
	http://plaza13.mbn.or.jp/~konton/TIPPRG/tipprg03.html
*/


// �����R�[�h
enum CharactorSet
{
	CS_NULL,
	CS_JIS,
	CS_SJIS,
	CS_EUC,
	CS_UTF8,
	CS_UTF16BE,
	CS_UTF16LE,
};

// �����R�[�h���ʁi�m���ł͂Ȃ��B���ʂɎ��s�����CS_NULL��Ԃ��j
CharactorSet	getCharactorSet(const char* p);
inline CharactorSet	getCharactorSet(const string& str) { return getCharactorSet(str.c_str()); } 

// �����R�[�h�ϊ�
string sjis2euc(const string& str);
string sjis2jis(const string& str);

string euc2sjis(const string& str);
string euc2jis(const string& str);

string jis2sjis(const string& str);
string jis2euc(const string& str);

string	UTF8toSJIS(const string& str);
string	UTF16BEtoSJIS(const wchar_t* p);
string	UTF16LEtoSJIS(const wchar_t* p);

// �s���̃R�[�h���������ʂ��A�Ƃɂ���sjis��Ԃ��B�����Ȃ��Ƃ�����B
inline string convertSomethingToSJIS(const char* p) {
	switch ( getCharactorSet(p) ) {
	case CS_JIS: return jis2sjis(p); 
	case CS_EUC: return euc2sjis(p);
	case CS_UTF8: return UTF8toSJIS(p);
	case CS_UTF16BE: return UTF16BEtoSJIS((const wchar_t*)p);
	case CS_UTF16LE: return UTF16LEtoSJIS((const wchar_t*)p);
	default: return p; // �ϊ����Ȃ�
	}
}

