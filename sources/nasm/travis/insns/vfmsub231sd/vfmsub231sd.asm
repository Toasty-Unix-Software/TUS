default rel
	vfmsub231sd xmm0, xmm0, xmm1
	vfmsub231sd xmm4, xmm6, xmm1
	vfmsub231sd xmm3, xmm5, xmm6
	vfmsub231sd xmm7, xmm1, qword [0xc75]
	vfmsub231sd xmm12, xmm13, xmm13
	vfmsub231sd xmm8, xmm11, xmm14
	vfmsub231sd xmm18, xmm18, xmm21
	vfmsub231sd xmm21, xmm30, xmm19
	vfmsub231sd xmm7{k1}, xmm5, xmm1
	vfmsub231sd xmm4{k1}{z}, xmm6, qword [0x784]
	vfmsub231sd xmm4, xmm5, xmm6, {ru-sae}
	vfmsub231sd xmm1, xmm5, qword [eax+1]
	vfmsub231sd xmm1, xmm5, qword [eax+64]
	vfmsub231sd xmm6, xmm6, qword [eax+1]
	vfmsub231sd xmm1, xmm3, qword [eax+64]
	vfmsub231sd xmm5, xmm7, [0x46c]
	vfmsub231sd xmm4, xmm2, [0x24f]
