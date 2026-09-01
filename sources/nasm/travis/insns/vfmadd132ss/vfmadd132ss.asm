default rel
	vfmadd132ss xmm1, xmm2, dword [0x1ec]
	vfmadd132ss xmm2, xmm6, xmm7
	vfmadd132ss xmm2, xmm0, xmm7
	vfmadd132ss xmm3, xmm6, dword [0xf3b]
	vfmadd132ss xmm11, xmm12, xmm13
	vfmadd132ss xmm13, xmm9, xmm9
	vfmadd132ss xmm16, xmm30, xmm29
	vfmadd132ss xmm18, xmm25, xmm23
	vfmadd132ss xmm6{k3}, xmm3, xmm3
	vfmadd132ss xmm5{k6}{z}, xmm1, xmm6
	vfmadd132ss xmm5, xmm3, xmm1, {ru-sae}
	vfmadd132ss xmm6, xmm1, dword [eax+1]
	vfmadd132ss xmm7, xmm5, dword [eax+64]
	vfmadd132ss xmm4, xmm4, dword [eax+1]
	vfmadd132ss xmm5, xmm3, dword [eax+64]
	vfmadd132ss xmm6, xmm7, [0xf31]
	vfmadd132ss xmm4, xmm5, [0x10f]
