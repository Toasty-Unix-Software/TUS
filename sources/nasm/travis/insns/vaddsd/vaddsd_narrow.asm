	vaddsd xmm1, xmm5, qword [0x3d5]
	vaddsd xmm1, qword [0x3d5]
	vaddsd xmm0, xmm2, qword [0x237]
	vaddsd xmm2, xmm5, xmm4
	vaddsd xmm2, xmm4
	vaddsd xmm3, xmm0, xmm2

%ifdef ERROR
	vaddsd xmm11, xmm10, xmm9
	vaddsd xmm15, xmm8, xmm11
	vaddsd xmm29, xmm22, xmm16
	vaddsd xmm26, xmm26, xmm20
%endif
