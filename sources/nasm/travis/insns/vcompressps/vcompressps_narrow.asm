	vcompressps oword [0x3f0], xmm3
	vcompressps oword [0xfa0], xmm1
	vcompressps yword [0xf85], ymm6
	vcompressps yword [0xc22], ymm3
	vcompressps zword [0x391], zmm1
	vcompressps zword [0xaf2], zmm0
	vcompressps xmm5, xmm1
	vcompressps xmm2, xmm7

%ifdef ERROR
	vcompressps oword [0x2cb], xmm12
	vcompressps yword [0x862], ymm13
	vcompressps zword [0xe1f], zmm11
	vcompressps xmm14, xmm14
	vcompressps oword [0xabc], xmm27
	vcompressps yword [0x153], ymm19
	vcompressps zword [0xe1c], zmm21
	vcompressps xmm27, xmm17
%endif
