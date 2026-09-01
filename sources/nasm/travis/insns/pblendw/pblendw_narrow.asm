	pblendw xmm0, xmm1, 0xab
	pblendw xmm5, oword [0xe8c], 0x7b

%ifdef ERROR
	pblendw xmm12, xmm11, 0x18
%endif
