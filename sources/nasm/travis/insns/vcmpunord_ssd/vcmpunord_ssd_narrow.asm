	vcmpunord_ssd xmm4, xmm6, qword [0x42f]
	vcmpunord_ssd xmm4, qword [0x42f]
	vcmpunord_ssd xmm6, xmm2, qword [0xb05]
	vcmpunord_ssd k2, xmm3, xmm5
	vcmpunord_ssd k4, xmm4, xmm6

%ifdef ERROR
	vcmpunord_ssd xmm11, xmm14, xmm15
	vcmpunord_ssd k4, xmm13, xmm8
%endif
