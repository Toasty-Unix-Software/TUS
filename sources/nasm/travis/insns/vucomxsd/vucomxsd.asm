default rel
	vucomxsd xmm1, xmm3
	vucomxsd xmm4, qword [0x38f]
	vucomxsd xmm15, xmm13
	vucomxsd xmm16, xmm22
	vucomxsd xmm1, xmm1, {sae}
	vucomxsd xmm0, qword [eax+1]
	vucomxsd xmm5, qword [eax+64]
	vucomxsd xmm1, [0x8cb]
