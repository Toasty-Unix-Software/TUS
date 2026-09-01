	vpmovsxdq xmm0, xmm4
	vpmovsxdq xmm3, qword [0x64e]
	vpmovsxdq ymm5, xmm6
	vpmovsxdq ymm1, xmm6
	vpmovsxdq xmm3, qword [0x708]
	vpmovsxdq xmm1, qword [0x423]
	vpmovsxdq ymm0, xmm6
	vpmovsxdq ymm4, oword [0x60a]

%ifdef ERROR
	vpmovsxdq xmm15, xmm13
	vpmovsxdq ymm11, xmm8
	vpmovsxdq xmm15, xmm8
	vpmovsxdq ymm11, xmm13
	vpmovsxdq xmm29, xmm28
	vpmovsxdq ymm23, xmm31
	vpmovsxdq xmm19, xmm29
	vpmovsxdq ymm17, xmm27
%endif
