	vcmpeq_oqps k1, xmm1, xmm7
	vcmpeq_oqps k4, xmm2, xmm6
	vcmpeq_oqps k6, ymm0, ymm6
	vcmpeq_oqps k1, ymm7, ymm5
	vcmpeq_oqps k2, zmm2, zmm1
	vcmpeq_oqps k6, zmm5, zmm7

%ifdef ERROR
	vcmpeq_oqps k6, xmm12, xmm10
	vcmpeq_oqps k5, ymm14, ymm9
	vcmpeq_oqps k4, zmm14, zmm10
	vcmpeq_oqps k2, xmm26, xmm27
	vcmpeq_oqps k2, ymm24, ymm17
	vcmpeq_oqps k2, zmm25, zmm30
%endif
