	vfnmadd132ss xmm5, xmm0, dword [0x896]
	vfnmadd132ss xmm7, xmm1, xmm5
	vfnmadd132ss xmm1, xmm3, xmm1
	vfnmadd132ss xmm2, xmm1, dword [0xe03]

%ifdef ERROR
	vfnmadd132ss xmm14, xmm11, xmm9
	vfnmadd132ss xmm12, xmm8, xmm8
	vfnmadd132ss xmm17, xmm23, xmm22
	vfnmadd132ss xmm30, xmm30, xmm26
%endif
