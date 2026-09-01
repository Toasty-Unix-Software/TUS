	vptestmq k4, xmm0, xmm6
	vptestmq k1, xmm3, oword [0xeb8]
	vptestmq k7, ymm5, ymm3
	vptestmq k7, ymm5, ymm0
	vptestmq k4, zmm7, zmm7
	vptestmq k6, zmm7, zmm5

%ifdef ERROR
	vptestmq k6, xmm10, xmm15
	vptestmq k4, ymm15, ymm15
	vptestmq k7, zmm14, zmm8
	vptestmq k3, xmm17, xmm17
	vptestmq k6, ymm18, ymm17
	vptestmq k1, zmm31, zmm24
%endif
