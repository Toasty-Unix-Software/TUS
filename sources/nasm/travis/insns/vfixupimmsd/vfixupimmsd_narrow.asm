	vfixupimmsd xmm2, xmm2, xmm6, 0xa5
	vfixupimmsd xmm2, xmm6, 0xa5
	vfixupimmsd xmm6, xmm1, qword [0xa3e], 0xbb

%ifdef ERROR
	vfixupimmsd xmm15, xmm11, xmm13, 0x32
	vfixupimmsd xmm17, xmm28, xmm19, 0xc8
%endif
