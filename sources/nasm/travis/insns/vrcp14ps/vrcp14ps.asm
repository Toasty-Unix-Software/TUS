default rel
	vrcp14ps xmm0, xmm6
	vrcp14ps xmm5, xmm1
	vrcp14ps ymm4, yword [0x90d]
	vrcp14ps ymm3, ymm1
	vrcp14ps zmm7, zmm6
	vrcp14ps zmm3, zmm5
	vrcp14ps xmm11, xmm12
	vrcp14ps ymm9, ymm10
	vrcp14ps zmm12, zmm10
	vrcp14ps xmm29, xmm20
	vrcp14ps ymm26, ymm27
	vrcp14ps zmm22, zmm28
	vrcp14ps xmm2{k7}, oword [0x4ec]
	vrcp14ps ymm2{k2}, ymm4
	vrcp14ps zmm5{k3}, zword [0xde8]
	vrcp14ps xmm5{k5}{z}, xmm1
	vrcp14ps ymm4{k2}{z}, yword [0x5cf]
	vrcp14ps zmm1{k5}{z}, zmm3
	vrcp14ps xmm6, oword [eax+1]
	vrcp14ps xmm7, oword [eax+64]
	vrcp14ps ymm2, yword [eax+1]
	vrcp14ps ymm1, yword [eax+64]
	vrcp14ps zmm5, zword [eax+1]
	vrcp14ps zmm6, zword [eax+64]
	vrcp14ps xmm6, [0x2c8]
	vrcp14ps ymm7, [0xa68]
	vrcp14ps zmm2, [0x8a6]
