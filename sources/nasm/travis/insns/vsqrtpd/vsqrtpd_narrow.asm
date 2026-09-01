	vsqrtpd xmm7, oword [0x3a8]
	vsqrtpd xmm1, xmm6
	vsqrtpd ymm1, ymm3
	vsqrtpd ymm0, ymm7
	vsqrtpd xmm0, oword [0xb0a]
	vsqrtpd xmm5, xmm7
	vsqrtpd ymm5, ymm5
	vsqrtpd ymm0, ymm2

%ifdef ERROR
	vsqrtpd xmm10, xmm10
	vsqrtpd ymm15, ymm14
	vsqrtpd xmm12, xmm11
	vsqrtpd ymm12, ymm12
	vsqrtpd xmm27, xmm24
	vsqrtpd ymm31, ymm18
	vsqrtpd xmm18, xmm23
	vsqrtpd ymm22, ymm29
%endif
