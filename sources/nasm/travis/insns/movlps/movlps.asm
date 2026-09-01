default rel
	movlps xmm3, qword [0x816]
	movlps xmm3, qword [0xf00]
	movlps qword [0x5f8], xmm3
	movlps qword [0x54f], xmm7
	movlps xmm14, qword [0x602]
	movlps qword [0x49e], xmm14
	movlps xmm2, qword [eax+1]
	movlps xmm3, qword [eax+64]
	movlps qword [eax+1], xmm4
	movlps qword [eax+64], xmm3
	movlps xmm7, [0x257]
	movlps [0x79d], xmm1
