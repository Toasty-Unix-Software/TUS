default rel
	vfmsub132sd xmm1, xmm2, xmm6
	vfmsub132sd xmm0, xmm7, xmm7
	vfmsub132sd xmm5, xmm5, xmm2
	vfmsub132sd xmm0, xmm7, xmm3
	vfmsub132sd xmm10, xmm9, xmm10
	vfmsub132sd xmm10, xmm11, xmm8
	vfmsub132sd xmm21, xmm18, xmm19
	vfmsub132sd xmm23, xmm20, xmm28
	vfmsub132sd xmm1{k5}, xmm5, xmm1
	vfmsub132sd xmm6{k2}{z}, xmm4, xmm2
	vfmsub132sd xmm4, xmm0, xmm7, {rn-sae}
	vfmsub132sd xmm4, xmm7, qword [eax+1]
	vfmsub132sd xmm3, xmm7, qword [eax+64]
	vfmsub132sd xmm6, xmm7, qword [eax+1]
	vfmsub132sd xmm6, xmm4, qword [eax+64]
	vfmsub132sd xmm1, xmm0, [0x562]
	vfmsub132sd xmm2, xmm5, [0xe44]
