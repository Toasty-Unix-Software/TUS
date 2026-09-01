	vpsravd xmm1, xmm5, xmm0
	vpsravd xmm1, xmm0
	vpsravd xmm1, xmm1, xmm1
	vpsravd ymm2, ymm0, ymm2
	vpsravd ymm2, ymm2
	vpsravd ymm4, ymm0, ymm2
	vpsravd xmm2, xmm1, xmm0
	vpsravd xmm2, xmm0
	vpsravd xmm1, xmm6, xmm7
	vpsravd ymm0, ymm0, ymm7
	vpsravd ymm0, ymm7
	vpsravd ymm1, ymm2, ymm1

%ifdef ERROR
	vpsravd xmm14, xmm9, xmm12
	vpsravd ymm11, ymm14, ymm15
	vpsravd xmm12, xmm13, xmm12
	vpsravd ymm9, ymm8, ymm14
	vpsravd xmm17, xmm20, xmm31
	vpsravd ymm24, ymm29, ymm27
	vpsravd xmm31, xmm17, xmm25
	vpsravd ymm26, ymm16, ymm28
%endif
