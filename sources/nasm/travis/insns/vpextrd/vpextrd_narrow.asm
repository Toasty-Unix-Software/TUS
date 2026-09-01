	vpextrd eax, xmm6, 0x5
	vpextrd ebp, xmm2, 0xad

%ifdef ERROR
	vpextrd rcx, xmm3, 0xe5
	vpextrd rdi, xmm3, 0xaf
	vpextrd r8, xmm12, 0x1b
	vpextrd r9d, xmm12, 0x6d
%endif
