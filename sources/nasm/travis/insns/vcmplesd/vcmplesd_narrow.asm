	vcmplesd xmm6, xmm7, qword [0xb0b]
	vcmplesd xmm6, qword [0xb0b]
	vcmplesd xmm0, xmm1, xmm4
	vcmplesd k3, xmm4, xmm6
	vcmplesd k1, xmm2, xmm2

%ifdef ERROR
	vcmplesd xmm13, xmm12, xmm9
	vcmplesd k1, xmm15, xmm8
%endif
