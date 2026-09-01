	vcmpgess xmm1, xmm0, xmm0
	vcmpgess xmm1, xmm0
	vcmpgess xmm2, xmm6, xmm7
	vcmpgess k2, xmm0, dword [0xf47]
	vcmpgess k7, xmm4, xmm3

%ifdef ERROR
	vcmpgess xmm11, xmm14, xmm10
	vcmpgess k1, xmm13, xmm14
%endif
