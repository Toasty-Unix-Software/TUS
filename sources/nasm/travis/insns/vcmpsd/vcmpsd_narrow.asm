	vcmpsd xmm6, xmm6, qword [0x8ea], 0x5b
	vcmpsd xmm6, qword [0x8ea], 0x5b
	vcmpsd xmm2, xmm2, xmm0, 0x5
	vcmpsd k5, xmm7, xmm7, 0x5e
	vcmpsd k6, xmm5, qword [0x2d6], 0xe8

%ifdef ERROR
	vcmpsd xmm13, xmm10, xmm8, 0x8b
	vcmpsd k7, xmm12, xmm11, 0x51
%endif
