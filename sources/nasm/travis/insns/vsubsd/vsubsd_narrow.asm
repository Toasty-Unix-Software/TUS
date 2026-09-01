	vsubsd xmm6, xmm6, qword [0x11e]
	vsubsd xmm6, qword [0x11e]
	vsubsd xmm5, xmm0, xmm1
	vsubsd xmm4, xmm4, xmm1
	vsubsd xmm4, xmm1
	vsubsd xmm7, xmm3, xmm4

%ifdef ERROR
	vsubsd xmm10, xmm13, xmm11
	vsubsd xmm15, xmm14, xmm15
	vsubsd xmm30, xmm29, xmm30
	vsubsd xmm30, xmm16, xmm28
%endif
