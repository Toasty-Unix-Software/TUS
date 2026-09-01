default rel
	vextractf128 xmm0, ymm7, 0x28
	vextractf128 xmm0, ymm6, 0x3e
	vextractf128 xmm11, ymm11, 0x75
	vextractf128 oword [eax+1], ymm0, 0x98
	vextractf128 oword [eax+64], ymm2, 0xeb
	vextractf128 [0x1ef], ymm7, 0xd5
