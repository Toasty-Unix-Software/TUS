default rel
	vsubbf16 xmm3, xmm3, xmm0
	vsubbf16 xmm5, xmm5, oword [0x63c]
	vsubbf16 ymm5, ymm0, yword [0x86e]
	vsubbf16 ymm1, ymm3, ymm0
	vsubbf16 zmm5, zmm1, zmm7
	vsubbf16 zmm0, zmm3, zmm7
	vsubbf16 xmm15, xmm8, xmm9
	vsubbf16 ymm14, ymm10, ymm9
	vsubbf16 zmm10, zmm13, zmm12
	vsubbf16 xmm27, xmm20, xmm23
	vsubbf16 ymm17, ymm26, ymm24
	vsubbf16 zmm18, zmm30, zmm16
	vsubbf16 xmm3{k6}, xmm2, oword [0x416]
	vsubbf16 ymm7{k7}, ymm5, ymm5
	vsubbf16 zmm2{k7}, zmm3, zmm0
	vsubbf16 xmm6{k4}{z}, xmm0, xmm3
	vsubbf16 ymm7{k6}{z}, ymm5, yword [0x1d0]
	vsubbf16 zmm3{k6}{z}, zmm4, zmm7
	vsubbf16 xmm6, xmm2, oword [eax+1]
	vsubbf16 xmm3, xmm1, oword [eax+64]
	vsubbf16 ymm1, ymm5, yword [eax+1]
	vsubbf16 ymm4, ymm3, yword [eax+64]
	vsubbf16 zmm6, zmm1, zword [eax+1]
	vsubbf16 zmm3, zmm1, zword [eax+64]
	vsubbf16 xmm1, xmm7, [0x542]
	vsubbf16 ymm2, ymm2, [0xdf4]
	vsubbf16 zmm0, zmm7, [0x551]
