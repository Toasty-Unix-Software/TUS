	vplzcntq xmm1, xmm0
	vplzcntq xmm2, oword [0x2d3]
	vplzcntq ymm0, yword [0xa12]
	vplzcntq ymm6, ymm7
	vplzcntq zmm0, zmm2
	vplzcntq zmm7, zmm5

%ifdef ERROR
	vplzcntq xmm14, xmm11
	vplzcntq ymm11, ymm15
	vplzcntq zmm14, zmm15
	vplzcntq xmm25, xmm17
	vplzcntq ymm23, ymm30
	vplzcntq zmm17, zmm24
%endif
