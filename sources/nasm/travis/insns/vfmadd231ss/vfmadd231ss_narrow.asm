	vfmadd231ss xmm0, xmm1, dword [0x388]
	vfmadd231ss xmm7, xmm4, xmm1
	vfmadd231ss xmm4, xmm6, dword [0x582]
	vfmadd231ss xmm7, xmm6, xmm3

%ifdef ERROR
	vfmadd231ss xmm14, xmm12, xmm11
	vfmadd231ss xmm13, xmm14, xmm8
	vfmadd231ss xmm17, xmm18, xmm16
	vfmadd231ss xmm24, xmm26, xmm25
%endif
