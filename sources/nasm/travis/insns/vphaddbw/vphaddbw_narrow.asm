	vphaddbw xmm6, xmm2
	vphaddbw xmm6
	vphaddbw xmm7, xmm2

%ifdef ERROR
	vphaddbw xmm8, xmm14
%endif
