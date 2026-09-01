	vcmpngesd xmm4, xmm1, xmm0
	vcmpngesd xmm4, xmm0
	vcmpngesd xmm3, xmm6, xmm7
	vcmpngesd k7, xmm5, qword [0xaf9]
	vcmpngesd k5, xmm0, xmm1

%ifdef ERROR
	vcmpngesd xmm9, xmm10, xmm13
	vcmpngesd k7, xmm10, xmm13
%endif
