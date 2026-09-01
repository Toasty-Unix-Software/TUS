	vextracti32x4 xmm4, ymm0, 0x29
	vextracti32x4 xmm4, ymm0, 0x75
	vextracti32x4 xmm1, zmm2, 0xc4
	vextracti32x4 xmm4, zmm7, 0x3b
	vextracti32x4 oword [0xe73], ymm4, 0xc7
	vextracti32x4 oword [0x626], ymm5, 0x4b
	vextracti32x4 oword [0x6ea], zmm5, 0x7a
	vextracti32x4 oword [0x74a], zmm7, 0x30

%ifdef ERROR
	vextracti32x4 xmm8, ymm14, 0xc7
	vextracti32x4 xmm10, zmm11, 0x2a
	vextracti32x4 oword [0xf7f], ymm9, 0x23
	vextracti32x4 oword [0xcc4], zmm14, 0x8f
	vextracti32x4 xmm28, ymm31, 0x5a
	vextracti32x4 xmm21, zmm24, 0x21
	vextracti32x4 oword [0xe3b], ymm24, 0xb
	vextracti32x4 oword [0x120], zmm20, 0xc7
%endif
