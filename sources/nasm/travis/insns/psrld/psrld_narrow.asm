	psrld mm4, mm6
	psrld mm5, mm2
	psrld mm2, 0xf
	psrld mm4, 0x7b
	psrld xmm6, xmm1
	psrld xmm2, xmm3
	psrld xmm7, 0x63
	psrld xmm7, 0x2c

%ifdef ERROR
	psrld xmm13, xmm11
	psrld xmm12, 0x64
%endif
