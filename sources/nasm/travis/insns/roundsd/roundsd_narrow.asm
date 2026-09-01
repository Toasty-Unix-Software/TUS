	roundsd xmm7, xmm6, 0x2a
	roundsd xmm4, xmm7, 0x67

%ifdef ERROR
	roundsd xmm13, xmm8, 0xf4
%endif
