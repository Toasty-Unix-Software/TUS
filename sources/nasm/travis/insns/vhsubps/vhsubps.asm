default rel
	vhsubps xmm7, xmm0, xmm0
	vhsubps xmm7, xmm0
	vhsubps xmm4, xmm0, xmm5
	vhsubps ymm5, ymm7, ymm2
	vhsubps ymm5, ymm2
	vhsubps ymm4, ymm0, ymm2
	vhsubps xmm13, xmm15, xmm15
	vhsubps ymm15, ymm15, ymm13
	vhsubps xmm3, xmm5, oword [eax+1]
	vhsubps xmm3, xmm1, oword [eax+64]
	vhsubps ymm4, ymm2, yword [eax+1]
	vhsubps ymm3, ymm7, yword [eax+64]
	vhsubps xmm7, xmm1, [0x179]
	vhsubps ymm2, ymm1, [0x38b]
