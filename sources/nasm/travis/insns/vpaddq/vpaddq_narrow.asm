	vpaddq xmm1, xmm2, xmm6
	vpaddq xmm1, xmm6
	vpaddq xmm2, xmm6, oword [0xdc5]
	vpaddq ymm2, ymm4, ymm1
	vpaddq ymm2, ymm1
	vpaddq ymm3, ymm6, ymm7
	vpaddq xmm6, xmm6, xmm2
	vpaddq xmm6, xmm2
	vpaddq xmm4, xmm7, xmm0
	vpaddq ymm1, ymm6, ymm5
	vpaddq ymm1, ymm5
	vpaddq ymm6, ymm1, ymm6

%ifdef ERROR
	vpaddq xmm14, xmm13, xmm13
	vpaddq ymm8, ymm12, ymm10
	vpaddq xmm12, xmm15, xmm15
	vpaddq ymm13, ymm13, ymm8
	vpaddq xmm25, xmm18, xmm30
	vpaddq ymm28, ymm23, ymm24
	vpaddq xmm30, xmm20, xmm27
	vpaddq ymm17, ymm25, ymm24
%endif
