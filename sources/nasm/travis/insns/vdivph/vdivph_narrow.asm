	vdivph xmm6, xmm4, xmm4
	vdivph xmm6, xmm4
	vdivph xmm0, xmm7, xmm5
	vdivph ymm3, ymm5, ymm3
	vdivph ymm3, ymm3
	vdivph ymm4, ymm4, ymm1
	vdivph zmm2, zmm0, zword [0x914]
	vdivph zmm2, zword [0x914]
	vdivph zmm3, zmm4, zmm5

%ifdef ERROR
	vdivph xmm13, xmm15, xmm13
	vdivph ymm15, ymm12, ymm8
	vdivph zmm12, zmm12, zmm15
	vdivph xmm18, xmm17, xmm29
	vdivph ymm30, ymm24, ymm27
	vdivph zmm16, zmm27, zmm20
%endif
