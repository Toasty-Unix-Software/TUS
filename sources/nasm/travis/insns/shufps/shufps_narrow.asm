	shufps xmm4, xmm2, 0x3e
	shufps xmm5, xmm7, 0x64

%ifdef ERROR
	shufps xmm14, xmm13, 0x4f
%endif
