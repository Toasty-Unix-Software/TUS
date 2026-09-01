default rel
	vphaddw xmm1, xmm0, xmm3
	vphaddw xmm1, xmm3
	vphaddw xmm4, xmm2, xmm6
	vphaddw ymm6, ymm4, ymm1
	vphaddw ymm6, ymm1
	vphaddw ymm0, ymm7, ymm3
	vphaddw xmm8, xmm15, xmm13
	vphaddw ymm13, ymm12, ymm10
	vphaddw xmm1, xmm0, oword [eax+1]
	vphaddw xmm2, xmm6, oword [eax+64]
	vphaddw ymm1, ymm0, yword [eax+1]
	vphaddw ymm0, ymm1, yword [eax+64]
	vphaddw xmm3, xmm5, [0xd1d]
	vphaddw ymm0, ymm3, [0x36b]
