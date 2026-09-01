	pblendvb xmm1, xmm7, xmm0
	pblendvb xmm1, xmm6, xmm0
	pblendvb xmm0, oword [0x893]
	pblendvb xmm1, oword [0x719]

%ifdef ERROR
	pblendvb xmm12, xmm9, xmm0
	pblendvb xmm10, xmm14
%endif
