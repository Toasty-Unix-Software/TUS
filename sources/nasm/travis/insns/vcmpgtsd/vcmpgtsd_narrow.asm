	vcmpgtsd xmm1, xmm6, qword [0xf5a]
	vcmpgtsd xmm1, qword [0xf5a]
	vcmpgtsd xmm0, xmm7, xmm0
	vcmpgtsd k4, xmm1, xmm3
	vcmpgtsd k5, xmm4, qword [0x514]

%ifdef ERROR
	vcmpgtsd xmm12, xmm10, xmm11
	vcmpgtsd k4, xmm15, xmm11
%endif
