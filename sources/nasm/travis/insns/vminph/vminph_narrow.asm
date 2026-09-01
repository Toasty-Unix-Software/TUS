	vminph xmm4, xmm2, xmm3
	vminph xmm4, xmm3
	vminph xmm5, xmm5, xmm7
	vminph ymm7, ymm1, ymm1
	vminph ymm7, ymm1
	vminph ymm6, ymm3, ymm5
	vminph zmm0, zmm3, zmm5
	vminph zmm0, zmm5
	vminph zmm5, zmm1, zmm2

%ifdef ERROR
	vminph xmm9, xmm9, xmm8
	vminph ymm15, ymm13, ymm11
	vminph zmm12, zmm9, zmm9
	vminph xmm19, xmm25, xmm25
	vminph ymm20, ymm29, ymm16
	vminph zmm26, zmm17, zmm29
%endif
