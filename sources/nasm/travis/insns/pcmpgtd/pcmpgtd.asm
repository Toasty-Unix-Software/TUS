default rel
	pcmpgtd mm5, mm4
	pcmpgtd mm6, mm2
	pcmpgtd xmm5, xmm2
	pcmpgtd xmm6, xmm3
	pcmpgtd xmm8, xmm8
	pcmpgtd mm1, qword [eax+1]
	pcmpgtd mm2, qword [eax+64]
	pcmpgtd xmm7, oword [eax+1]
	pcmpgtd xmm1, oword [eax+64]
	pcmpgtd mm2, [0x203]
	pcmpgtd xmm7, [0xd18]
