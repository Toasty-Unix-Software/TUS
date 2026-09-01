	vfmadd321ss xmm6, xmm2, dword [0x3db]
	vfmadd321ss xmm7, xmm6, xmm7

%ifdef ERROR
	vfmadd321ss xmm14, xmm13, xmm10
%endif
