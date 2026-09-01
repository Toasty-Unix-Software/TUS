	bswap edx
	bswap esi
	bswap ax
	bswap ax
	bswap cx
	bswap cx

%ifdef ERROR
	bswap rcx
	bswap rbx
	bswap r10d
	bswap r10
	bswap r23d
	bswap r18
%endif
