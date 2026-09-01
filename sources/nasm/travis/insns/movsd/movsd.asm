default rel
	movsd
	movsd
	movsd xmm0, qword [0xd65]
	movsd xmm1, qword [0x792]
	movsd qword [0x3c8], xmm0
	movsd qword [0xf2d], xmm6
	movsd xmm14, xmm10
	movsd xmm14, xmm13
	movsd xmm0, qword [eax+1]
	movsd xmm1, qword [eax+64]
	movsd qword [eax+1], xmm4
	movsd qword [eax+64], xmm2
	movsd xmm1, [0x3b2]
	movsd [0x10e], xmm7
