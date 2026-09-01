	vfmadd132ss xmm1, xmm2, dword [0x1ec]
	vfmadd132ss xmm2, xmm6, xmm7
	vfmadd132ss xmm2, xmm0, xmm7
	vfmadd132ss xmm3, xmm6, dword [0xf3b]

%ifdef ERROR
	vfmadd132ss xmm11, xmm12, xmm13
	vfmadd132ss xmm13, xmm9, xmm9
	vfmadd132ss xmm16, xmm30, xmm29
	vfmadd132ss xmm18, xmm25, xmm23
%endif
