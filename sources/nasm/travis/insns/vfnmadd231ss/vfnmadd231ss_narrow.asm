	vfnmadd231ss xmm0, xmm5, xmm1
	vfnmadd231ss xmm0, xmm5, xmm1
	vfnmadd231ss xmm3, xmm6, xmm0
	vfnmadd231ss xmm2, xmm2, xmm2

%ifdef ERROR
	vfnmadd231ss xmm13, xmm9, xmm9
	vfnmadd231ss xmm14, xmm9, xmm14
	vfnmadd231ss xmm16, xmm22, xmm18
	vfnmadd231ss xmm16, xmm17, xmm19
%endif
