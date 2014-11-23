
#include <list>
#include <set>
#include "random.h"

// �e���\�b�h�ɂ��A�u��₩��̑I���v�N���X�Q
// ���T��ID��|�C���^���A���j�[�N������\�łȂ���΂Ȃ�Ȃ��B


// �����l
#define INVALID_VALUE NULL
//#define INVALID_VALUE -1



// ----------------------------------------------------------------------
// �e���\�b�h�̒��ۊ��N���X
template<typename T>
class OverlapController
{
public:
	OverlapController() {
		//
	}
	virtual ~OverlapController() {
		//
	}

	// ��₩����I��
	// ���͈�ȏ゠�邱�Ƃ��ۏ؂���Ă���B
	virtual T select(const list<T>&) =0;

	virtual int type(void) = 0;


	// �C�x���g�ʒm�n���h��

	// ��₪�ǉ����ꂽ
	virtual void on_add(const list<T>& i_candidates, typename list<T>::const_iterator i_it) {}
	// ��₪��������悤�Ƃ��Ă���
	virtual void on_erase(const list<T>& i_candidates, typename list<T>::const_iterator i_it) {}
	// �d������󋵂�������
	virtual void on_clear() {}
};

// ----------------------------------------------------------------------
// ���S�����_��
template<typename T>
class OC_Random : public OverlapController<T>
{
public:
	//�R���g���[���^�C�v�F�����炵���̂�ǉ�����Ƃ��͕K����ID�ɂ��邱�ƁItypeid�̒x�������΍�B
	virtual int type(void) {
		return 100;
	}

	// ��₩����I��
	virtual T select(const list<T>& i_candidates)
	{
		typename list<T>::const_iterator it = i_candidates.begin();
		advance( it, random(i_candidates.size()) );
		return *it;
	}
};

// ----------------------------------------------------------------------
// �S�Ďg���؂�܂ŏd�����
template<typename T>
class OC_NonOverlap : public OverlapController<T>
{
	set<T> m_used;
	set<T> m_unused;
	T m_last;

public:
	OC_NonOverlap() : m_last(INVALID_VALUE) {}

	//�R���g���[���^�C�v�F�����炵���̂�ǉ�����Ƃ��͕K����ID�ɂ��邱�ƁItypeid�̒x�������΍�B
	virtual int type(void) {
		return 200;
	}

	// ��₩����I��
	virtual T select(const list<T>&)
	{
		for ( ; ; ) {
			// �u���g�p�v������ۂȂ�u�g�p�ς݁v��S�āu���g�p�v�ɂ���B
			if ( m_unused.empty() )
			{
				m_used.swap( m_unused );
			}

			// �u���g�p�v���烉���_���Ɉ��I�яo��
			typename set<T>::iterator it = m_unused.begin();
			advance( it, random(m_unused.size()) );

			// �I�񂾈���u���g�p�v����u�g�p�ς݁v�Ɉڂ�
			T t = *it;
			m_unused.erase(it);
			m_used.insert(t);

			if ( m_last != INVALID_VALUE && ((m_unused.size()+m_used.size()) >= 2) ) {
				if ( m_last == t ) {
					continue;
				}
			}
			m_last = t;
			return t;
		}
	}

	// ��₪�ǉ����ꂽ
	virtual void on_add(const list<T>&, typename list<T>::const_iterator i_it)
	{
		m_unused.insert(*i_it);
	}

	// ��₪��������悤�Ƃ��Ă���
	virtual void on_erase(const list<T>&, typename list<T>::const_iterator i_it)
	{
		// ���݂ǂ����ɂ��邩�킩��Ȃ��̂ŁA�����Ɏw�����o��
		m_unused.erase(*i_it);
		m_used.erase(*i_it);
		m_last = INVALID_VALUE;
	}
	
	// �d������󋵂�������
	virtual void on_clear() 
	{
		for ( typename set<T>::const_iterator it = m_used.begin() ; it != m_used.end() ; ++it )
		{
			m_unused.insert(*it);
		}
		m_used.clear();
		m_last = INVALID_VALUE;
	}
};

// ----------------------------------------------------------------------
// ���O�Ƃ̏d�������͉��
template<typename T>
class OC_NonDual : public OverlapController<T>
{
	T m_last;
public:
	OC_NonDual() : m_last(INVALID_VALUE) {}

	//�R���g���[���^�C�v�F�����炵���̂�ǉ�����Ƃ��͕K����ID�ɂ��邱�ƁItypeid�̒x�������΍�B
	virtual int type(void) {
		return 300;
	}

