	vptestmw k4, xmm0, oword [0xcd6]
	vptestmw k2, xmm7, xmm3
	vptestmw k3, ymm5, ymm6
	vptestmw k6, ymm2, ymm3
	vptestmw k3, zmm6, zmm7
	vptestmw k1, zmm3, zword [0xa5d]

%ifdef ERROR
	vptestmw k3, xmm12, xmm14
	vptestmw k1, ymm15, ymm15
	vptestmw k3, zmm10, zmm14
	vptestmw k4, xmm26, xmm28
	vptestmw k7, ymm24, ymm27
	vptestmw k4, zmm27, zmm31
%endif
