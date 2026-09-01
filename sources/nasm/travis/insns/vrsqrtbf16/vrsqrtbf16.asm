default rel
	vrsqrtbf16 xmm6, xmm6
	vrsqrtbf16 xmm1, oword [0x7fd]
	vrsqrtbf16 ymm3, yword [0x1b2]
	vrsqrtbf16 ymm3, ymm4
	vrsqrtbf16 zmm0, zmm5
	vrsqrtbf16 zmm1, zmm5
	vrsqrtbf16 xmm11, xmm10
	vrsqrtbf16 ymm13, ymm15
	vrsqrtbf16 zmm12, zmm11
	vrsqrtbf16 xmm19, xmm28
	vrsqrtbf16 ymm23, ymm18
	vrsqrtbf16 zmm29, zmm22
	vrsqrtbf16 xmm3{k2}, oword [0x699]
	vrsqrtbf16 ymm7{k5}, ymm6
	vrsqrtbf16 zmm2{k7}, zmm1
	vrsqrtbf16 xmm5{k6}{z}, xmm3
	vrsqrtbf16 ymm7{k3}{z}, ymm7
	vrsqrtbf16 zmm4{k6}{z}, zmm7
	vrsqrtbf16 xmm1, oword [eax+1]
	vrsqrtbf16 xmm4, oword [eax+64]
	vrsqrtbf16 ymm2, yword [eax+1]
	vrsqrtbf16 ymm1, yword [eax+64]
	vrsqrtbf16 zmm0, zword [eax+1]
	vrsqrtbf16 zmm0, zword [eax+64]
	vrsqrtbf16 xmm7, [0xbca]
	vrsqrtbf16 ymm5, [0x26f]
	vrsqrtbf16 zmm0, [0xf3d]
