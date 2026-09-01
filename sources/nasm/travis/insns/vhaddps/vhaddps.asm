default rel
	vhaddps xmm4, xmm4, xmm5
	vhaddps xmm4, xmm5
	vhaddps xmm3, xmm6, xmm3
	vhaddps ymm2, ymm5, yword [0xa8f]
	vhaddps ymm2, yword [0xa8f]
	vhaddps ymm7, ymm6, ymm3
	vhaddps xmm13, xmm13, xmm11
	vhaddps ymm11, ymm15, ymm10
	vhaddps xmm5, xmm4, oword [eax+1]
	vhaddps xmm6, xmm1, oword [eax+64]
	vhaddps ymm7, ymm6, yword [eax+1]
	vhaddps ymm4, ymm5, yword [eax+64]
	vhaddps xmm3, xmm2, [0xbe3]
	vhaddps ymm2, ymm1, [0xa5c]
