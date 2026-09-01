default rel
	vpsignw xmm6, xmm6, xmm2
	vpsignw xmm6, xmm2
	vpsignw xmm4, xmm7, xmm1
	vpsignw ymm7, ymm4, yword [0x3c3]
	vpsignw ymm7, yword [0x3c3]
	vpsignw ymm5, ymm5, ymm4
	vpsignw xmm10, xmm10, xmm13
	vpsignw ymm10, ymm13, ymm15
	vpsignw xmm3, xmm4, oword [eax+1]
	vpsignw xmm3, xmm3, oword [eax+64]
	vpsignw ymm4, ymm3, yword [eax+1]
	vpsignw ymm7, ymm6, yword [eax+64]
	vpsignw xmm4, xmm6, [0xbc1]
	vpsignw ymm5, ymm1, [0xf58]
