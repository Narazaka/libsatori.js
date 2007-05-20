#include	"Font.h"
#include	<assert.h>

Font::Font() {
	init();
	mChanged=true;	// �P��ڂ͕K�����
	mFont=NULL;
}

Font::~Font() {
	if ( mFont != NULL )
		::DeleteObject(mFont);
}

void	Font::init() {
	Bold(false);
	Italic(false);
	Strike(false);
	Underline(false);
	Color(255,255,255);
	Size(24);
	Face("�l�r �S�V�b�N");
}


void	Font::Bold(bool iBold) {
	if ( mBold==iBold )
		return;
	mBold=iBold;
	mChanged=true;
}
void	Font::Italic(bool iItalic) {
	if ( mItalic==iItalic )
		return;
	mItalic=iItalic;
	mChanged=true;
}
void	Font::Strike(bool iStrike) {
	if ( mStrike==iStrike )
		return;
	mStrike=iStrike;
	mChanged=true;
}
void	Font::Underline(bool iUnderline) {
	if ( mUnderline==iUnderline )
		return;
	mUnderline=iUnderline;
	mChanged=true;
}

void	Font::Color(COLORREF iColor) {
	if ( mColor==iColor )
		return;
	mColor=iColor;
}
void	Font::Width(int iWidth) {
	if ( mSize.cx==iWidth/2 )
		return;
	mSize.cx=iWidth/2;
	mChanged=true;
}
void	Font::Height(int iHeight) {
	if ( mSize.cy==iHeight )
		return;
	mSize.cy=iHeight;
	mChanged=true;
}
void	Font::Size(int iSize) {
	if ( mSize.cx==iSize/2 && mSize.cy==iSize )
		return;
	mSize.cx=iSize/2;
	mSize.cy=iSize;
	mChanged=true;
}
void	Font::Size(SIZE iSize) {
	if ( mSize.cx==iSize.cx && mSize.cy==iSize.cy )
		return;
	mSize=iSize;
	mChanged=true;
}
void	Font::Face(LPCTSTR iFace) {
	assert(lstrlen(iFace)<LF_FACESIZE);
	if ( lstrcmp(mFace, iFace)==0 )
		return;
	lstrcpy(mFace, iFace);
	mChanged=true;
}

bool	Font::update() {

	if ( !mChanged )
		return	true;	// �ύX����Ė���

	// �ύX���ꂽ�̂ō�蒼���B
	if ( mFont != NULL )
		::DeleteObject(mFont);

	LOGFONT	theLogFont;
	::ZeroMemory(&theLogFont, sizeof(LOGFONT));
	theLogFont.lfWidth = mSize.cx;// �t�H���g�\����̉���
	theLogFont.lfHeight = mSize.cy;	// �t�H���g�\����̍���
	theLogFont.lfEscapement = 0;	// �c�X��
	theLogFont.lfOrientation = 0;	// ���X��
	theLogFont.lfWeight = mBold ? FW_BOLD : 0;	// �t�H���g�̑����i 0�̏ꍇ�̓f�t�H���g�j
	theLogFont.lfItalic = mItalic;	// �Α̎w��
	theLogFont.lfUnderline = mUnderline;	// �����w��
	theLogFont.lfStrikeOut = mStrike;	// ���������w��
	theLogFont.lfCharSet = SHIFTJIS_CHARSET;		// �L�����N�^�Z�b�g
	theLogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;	// �o�͐��x
	theLogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;// �N���b�s���O���@
	theLogFont.lfQuality = DEFAULT_QUALITY;			// �O�ς̃N�I���e�B
	theLogFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;	// �s�b�`�Ǝ��
	::lstrcpy(theLogFont.lfFaceName, mFace);
	mFont = ::CreateFontIndirect(&theLogFont);
	if ( mFont == NULL )
		return	false;

	mChanged = false;
	return	true;
}



