default rel
	psrld mm4, mm6
	psrld mm5, mm2
	psrld mm2, 0xf
	psrld mm4, 0x7b
	psrld xmm6, xmm1
	psrld xmm2, xmm3
	psrld xmm7, 0x63
	psrld xmm7, 0x2c
	psrld xmm13, xmm11
	psrld xmm12, 0x64
	psrld mm5, qword [eax+1]
	psrld mm1, qword [eax+64]
	psrld xmm0, oword [eax+1]
	psrld xmm1, oword [eax+64]
	psrld mm2, [0xb5e]
	psrld xmm1, [0x851]
