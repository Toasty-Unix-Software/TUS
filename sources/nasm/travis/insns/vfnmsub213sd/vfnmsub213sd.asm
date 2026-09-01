default rel
	vfnmsub213sd xmm1, xmm0, qword [0xee0]
	vfnmsub213sd xmm2, xmm4, xmm4
	vfnmsub213sd xmm3, xmm0, xmm5
	vfnmsub213sd xmm3, xmm6, xmm7
	vfnmsub213sd xmm15, xmm12, xmm15
	vfnmsub213sd xmm8, xmm12, xmm12
	vfnmsub213sd xmm20, xmm22, xmm26
	vfnmsub213sd xmm25, xmm20, xmm28
	vfnmsub213sd xmm0{k2}, xmm6, qword [0x72f]
	vfnmsub213sd xmm0{k1}{z}, xmm3, qword [0xe0b]
	vfnmsub213sd xmm0, xmm4, xmm3, {ru-sae}
	vfnmsub213sd xmm4, xmm6, qword [eax+1]
	vfnmsub213sd xmm7, xmm6, qword [eax+64]
	vfnmsub213sd xmm1, xmm2, qword [eax+1]
	vfnmsub213sd xmm6, xmm4, qword [eax+64]
	vfnmsub213sd xmm5, xmm5, [0x40f]
	vfnmsub213sd xmm7, xmm1, [0x9ba]
