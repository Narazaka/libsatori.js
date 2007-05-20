#include	"Task.h"
#include	<assert.h>

Task::Task() : 
	parent(NULL), 
	next(NULL), 
	prev(NULL), 
	top_child(NULL), 
	bottom_child(NULL), 
	child_num(0) {
}

Task::~Task() { 
	remove(); 
	while ( top_child != NULL )
		delete top_child;
}

void	Task::run() {
	this->handle();
	Task	*now=top_child, *next;
	while ( now != NULL ) {
		next = now->next;
		now->run();
		now = next;
	}
}

//---------------------------------------------------------------------------------
// �ȉ��͎��\�������B

void	Task::remove() {

	// �����̎q���ɂ͐G��Ȃ��B

	// �e������ΐ؂藣��
	if ( parent ) {
		// �e�̐擪�q���������ł���΁A���̎q���ɂ䂾�˂�B
		// �������B��̎q���������ꍇ�́A�e�̃|�C���^�� NULL �ɂȂ�B
		if ( parent->top_child == this )
			parent->top_child = next;
		// ���������l�ɁB
		if ( parent->bottom_child == this )
			parent->bottom_child = prev;
		--(parent->child_num);

		parent = NULL;
	}

	// �O�オ����΋l�߂��킹��
	if ( next )
		next->prev = prev;
	if ( prev )
		prev->next = next;
	next = prev = NULL;	// NULL�ɂ���͍̂Ō�ˁB
}

void	Task::addChildTop(Task* p) {
	assert( p != NULL );

	// �V����̐e�Z���؂藣���A������e�Ƃ���
	p->remove();
	p->parent = this;
	
	if ( top_child == NULL ) 
		top_child = bottom_child = p;	// ���q��
	else {
		assert( top_child->prev == NULL );

		// ���łɎq��������
		// �����擪�̎�O�ɒǉ����A�V�q����擪�ɁB
		top_child->prev = p;
		p->next = top_child;
		top_child = p;
	} 
	++child_num;
}

void	Task::addChildBottom(Task* p) {
	assert( p != NULL );

	// �V����̐e�Z���؂藣���A������e�Ƃ���
	p->remove();
	p->parent = this;
	
	if ( bottom_child == NULL ) 
		bottom_child = top_child = p;	// ���q��
	else {
		assert( bottom_child->next == NULL );

		// ���łɎq��������
		// ���������̎�O�ɒǉ����A�V�q���𖖔��ɁB
		bottom_child->next = p;
		p->prev = bottom_child;
		bottom_child = p;
	} 
	++child_num;
}

void	Task::insertNext(Task* p) {
	assert( p != NULL );
	assert( parent != NULL );	// �Z���������ɂ͂܂��e���v��

	// �q���ȊO��؂藣��
	p->remove();
	// �e�͋��ʉ�
	p->parent = parent;
	++(parent->child_num);
	// ���������킾�����ꍇ�Ap�ɗ��������
	if ( parent->bottom_child == this )
		parent->bottom_child = p;

	// �O��֌W
	p->next = next;
	p->prev = this;
	if ( next )
		next->prev = p;
	next = p;

}

void	Task::insertPrev(Task* p) {
	assert( p != NULL );
	assert( parent != NULL );	// �Z���������ɂ͂܂��e���v��

	// �q���ȊO��؂藣��
	p->remove();
	// �e�͋��ʉ�
	p->parent = parent;
	++(parent->child_num);
	// ���������Z�������ꍇ�Ap�ɗ��������
	if ( parent->top_child == this )
		parent->top_child = p;

	// �O��֌W
	p->prev = prev;
	p->next = this;
	if ( prev )
		prev->next = p;
	prev = p;
}

