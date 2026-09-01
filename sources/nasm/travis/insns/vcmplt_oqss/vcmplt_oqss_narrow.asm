	vcmplt_oqss xmm2, xmm7, xmm1
	vcmplt_oqss xmm2, xmm1
	vcmplt_oqss xmm3, xmm3, qword [0x661]
	vcmplt_oqss k4, xmm7, xmm4
	vcmplt_oqss k6, xmm0, dword [0x90a]

%ifdef ERROR
	vcmplt_oqss xmm9, xmm11, xmm11
	vcmplt_oqss k6, xmm8, xmm15
%endif
