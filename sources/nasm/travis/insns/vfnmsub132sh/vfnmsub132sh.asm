default rel
	vfnmsub132sh xmm0, xmm1, xmm6
	vfnmsub132sh xmm0, xmm6
	vfnmsub132sh xmm1, xmm1, xmm6
	vfnmsub132sh xmm10, xmm10, xmm14
	vfnmsub132sh xmm29, xmm16, xmm18
	vfnmsub132sh xmm3{k6}, xmm6, xmm0
	vfnmsub132sh xmm7{k5}{z}, xmm5, word [0x8bc]
	vfnmsub132sh xmm4, xmm1, xmm2, {ru-sae}
	vfnmsub132sh xmm1, xmm7, word [eax+1]
	vfnmsub132sh xmm7, xmm4, word [eax+64]
	vfnmsub132sh xmm3, xmm2, [0xf40]
