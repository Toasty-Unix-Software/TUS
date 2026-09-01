	vcmpnlt_usss xmm1, xmm2, xmm3
	vcmpnlt_usss xmm1, xmm3
	vcmpnlt_usss xmm3, xmm4, qword [0x6e9]
	vcmpnlt_usss k3, xmm1, dword [0xd9d]
	vcmpnlt_usss k6, xmm2, xmm6

%ifdef ERROR
	vcmpnlt_usss xmm14, xmm9, xmm9
	vcmpnlt_usss k6, xmm11, xmm9
%endif
