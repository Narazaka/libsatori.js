#include	"TextPositionManager.h"
#include	<assert.h>
#include	<mbctype.h>	// for _ismbblead,_ismbbtrail
#include	<string.h>	// for NULL

TextPositionManager::TextPositionManager() {
}

TextPositionManager::~TextPositionManager() {
}

void	TextPositionManager::BasePoint(int iX, int iY) {
	mBasePoint = CPoint(iX, iY);
}

void	TextPositionManager::Size(int iWidth, int iHeight) {
	mSize = CSize(iWidth, iHeight);
}

void	TextPositionManager::Format(int iWidth, int iHeight) {
	mFormat = CSize(iWidth, iHeight);
}

void	TextPositionManager::Return() {
	mPosition.x = 0;
	mPosition.y++;
}

void	TextPositionManager::Home() {
	mPosition.x = 0;
	mPosition.y = 0;
}


// �s���Ƌ֑����l�����āA���s�����B
void	TextPositionManager::Before(const char* p) {
	int	len = _ismbblead(*p) ? 2 : 1;

	const char kinsoku[] = "�I�H�B�A";
	if ( len == 2 )
		for ( const char* pk=kinsoku ; *pk!='\0' ; pk+=2 )
			if ( strncmp(p, pk, 2) == 0 )
				return;

	if ( mPosition.x >= mFormat.w-2 )	// -2 �֑͋��\�蕔��
		Return();
}

// ���̕�����\�����ׂ��ʒu��Ԃ��B
// �\�����Ɏg���t�H���g�i��L�����j�̑傫���������Ƃ��ēn���B
CPoint	TextPositionManager::Position(CSize iFontSize) {

	assert(mFormat.w > 0);
	assert(mFormat.h > 0);

	// �ꕶ���̊��蓖�ė̈�
	double oneW = mSize.w / (double)mFormat.w;
	double oneH = mSize.h / (double)mFormat.h;

	//             ��_�@�@�@�@�@�\���͈͂̒����_�@�@�@�@�t�H���g�T�C�Y
	int	x = int(mBasePoint.x + (mPosition.x+1)*oneW - iFontSize.w);
	int	y = int(mBasePoint.y + (mPosition.y+0.5)*oneH - iFontSize.h/2.0);

	return	CPoint(x, y);
}

// �����ʒu���C���N�������g�B
int		TextPositionManager::After(const char* p) {
	int	len = _ismbblead(*p) ? 2 : 1;
	mPosition.x += len;
	return	len;
}

