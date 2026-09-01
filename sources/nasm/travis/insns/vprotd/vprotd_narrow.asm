	vprotd xmm5, oword [0xd6a], xmm3
	vprotd xmm5, xmm3
	vprotd xmm6, xmm6, xmm3
	vprotd xmm2, xmm6, oword [0xddc]
	vprotd xmm2, oword [0xddc]
	vprotd xmm4, xmm4, xmm0
	vprotd xmm1, xmm6, 0xa9
	vprotd xmm1, 0xa9
	vprotd xmm3, xmm7, 0xb

%ifdef ERROR
	vprotd xmm14, xmm12, xmm8
	vprotd xmm12, xmm14, xmm13
	vprotd xmm10, xmm9, 0x8
%endif
