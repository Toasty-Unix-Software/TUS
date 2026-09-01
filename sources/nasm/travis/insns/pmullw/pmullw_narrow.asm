	pmullw mm7, mm1
	pmullw mm3, mm2
	pmullw xmm4, oword [0x4ef]
	pmullw xmm5, oword [0x512]

%ifdef ERROR
	pmullw xmm8, xmm14
%endif
