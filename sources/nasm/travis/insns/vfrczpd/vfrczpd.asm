default rel
	vfrczpd xmm3, xmm0
	vfrczpd xmm3
	vfrczpd xmm1, xmm7
	vfrczpd ymm7, yword [0xa82]
	vfrczpd ymm7
	vfrczpd ymm6, ymm5
	vfrczpd xmm8, xmm8
	vfrczpd ymm15, ymm11
	vfrczpd xmm2, oword [eax+1]
	vfrczpd xmm3, oword [eax+64]
	vfrczpd ymm0, yword [eax+1]
	vfrczpd ymm3, yword [eax+64]
	vfrczpd xmm0, [0xd5f]
	vfrczpd ymm2, [0x834]
