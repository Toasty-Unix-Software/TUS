default rel
	vunpckhpd xmm6, xmm1, xmm6
	vunpckhpd xmm6, xmm6
	vunpckhpd xmm7, xmm7, xmm5
	vunpckhpd ymm1, ymm4, yword [0x335]
	vunpckhpd ymm1, yword [0x335]
	vunpckhpd ymm2, ymm5, ymm1
	vunpckhpd xmm5, xmm2, xmm1
	vunpckhpd xmm5, xmm1
	vunpckhpd xmm5, xmm4, xmm1
	vunpckhpd ymm5, ymm4, ymm4
	vunpckhpd ymm5, ymm4
	vunpckhpd ymm6, ymm2, ymm3
	vunpckhpd xmm13, xmm15, xmm11
	vunpckhpd ymm8, ymm14, ymm15
	vunpckhpd xmm10, xmm12, xmm13
	vunpckhpd ymm10, ymm9, ymm11
	vunpckhpd xmm30, xmm24, xmm30
	vunpckhpd ymm27, ymm17, ymm20
	vunpckhpd xmm22, xmm18, xmm16
	vunpckhpd ymm30, ymm28, ymm30
	vunpckhpd xmm2{k5}, xmm3, oword [0x9ab]
	vunpckhpd ymm5{k7}, ymm1, yword [0xbe8]
	vunpckhpd zmm6{k3}, zmm4, zword [0xfcd]
	vunpckhpd xmm3{k5}{z}, xmm2, xmm5
	vunpckhpd ymm0{k1}{z}, ymm6, yword [0x361]
	vunpckhpd zmm6{k7}{z}, zmm6, zword [0xcb9]
	vunpckhpd xmm2, xmm0, oword [eax+1]
	vunpckhpd xmm4, xmm5, oword [eax+64]
	vunpckhpd ymm3, ymm7, yword [eax+1]
	vunpckhpd ymm3, ymm1, yword [eax+64]
	vunpckhpd xmm1, xmm1, oword [eax+1]
	vunpckhpd xmm1, xmm4, oword [eax+64]
	vunpckhpd ymm5, ymm7, yword [eax+1]
	vunpckhpd ymm4, ymm3, yword [eax+64]
	vunpckhpd zmm5, zmm6, zword [eax+1]
	vunpckhpd zmm4, zmm2, zword [eax+64]
	vunpckhpd xmm2, xmm2, [0x2e8]
	vunpckhpd ymm1, ymm7, [0xd8d]
	vunpckhpd xmm7, xmm6, [0x7de]
	vunpckhpd ymm6, ymm3, [0x7e0]
	vunpckhpd zmm7, zmm1, [0x163]
