	pextrd eax, xmm5, 0xcc
	pextrd dword [0x3aa], xmm5, 0x85

%ifdef ERROR
	pextrd r14d, xmm14, 0x9
%endif
