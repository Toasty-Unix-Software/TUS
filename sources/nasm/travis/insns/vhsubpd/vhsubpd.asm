default rel
	vhsubpd xmm5, xmm1, xmm7
	vhsubpd xmm5, xmm7
	vhsubpd xmm2, xmm7, xmm7
	vhsubpd ymm3, ymm1, ymm5
	vhsubpd ymm3, ymm5
	vhsubpd ymm3, ymm6, ymm3
	vhsubpd xmm9, xmm10, xmm15
	vhsubpd ymm10, ymm10, ymm15
	vhsubpd xmm5, xmm5, oword [eax+1]
	vhsubpd xmm1, xmm2, oword [eax+64]
	vhsubpd ymm0, ymm5, yword [eax+1]
	vhsubpd ymm6, ymm5, yword [eax+64]
	vhsubpd xmm0, xmm2, [0xbf2]
	vhsubpd ymm2, ymm6, [0xa81]
