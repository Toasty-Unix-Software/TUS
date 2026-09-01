	vpandq xmm3, xmm0, oword [0x9e9]
	vpandq xmm3, oword [0x9e9]
	vpandq xmm7, xmm6, xmm0
	vpandq ymm2, ymm5, yword [0x23e]
	vpandq ymm2, yword [0x23e]
	vpandq ymm2, ymm5, ymm1
	vpandq zmm2, zmm5, zmm0
	vpandq zmm2, zmm0
	vpandq zmm6, zmm0, zmm4

%ifdef ERROR
	vpandq xmm10, xmm10, xmm14
	vpandq ymm8, ymm15, ymm15
	vpandq zmm14, zmm10, zmm11
	vpandq xmm16, xmm28, xmm26
	vpandq ymm19, ymm24, ymm26
	vpandq zmm16, zmm26, zmm25
%endif
