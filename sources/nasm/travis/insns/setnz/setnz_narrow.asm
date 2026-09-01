	setnz byte [0x816]
	setnz cl
	setnz ebp
	setnz esi

%ifdef ERROR
	setnz rbx
	setnz rdi
	setnz r8b
	setnz r12
	setnz r12d
	setnz r20b
	setnz r19
	setnz r17d
%endif
