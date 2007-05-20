#include	"Task.h"

/*
	�t���[�̊T�O�F
		�d���i����j�̒P��
	�t���[�̎����F
		�^�X�N�c���[�Ɠ����B

	�t���[�ԍ��iID)�ƃ��[�g�^�X�N�͂P�΂P�őΉ�����B

	FlowSystem�̓t���[�̐؂�ւ����Ǘ�����B
	
	���̃N���X���A�ÓI�Ɏ����|�C���^�������� new �Ŋm�ۂ���ق���
	�^�C�~���O�Ǘ���D�܂����Ǝv���܂��B�Ō�Ɏ����Ă�task�̓f�X�g���N�^��delete����̂ŁB

2001/07/14 m
	�폜�i�^�C�~���O�Ǘ���̓s���j�A�t���[�ԍ���`��ǉ��B

2001/07/14 a
	�V�X�e���ɗB��ł��闝�R�������̂ł��̂��߂̃R�[�h���폜�B
	�������E����̓R���X�g���N�^�E�f�X�g���N�^�ōs���`�ɁB
	�t���[�ԍ��Ɍ^�����������A�󂯓n���͂��̌^�ōs���悤�ɁB
	�R���X�g���N�^�̈����Ƃ��āA���񎞂�ID���w�肷��悤�ɁB

2001/07/14,2 a
	FlowID�̓N���X���Ƃ������O��ԂƏd������̂�ID�ɕύX�B
	�g�����肪���������̂�FlowSystem::I�ɂ��A�N�Z�X�𕜋A�B
	�ێ��t���[����^�C�~���O�Ǘ��p��release()�𕜋A�B
	�f�X�g���N�^�͗L���������B

2001/07/14,3 a
	�R���X�g���N�^�����ύX�B
	�Ӗ��I�ȕω�����release��initialize�ɖ��O�ύX�B
	EMPTYTASK���f�t�H���g�l�ɍ̗p�A���Ӗ�������DUMMY���폜�B
*/


class FlowSystem {
public:
	enum ID {
		EMPTYTASK,
		FLOWSELECT,
		TESTRENDER = 29,
	};

	// �t���[�؂�ւ����w���B����run���ɔ��f�����B�����run�O�ɕK�����s�̂��ƁB
	void	requestChange(ID iID);
	// ���ݑI�𒆂̃t���[�����s����B
	// ID�ɂ��w������Ă���t���[�������ĂȂ���΂����ō쐬(new)�A�ێ�����B
	void	run();
	// �t���[��ێ����Ă���Ή�����A������Ԃɖ߂�B
	void	initialize();

private:
	Task*	mTask;	// ���݂̃t���[�ɑΉ��������[�g�^�X�N
	ID	mID;	// ���݂̃t���[��ID
	ID	mRequestedID;	// ���s���v������Ă���ID

	void	remake();	// ���݂̃t���[��delete,RequestID�ɉ������V�����t���[��new�B

// �ȉ��͂��̃N���X�̑��݂��V�X�e���ŗB��Ƃ��邽�߂̃R�[�h
private:
	static FlowSystem	mInstance;
	FlowSystem();
public:
	static FlowSystem&	I;
	~FlowSystem();
};

