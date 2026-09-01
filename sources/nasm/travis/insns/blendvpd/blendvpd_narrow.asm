	blendvpd xmm3, xmm3, xmm0
	blendvpd xmm7, oword [0x2fe], xmm0
	blendvpd xmm3, oword [0xab1]
	blendvpd xmm5, oword [0xcd6]

%ifdef ERROR
	blendvpd xmm12, xmm8, xmm0
	blendvpd xmm8, xmm8
%endif
