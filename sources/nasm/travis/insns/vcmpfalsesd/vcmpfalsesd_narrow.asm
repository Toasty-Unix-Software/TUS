	vcmpfalsesd xmm1, xmm3, qword [0x7b2]
	vcmpfalsesd xmm1, qword [0x7b2]
	vcmpfalsesd xmm4, xmm3, xmm5
	vcmpfalsesd k1, xmm7, xmm5
	vcmpfalsesd k5, xmm7, xmm1

%ifdef ERROR
	vcmpfalsesd xmm15, xmm10, xmm9
	vcmpfalsesd k1, xmm15, xmm14
%endif
