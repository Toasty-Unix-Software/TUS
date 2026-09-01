	vprotw xmm1, oword [0x51d], xmm5
	vprotw xmm1, xmm5
	vprotw xmm1, oword [0x30a], xmm5
	vprotw xmm0, xmm3, oword [0xc2a]
	vprotw xmm0, oword [0xc2a]
	vprotw xmm1, xmm3, xmm6
	vprotw xmm7, xmm4, 0x50
	vprotw xmm7, 0x50
	vprotw xmm6, xmm1, 0x21

%ifdef ERROR
	vprotw xmm12, xmm10, xmm14
	vprotw xmm13, xmm13, xmm13
	vprotw xmm12, xmm14, 0xea
%endif
