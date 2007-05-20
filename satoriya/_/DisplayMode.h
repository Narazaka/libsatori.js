#ifndef	DISPLAYMODE_H
#define	DISPLAYMODE_H

#include	<windows.h>

class	DisplayMode {
	DisplayMode();				// �C���X�^���X�������֎~

public:
	// ��ʃ��[�h��ύX
	static	BOOL	Change(DWORD iWidth, DWORD iHeight, DWORD iBits);
	// Change�ɂ���ĕύX����ARestore�ɂ���Ė߂���Ă��Ȃ����TRUE
	static	BOOL	isChanged() { return mChangedFlag; }
	// ������Ԃ̉�ʃ��[�h�ɕ��A
	static	void	Restore();
	// ���݂̉�ʃ��[�h���擾
	static	void	Get(LPDWORD oWidth, LPDWORD oHeight, LPDWORD oBits);
	
	// 24->32->16->8 �̏��ő��݂����ʃ��[�h�������A�r�b�g����Ԃ��B
	// ����������݂��Ȃ���� 0 ��Ԃ��B
	static	int		GetFullColorBits(int iWidth, int iHeight);
	
private:
	// ��x�ł��ύX���ׂ��ꂽ��
	static	BOOL	mChangedFlag;
	// ����Ăяo���O�̉�ʃ��[�h���L��
	static	DWORD	mWidth,mHeight,mBits;
};

#endif	//	DISPLAYMODE_H
