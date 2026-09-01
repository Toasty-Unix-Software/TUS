	vandpd xmm4, xmm4, oword [0xdfc]
	vandpd xmm4, oword [0xdfc]
	vandpd xmm4, xmm0, xmm6
	vandpd ymm2, ymm3, ymm3
	vandpd ymm2, ymm3
	vandpd ymm3, ymm4, ymm7
	vandpd xmm3, xmm5, oword [0x9f4]
	vandpd xmm3, oword [0x9f4]
	vandpd xmm1, xmm3, xmm1
	vandpd ymm2, ymm1, ymm6
	vandpd ymm2, ymm6
	vandpd ymm7, ymm5, ymm1

%ifdef ERROR
	vandpd xmm9, xmm10, xmm13
	vandpd ymm9, ymm11, ymm12
	vandpd xmm8, xmm14, xmm13
	vandpd ymm14, ymm9, ymm15
	vandpd xmm25, xmm24, xmm27
	vandpd ymm17, ymm26, ymm25
	vandpd xmm26, xmm31, xmm25
	vandpd ymm25, ymm30, ymm21
%endif
