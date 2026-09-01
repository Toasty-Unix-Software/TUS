default rel
	vextractf32x8 ymm2, zmm4, 0xb0
	vextractf32x8 ymm2, zmm0, 0x5e
	vextractf32x8 yword [0xb36], zmm2, 0xc8
	vextractf32x8 yword [0x610], zmm0, 0x42
	vextractf32x8 ymm12, zmm15, 0x86
	vextractf32x8 yword [0x683], zmm9, 0x1e
	vextractf32x8 ymm17, zmm31, 0xcd
	vextractf32x8 yword [0x666], zmm24, 0x9d
	vextractf32x8 ymm4{k3}, zmm0, 0xb9
	vextractf32x8 yword [0x4a8]{k3}, zmm0, 0xa5
	vextractf32x8 yword [eax+1], zmm4, 0x7b
	vextractf32x8 yword [eax+64], zmm7, 0x39
	vextractf32x8 [0xd51], zmm3, 0xbe
