default rel
	vpdpbuud xmm3, xmm3, oword [0xa1c]
	vpdpbuud xmm0, xmm4, xmm7
	vpdpbuud ymm2, ymm3, ymm2
	vpdpbuud ymm7, ymm1, ymm7
	vpdpbuud xmm2, xmm7, xmm3
	vpdpbuud xmm3, xmm6, xmm5
	vpdpbuud ymm4, ymm7, ymm2
	vpdpbuud ymm7, ymm2, yword [0x50b]
	vpdpbuud xmm14, xmm14, xmm13
	vpdpbuud ymm11, ymm15, ymm14
	vpdpbuud xmm8, xmm8, xmm11
	vpdpbuud ymm8, ymm13, ymm8
	vpdpbuud xmm16, xmm29, xmm28
	vpdpbuud ymm19, ymm31, ymm29
	vpdpbuud xmm31, xmm23, xmm28
	vpdpbuud ymm26, ymm24, ymm23
	vpdpbuud xmm2{k4}, xmm3, xmm4
	vpdpbuud ymm0{k2}, ymm4, yword [0x9b9]
	vpdpbuud zmm6{k6}, zmm7, zword [0x2b3]
	vpdpbuud xmm7{k2}{z}, xmm3, xmm4
	vpdpbuud ymm5{k7}{z}, ymm4, ymm4
	vpdpbuud zmm7{k2}{z}, zmm5, zmm5
	vpdpbuud xmm1, xmm5, oword [eax+1]
	vpdpbuud xmm3, xmm0, oword [eax+64]
	vpdpbuud ymm2, ymm6, yword [eax+1]
	vpdpbuud ymm1, ymm6, yword [eax+64]
	vpdpbuud xmm6, xmm5, oword [eax+1]
	vpdpbuud xmm0, xmm4, oword [eax+64]
	vpdpbuud ymm3, ymm1, yword [eax+1]
	vpdpbuud ymm7, ymm6, yword [eax+64]
	vpdpbuud zmm3, zmm6, zword [eax+1]
	vpdpbuud zmm6, zmm4, zword [eax+64]
	vpdpbuud xmm4, xmm2, [0x616]
	vpdpbuud ymm0, ymm3, [0xe2e]
	vpdpbuud xmm1, xmm0, [0x16b]
	vpdpbuud ymm2, ymm0, [0x7ab]
	vpdpbuud zmm6, zmm0, [0x91c]
