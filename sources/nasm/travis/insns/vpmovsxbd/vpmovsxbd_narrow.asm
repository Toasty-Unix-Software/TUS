	vpmovsxbd xmm4, xmm3
	vpmovsxbd xmm5, dword [0x676]
	vpmovsxbd ymm5, qword [0x194]
	vpmovsxbd ymm6, qword [0xced]
	vpmovsxbd ymm0, xmm5
	vpmovsxbd ymm0, xmm2
	vpmovsxbd xmm7, xmm2
	vpmovsxbd xmm5, xmm6

%ifdef ERROR
	vpmovsxbd xmm14, xmm14
	vpmovsxbd ymm8, qword [0x6d7]
	vpmovsxbd ymm11, xmm10
	vpmovsxbd xmm15, xmm12
	vpmovsxbd xmm20, xmm24
	vpmovsxbd ymm17, qword [0xd9b]
	vpmovsxbd ymm19, xmm24
	vpmovsxbd xmm19, xmm29
%endif
