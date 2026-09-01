	vcmpge_ossd xmm3, xmm2, xmm3
	vcmpge_ossd xmm3, xmm3
	vcmpge_ossd xmm6, xmm5, xmm6
	vcmpge_ossd k4, xmm0, xmm2
	vcmpge_ossd k7, xmm6, qword [0x8a4]

%ifdef ERROR
	vcmpge_ossd xmm11, xmm8, xmm12
	vcmpge_ossd k3, xmm11, xmm10
%endif
