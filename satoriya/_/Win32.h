#ifndef	WIN32_H
#define	WIN32_H

#define	WIN32_LEAN_AND_MEAN
#include	<windows.h>
#include	<assert.h>
#include	"Utilities.h"

//----------------------------------------------------------
// �N���b�v�{�[�h

// �������ݒ肷��BhWnd��NULL�ł��\��Ȃ��B
bool	SetClipBoard(HWND iWnd, const char* iFormat, ...);
#ifdef	_STRING_
std::string	GetClipBoard();
#endif

//----------------------------------------------------------
// �v���Z�X�E�X���b�h

BOOL	CreateThread( DWORD (* iCallBack)(LPVOID), LPVOID iParam);

// ShellExecute
BOOL	Start( LPSTR lpstrFileName );
// CreateProcess
BOOL	Start( LPCSTR lpszImageName, LPSTR lpszCommandLine );

//----------------------------------------------------------
// �L�[�{�[�h�E�}�E�X

// �w��̃L�[�͉�����Ă��邩�H
inline	BOOL isKeyDown( int nVirtKey ) { return ( ::GetAsyncKeyState(nVirtKey) & 0x8000 ); }

// �J�[�\���ʒu��ݒ�E�擾
POINT	GetCursorPos();
void	SetCursorPos( POINT pt );
// �N���C�A���g���W�n�ŃJ�[�\���ʒu��ݒ�E�擾
POINT	GetCursorPos( HWND hWnd );
void	SetCursorPos( HWND hWnd, POINT pt );
// �J�[�\���̓N���C�A���g�̈�����H
BOOL	isCursorOnClient( HWND hWnd );

// �}�E�X�L���v�`���̊J�n�^�I��
BOOL	StartCapture( HWND hWnd, const RECT* =NULL );
BOOL	isCapture( HWND hWnd );
void	EndCapture();
// �w��window�������Ă����ꍇ�Ɍ�������TRUE��Ԃ��B
BOOL	EndCapture( HWND hWnd ); 

// �}�E�X�z�C�[������̓��͂��T�|�[�g�B
// WinProc() { if ( message == msgMOUSEWHEEL ) message = WM_MOUSEWHEEL; ...
extern const UINT msgMOUSEWHEEL;

//----------------------------------------------------------
// �_

inline POINT MAKEPOINT( int x, int y ) { POINT	pt = { x, y }; return pt; }
inline POINT MAKEPOINT( LPARAM lParam ) { POINT	pt = { LOWORD(lParam), HIWORD(lParam) }; return	pt; }

inline POINT& operator += ( POINT& lhs, POINT& rhs ) { lhs.x+=rhs.x; lhs.y+=rhs.y; return lhs; }
inline POINT& operator -= ( POINT& lhs, POINT& rhs ) { lhs.x-=rhs.x; lhs.y-=rhs.y; return lhs; }
inline POINT operator + ( POINT& lhs, POINT& rhs ) { return MAKEPOINT( lhs.x+rhs.x, lhs.y+rhs.y ); }
inline POINT operator - ( POINT& lhs, POINT& rhs ) { return MAKEPOINT( lhs.x-rhs.x, lhs.y-rhs.y ); }
inline POINT& operator *= ( POINT& pt, int i ) { pt.x*=i; pt.y*=i; return pt; }
inline POINT& operator /= ( POINT& pt, int i ) { pt.x/=i; pt.y/=i; return pt; }
inline POINT operator - ( POINT& pt ) { return MAKEPOINT( -pt.x, -pt.y ); }

inline bool operator == (const POINT& lhs, const POINT& rhs) { return (lhs.x == rhs.x && lhs.y == rhs.y); }
inline bool operator != (const POINT& lhs, const POINT& rhs) { return (lhs.x != rhs.x || lhs.y != rhs.y); }

// iBase�����_�Ƃ��āA�E����90�x��]
POINT	RotateRight90(POINT iBase, POINT iPoint);
// iBase�����_�Ƃ��āA������90�x��]
POINT	RotateLeft90(POINT iBase, POINT iPoint);
// iBase�����_�Ƃ��āA180�x��]
POINT	Rotate180(POINT iBase, POINT iPoint);

//----------------------------------------------------------
// �����`

// ��`�̍��W�t�]���C������B�I�_����l���B
RECT NormalizeRect( RECT rc );
// �Q�_�����`���쐬����B�t�]�ƏI�_������l���B
RECT PointToRect( POINT pt1, POINT pt2 );
// ��`�̈�_���擾�B�I�_����l���B
inline POINT LeftTop( RECT rc ) { return MAKEPOINT(rc.left,rc.top); }
inline POINT RightTop( RECT rc ) { return MAKEPOINT(rc.right-1,rc.top); }
inline POINT LeftBottom( RECT rc ) { return MAKEPOINT(rc.left,rc.bottom-1); }
inline POINT RightBottom( RECT rc ) { return MAKEPOINT(rc.right-1,rc.bottom-1); }
// �_����`��ɂ��邩�ǂ����B�I�_����l���B
inline BOOL isOnRect( RECT rc, POINT pt ) { return ( pt.x >= rc.left && pt.x < rc.right && pt.y >= rc.top && pt.y < rc.bottom ); }
// ��`���ړ�
inline RECT& MoveRect( RECT& rc, POINT pt ) { rc.left+=pt.x; rc.top+=pt.y;  rc.right+=pt.x; rc.bottom+=pt.y; return rc; }

// operators
inline RECT& operator *= ( RECT& rc, int i ) { rc.left*=i; rc.top*=i; rc.right*=i; rc.bottom*=i; return rc; }
inline RECT& operator /= ( RECT& rc, int i ) { rc.left/=i; rc.top/=i; rc.right/=i; rc.bottom/=i; return rc; }

