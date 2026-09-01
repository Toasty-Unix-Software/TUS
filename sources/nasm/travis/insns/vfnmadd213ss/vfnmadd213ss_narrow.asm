	vfnmadd213ss xmm2, xmm7, xmm4
	vfnmadd213ss xmm6, xmm4, xmm4
	vfnmadd213ss xmm6, xmm3, dword [0x2ff]
	vfnmadd213ss xmm7, xmm3, dword [0x452]

%ifdef ERROR
	vfnmadd213ss xmm10, xmm12, xmm10
	vfnmadd213ss xmm14, xmm15, xmm9
	vfnmadd213ss xmm21, xmm22, xmm21
	vfnmadd213ss xmm23, xmm23, xmm26
%endif
