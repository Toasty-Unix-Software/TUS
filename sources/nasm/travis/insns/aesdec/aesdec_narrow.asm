	aesdec xmm0, oword [0x639]
	aesdec xmm4, xmm5

%ifdef ERROR
	aesdec xmm14, xmm8
%endif
