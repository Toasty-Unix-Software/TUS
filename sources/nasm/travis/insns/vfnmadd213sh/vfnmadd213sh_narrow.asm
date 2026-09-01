	vfnmadd213sh xmm2, xmm3, xmm4
	vfnmadd213sh xmm2, xmm4
	vfnmadd213sh xmm4, xmm6, word [0x3b6]

%ifdef ERROR
	vfnmadd213sh xmm12, xmm9, xmm14
	vfnmadd213sh xmm20, xmm18, xmm24
%endif
