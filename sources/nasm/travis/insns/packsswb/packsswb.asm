default rel
	packsswb mm5, mm2
	packsswb mm1, mm0
	packsswb xmm7, oword [0xd0c]
	packsswb xmm5, xmm1
	packsswb xmm8, xmm13
	packsswb mm1, qword [eax+1]
	packsswb mm2, qword [eax+64]
	packsswb xmm5, oword [eax+1]
	packsswb xmm4, oword [eax+64]
	packsswb mm5, [0xbd3]
	packsswb xmm4, [0x59b]
