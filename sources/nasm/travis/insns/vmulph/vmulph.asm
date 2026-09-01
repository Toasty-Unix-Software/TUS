default rel
	vmulph xmm6, xmm6, xmm4
	vmulph xmm6, xmm4
	vmulph xmm5, xmm3, xmm1
	vmulph ymm2, ymm5, ymm0
	vmulph ymm2, ymm0
	vmulph ymm0, ymm2, ymm1
	vmulph zmm7, zmm6, zmm6
	vmulph zmm7, zmm6
	vmulph zmm3, zmm7, zword [0xf3e]
	vmulph xmm13, xmm11, xmm12
	vmulph ymm9, ymm14, ymm8
	vmulph zmm8, zmm11, zmm8
	vmulph xmm26, xmm19, xmm30
	vmulph ymm26, ymm19, ymm30
	vmulph zmm21, zmm31, zmm17
	vmulph xmm7{k5}, xmm6, oword [0x6c0]
	vmulph ymm6{k7}, ymm2, ymm6
	vmulph zmm0{k1}, zmm1, zmm7
	vmulph xmm3{k4}{z}, xmm7, xmm4
	vmulph ymm4{k6}{z}, ymm7, ymm1
	vmulph zmm1{k6}{z}, zmm2, zmm3
	vmulph xmm5, xmm7, oword [eax+1]
	vmulph xmm5, xmm6, oword [eax+64]
	vmulph ymm3, ymm5, yword [eax+1]
	vmulph ymm4, ymm5, yword [eax+64]
	vmulph zmm1, zmm5, zword [eax+1]
	vmulph zmm7, zmm6, zword [eax+64]
	vmulph xmm5, xmm3, [0x1e8]
	vmulph ymm0, ymm4, [0x2fd]
	vmulph zmm6, zmm6, [0xabc]
