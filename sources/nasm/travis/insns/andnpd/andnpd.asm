default rel
	andnpd xmm1, oword [0x780]
	andnpd xmm2, oword [0xaef]
	andnpd xmm15, xmm13
	andnpd xmm5, oword [eax+1]
	andnpd xmm7, oword [eax+64]
	andnpd xmm2, [0x60c]
