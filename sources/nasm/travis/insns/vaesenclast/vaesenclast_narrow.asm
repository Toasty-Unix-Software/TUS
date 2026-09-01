	vaesenclast xmm2, xmm7, xmm3
	vaesenclast xmm2, xmm3
	vaesenclast xmm7, xmm4, xmm6
	vaesenclast ymm1, ymm5, ymm5
	vaesenclast ymm1, ymm5
	vaesenclast ymm0, ymm0, ymm6
	vaesenclast zmm5, zmm0, zword [0xd18]
	vaesenclast zmm5, zword [0xd18]
	vaesenclast zmm1, zmm4, zmm0

%ifdef ERROR
	vaesenclast xmm9, xmm13, xmm12
	vaesenclast ymm11, ymm8, ymm15
	vaesenclast zmm8, zmm8, zmm15
	vaesenclast xmm19, xmm26, xmm23
	vaesenclast ymm17, ymm16, ymm19
	vaesenclast zmm16, zmm20, zmm29
%endif
