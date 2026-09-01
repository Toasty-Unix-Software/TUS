default rel
	vrsqrt14ps xmm2, oword [0x188]
	vrsqrt14ps xmm2, oword [0xb04]
	vrsqrt14ps ymm2, ymm2
	vrsqrt14ps ymm5, ymm7
	vrsqrt14ps zmm0, zmm2
	vrsqrt14ps zmm3, zword [0x1b5]
	vrsqrt14ps xmm11, xmm13
	vrsqrt14ps ymm8, ymm14
	vrsqrt14ps zmm14, zmm8
	vrsqrt14ps xmm19, xmm25
	vrsqrt14ps ymm20, ymm27
	vrsqrt14ps zmm20, zmm31
	vrsqrt14ps xmm6{k6}, oword [0xda0]
	vrsqrt14ps ymm2{k5}, yword [0x195]
	vrsqrt14ps zmm0{k2}, zmm5
	vrsqrt14ps xmm5{k7}{z}, oword [0x24f]
	vrsqrt14ps ymm0{k5}{z}, ymm3
	vrsqrt14ps zmm1{k5}{z}, zmm6
	vrsqrt14ps xmm5, oword [eax+1]
	vrsqrt14ps xmm0, oword [eax+64]
	vrsqrt14ps ymm6, yword [eax+1]
	vrsqrt14ps ymm6, yword [eax+64]
	vrsqrt14ps zmm5, zword [eax+1]
	vrsqrt14ps zmm1, zword [eax+64]
	vrsqrt14ps xmm7, [0xe7b]
	vrsqrt14ps ymm7, [0x2ae]
	vrsqrt14ps zmm4, [0x878]
