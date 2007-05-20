#include	"FlowSystem.h"
#include	"Task.h"
#include	<assert.h>
#include	<windows.h>	// for OutputDebugString

FlowSystem&	FlowSystem::I=mInstance;
FlowSystem	FlowSystem::mInstance;

FlowSystem::FlowSystem() :
	mTask(NULL),
	mID(EMPTYTASK),	
	mRequestedID(EMPTYTASK)
{
}

FlowSystem::~FlowSystem() {
	initialize();
}

void	FlowSystem::initialize() {
	mID=EMPTYTASK;
	mRequestedID=EMPTYTASK;
	delete mTask;
	mTask=NULL;
}

void	FlowSystem::requestChange(ID iID) {
	mRequestedID = iID;
}

void	FlowSystem::run() {

	// �v������Ă���t���[ID�����݂�ID�ƈقȂ�΁A
	// ���݂̃t���[�i�ɑΉ��������[�g�^�X�N�A�Ђ��Ă̓^�X�N�c���[�j����蒼��
	if ( mID != mRequestedID )
		remake();

	// �������s���Ȃ瓊���锤�Ȃ̂�NULL�ɂȂ�͉̂��炩�̃o�O
	assert(mTask != NULL);	

	mTask->run();
}

#if 0
#include	"FlowSelect.h"
#include	"testrender.h"
void	FlowSystem::remake() {

	mID = mRequestedID;
	delete mTask;

	switch ( mID ) {

	// �����Ńt���[�̈ꗗ���`�B
	case EMPTYTASK: mTask=new EmptyTask(); break;

	case FLOWSELECT: mTask=new FlowSelect(); break;

	case TESTRENDER: mTask=new TestRender(); break;

	default:
		::OutputDebugString("\n�t���[ID�ɑΉ����鏈������`����Ă��܂���B\n");
		break;
	}
}
#endif
