	movups xmm5, xmm7
	movups xmm3, oword [0xa2f]
	movups oword [0xee1], xmm6
	movups xmm2, xmm7

%ifdef ERROR
	movups xmm14, xmm13
	movups xmm8, xmm8
%endif
