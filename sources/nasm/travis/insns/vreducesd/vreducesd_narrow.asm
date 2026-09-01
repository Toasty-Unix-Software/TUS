	vreducesd xmm4, xmm5, xmm0, 0x77
	vreducesd xmm4, xmm0, 0x77
	vreducesd xmm6, xmm7, xmm3, 0xcc

%ifdef ERROR
	vreducesd xmm13, xmm10, xmm13, 0x3c
	vreducesd xmm31, xmm23, xmm18, 0x29
%endif
