	vpmovsxbq xmm7, xmm5
	vpmovsxbq xmm7, xmm5
	vpmovsxbq ymm3, dword [0x898]
	vpmovsxbq ymm4, dword [0x643]
	vpmovsxbq ymm2, xmm5
	vpmovsxbq ymm5, xmm3
	vpmovsxbq xmm0, xmm1
	vpmovsxbq xmm2, xmm7

%ifdef ERROR
	vpmovsxbq xmm11, xmm10
	vpmovsxbq ymm10, dword [0x104]
	vpmovsxbq ymm9, xmm10
	vpmovsxbq xmm9, xmm14
	vpmovsxbq xmm24, xmm29
	vpmovsxbq ymm26, dword [0xe37]
	vpmovsxbq ymm20, xmm30
	vpmovsxbq xmm16, xmm16
%endif
