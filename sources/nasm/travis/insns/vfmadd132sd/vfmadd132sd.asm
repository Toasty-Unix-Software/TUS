default rel
	vfmadd132sd xmm4, xmm3, qword [0x3db]
	vfmadd132sd xmm5, xmm6, xmm6
	vfmadd132sd xmm3, xmm5, xmm6
	vfmadd132sd xmm4, xmm6, xmm5
	vfmadd132sd xmm8, xmm11, xmm15
	vfmadd132sd xmm14, xmm8, xmm9
	vfmadd132sd xmm31, xmm27, xmm30
	vfmadd132sd xmm29, xmm24, xmm19
	vfmadd132sd xmm4{k2}, xmm2, xmm7
	vfmadd132sd xmm6{k6}{z}, xmm4, xmm3
	vfmadd132sd xmm7, xmm0, xmm5, {rn-sae}
	vfmadd132sd xmm5, xmm4, qword [eax+1]
	vfmadd132sd xmm2, xmm3, qword [eax+64]
	vfmadd132sd xmm7, xmm4, qword [eax+1]
	vfmadd132sd xmm2, xmm6, qword [eax+64]
	vfmadd132sd xmm0, xmm7, [0xb0a]
	vfmadd132sd xmm7, xmm2, [0x672]
