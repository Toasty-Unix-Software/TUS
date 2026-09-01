default rel
	vpcompressw oword [0x2be], xmm7
	vpcompressw oword [0xead], xmm1
	vpcompressw yword [0x7a3], ymm3
	vpcompressw yword [0x859], ymm4
	vpcompressw zword [0x1a6], zmm0
	vpcompressw zword [0x2df], zmm6
	vpcompressw xmm6, xmm4
	vpcompressw xmm6, xmm0
	vpcompressw oword [0x358], xmm12
	vpcompressw yword [0xd1c], ymm14
	vpcompressw zword [0xbd9], zmm14
	vpcompressw xmm15, xmm12
	vpcompressw oword [0xf68], xmm25
	vpcompressw yword [0x88c], ymm24
	vpcompressw zword [0x7b6], zmm18
	vpcompressw xmm28, xmm16
	vpcompressw oword [0x5f8]{k5}, xmm6
	vpcompressw yword [0xe10]{k4}, ymm2
	vpcompressw zword [0xc48]{k5}, zmm5
	vpcompressw xmm2{k6}, xmm7
	vpcompressw ymm7{k1}, ymm4
	vpcompressw zmm7{k6}, zmm4
	vpcompressw oword [eax+1], xmm5
	vpcompressw oword [eax+64], xmm2
	vpcompressw yword [eax+1], ymm5
	vpcompressw yword [eax+64], ymm0
	vpcompressw zword [eax+1], zmm4
	vpcompressw zword [eax+64], zmm4
	vpcompressw [0x611], xmm4
	vpcompressw [0x720], ymm4
	vpcompressw [0xae8], zmm7
