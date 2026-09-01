default rel
	vrcpps xmm5, xmm5
	vrcpps xmm5, oword [0xedd]
	vrcpps ymm2, ymm2
	vrcpps ymm1, ymm5
	vrcpps xmm15, xmm13
	vrcpps ymm8, ymm12
	vrcpps xmm5, oword [eax+1]
	vrcpps xmm0, oword [eax+64]
	vrcpps ymm6, yword [eax+1]
	vrcpps ymm0, yword [eax+64]
	vrcpps xmm6, [0xe1b]
	vrcpps ymm1, [0x9a3]
