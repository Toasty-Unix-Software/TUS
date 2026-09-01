	vprotq xmm0, oword [0x15f], xmm6
	vprotq xmm0, xmm6
	vprotq xmm0, oword [0x5f9], xmm4
	vprotq xmm2, xmm4, oword [0x403]
	vprotq xmm2, oword [0x403]
	vprotq xmm5, xmm7, oword [0xf56]
	vprotq xmm6, xmm3, 0xbc
	vprotq xmm6, 0xbc
	vprotq xmm1, oword [0x7c3], 0x4d

%ifdef ERROR
	vprotq xmm8, xmm14, xmm11
	vprotq xmm11, xmm12, xmm8
	vprotq xmm13, xmm8, 0x9
%endif
