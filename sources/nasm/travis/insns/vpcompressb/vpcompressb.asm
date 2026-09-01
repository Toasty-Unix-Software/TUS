default rel
	vpcompressb oword [0x35a], xmm3
	vpcompressb oword [0x22f], xmm7
	vpcompressb yword [0x161], ymm6
	vpcompressb yword [0x9dd], ymm2
	vpcompressb zword [0x972], zmm2
	vpcompressb zword [0x14f], zmm0
	vpcompressb xmm4, xmm2
	vpcompressb xmm6, xmm0
	vpcompressb oword [0xbee], xmm8
	vpcompressb yword [0x25a], ymm8
	vpcompressb zword [0x6ee], zmm9
	vpcompressb xmm9, xmm13
	vpcompressb oword [0xab5], xmm30
	vpcompressb yword [0xb23], ymm18
	vpcompressb zword [0xc7b], zmm21
	vpcompressb xmm16, xmm17
	vpcompressb oword [0xab9]{k4}, xmm6
	vpcompressb yword [0x2df]{k5}, ymm7
	vpcompressb zword [0x693]{k4}, zmm6
	vpcompressb xmm3{k2}, xmm4
	vpcompressb ymm6{k4}, ymm1
	vpcompressb zmm0{k5}, zmm0
	vpcompressb oword [eax+1], xmm0
	vpcompressb oword [eax+64], xmm6
	vpcompressb yword [eax+1], ymm2
	vpcompressb yword [eax+64], ymm5
	vpcompressb zword [eax+1], zmm0
	vpcompressb zword [eax+64], zmm6
	vpcompressb [0x943], xmm6
	vpcompressb [0xbcd], ymm5
	vpcompressb [0x3ea], zmm1
