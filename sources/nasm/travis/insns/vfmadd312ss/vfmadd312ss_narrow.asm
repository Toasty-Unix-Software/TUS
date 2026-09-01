	vfmadd312ss xmm0, xmm7, dword [0x62c]
	vfmadd312ss xmm7, xmm7, xmm7

%ifdef ERROR
	vfmadd312ss xmm11, xmm8, xmm14
%endif
