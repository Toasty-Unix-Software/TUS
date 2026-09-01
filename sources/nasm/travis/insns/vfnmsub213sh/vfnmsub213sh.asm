default rel
	vfnmsub213sh xmm4, xmm3, xmm3
	vfnmsub213sh xmm4, xmm3
	vfnmsub213sh xmm4, xmm4, xmm2
	vfnmsub213sh xmm8, xmm14, xmm12
	vfnmsub213sh xmm24, xmm31, xmm24
	vfnmsub213sh xmm0{k1}, xmm1, xmm5
	vfnmsub213sh xmm0{k2}{z}, xmm0, word [0x488]
	vfnmsub213sh xmm3, xmm4, xmm1, {rz-sae}
	vfnmsub213sh xmm2, xmm0, word [eax+1]
	vfnmsub213sh xmm7, xmm6, word [eax+64]
	vfnmsub213sh xmm3, xmm5, [0xa98]
