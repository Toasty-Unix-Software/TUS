default rel
	packusdw xmm5, xmm1
	packusdw xmm4, xmm1
	packusdw xmm9, xmm9
	packusdw xmm0, oword [eax+1]
	packusdw xmm0, oword [eax+64]
	packusdw xmm1, [0x204]
