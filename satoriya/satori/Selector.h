
#include <list>
#include <exception>
#include <stdexcept>
using namespace std;

#include "OverlapController.h"

// �I���W

//  �I����@��set_OC�A���ƂȂ�T��update�Őݒ肷��B
//  select�Őݒ肳�ꂽT�Q����P��I�����ĕԂ��B

template<typename T>
class Selector
{
	list<T> m_candidates; // �I���̑ΏۂƂȂ���
	OverlapController<T>* m_OC; // �I�����\�b�h

public:

	Selector() :
		m_candidates(),
		m_OC(NULL)
	{
		//cout << "Selector(), m_OC:" << m_OC << ", this:" << this << endl;
	}

	Selector(const Selector& other) :
		m_candidates(),
		m_OC(NULL) 
	{
		// �g���Ă�OverlapController�̓R�s�[�ł��Ȃ��B
		assert(other.m_candidates.empty());
		assert(other.m_OC == NULL);
		
		// �܂����C������B
		// ������ candidates �Ƃ�[�V�X�e�������������R�s�[����邱�Ƃ��l���ĂȂ��B
		// �����炱�����V�X�e���͐����ň����Ă����킯�����B���x�������Ȃ��B
	}


	~Selector()
	{
		if ( m_OC != NULL )
		{
			//cout << "~Selector(), m_OC:" << m_OC << ", this:" << this << endl;
			delete m_OC;
			m_OC = NULL;
			//cout << "             m_OC:" << m_OC << ", this:" << this << endl;
		}
	}

	// �I��Ώی����X�V����B
	// i_candidates�͍~���Ƀ\�[�g����Ă���A���A��ł����Ă͂Ȃ�Ȃ��B
	void update_candidates(const list<T>& i_candidates)
	{
		if ( m_OC == NULL )
		{
			//cout << "Selector::update_candidates(), m_OC:" << m_OC << ", this:" << this << endl;
			m_OC = new OC_Random<T>;
			//cout << "                               m_OC:" << m_OC << ", this:" << this << endl;
		}

		#define NOW i_candidates
		#define OLD m_candidates

		typename list<T>::const_iterator now = NOW.begin();
		typename list<T>::iterator old = OLD.begin();

		while (true)
		{
			if ( now == NOW.end() )
			{
				//OLD.erase(old, OLD.end());
				while ( old != OLD.end() )
				{
					m_OC->on_erase(OLD, old);
					old = OLD.erase(old);
				}
				break;
			}
			if ( old == OLD.end() )
			{
				//OLD.insert(old, now, NOW.end());
				for (; now != NOW.end() ; ++now )
				{
					OLD.insert(old, *now);
					m_OC->on_add(OLD, now);
				}
				break;
			}
	
			if ( *now < *old )
			{
				OLD.insert(old, *now);
				m_OC->on_add(OLD, now);
				++now;
			}
			else if ( *now > *old )
			{
				m_OC->on_erase(OLD, old);
				old = OLD.erase(old);
				++old;
			}
			else
			{
				++now;
				++old;
			}
		}
	}
	
	// �I���󋵂��N���A
	void clear_OC()
	{
		//cout << "Selector::clear_OC(), m_OC:" << m_OC << ", this:" << this << endl;
		if ( m_OC != NULL )
		{
			m_OC->on_clear();
		}
	}
	
	// �I����@��ύX�B
	// �����ɂ� new �ō�������̂�n�����ƁB����͂��̃N���X�ōs���B
	void attach_OC(OverlapController<T>* i_OC)
	{
		//cout << "Selector::attach_OC(), m_OC:" << m_OC << ", this:" << this << endl;
		if ( m_OC != NULL )
		{
			delete m_OC;
		}
		m_OC = i_OC;
		//cout << "                       m_OC:" << m_OC << ", this:" << this << endl;
	}
	
	// �I�����s���B��������₪��̎��͎��s���Ă͂����Ȃ��B
	T select()
	{
		if ( m_OC == NULL )
		{
			//cout << "Selector::select(), m_OC:" << m_OC << ", this:" << this << endl;
			m_OC = new OC_Random<T>;
			//cout << "                    m_OC:" << m_OC << ", this:" << this << endl;
		}

		if ( m_candidates.empty() )
		{
			throw runtime_error("select: candidates list is empty!");
		}
		
		return m_OC->select(m_candidates);
	}
};
