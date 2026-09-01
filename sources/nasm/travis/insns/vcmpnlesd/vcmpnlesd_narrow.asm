	vcmpnlesd xmm6, xmm7, xmm3
	vcmpnlesd xmm6, xmm3
	vcmpnlesd xmm4, xmm0, xmm3
	vcmpnlesd k6, xmm6, xmm3
	vcmpnlesd k4, xmm4, xmm4

%ifdef ERROR
	vcmpnlesd xmm14, xmm9, xmm14
	vcmpnlesd k5, xmm13, xmm9
%endif
