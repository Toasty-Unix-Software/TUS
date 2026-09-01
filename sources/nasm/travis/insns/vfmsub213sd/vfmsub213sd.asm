default rel
	vfmsub213sd xmm2, xmm6, xmm3
	vfmsub213sd xmm1, xmm5, qword [0x9e1]
	vfmsub213sd xmm2, xmm0, qword [0xab6]
	vfmsub213sd xmm0, xmm3, xmm0
	vfmsub213sd xmm12, xmm13, xmm12
	vfmsub213sd xmm15, xmm13, xmm11
	vfmsub213sd xmm28, xmm30, xmm28
	vfmsub213sd xmm24, xmm30, xmm16
	vfmsub213sd xmm7{k6}, xmm2, xmm0
	vfmsub213sd xmm5{k3}{z}, xmm6, qword [0xe97]
	vfmsub213sd xmm1, xmm0, xmm1, {rd-sae}
	vfmsub213sd xmm4, xmm3, qword [eax+1]
	vfmsub213sd xmm7, xmm0, qword [eax+64]
	vfmsub213sd xmm4, xmm3, qword [eax+1]
	vfmsub213sd xmm6, xmm0, qword [eax+64]
	vfmsub213sd xmm5, xmm4, [0x268]
	vfmsub213sd xmm6, xmm4, [0x805]
