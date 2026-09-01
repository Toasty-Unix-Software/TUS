	vpandnq xmm4, xmm3, xmm3
	vpandnq xmm4, xmm3
	vpandnq xmm3, xmm7, oword [0xb4b]
	vpandnq ymm0, ymm6, ymm4
	vpandnq ymm0, ymm4
	vpandnq ymm0, ymm7, ymm4
	vpandnq zmm6, zmm2, zmm7
	vpandnq zmm6, zmm7
	vpandnq zmm7, zmm4, zmm1

%ifdef ERROR
	vpandnq xmm11, xmm9, xmm11
	vpandnq ymm13, ymm15, ymm15
	vpandnq zmm8, zmm15, zmm10
	vpandnq xmm24, xmm22, xmm30
	vpandnq ymm30, ymm24, ymm30
	vpandnq zmm17, zmm25, zmm20
%endif
