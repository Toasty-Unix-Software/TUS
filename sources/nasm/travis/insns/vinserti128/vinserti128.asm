default rel
	vinserti128 ymm7, ymm1, oword [0x461], 0xfd
	vinserti128 ymm7, oword [0x461], 0xfd
	vinserti128 ymm1, ymm3, xmm0, 0xb
	vinserti128 ymm14, ymm11, xmm15, 0xa3
	vinserti128 ymm2, ymm0, oword [eax+1], 0xa5
	vinserti128 ymm0, ymm1, oword [eax+64], 0x9a
	vinserti128 ymm3, ymm3, [0xd20], 0x42
