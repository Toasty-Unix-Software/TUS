default rel
	vextracti32x4 xmm4, ymm0, 0x29
	vextracti32x4 xmm4, ymm0, 0x75
	vextracti32x4 xmm1, zmm2, 0xc4
	vextracti32x4 xmm4, zmm7, 0x3b
	vextracti32x4 oword [0xe73], ymm4, 0xc7
	vextracti32x4 oword [0x626], ymm5, 0x4b
	vextracti32x4 oword [0x6ea], zmm5, 0x7a
	vextracti32x4 oword [0x74a], zmm7, 0x30
	vextracti32x4 xmm8, ymm14, 0xc7
	vextracti32x4 xmm10, zmm11, 0x2a
	vextracti32x4 oword [0xf7f], ymm9, 0x23
	vextracti32x4 oword [0xcc4], zmm14, 0x8f
	vextracti32x4 xmm28, ymm31, 0x5a
	vextracti32x4 xmm21, zmm24, 0x21
	vextracti32x4 oword [0xe3b], ymm24, 0xb
	vextracti32x4 oword [0x120], zmm20, 0xc7
	vextracti32x4 xmm0{k1}, ymm1, 0x71
	vextracti32x4 xmm4{k3}, zmm2, 0x9a
	vextracti32x4 oword [0x28c]{k5}, ymm2, 0x55
	vextracti32x4 oword [0xe3f]{k1}, zmm5, 0xd
	vextracti32x4 oword [eax+1], ymm6, 0x6f
	vextracti32x4 oword [eax+64], ymm3, 0x93
	vextracti32x4 oword [eax+1], zmm2, 0xbd
	vextracti32x4 oword [eax+64], zmm3, 0x60
	vextracti32x4 [0xeab], ymm0, 0xc5
	vextracti32x4 [0xfb1], zmm1, 0x7d
