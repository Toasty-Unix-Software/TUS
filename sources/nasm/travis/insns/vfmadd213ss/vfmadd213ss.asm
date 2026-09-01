default rel
	vfmadd213ss xmm7, xmm1, dword [0x2bd]
	vfmadd213ss xmm4, xmm1, xmm3
	vfmadd213ss xmm3, xmm2, dword [0x38c]
	vfmadd213ss xmm7, xmm6, xmm0
	vfmadd213ss xmm12, xmm13, xmm15
	vfmadd213ss xmm13, xmm14, xmm8
	vfmadd213ss xmm28, xmm30, xmm20
	vfmadd213ss xmm16, xmm16, xmm29
	vfmadd213ss xmm6{k6}, xmm0, dword [0xec1]
	vfmadd213ss xmm3{k1}{z}, xmm1, xmm0
	vfmadd213ss xmm6, xmm4, xmm6, {rz-sae}
	vfmadd213ss xmm1, xmm6, dword [eax+1]
	vfmadd213ss xmm7, xmm3, dword [eax+64]
	vfmadd213ss xmm2, xmm6, dword [eax+1]
	vfmadd213ss xmm3, xmm4, dword [eax+64]
	vfmadd213ss xmm5, xmm2, [0x371]
	vfmadd213ss xmm6, xmm0, [0xb27]
