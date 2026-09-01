default rel
	vcompressps oword [0x3f0], xmm3
	vcompressps oword [0xfa0], xmm1
	vcompressps yword [0xf85], ymm6
	vcompressps yword [0xc22], ymm3
	vcompressps zword [0x391], zmm1
	vcompressps zword [0xaf2], zmm0
	vcompressps xmm5, xmm1
	vcompressps xmm2, xmm7
	vcompressps oword [0x2cb], xmm12
	vcompressps yword [0x862], ymm13
	vcompressps zword [0xe1f], zmm11
	vcompressps xmm14, xmm14
	vcompressps oword [0xabc], xmm27
	vcompressps yword [0x153], ymm19
	vcompressps zword [0xe1c], zmm21
	vcompressps xmm27, xmm17
	vcompressps oword [0xc41]{k7}, xmm0
	vcompressps yword [0x7dd]{k3}, ymm0
	vcompressps zword [0xd66]{k5}, zmm5
	vcompressps xmm1{k7}, xmm4
	vcompressps ymm0{k5}, ymm2
	vcompressps zmm3{k2}, zmm4
	vcompressps oword [eax+1], xmm3
	vcompressps oword [eax+64], xmm0
	vcompressps yword [eax+1], ymm6
	vcompressps yword [eax+64], ymm3
	vcompressps zword [eax+1], zmm1
	vcompressps zword [eax+64], zmm7
	vcompressps [0x997], xmm1
	vcompressps [0x704], ymm1
	vcompressps [0x9a8], zmm1
