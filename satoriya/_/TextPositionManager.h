#ifndef	TEXTPOSITIONMANAGER_H
#define	TEXTPOSITIONMANAGER_H

#include	"../Canvas/Shape.h"

class TextPositionManager {

	CPoint	mBasePoint;	// �`��͈͂̉E����W
	CSize	mSize;		// �`��͈͂̍L��
	CSize	mFormat;	// �`��\�������i�͈͂ɑ΂��Ă̕���j

	CPoint	mPosition;	// ���݂̕`��ʒu

public:
	TextPositionManager();
	~TextPositionManager();

	// �\�����ݒ�
	void	BasePoint(int iX, int iY);
	void	Size(int iWidth, int iHeight);
	void	Area(int iL, int iT, int iR, int iB) { BasePoint(iL,iT); Size(iR-iL, iB-iT); }
	void	Margin(CRect iOut, int iL, int iT, int iR, int iB) { Area(iOut.l+iL, iOut.t+iT, iOut.r-iR, iOut.b-iB); }
	// �\��������ݒ�
	void	Format(int iWidth, int iHeight);

	// �ʒu�����_�ɖ߂�
	void	Home();		
	// �ʒu�����s����
	void	Return();	
	// ���_�ɂ���H
	bool	isHome() { return mPosition.x==0 && mPosition.y==0; }

	// �s���Ƌ֑����l�����āA���s�����B
	void	Before(const char* p);
	// ���̕�����\�����ׂ��ʒu��Ԃ��B
	// �\�����Ɏg���t�H���g�i��L�����j�̑傫���������Ƃ��ēn���B
	CPoint	Position(CSize iFontSize);
	// �����ʒu���C���N�������g�B
	int		After(const char* p);
};

#endif	//	TEXTPOSITIONMANAGER_H
