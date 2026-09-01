	vfixupimmss xmm4, xmm2, dword [0x998], 0x9c
	vfixupimmss xmm4, dword [0x998], 0x9c
	vfixupimmss xmm0, xmm0, xmm6, 0x77

%ifdef ERROR
	vfixupimmss xmm15, xmm14, xmm11, 0xf6
	vfixupimmss xmm18, xmm30, xmm16, 0x6a
%endif
