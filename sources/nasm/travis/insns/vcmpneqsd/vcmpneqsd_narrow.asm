	vcmpneqsd xmm0, xmm7, qword [0x185]
	vcmpneqsd xmm0, qword [0x185]
	vcmpneqsd xmm3, xmm1, xmm0
	vcmpneqsd k6, xmm1, qword [0x54b]
	vcmpneqsd k1, xmm1, xmm0

%ifdef ERROR
	vcmpneqsd xmm8, xmm11, xmm15
	vcmpneqsd k2, xmm11, xmm12
%endif
