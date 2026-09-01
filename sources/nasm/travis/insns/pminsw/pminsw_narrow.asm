	pminsw mm0, mm0
	pminsw mm0, mm3
	pminsw xmm7, xmm0
	pminsw xmm6, oword [0x7fa]

%ifdef ERROR
	pminsw xmm8, xmm15
%endif
