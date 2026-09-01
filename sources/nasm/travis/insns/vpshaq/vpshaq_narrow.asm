	vpshaq xmm6, xmm0, xmm5
	vpshaq xmm6, xmm5
	vpshaq xmm7, xmm0, xmm7
	vpshaq xmm1, xmm7, oword [0x436]
	vpshaq xmm1, oword [0x436]
	vpshaq xmm6, xmm1, xmm3

%ifdef ERROR
	vpshaq xmm10, xmm10, xmm8
	vpshaq xmm13, xmm9, xmm8
%endif
