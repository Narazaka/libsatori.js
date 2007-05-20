#ifndef	PARAMETER_H
#define	PARAMETER_H


/*-----------------------------------------------
	�p�����[�^�t�@�C���A�N�Z�T Parameter
	
�p�����[�^�t�@�C���̏����F
�E�f���~�^��space, comma, tab�B
�E��s�͖���
�E��P��̍s�̓Z�N�V������
�Ecolon�ȍ~�s���܂ŃR�����g

��F

����
����		100, 100
��������	64,80
�ς��[��	0,0,640, 480	; ��������͑傫�߂Ȃ́B
�Ȃ���		2

����
����		100, 100
��������	64,80
�ς��[��	0,0,640, 480
�Ȃ���		2

-----------------------------------------------*/

#pragma warning( disable : 4786 ) // �u�f�o�b�O�����ł̎��ʎq�؎̂āv
#pragma warning( disable : 4503 ) //�u�������ꂽ���O�̒��������E���z���܂����B���O�͐؂�̂Ă��܂��B�v
#include	<map>
#include	<string>
using namespace std;

#include	"../Canvas/Shape.h"
#include	"../Canvas/Color24.h"

typedef	map<string, string>	strmap;

class	Parameter {

	map<string, strmap>	mData;
	strmap*				mSection;

	// ��s�ɑ΂��ď����BloadFile�̕⏕�֐��B
	void	processLine(char* iLine);

public:
	Parameter();
	~Parameter();
	void	clear();

	// �t�@�C���ǂ��
	bool	loadFile(const char* iFileName);
	// �Z�N�V�����I���
	bool	selectSection(const char* iSection);
	// �l���擾
	bool	getValue(const char* iKey, int& oInt);
	bool	getValue(const char* iKey, string& oString);
	bool	getValue(const char* iKey, CRect& oRect);
	bool	getValue(const char* iKey, CSize& oSize);
	bool	getValue(const char* iKey, CPoint& oPoint);
	bool	getValue(const char* iKey, Color24& oColor);
};




#endif	//	PARAMETER_H
