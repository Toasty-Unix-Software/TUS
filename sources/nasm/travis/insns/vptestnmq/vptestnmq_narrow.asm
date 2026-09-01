	vptestnmq k5, xmm2, oword [0x889]
	vptestnmq k3, xmm0, oword [0xcae]
	vptestnmq k4, ymm1, ymm5
	vptestnmq k3, ymm3, ymm1
	vptestnmq k5, zmm1, zmm1
	vptestnmq k5, zmm7, zmm7

%ifdef ERROR
	vptestnmq k3, xmm13, xmm9
	vptestnmq k3, ymm9, ymm10
	vptestnmq k4, zmm14, zmm14
	vptestnmq k7, xmm19, xmm21
	vptestnmq k4, ymm16, ymm28
	vptestnmq k4, zmm30, zmm19
%endif
