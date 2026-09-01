	vcmpunordsd xmm6, xmm7, xmm3
	vcmpunordsd xmm6, xmm3
	vcmpunordsd xmm7, xmm0, qword [0x799]
	vcmpunordsd k1, xmm5, xmm5
	vcmpunordsd k4, xmm0, qword [0x77e]

%ifdef ERROR
	vcmpunordsd xmm10, xmm14, xmm13
	vcmpunordsd k4, xmm10, xmm15
%endif
