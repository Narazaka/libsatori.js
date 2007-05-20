#ifndef	FONT_H
#define	FONT_H

#include	<windows.h>

class Font {

	// �t�H���g�̊e��ݒ�
	bool		mBold, mItalic, mStrike, mUnderline;
	COLORREF	mColor;
	SIZE		mSize;
	TCHAR		mFace[LF_FACESIZE];

	// �t�H���g���̕ύX�L���B
	// �`�掞�A�ύX�������mFont����蒼���B
	bool		mChanged;

	// �쐬�ς݃t�H���g�̃n���h��
	HFONT		mFont;	
	
public:
	Font();
	~Font();
	void	init();		// �ݒ��������Ԃɖ߂�
	// �ݒ�𔽉f����mFont���ŐV��Ԃɂ���BTextOut�O�ɂ͖Y�ꂸ�Ɏ��s���܂��傤�B
	bool	update();	
	void	del() { if (mFont!=NULL) ::DeleteObject(mFont); }
	HFONT	getFont() { return mFont; }
	COLORREF	getColor() const { return mColor; }

	bool	Bold() const { return mBold; }
	bool	Italic() const { return mItalic; }
	bool	Strike() const { return mStrike; }
	bool	Underline() const { return mUnderline; }
	SIZE	Size() const { return mSize; }
	const char*	Face() const { return mFace; }

	void	Bold(bool iBold);
	void	Italic(bool iItalic);
	void	Strike(bool iStrike);
	void	Underline(bool iUnderline);
	void	Color(COLORREF iColor);
	void	Color(int iR, int iG, int iB) { Color(RGB(iR,iG,iB)); }
	void	Size(int iSize);
	void	Size(SIZE iSize);
	void	Height(int iHeight);
	void	Width(int iWidth);
	void	Face(LPCTSTR iFace);
};

#endif	//	FONT_H
