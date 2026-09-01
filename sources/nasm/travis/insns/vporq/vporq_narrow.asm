	vporq xmm2, xmm2, oword [0x2ad]
	vporq xmm2, oword [0x2ad]
	vporq xmm2, xmm0, xmm7
	vporq ymm2, ymm0, ymm0
	vporq ymm2, ymm0
	vporq ymm3, ymm3, ymm5
	vporq zmm2, zmm2, zword [0x49a]
	vporq zmm2, zword [0x49a]
	vporq zmm1, zmm3, zmm6

%ifdef ERROR
	vporq xmm15, xmm10, xmm15
	vporq ymm15, ymm14, ymm9
	vporq zmm8, zmm8, zmm13
	vporq xmm27, xmm26, xmm29
	vporq ymm18, ymm24, ymm25
	vporq zmm29, zmm20, zmm25
%endif
