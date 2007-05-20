#include "Family.h"
#include "random.h"


// �v�f�͒P��܂��̓g�[�N�B

// �������O�����v�f�̏W����Family�B

// Families�͖��O�ɂ����肳���Family�̏W���B
// �ق� map< string, Family<T> > ����public�p���͂����A�C���^�t�F�[�X�����肷��

template<typename T>
class Families : private map< string, Family<T> >
{
        typedef typename map< string, Family<T> >::iterator iterator;
        typedef typename map< string, Family<T> >::const_iterator const_iterator;
	set<string> m_clearOC_at_talk_end;

public:
	//Families() { cout << "Families()" << endl; }
	//~Families() { cout << "~Families()" << endl; }

	// �v�f�̓o�^
	const T* add_element(const string& i_name, const T& i_t, const Condition& i_condition=Condition())
	{
		Family<T>& f = (*this)[i_name];
		return f.add_element(i_t, i_condition);
	}

	// �ߋ��݊��̒�
	const map< string, Family<T> >& compatible() const
	{
		return *this;
	}

	// ���O����Family���擾
	Family<T>* get_family(string i_name)
	{
	        iterator i = find(i_name);
		return ( i == end() ) ? NULL : &(i->second);
	}

	// ���O�̑��݂��m�F
	bool is_exist(const string& i_name) const
	{
		return find(i_name) != end();
	}

	// T���P�I�����A���̃|�C���^��Ԃ�
	const T* select(const string& i_name, Evalcator& i_evalcator)
	{
		iterator it = find(i_name);
		if ( it == end() )
		{
			return NULL;
		}
		return it->second.select(i_evalcator);
	}

	// �폜
	void erase(const string& i_name)
	{
		iterator it = find(i_name);
		if ( it == end() ) {
			return;
		}
		erase(it);
	}
	
	// �g�[�N�̏I����ʒm�B�d��������Ԃ��u�g�[�N���v�ł���Family�̏d����𐧌���N���A����
	void handle_talk_end()
	{
		for ( set<string>::iterator it = m_clearOC_at_talk_end.begin() ; it != m_clearOC_at_talk_end.end() ; ++it )
		{
			get_family(*it)->clear_OC();
		}
	}

	// family��
	int size_of_family() const
	{
		return size();
	}

	// �SFamily�̑S�v�f�����v�Z
	int size_of_element() const
	{
		int r = 0;
		for ( const_iterator it = begin() ; it != end() ; ++it )
		{
			r += it->second.size_of_element();
		}
		return r;
	}

	// �S�N���A
	void clear()
	{
		map< string, Family<T> >::clear();
		m_clearOC_at_talk_end.clear();
	}

	// �d����𐧌��I������B�����̓^�C�v�A����
	void setOC(string i_name, string i_value)
	{
		iterator st, ed;
		if ( i_name == "��" )
		{
			st = begin();
			ed = end();
		}
		else
		{
			st = find(i_name);
			if ( st == end() )
			{
				sender << "'" << i_name << "' �͑��݂��܂���B" << endl;
				return;
			}
			++(ed = st);
		}
		
		strvec argv;
		const int n = split(i_value, "�A,", argv);
		const string method = (n>=1) ? argv[0] : "����";
		const string span = (n>=2) ? argv[1] : "�N����";
		
		for ( iterator it = st; it != ed ; ++it )
		{
			Family<T>& family = it->second;
			if ( family.empty() )
			{
				continue;
			}

			if (0)
				NULL;
			else if ( method=="���O" )
				family.set_OC(new OC_NonDual<const T*>);
			else if ( method=="�~��" || method=="����" )
				family.set_OC(new OC_Sequential<const T*>);
			else if ( method=="����" || method=="�t��" )
				family.set_OC(new OC_SequentialDesc<const T*>);
			else if ( method=="�L��" || method=="���S" )
				family.set_OC(new OC_NonOverlap<const T*>);
			else if ( method=="����" )
				family.set_OC(new OC_Random<const T*>);
			else
				sender << "�d����𐧌�̕��@'" << method << "' �͒�`����Ă��܂���B" << endl;
			
			if ( span == "�g�[�N��" )
				m_clearOC_at_talk_end.insert(it->first);
			else if ( span == "�N����")
				NULL;
			else
				sender << "�d������̊���'" << method << "' �͒�`����Ă��܂���B" << endl;
		
		}
	}

	const Talk* communicate_search(const string& iSentence, bool iAndMode)
	{
		sender << "�����̌������J�n" << endl;
		sender << "�@�Ώە�����: " << iSentence << endl;
		sender << "�@�S�P���v���[�h: " << (iAndMode?"true":"false") << endl;

		vector<const Talk*>	result;
		int	max_hit_point=0;
		for ( iterator it = begin() ; it != end() ; ++it )
		{
			// ��Q��S�p�X�y�[�X�ŋ�؂�
			strvec	words;
			if ( split(it->first, "�@", words)<2 )
			{
				continue; // �S�p�X�y�[�X�������B�Y���O�B
			}

			// �����̒P�ꂪ�q�b�g�������B�P��P��10�Ă�A�����P������1�Ă�
			int	hit_point=0;
			strvec::iterator wds_it=words.begin();
			for ( ; wds_it!=words.end() ; ++wds_it )
			{
				if ( iSentence.find(*wds_it) != string::npos )
				{
					if ( compare_tail(*wds_it, "�u") )	// ������ �u �ł�����̂����̏ꍇ�̓q�b�g�ƌ��Ȃ��Ȃ��悤�ɁB
						hit_point += 4;
					else
						hit_point += 10+(wds_it->size()/4);	// ��v�����P��B
				}
				else
				{
					hit_point -= (iAndMode ? 999 : 1);	// ��v���Ȃ������A������Ȃ������P��
				}
			}
			if ( hit_point<=4 )
			{
				continue;	// ����������v���Ȃ��ꍇ
			}

			sender << "'" << it->first << "' : " << hit_point << "pt ,";

			if ( hit_point<max_hit_point) {
				sender << "�p��" << endl;
				continue;
			} else if ( hit_point == max_hit_point ) {
				sender << "���Ƃ��Ēǉ�" << endl;
			} else {
				max_hit_point = hit_point;
				sender << "�P�Ƃō̗p" << endl;
				result.clear();
			}


			it->second.get_elements_pointers(result);
		}

		sender << "����: ";
		if ( result.size() <= 0 ) {
			sender << "�Y���Ȃ�" << endl;
			return	NULL;
		}

		return result[ random(result.size()) ];
	}

};


