	vfmsub321ss xmm6, xmm4, xmm4
	vfmsub321ss xmm0, xmm2, xmm0

%ifdef ERROR
	vfmsub321ss xmm8, xmm13, xmm12
%endif
