	vcmptruesd xmm1, xmm4, xmm7
	vcmptruesd xmm1, xmm7
	vcmptruesd xmm2, xmm3, qword [0xa5b]
	vcmptruesd k7, xmm7, xmm0
	vcmptruesd k4, xmm5, xmm4

%ifdef ERROR
	vcmptruesd xmm12, xmm11, xmm12
	vcmptruesd k3, xmm12, xmm10
%endif
