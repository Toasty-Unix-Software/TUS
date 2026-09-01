	vcmpgtss xmm6, xmm0, qword [0x167]
	vcmpgtss xmm6, qword [0x167]
	vcmpgtss xmm1, xmm6, xmm2
	vcmpgtss k4, xmm5, xmm2
	vcmpgtss k1, xmm7, xmm1

%ifdef ERROR
	vcmpgtss xmm13, xmm11, xmm8
	vcmpgtss k5, xmm15, xmm14
%endif
