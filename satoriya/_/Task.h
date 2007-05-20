#ifndef	TASK_H
#define	TASK_H

#define	NULL	0

class Task {

	Task	*parent, *next, *prev, *top_child, *bottom_child;
	int		child_num;

	virtual void	handle() {}	

public:
	void	insertNext(Task*);
	void	insertPrev(Task*);
	void	addChildTop(Task*);
	void	addChildBottom(Task*);
	Task*	getParent(Task*) { return parent; }
	Task*	getNext() { return next; }
	Task*	getPrev() { return prev; }
	Task*	getChildTop() { return top_child; }
	Task*	getChildBottom() { return bottom_child; }
	int		getChildNum() const { return child_num; }
	void	remove();	// �e�Z���؂藣���B�q���ɂ͉e���Ȃ��B

	Task();
	virtual ~Task();
	void	run();
};

// �R�s�y�p�E�p���T���v��
class EmptyTask : public Task {
	virtual void	handle() {}
public:
	EmptyTask() : Task() {}
	virtual ~EmptyTask() {}
};

#endif	//	TASK_H




#if 0	//----------------------------------------------------------------------

// �R�s�y�p�E�p���T���v��������

// �錾��
class EmptyTask : public Task {
	virtual void	handle();
public:
	EmptyTask();
	virtual ~EmptyTask();
};

// ������
EmptyTask::EmptyTask() : 
Task() {
}

EmptyTask::~EmptyTask() {
}

void	EmptyTask::handle() {
}

#endif
