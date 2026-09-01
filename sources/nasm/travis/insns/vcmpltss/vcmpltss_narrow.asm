	vcmpltss xmm4, xmm7, qword [0x6ad]
	vcmpltss xmm4, qword [0x6ad]
	vcmpltss xmm2, xmm5, xmm0
	vcmpltss k1, xmm5, xmm3
	vcmpltss k7, xmm7, dword [0x5e3]

%ifdef ERROR
	vcmpltss xmm9, xmm12, xmm8
	vcmpltss k1, xmm11, xmm15
%endif
