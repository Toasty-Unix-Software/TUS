default rel
	vextracti64x2 xmm7, ymm4, 0xee
	vextracti64x2 xmm7, ymm6, 0x61
	vextracti64x2 xmm1, zmm7, 0xdc
	vextracti64x2 xmm1, zmm3, 0x8f
	vextracti64x2 oword [0xabf], ymm5, 0x1c
	vextracti64x2 oword [0xad8], ymm2, 0xbd
	vextracti64x2 oword [0x33b], zmm3, 0x22
	vextracti64x2 oword [0x2b2], zmm2, 0xa4
	vextracti64x2 xmm10, ymm11, 0x49
	vextracti64x2 xmm11, zmm12, 0x90
	vextracti64x2 oword [0xd89], ymm14, 0x91
	vextracti64x2 oword [0x229], zmm15, 0x30
	vextracti64x2 xmm29, ymm18, 0x79
	vextracti64x2 xmm16, zmm26, 0xc1
	vextracti64x2 oword [0x23f], ymm23, 0xa9
	vextracti64x2 oword [0x298], zmm17, 0x38
	vextracti64x2 xmm1{k3}, ymm5, 0xa0
	vextracti64x2 xmm6{k7}, zmm5, 0xd0
	vextracti64x2 oword [0xde6]{k6}, ymm4, 0x75
	vextracti64x2 oword [0x3f9]{k6}, zmm6, 0xdf
	vextracti64x2 oword [eax+1], ymm3, 0x23
	vextracti64x2 oword [eax+64], ymm3, 0xf2
	vextracti64x2 oword [eax+1], zmm6, 0x88
	vextracti64x2 oword [eax+64], zmm6, 0x36
	vextracti64x2 [0x570], ymm7, 0xea
	vextracti64x2 [0x20f], zmm2, 0xb3
