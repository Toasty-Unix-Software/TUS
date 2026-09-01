	vptestmd k1, xmm2, xmm0
	vptestmd k2, xmm6, xmm7
	vptestmd k2, ymm2, yword [0x1b3]
	vptestmd k7, ymm4, ymm1
	vptestmd k4, zmm1, zmm7
	vptestmd k6, zmm0, zmm4

%ifdef ERROR
	vptestmd k1, xmm9, xmm8
	vptestmd k7, ymm15, ymm14
	vptestmd k5, zmm9, zmm13
	vptestmd k1, xmm17, xmm19
	vptestmd k1, ymm22, ymm31
	vptestmd k6, zmm17, zmm16
%endif
