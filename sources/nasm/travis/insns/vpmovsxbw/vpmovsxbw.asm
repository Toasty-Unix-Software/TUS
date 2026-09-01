default rel
	vpmovsxbw xmm2, qword [0x414]
	vpmovsxbw xmm0, xmm2
	vpmovsxbw ymm6, oword [0xc5d]
	vpmovsxbw ymm2, oword [0xff9]
	vpmovsxbw xmm4, xmm6
	vpmovsxbw xmm7, qword [0xeb1]
	vpmovsxbw ymm3, xmm0
	vpmovsxbw ymm0, xmm7
	vpmovsxbw xmm14, xmm8
	vpmovsxbw ymm14, xmm14
	vpmovsxbw xmm9, xmm13
	vpmovsxbw ymm10, xmm10
	vpmovsxbw xmm23, xmm20
	vpmovsxbw ymm18, xmm21
	vpmovsxbw xmm21, xmm19
	vpmovsxbw ymm21, xmm23
	vpmovsxbw xmm4{k1}, qword [0x9f5]
	vpmovsxbw ymm3{k6}, xmm2
	vpmovsxbw zmm3{k7}, ymm4
	vpmovsxbw xmm6{k4}{z}, xmm2
	vpmovsxbw ymm5{k7}{z}, xmm0
	vpmovsxbw zmm0{k1}{z}, ymm7
	vpmovsxbw xmm0, qword [eax+1]
	vpmovsxbw xmm0, qword [eax+64]
	vpmovsxbw ymm0, oword [eax+1]
	vpmovsxbw ymm3, oword [eax+64]
	vpmovsxbw xmm6, qword [eax+1]
	vpmovsxbw xmm2, qword [eax+64]
	vpmovsxbw ymm2, oword [eax+1]
	vpmovsxbw ymm1, oword [eax+64]
	vpmovsxbw zmm1, yword [eax+1]
	vpmovsxbw zmm4, yword [eax+64]
	vpmovsxbw xmm6, [0xfc7]
	vpmovsxbw ymm3, [0xa57]
	vpmovsxbw xmm7, [0x1d0]
	vpmovsxbw ymm5, [0xe98]
	vpmovsxbw zmm1, [0xe81]
