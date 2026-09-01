default rel
	vrndscalebf16 xmm4, xmm1, 0x5f
	vrndscalebf16 xmm6, xmm7, 0x85
	vrndscalebf16 ymm2, yword [0xb25], 0xf2
	vrndscalebf16 ymm0, ymm1, 0xae
	vrndscalebf16 zmm7, zword [0x6ea], 0x9d
	vrndscalebf16 zmm5, zmm0, 0x6c
	vrndscalebf16 xmm10, xmm8, 0xb4
	vrndscalebf16 ymm8, ymm15, 0x19
	vrndscalebf16 zmm14, zmm11, 0x18
	vrndscalebf16 xmm30, xmm28, 0x25
	vrndscalebf16 ymm22, ymm16, 0x5c
	vrndscalebf16 zmm20, zmm22, 0x3
	vrndscalebf16 xmm3{k2}, xmm3, 0x7e
	vrndscalebf16 ymm1{k6}, ymm2, 0xcb
	vrndscalebf16 zmm5{k3}, zmm7, 0xec
	vrndscalebf16 xmm4{k6}{z}, xmm7, 0x7a
	vrndscalebf16 ymm2{k7}{z}, yword [0x820], 0x24
	vrndscalebf16 zmm2{k4}{z}, zmm0, 0x2e
	vrndscalebf16 xmm4, oword [eax+1], 0x14
	vrndscalebf16 xmm0, oword [eax+64], 0xda
	vrndscalebf16 ymm6, yword [eax+1], 0xf5
	vrndscalebf16 ymm4, yword [eax+64], 0xe2
	vrndscalebf16 zmm0, zword [eax+1], 0x4f
	vrndscalebf16 zmm2, zword [eax+64], 0x84
	vrndscalebf16 xmm5, [0x59a], 0xe7
	vrndscalebf16 ymm6, [0x7c6], 0x4d
	vrndscalebf16 zmm3, [0xc62], 0xa2