	// ��₩����I��
	virtual T select(const list<T>& i_candidates)
	{
		// ����������Ȃ����̂��悤���Ȃ�
		if ( i_candidates.size() == 1 )
		{
			return *(i_candidates.begin());
		}
		
		// �����_���Ɉ�I��
		typename list<T>::const_iterator it = i_candidates.begin();
		advance( it, random(i_candidates.size()) );
		
		if ( m_last != INVALID_VALUE )
		{
			// ���O������΁A���O�����͔�����B
			if ( m_last == *it )
			{
				++it;
				if ( it == i_candidates.end() )
				{
					it = i_candidates.begin();
				}
			}
		}

		return (m_last = *it);
	}

	// ��₪��������悤�Ƃ��Ă���
	virtual void on_erase(const list<T>& i_candidates, typename list<T>::const_iterator i_it)
	{
		if ( m_last == *i_it ) 
			m_last = INVALID_VALUE;
	}
	
	// �d������󋵂�������
	virtual void on_clear() 
	{
		m_last = INVALID_VALUE;
	}
};


// ----------------------------------------------------------------------
// �~��
template<typename T>
class OC_Sequential : public OverlapController<T>
{
	T m_last;
public:
	OC_Sequential() : m_last(INVALID_VALUE) {}

	//�R���g���[���^�C�v�F�����炵���̂�ǉ�����Ƃ��͕K����ID�ɂ��邱�ƁItypeid�̒x�������΍�B
	virtual int type(void) {
		return 400;
	}

	// ��₩����I��
	virtual T select(const list<T>& i_candidates)
	{
		typename list<T>::const_iterator it = i_candidates.begin();

		if ( m_last != INVALID_VALUE )
		{
			while ( m_last != *it )
			{
				++it;
				if ( it == i_candidates.end() ) { break; }
			}
			// assert( m_last == *it );

			// ���O�̂��̂��P�����i�߂� ���O���������Ō�܂ł�������ŏ�����
			if ( it == i_candidates.end() )
			{
				it = i_candidates.begin();
			}
			else if ( ++it == i_candidates.end() )
			{
				it = i_candidates.begin();
			}
		}
		return (m_last = *it);
	}

	// ��₪��������悤�Ƃ��Ă���
	virtual void on_erase(const list<T>& i_candidates, typename list<T>::const_iterator& i_it)
	{
		if ( m_last == *i_it ) 
		{
			if ( i_it == i_candidates.begin() )
			{
				i_it = i_candidates.end();
			}
			m_last = *(--i_it);
		}
	}
	
	// �d������󋵂�������
	virtual void on_clear() 
	{
		m_last = INVALID_VALUE;
	}
};

// ----------------------------------------------------------------------
// ����
template<typename T>
class OC_SequentialDesc : public OverlapController<T>
{
	T m_last;
public:
	OC_SequentialDesc() : m_last(INVALID_VALUE) {}

	//�R���g���[���^�C�v�F�����炵���̂�ǉ�����Ƃ��͕K����ID�ɂ��邱�ƁItypeid�̒x�������΍�B
	virtual int type(void) {
		return 500;
	}

	// ��₩����I��
	virtual T select(const list<T>& i_candidates)
	{
		typename list<T>::const_reverse_iterator it = i_candidates.rbegin();
		if ( m_last != INVALID_VALUE )
		{
			while ( m_last != *it )
			{
				++it;
				if ( it == i_candidates.rend() ) { break; }
			}
			//assert( m_last == *it );

			// ���O�̂��̂��P�����i�߂� ���O���������Ō�܂ł�������ŏ�����
			if ( it == i_candidates.rend() )
			{
				it = i_candidates.rbegin();
			}
			else if ( ++it == i_candidates.rend() )
			{
				it = i_candidates.rbegin();
			}
		}
		return (m_last = *it);
	}

	// ��₪��������悤�Ƃ��Ă���
	virtual void on_erase(const list<T>& i_candidates, typename list<T>::const_iterator& i_it)
	{
		if ( m_last == *i_it ) 
		{
			++i_it;
			if ( i_it == i_candidates.end() )
			{
				i_it = i_candidates.begin();
			}
			m_last = *i_it;
		}
	}
	
	// �d������󋵂�������
	virtual void on_clear() 
	{
		m_last = INVALID_VALUE;
	}
};


#undef INVALID_VALUE

