default rel
	phsubd mm3, qword [0x3ed]
	phsubd mm5, qword [0x208]
	phsubd xmm3, xmm7
	phsubd xmm5, xmm2
	phsubd xmm12, xmm15
	phsubd mm6, qword [eax+1]
	phsubd mm5, qword [eax+64]
	phsubd xmm2, oword [eax+1]
	phsubd xmm5, oword [eax+64]
	phsubd mm1, [0xcab]
	phsubd xmm7, [0x86f]
