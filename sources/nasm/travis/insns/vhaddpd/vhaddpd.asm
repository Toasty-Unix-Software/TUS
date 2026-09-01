default rel
	vhaddpd xmm7, xmm4, oword [0x9cc]
	vhaddpd xmm7, oword [0x9cc]
	vhaddpd xmm4, xmm0, xmm3
	vhaddpd ymm1, ymm2, ymm4
	vhaddpd ymm1, ymm4
	vhaddpd ymm1, ymm2, ymm6
	vhaddpd xmm11, xmm14, xmm12
	vhaddpd ymm8, ymm15, ymm15
	vhaddpd xmm2, xmm6, oword [eax+1]
	vhaddpd xmm2, xmm4, oword [eax+64]
	vhaddpd ymm6, ymm3, yword [eax+1]
	vhaddpd ymm5, ymm1, yword [eax+64]
	vhaddpd xmm7, xmm4, [0x932]
	vhaddpd ymm4, ymm3, [0x97c]
