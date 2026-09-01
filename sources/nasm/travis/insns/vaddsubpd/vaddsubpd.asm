default rel
	vaddsubpd xmm1, xmm2, xmm2
	vaddsubpd xmm1, xmm2
	vaddsubpd xmm1, xmm3, xmm7
	vaddsubpd ymm3, ymm0, yword [0x58b]
	vaddsubpd ymm3, yword [0x58b]
	vaddsubpd ymm1, ymm4, ymm2
	vaddsubpd xmm8, xmm12, xmm12
	vaddsubpd ymm9, ymm10, ymm12
	vaddsubpd xmm2, xmm5, oword [eax+1]
	vaddsubpd xmm5, xmm0, oword [eax+64]
	vaddsubpd ymm6, ymm0, yword [eax+1]
	vaddsubpd ymm7, ymm6, yword [eax+64]
	vaddsubpd xmm5, xmm4, [0x787]
	vaddsubpd ymm1, ymm5, [0xd06]
