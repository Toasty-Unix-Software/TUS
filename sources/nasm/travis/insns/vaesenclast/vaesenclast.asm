default rel
	vaesenclast xmm2, xmm7, xmm3
	vaesenclast xmm2, xmm3
	vaesenclast xmm7, xmm4, xmm6
	vaesenclast ymm1, ymm5, ymm5
	vaesenclast ymm1, ymm5
	vaesenclast ymm0, ymm0, ymm6
	vaesenclast zmm5, zmm0, zword [0xd18]
	vaesenclast zmm5, zword [0xd18]
	vaesenclast zmm1, zmm4, zmm0
	vaesenclast xmm9, xmm13, xmm12
	vaesenclast ymm11, ymm8, ymm15
	vaesenclast zmm8, zmm8, zmm15
	vaesenclast xmm19, xmm26, xmm23
	vaesenclast ymm17, ymm16, ymm19
	vaesenclast zmm16, zmm20, zmm29
	vaesenclast xmm3, xmm5, oword [eax+1]
	vaesenclast xmm1, xmm1, oword [eax+64]
	vaesenclast ymm2, ymm6, yword [eax+1]
	vaesenclast ymm7, ymm6, yword [eax+64]
	vaesenclast zmm2, zmm0, zword [eax+1]
	vaesenclast zmm6, zmm3, zword [eax+64]
	vaesenclast xmm6, xmm5, [0xed8]
	vaesenclast ymm6, ymm6, [0xfcf]
	vaesenclast zmm0, zmm1, [0x7a9]
