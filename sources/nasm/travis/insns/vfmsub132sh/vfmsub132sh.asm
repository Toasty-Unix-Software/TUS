default rel
	vfmsub132sh xmm1, xmm5, word [0x7e6]
	vfmsub132sh xmm1, word [0x7e6]
	vfmsub132sh xmm6, xmm6, word [0x9d0]
	vfmsub132sh xmm8, xmm11, xmm10
	vfmsub132sh xmm25, xmm30, xmm25
	vfmsub132sh xmm6{k3}, xmm3, xmm4
	vfmsub132sh xmm4{k6}{z}, xmm1, word [0xa33]
	vfmsub132sh xmm5, xmm1, xmm6, {rd-sae}
	vfmsub132sh xmm6, xmm4, word [eax+1]
	vfmsub132sh xmm7, xmm5, word [eax+64]
	vfmsub132sh xmm0, xmm7, [0x79f]
