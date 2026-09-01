	vpshlb xmm3, oword [0x4ba], xmm6
	vpshlb xmm3, xmm6
	vpshlb xmm6, xmm1, xmm6
	vpshlb xmm5, xmm3, xmm4
	vpshlb xmm5, xmm4
	vpshlb xmm0, xmm4, xmm4

%ifdef ERROR
	vpshlb xmm12, xmm9, xmm14
	vpshlb xmm11, xmm11, xmm11
%endif
