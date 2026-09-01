	vcmpgesd xmm3, xmm4, xmm3
	vcmpgesd xmm3, xmm3
	vcmpgesd xmm5, xmm6, xmm6
	vcmpgesd k7, xmm7, xmm4
	vcmpgesd k4, xmm4, qword [0x7b8]

%ifdef ERROR
	vcmpgesd xmm8, xmm10, xmm12
	vcmpgesd k1, xmm11, xmm10
%endif
