	vcmpordsd xmm1, xmm7, qword [0xedf]
	vcmpordsd xmm1, qword [0xedf]
	vcmpordsd xmm3, xmm2, qword [0x1a0]
	vcmpordsd k4, xmm0, qword [0xef1]
	vcmpordsd k4, xmm2, xmm2

%ifdef ERROR
	vcmpordsd xmm15, xmm15, xmm10
	vcmpordsd k2, xmm8, xmm9
%endif
