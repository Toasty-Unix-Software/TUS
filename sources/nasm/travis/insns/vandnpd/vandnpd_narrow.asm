	vandnpd xmm4, xmm1, xmm6
	vandnpd xmm4, xmm6
	vandnpd xmm2, xmm4, oword [0x1c7]
	vandnpd ymm5, ymm1, ymm2
	vandnpd ymm5, ymm2
	vandnpd ymm4, ymm7, yword [0xdac]
	vandnpd xmm7, xmm7, xmm2
	vandnpd xmm7, xmm2
	vandnpd xmm6, xmm0, xmm6
	vandnpd ymm4, ymm6, ymm0
	vandnpd ymm4, ymm0
	vandnpd ymm5, ymm3, ymm6

%ifdef ERROR
	vandnpd xmm11, xmm14, xmm10
	vandnpd ymm15, ymm10, ymm11
	vandnpd xmm11, xmm9, xmm8
	vandnpd ymm8, ymm10, ymm15
	vandnpd xmm16, xmm25, xmm17
	vandnpd ymm26, ymm19, ymm30
	vandnpd xmm19, xmm24, xmm24
	vandnpd ymm24, ymm16, ymm29
%endif
