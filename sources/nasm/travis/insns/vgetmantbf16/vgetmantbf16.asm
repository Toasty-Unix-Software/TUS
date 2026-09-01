default rel
	vgetmantbf16 xmm5, oword [0xe78], 0xb
	vgetmantbf16 xmm0, xmm3, 0xe3
	vgetmantbf16 ymm2, ymm3, 0x3f
	vgetmantbf16 ymm4, ymm5, 0x1
	vgetmantbf16 zmm6, zmm1, 0x50
	vgetmantbf16 zmm3, zmm1, 0x7d
	vgetmantbf16 xmm10, xmm11, 0xe3
	vgetmantbf16 ymm10, ymm9, 0x9e
	vgetmantbf16 zmm12, zmm8, 0x4
	vgetmantbf16 xmm24, xmm17, 0xf3
	vgetmantbf16 ymm25, ymm21, 0xa8
	vgetmantbf16 zmm25, zmm23, 0x70
	vgetmantbf16 xmm5{k5}, xmm5, 0x68
	vgetmantbf16 ymm3{k2}, ymm6, 0x11
	vgetmantbf16 zmm5{k4}, zmm2, 0xab
	vgetmantbf16 xmm7{k4}{z}, xmm6, 0x96
	vgetmantbf16 ymm7{k4}{z}, ymm5, 0x2
	vgetmantbf16 zmm2{k1}{z}, zmm0, 0x24
	vgetmantbf16 xmm4, oword [eax+1], 0x16
	vgetmantbf16 xmm7, oword [eax+64], 0xbd
	vgetmantbf16 ymm6, yword [eax+1], 0xe2
	vgetmantbf16 ymm1, yword [eax+64], 0x1
	vgetmantbf16 zmm3, zword [eax+1], 0xa3
	vgetmantbf16 zmm2, zword [eax+64], 0xd6
	vgetmantbf16 xmm7, [0xd70], 0xb1
	vgetmantbf16 ymm6, [0x532], 0x44
	vgetmantbf16 zmm4, [0x3de], 0x3d
