	vfmsub123sd xmm0, xmm5, xmm7
	vfmsub123sd xmm4, xmm1, xmm2

%ifdef ERROR
	vfmsub123sd xmm14, xmm13, xmm13
%endif