inline RECT MAKERECT( int l, int t, int r, int b ) { RECT rc={l,t,r,b}; return rc; }
inline RECT MAKERECT( POINT pt1, POINT pt2 ) { RECT rc={pt1.x,pt1.y,pt2.x,pt2.y}; return rc; }
inline RECT MAKERECT( POINT pt, SIZE sz ) { RECT rc={ pt.x, pt.y, pt.x+sz.cx, pt.y+sz.cy }; return rc; }
inline SIZE MAKESIZE( int cx, int cy ) { SIZE size={ cx, cy }; return size; }
inline SIZE RECTSIZE( RECT rc ) { SIZE size={ rc.right - rc.left, rc.bottom - rc.top }; return size; }
inline RECT SIZERECT( SIZE siz ) { RECT rc={ 0, 0, siz.cx, siz.cy }; return rc; }

// �����`���ړ�
inline void Move( RECT* rc, int x, int y ) { assert(rc!=NULL); rc->left+=x; rc->top+=y; rc->right+=x; rc->bottom+=y; }
// �ړ���̒����`���擾
inline RECT Move( RECT rc, int x, int y ) { return MAKERECT( rc.left+x, rc.top+y, rc.right+x, rc.bottom+y ); }
// �����`�̕��E�����𓾂�
inline int Width(const RECT& rc) { return rc.right - rc.left; }
inline int Height(const RECT& rc) { return rc.bottom - rc.top; }
// �_�������`�̒��ɂ��邩
inline BOOL isOn( const RECT& rect, int x, int y ) { return (x >= rect.left && x < rect.right &&	y >= rect.top && y < rect.bottom ); }
// �����`�͋�i�傫���������Ă��Ȃ��j��
inline BOOL isEmpty( const RECT& rc ) { return ( rc.right <= rc.left || rc.bottom <= rc.top ); }
// rcChild��rcParent�Ɏ��܂�悤����
inline void Clip( const RECT& rcParent, RECT* rcChild ) {
	assert( rcChild != NULL );
	if ( rcChild->left < rcParent.left ) rcChild->left = rcParent.left;
	if ( rcChild->top < rcParent.top ) rcChild->top = rcParent.top;
	if ( rcChild->right > rcParent.right ) rcChild->right = rcParent.right;
	if ( rcChild->bottom > rcParent.bottom ) rcChild->bottom = rcParent.bottom;
}

#define	OpenRect(rc)	rc.left, rc.top, rc.right, rc.bottom
#define	OpenPoint(pt)	pt.x, pt.y

//----------------------------------------------------------
// �E�B���h�E����

// �N���C�A���g�̈�̃T�C�Y��ύX����B
// ����ɁA�E�B���h�E�ʒu����ʓ��Ɏ��܂�悤��������B
BOOL	ResizeClientRect( HWND hWnd, int w, int h );
inline	BOOL	ResizeClientRect( HWND hWnd, SIZE siz ) { return ResizeClientRect(hWnd, siz.cx, siz.cy ); }

// �N���C�A���g�����`���擾
inline	RECT	GetClientRect(HWND iWnd) { RECT rc; ::GetClientRect(iWnd, &rc); return rc; }
// �N���C�A���g�����`���X�N���[�����W�ŕԂ��B
RECT	GetClientRectOnScreen( HWND hWnd );
// ���W�ϊ� for RECT
RECT	ClientToScreen(HWND iWnd, RECT iRect);
RECT	ScreenToClient(HWND iWnd, RECT iRect);
// �E�B���h�E�ʒu���W���擾
RECT	GetWindowRect( HWND hWnd );
POINT	GetWindowPoint( HWND hWnd );
// �E�B���h�E�̑傫�����擾
inline SIZE	GetSize(HWND iWnd) { RECT rc=GetWindowRect(iWnd); SIZE siz={Width(rc), Height(rc)}; return siz; }


//----------------------------------------------------------
// ����

// DWORD�l�������b�~���b�ɕ���
void	DwordToSystemTime( DWORD dw, SYSTEMTIME* pst );
// SYSTEMTIME�\���̂�DWORD�l�ɕϊ��i������49���܂Łj
void	SystemTimeToDword( const SYSTEMTIME*pst, DWORD pdw );

// SYSTEMTIME�\���̓��m���r�B�V�������t���u�傫���v�Ƃ���B
bool operator < ( const SYSTEMTIME& lhs, const SYSTEMTIME& rhs );
bool operator > ( const SYSTEMTIME& lhs, const SYSTEMTIME& rhs );
bool operator <= ( const SYSTEMTIME& lhs, const SYSTEMTIME& rhs );
bool operator >= ( const SYSTEMTIME& lhs, const SYSTEMTIME& rhs );
bool operator == ( const SYSTEMTIME& lhs, const SYSTEMTIME& rhs );
bool operator != ( const SYSTEMTIME& lhs, const SYSTEMTIME& rhs );

//----------------------------------------------------------
// ��

// �E�B���h�E���b�Z�[�W��Output�ɕ\��
void	PutWindowMessage( UINT message, WPARAM wParam, LPARAM lParam );

// �ψ����Ή�TextOut
void	TextOutF( HDC hDC, int xPos, int yPos, char* format, ... );

// �ψ����Ή���OutputDebugString
void	DbgStr( const char* format, ... );
// �ψ����Ή��̊ȈՕ񍐗p���b�Z�[�W�{�b�N�X
void	MesBox( const char* format, ... );
// �ψ����Ή��̊ȈՕ񍐗pSetWindowText
void	SetWinText( HWND hWnd, const char* format, ... );

#endif	// WIN32_H
