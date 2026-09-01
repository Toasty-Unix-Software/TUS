default rel
	vsqrtbf16 xmm5, xmm7
	vsqrtbf16 xmm6, xmm6
	vsqrtbf16 ymm2, ymm4
	vsqrtbf16 ymm6, yword [0x500]
	vsqrtbf16 zmm1, zmm1
	vsqrtbf16 zmm6, zmm1
	vsqrtbf16 xmm8, xmm8
	vsqrtbf16 ymm15, ymm15
	vsqrtbf16 zmm12, zmm14
	vsqrtbf16 xmm28, xmm27
	vsqrtbf16 ymm27, ymm16
	vsqrtbf16 zmm26, zmm18
	vsqrtbf16 xmm1{k7}, xmm5
	vsqrtbf16 ymm6{k1}, yword [0x963]
	vsqrtbf16 zmm4{k7}, zmm7
	vsqrtbf16 xmm6{k4}{z}, xmm7
	vsqrtbf16 ymm0{k2}{z}, ymm2
	vsqrtbf16 zmm7{k5}{z}, zword [0xef5]
	vsqrtbf16 xmm4, oword [eax+1]
	vsqrtbf16 xmm7, oword [eax+64]
	vsqrtbf16 ymm0, yword [eax+1]
	vsqrtbf16 ymm3, yword [eax+64]
	vsqrtbf16 zmm3, zword [eax+1]
	vsqrtbf16 zmm4, zword [eax+64]
	vsqrtbf16 xmm5, [0xb04]
	vsqrtbf16 ymm2, [0xf55]
	vsqrtbf16 zmm5, [0x202]
