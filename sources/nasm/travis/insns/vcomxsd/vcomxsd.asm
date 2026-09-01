default rel
	vcomxsd xmm5, xmm7
	vcomxsd xmm4, xmm7
	vcomxsd xmm12, xmm13
	vcomxsd xmm25, xmm18
	vcomxsd xmm4, xmm2, {sae}
	vcomxsd xmm0, qword [eax+1]
	vcomxsd xmm4, qword [eax+64]
	vcomxsd xmm1, [0x8ab]
