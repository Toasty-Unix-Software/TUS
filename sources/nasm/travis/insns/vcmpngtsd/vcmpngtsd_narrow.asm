	vcmpngtsd xmm5, xmm4, xmm4
	vcmpngtsd xmm5, xmm4
	vcmpngtsd xmm3, xmm1, qword [0x431]
	vcmpngtsd k5, xmm2, xmm0
	vcmpngtsd k6, xmm7, xmm7

%ifdef ERROR
	vcmpngtsd xmm13, xmm8, xmm12
	vcmpngtsd k5, xmm12, xmm13
%endif
