	pmulhuw mm3, mm0
	pmulhuw mm6, mm3
	pmulhuw xmm5, xmm2
	pmulhuw xmm1, oword [0x6a0]

%ifdef ERROR
	pmulhuw xmm9, xmm10
%endif
