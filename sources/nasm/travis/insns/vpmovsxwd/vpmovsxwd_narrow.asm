	vpmovsxwd xmm1, xmm3
	vpmovsxwd xmm5, xmm5
	vpmovsxwd ymm2, oword [0x344]
	vpmovsxwd ymm5, xmm1
	vpmovsxwd xmm3, xmm6
	vpmovsxwd xmm7, qword [0xb57]
	vpmovsxwd ymm6, xmm6
	vpmovsxwd ymm0, xmm6

%ifdef ERROR
	vpmovsxwd xmm9, xmm15
	vpmovsxwd ymm13, xmm11
	vpmovsxwd xmm14, xmm13
	vpmovsxwd ymm9, xmm9
	vpmovsxwd xmm30, xmm17
	vpmovsxwd ymm18, xmm17
	vpmovsxwd xmm29, xmm17
	vpmovsxwd ymm18, xmm27
%endif
