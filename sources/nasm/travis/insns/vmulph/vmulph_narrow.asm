	vmulph xmm6, xmm6, xmm4
	vmulph xmm6, xmm4
	vmulph xmm5, xmm3, xmm1
	vmulph ymm2, ymm5, ymm0
	vmulph ymm2, ymm0
	vmulph ymm0, ymm2, ymm1
	vmulph zmm7, zmm6, zmm6
	vmulph zmm7, zmm6
	vmulph zmm3, zmm7, zword [0xf3e]

%ifdef ERROR
	vmulph xmm13, xmm11, xmm12
	vmulph ymm9, ymm14, ymm8
	vmulph zmm8, zmm11, zmm8
	vmulph xmm26, xmm19, xmm30
	vmulph ymm26, ymm19, ymm30
	vmulph zmm21, zmm31, zmm17
%endif
