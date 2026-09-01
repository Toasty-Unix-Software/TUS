	vcmpneq_ossd xmm5, xmm1, xmm3
	vcmpneq_ossd xmm5, xmm3
	vcmpneq_ossd xmm4, xmm4, qword [0xb67]
	vcmpneq_ossd k7, xmm6, xmm1
	vcmpneq_ossd k4, xmm7, xmm7

%ifdef ERROR
	vcmpneq_ossd xmm14, xmm10, xmm14
	vcmpneq_ossd k5, xmm9, xmm12
%endif
