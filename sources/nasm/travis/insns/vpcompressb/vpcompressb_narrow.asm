	vpcompressb oword [0x35a], xmm3
	vpcompressb oword [0x22f], xmm7
	vpcompressb yword [0x161], ymm6
	vpcompressb yword [0x9dd], ymm2
	vpcompressb zword [0x972], zmm2
	vpcompressb zword [0x14f], zmm0
	vpcompressb xmm4, xmm2
	vpcompressb xmm6, xmm0

%ifdef ERROR
	vpcompressb oword [0xbee], xmm8
	vpcompressb yword [0x25a], ymm8
	vpcompressb zword [0x6ee], zmm9
	vpcompressb xmm9, xmm13
	vpcompressb oword [0xab5], xmm30
	vpcompressb yword [0xb23], ymm18
	vpcompressb zword [0xc7b], zmm21
	vpcompressb xmm16, xmm17
%endif
