
/*------------------------------------------------------------------
	BinaryTree Element

            ��parent
            �b    
           �^�_
     this��    ��brother
         �b    
        �^�_
      ��    ��
left-child    right-child


	'this' has pointer to children, pointer to parent, and template instance.

	�e�q�̐ڑ��́A�e����Left�܂���Right�ɐݒ肷�邱�ƁB
	�q������ł͐e�̂ǂ���ɐڑ����邩����ł��Ȃ����߁B

	methods:

	BTE();
	BTE( const T& iT );
	~BTE();

	void	LeftChild( BTE* iLeftChild );	// ���q��ڑ�
	void	RightChild( BTE* iRightChild );	// �E�q��ڑ�
	void	Independent();					// �e����Ɨ�

	BTE*	LeftChild();	// ���q�擾
	BTE*	RightChild();	// �E�q�擾
	BTE*	Parent();		// �e�擾
	T&	operator * ();		// ���̂��擾

------------------------------------------------------------------*/

#include	<assert.h>

#ifdef	DBG
#undef	DBG
#endif	// DBG

#ifdef	_DEBUG
#define	DBG(v)	v
#else	// _DEBUG
#define	DBG(v)	((void)0)
#endif	// _DEBUG

template<class T>
class BTE {

private:
	BTE*	mLeftChild;
	BTE*	mRightChild;
	BTE*	mParent;
	T		mT;
	
	BTE( const BTE& );
	BTE& operator = ( const BTE& );

public:
	void	LeftChild( BTE* iLeftChild ) {
		assert(iLeftChild != this);
		DBG( CheckValid() );

		// ���݂̍��q��؂藣��
		if ( mLeftChild != NULL ) {
			mLeftChild->mParent = NULL;
			mLeftChild = NULL;
		}
		
		// �V�������q��ڑ�����
		if ( iLeftChild != NULL ) {
			iLeftChild->mParent = this;
			mLeftChild = iLeftChild;
		}
	}
	void	RightChild( BTE* iRightChild ) {
		assert(iRightChild != this);
		DBG( CheckValid() );

		// ���݂̉E�q��؂藣��
		if ( mRightChild != NULL ) {
			mRightChild->mParent = NULL;
			mRightChild = NULL;
		}
		
		// �V�����E�q��ڑ�����
		if ( iRightChild != NULL ) {
			iRightChild->mParent = this;
			mRightChild = iRightChild;
		}
	}
	// �e����Ɨ�
	void	Independent() {
		DBG( CheckValid() );
		if ( mParent == NULL )
			NULL;
		else if ( mParent->LeftChild() == this )
			mParent->LeftChild(NULL);
		else if ( mParent->RightChild() == this )
			mParent->RightChild(NULL);
	}

	// �����o�擾
	BTE*	LeftChild() {
		DBG( CheckValid() );
		return mLeftChild; 
	}
	BTE*	RightChild() {
		DBG( CheckValid() );
		return mRightChild;
	}
	BTE*	Parent() {
		DBG( CheckValid() );
		return mParent;
	}
	T&	operator * () {
		DBG( CheckValid() );
		return mT;
	}
	T*	operator -> () {
		DBG( CheckValid() );
		return &mT;
	}
	T&	Real() {
		DBG( CheckValid() );
		return mT;
	}

	// �f�t�H���g�R���X�g���N�^
	BTE()
	 : mLeftChild(NULL)
	 , mRightChild(NULL)
	 , mParent(NULL)
	{
	}
	// T �̒l���R�s�[���ăR���X�g���N�g
	BTE( const T& iT )
	 : mLeftChild(NULL)
	 , mRightChild(NULL)
	 , mParent(NULL)
	 , mT(iT)
	{
	}
	// �f�X�g���N�^
	~BTE() {
		DBG( CheckValid() );
		LeftChild(NULL);
		RightChild(NULL);
		Independent();
	}

	// ���q�ɓn���čċA����
	template<class Function>
	void	Each( Function iFunction ) {
		if ( mLeftChild != NULL )
			mLeftChild->Each( iFunction );
		if ( mRightChild != NULL )
			mRightChild->Each( iFunction );
		iFunction( this );
	}

	// �������`�F�b�N�B�\�z��ɗL���B
	void	CheckValid() {
		// ���͑��݂���B
		assert( this!=NULL );
		// ���ɐe������Ƃ��A���͂��̐e�̎q�ł���B
		assert( mParent==NULL || (mParent->mLeftChild==this || mParent->mRightChild==this ) );
		// ���Ɏq������΁A���̐e�͎��ł���B
		assert( mLeftChild==NULL || mLeftChild->mParent==this );
		assert( mRightChild==NULL || mRightChild->mParent==this );
	}
};


