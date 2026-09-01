	vfnmadd321ss xmm0, xmm7, xmm1
	vfnmadd321ss xmm2, xmm3, dword [0x4c4]

%ifdef ERROR
	vfnmadd321ss xmm9, xmm14, xmm8
%endif
