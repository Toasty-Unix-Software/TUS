	vfmadd213ss xmm7, xmm1, dword [0x2bd]
	vfmadd213ss xmm4, xmm1, xmm3
	vfmadd213ss xmm3, xmm2, dword [0x38c]
	vfmadd213ss xmm7, xmm6, xmm0

%ifdef ERROR
	vfmadd213ss xmm12, xmm13, xmm15
	vfmadd213ss xmm13, xmm14, xmm8
	vfmadd213ss xmm28, xmm30, xmm20
	vfmadd213ss xmm16, xmm16, xmm29
%endif
