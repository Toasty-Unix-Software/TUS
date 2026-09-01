	vgetmantsd xmm1, xmm3, xmm6, 0x8d
	vgetmantsd xmm4, xmm4, xmm0, 0x29

%ifdef ERROR
	vgetmantsd xmm8, xmm10, xmm8, 0x3f
	vgetmantsd xmm30, xmm27, xmm27, 0x76
%endif
