
#include	<assert.h>
#include	<ctype.h>
#include	<stddef.h>

// �啶�����������킸��r
static const char* stristr(const char* p, const char* substr) {
	assert(p!=NULL);
	for(;*p;++p) {
		for(int i=0;substr[i];++i)
			if (tolower(substr[i])!=tolower(p[i]))
				break;
		if(substr[i]=='\0')
			return	p;
	}
	return	NULL;
}

