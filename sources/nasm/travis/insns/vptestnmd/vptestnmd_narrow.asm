	vptestnmd k2, xmm0, xmm2
	vptestnmd k5, xmm5, xmm6
	vptestnmd k1, ymm3, ymm0
	vptestnmd k3, ymm3, ymm2
	vptestnmd k6, zmm7, zmm0
	vptestnmd k2, zmm3, zmm6

%ifdef ERROR
	vptestnmd k2, xmm11, xmm9
	vptestnmd k1, ymm14, ymm14
	vptestnmd k1, zmm14, zmm12
	vptestnmd k4, xmm17, xmm27
	vptestnmd k4, ymm27, ymm26
	vptestnmd k3, zmm16, zmm24
%endif
