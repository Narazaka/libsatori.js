
#include	"Selector.h"

#include <string>
#include <algorithm>
#include <map>
using namespace std;

typedef string Condition;

class Evalcator
{
public:
	virtual bool evalcate_to_bool(const Condition&) =0;

	virtual ~Evalcator(void) { }
};



// �������O������Q�B
template<typename T>
class Family
{
	strvec NameVector;
	bool ComNameFound;

	// ���������L�[�A�v�f���X�g��l�Ƃ���map
	typedef vector<T> Elements;
	typedef map<Condition, Elements> CondsMap;
	CondsMap m_conds_map;

#if 0
	// map���Ə��������܂ޏꍇ�ɏ��Ԃ����ʂ킯��

	struct Condition
	{
		string cond_str;
		enum { NOT_EVAL, TRUE, FALSE } eval_result;
		int ref_coutner;
	};
	//�݂����Ȃ̂�ʂɎ�����

	class Conditions : set<Condition> {
		iterator ref(string name);
		void unref(iterator);
	};

	class Elements
	{
		T m_t;
		Conditions::iterator m_condition;
	public:
		Elements(Conditions i_conditions) {
			m_condition = i_conditions.ref();
		}
		~Elements() {
			m_condition.unref(m_condition)
		}

		bool eval(Evalcator)

	};
/*
	vector<Elements> m_elements;
	�Ƃ����Ȃ�
	- - - - - - - - - - - - - - - - - - 
	Family���u�������v�������Ă�̂����������̊ԈႢ����ˁB
	�������ւ̃|�C���^���������āA���������͓̂Ɨ����������������B
	�u����̏������͈�񂾂��]�������v��O��Ƃ���Ȃ�΁B
	���x���l���Ȃ��Ȃ�S�v�f�����]�����Ȃ����̂��������C�����邪�c�c
	- - - - - - - - - - - - - - - - - - 
  �@���������O���Ɏ����Ă����ׂ����B
�@�@�K�v�ɉ����ĕ]�����A���ʂ��L���b�V�����A�C�Ӄ^�C�~���O�ŕ]�������Z�b�g����B
  �@Evalcator�ɕێ�������Ƃ��B
*/

#endif



	// ���[�U��`�̏d�����ݒ��K�p���邽�߂́u�I���W�v�B
	// Word/Talk�ւ̃|�C���^�����Ƃ��ĕێ�����B
	Selector<const T*> m_selector;


public:
	Family() {
		//cout << "Family()" << endl;
		ComNameFound = false;
	}
	//~Family() { cout << "~Family()" << endl; }


	template<typename C>
	void get_elements_pointers(C& o_c) const
	{
		for ( typename CondsMap::const_iterator i = m_conds_map.begin() ; i != m_conds_map.end() ; ++i )
		{
			for ( typename Elements::const_iterator j = i->second.begin() ; j != i->second.end() ; ++j )
			{
				o_c.push_back(&(*j));
			}
		}
	}

	void set_namevec(const std::string &name)
	{
		split(name,"\t�@",NameVector);
		
		for ( strvec::iterator wds_it=NameVector.begin() ; wds_it!=NameVector.end() ; ++wds_it ) {
			if ( compare_tail(*wds_it, "�u") ) {
				if ( wds_it != NameVector.begin() ) {
					string comName = *wds_it;
					NameVector.erase(wds_it);
					NameVector.insert(NameVector.begin(),comName);
				}
				ComNameFound = true;
				break;
			}
		}
	}
	const strvec& get_namevec(void) const
	{
		return NameVector;
	}
	string get_comname(void) const
	{
		if ( ! ComNameFound || NameVector.empty() ) { return ""; }
		return NameVector[0];
	}
	bool is_comname(void) const
	{
		return ComNameFound && !NameVector.empty();
	}

	bool empty() const { return m_conds_map.empty(); }
	int size_of_element() const 
	{
		int r = 0;
		for ( typename CondsMap::const_iterator i = m_conds_map.begin() ; i != m_conds_map.end() ; ++i )
		{
			r += i->second.size();
		}
		return r;
	}
	bool is_exist_element(const T& i_t) const
	{
		for ( typename CondsMap::const_iterator i = m_conds_map.begin() ; i != m_conds_map.end() ; ++i )
			for ( typename Elements::const_iterator j = i->second.begin() ; j != i->second.end() ; ++j )
				if ( *j == i_t )
					return true;
		return false;
	}

	// �o�^
	const T& add_element(const T& i_t, const Condition& i_condition = Condition())
	{
		Elements& lt = m_conds_map[i_condition];
		lt.push_back(i_t);
		return lt.back();
	}
	// �폜
	void delete_element(const T& i_t, const Condition& i_condition = Condition())
	{
		CondsMap::iterator it = m_conds_map.find(i_condition);
		if ( it == m_conds_map.end() ) { return; }
		Elements& lt = it->second;
		lt.erase(remove(lt.begin(),lt.end(),i_t),lt.end());
		if ( lt.empty() ) {
			m_conds_map.erase(it);
		}
	}
	
	// �d�������ݒ�
	void set_OC(OverlapController<const T*>* i_oc)
	{
		m_selector.attach_OC(i_oc);
	}
	// �d������󋵂��N���A
	void clear_OC()
	{
		m_selector.clear_OC();
	}

	// ��������]�����Č������ׂđI�ԁB
	// �����́u�]���ҁv�ƌ���vector�B
	template <typename Candidates> select_all(Evalcator& i_evalcator,Candidates &candidates)
	{
		assert(m_conds_map.size() > 0);
		if ( m_conds_map.size() == 1 && m_conds_map.begin()->first.empty() ) {
			// �������̌Q�݂̂����݂���i�����������O�Ɠ�����ԁj
			Elements& e = m_conds_map.begin()->second;
			assert(e.size() > 0);

			for ( typename vector<T>::const_iterator j = e.begin() ; j != e.end() ; ++j ) {
				candidates.push_back( &(*j) );
			}
		}
		else {
			//  ����I��
			cout << "selecting" << endl;
			for ( typename CondsMap::const_iterator i = m_conds_map.begin() ; i != m_conds_map.end() ; ++i )
			{
				// �u�������v�ł��邩�u��������]���������ʁA0/�O��Ԃ��Ȃ��������́v���̗p
				if ( i->first.empty() || i_evalcator.evalcate_to_bool(i->first) )
				{
					for ( typename vector<T>::const_iterator j = i->second.begin() ; j != i->second.end() ; ++j )
					{
						candidates.push_back( &(*j) );
						cout << "[" << *j << "]" << endl;
					}
				}
			}
		}
	}

	// �I�񂾌�₩�炳��Ƀ��[�U��`�̏d�����ݒ��K�p�����I�����s���B
	// �����́u�]���ҁv�B
	const T* select(Evalcator& i_evalcator)
	{
		list<const T*> candidates;
		select_all(i_evalcator,candidates);

		if ( candidates.empty() ) {
			return NULL;
		}
		else if ( candidates.size() == 1 ) {
			return *candidates.begin();
		}
		else {
			m_selector.update_candidates(candidates);
			return	m_selector.select();
		}
	}
};
