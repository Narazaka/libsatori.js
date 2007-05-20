#include	"DisplayMode.h"

// �w��̉𑜓x�E�F���Ɉ�v����DevMode�������A
// ���݂����oDevMode�Ɋi�[���� true ��Ԃ��B������Ȃ���� false ��Ԃ��B
static BOOL	GetDevMode( DEVMODE* oDevMode, DWORD iX, DWORD iY, DWORD iBits ) {
	DEVMODE	theDevMode;
	for (int i=0;
		 ::EnumDisplaySettings(NULL, i, &theDevMode);
		 i++ ) {
		if ( theDevMode.dmBitsPerPel == iBits &&
			theDevMode.dmPelsWidth == iX &&
			theDevMode.dmPelsHeight == iY ) {
			*oDevMode = theDevMode;
			return	TRUE;
		}
	}
	return	FALSE;
}

BOOL	DisplayMode::mChangedFlag = FALSE;
DWORD	DisplayMode::mWidth = 0;
DWORD	DisplayMode::mHeight = 0;
DWORD	DisplayMode::mBits = 0;

// ��ʃ��[�h��ύX
BOOL	DisplayMode::Change(DWORD iX, DWORD iY, DWORD iBits) {
	if ( !mChangedFlag )
		Get(&mWidth, &mHeight, &mBits);	// ��ʃ��[�h�̏�����Ԃ�ۑ�

	// DevMode���擾
	DEVMODE	theDevMode;
	if ( !GetDevMode( &theDevMode, iX, iY, iBits ) )
		return	FALSE;	// �w��̉�ʃ��[�h�����݂��Ȃ�

	// ��ʃ��[�h��ύX
	OutputDebugString("DisplayMode::Change -- ��ʃ��[�h��؂芷���܂��B\n");
	::ChangeDisplaySettings(&theDevMode, 0 );// CDS_UPDATEREGISTRY);
	OutputDebugString("DisplayMode::Change -- ��ʃ��[�h��؂芷���܂����B\n");
	mChangedFlag = TRUE;
	return	TRUE;
}

// ������Ԃ̉�ʃ��[�h�ɕ��A
void	DisplayMode::Restore() {
	if ( !mChangedFlag )
		return;	// ��ʃ��[�h�͕ύX����Ă��Ȃ�

	DEVMODE	theDevMode;
	GetDevMode( &theDevMode, mWidth, mHeight, mBits );
	OutputDebugString("DisplayMode::Restore -- ��ʃ��[�h�����ɖ߂��܂��B\n");
	::ChangeDisplaySettings(&theDevMode, 0 );// CDS_UPDATEREGISTRY);
	OutputDebugString("DisplayMode::Restore -- ��ʃ��[�h�����ɖ߂��܂����B\n");
	mChangedFlag = FALSE;
}

// ���݂̉�ʃ��[�h���擾
void	DisplayMode::Get(LPDWORD oX, LPDWORD oY, LPDWORD oBits) {
	if ( oBits != NULL ) {
		HDC	hDC = ::GetDC(NULL);
		*oBits = ::GetDeviceCaps(hDC, BITSPIXEL);
		::ReleaseDC(NULL, hDC);
	}
	if ( oX != NULL )
		*oX = ::GetSystemMetrics(SM_CXSCREEN);
	if ( oY != NULL )
		*oY = ::GetSystemMetrics(SM_CYSCREEN);
}


int		DisplayMode::GetFullColorBits(int iWidth, int iHeight) {

	int	bpp=0;
	DEVMODE	theDevMode;
	for (int i=0 ; ::EnumDisplaySettings(NULL, i, &theDevMode); i++ ) {
		if ( theDevMode.dmPelsWidth != (unsigned)iWidth ) continue;
		if ( theDevMode.dmPelsHeight != (unsigned)iHeight ) continue;

		if ( theDevMode.dmBitsPerPel==24 ) {
			return	24;	// ��������Ō���
		}
		else if ( theDevMode.dmBitsPerPel==32 ) {
			bpp=32;
		}
		else if ( theDevMode.dmBitsPerPel==16 && bpp!=32 ) {
			bpp=16;
		}
		else if ( theDevMode.dmBitsPerPel==8 && bpp==0 ) {
			bpp=8;
		}
	}
	return	bpp;
}


