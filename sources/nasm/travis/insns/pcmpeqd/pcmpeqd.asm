default rel
	pcmpeqd mm4, qword [0x5d6]
	pcmpeqd mm6, qword [0xdae]
	pcmpeqd xmm0, oword [0x8eb]
	pcmpeqd xmm5, oword [0xee9]
	pcmpeqd xmm15, xmm9
	pcmpeqd mm1, qword [eax+1]
	pcmpeqd mm6, qword [eax+64]
	pcmpeqd xmm5, oword [eax+1]
	pcmpeqd xmm7, oword [eax+64]
	pcmpeqd mm5, [0x880]
	pcmpeqd xmm0, [0xe55]
