default rel
	vpsravw xmm4, xmm6, xmm0
	vpsravw xmm4, xmm0
	vpsravw xmm7, xmm6, xmm7
	vpsravw ymm2, ymm3, ymm2
	vpsravw ymm2, ymm2
	vpsravw ymm1, ymm1, yword [0x115]
	vpsravw zmm2, zmm3, zword [0x180]
	vpsravw zmm2, zword [0x180]
	vpsravw zmm5, zmm3, zword [0x2b8]
	vpsravw xmm13, xmm15, xmm13
	vpsravw ymm8, ymm15, ymm8
	vpsravw zmm8, zmm14, zmm14
	vpsravw xmm19, xmm28, xmm18
	vpsravw ymm24, ymm28, ymm21
	vpsravw zmm20, zmm21, zmm26
	vpsravw xmm4{k5}, xmm1, xmm3
	vpsravw ymm7{k2}, ymm6, ymm1
	vpsravw zmm4{k2}, zmm0, zword [0xce5]
	vpsravw xmm0{k7}{z}, xmm3, xmm2
	vpsravw ymm5{k2}{z}, ymm2, ymm5
	vpsravw zmm4{k4}{z}, zmm5, zword [0x35e]
	vpsravw xmm5, xmm5, oword [eax+1]
	vpsravw xmm2, xmm3, oword [eax+64]
	vpsravw ymm1, ymm5, yword [eax+1]
	vpsravw ymm0, ymm4, yword [eax+64]
	vpsravw zmm3, zmm2, zword [eax+1]
	vpsravw zmm4, zmm4, zword [eax+64]
	vpsravw xmm3, xmm7, [0xdcc]
	vpsravw ymm0, ymm2, [0xded]
	vpsravw zmm4, zmm2, [0xf20]
