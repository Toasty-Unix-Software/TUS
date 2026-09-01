	vpcompressw oword [0x2be], xmm7
	vpcompressw oword [0xead], xmm1
	vpcompressw yword [0x7a3], ymm3
	vpcompressw yword [0x859], ymm4
	vpcompressw zword [0x1a6], zmm0
	vpcompressw zword [0x2df], zmm6
	vpcompressw xmm6, xmm4
	vpcompressw xmm6, xmm0

%ifdef ERROR
	vpcompressw oword [0x358], xmm12
	vpcompressw yword [0xd1c], ymm14
	vpcompressw zword [0xbd9], zmm14
	vpcompressw xmm15, xmm12
	vpcompressw oword [0xf68], xmm25
	vpcompressw yword [0x88c], ymm24
	vpcompressw zword [0x7b6], zmm18
	vpcompressw xmm28, xmm16
%endif
