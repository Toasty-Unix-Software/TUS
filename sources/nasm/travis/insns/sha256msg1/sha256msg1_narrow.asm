	sha256msg1 xmm4, oword [0xe35]
	sha256msg1 xmm4, oword [0xf14]

%ifdef ERROR
	sha256msg1 xmm12, xmm8
%endif
