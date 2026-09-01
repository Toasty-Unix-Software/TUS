	vcmplt_ossd xmm6, xmm7, qword [0xb27]
	vcmplt_ossd xmm6, qword [0xb27]
	vcmplt_ossd xmm7, xmm7, xmm1
	vcmplt_ossd k3, xmm2, xmm7
	vcmplt_ossd k2, xmm5, qword [0x18f]

%ifdef ERROR
	vcmplt_ossd xmm10, xmm15, xmm13
	vcmplt_ossd k3, xmm14, xmm15
%endif
