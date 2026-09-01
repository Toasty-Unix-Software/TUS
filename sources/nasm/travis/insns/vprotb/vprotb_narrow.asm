	vprotb xmm3, oword [0x4bc], xmm2
	vprotb xmm3, xmm2
	vprotb xmm7, oword [0xdb1], xmm6
	vprotb xmm3, xmm3, xmm2
	vprotb xmm3, xmm2
	vprotb xmm0, xmm2, xmm3
	vprotb xmm5, oword [0xd3d], 0xd5
	vprotb xmm5, 0xd5
	vprotb xmm5, xmm7, 0x4e

%ifdef ERROR
	vprotb xmm9, xmm12, xmm11
	vprotb xmm14, xmm13, xmm14
	vprotb xmm11, xmm11, 0x45
%endif
