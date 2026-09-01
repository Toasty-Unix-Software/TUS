default rel
	vfmaddcsh xmm7, xmm7, xmm4
	vfmaddcsh xmm7, xmm4
	vfmaddcsh xmm0, xmm4, xmm2
	vfmaddcsh xmm8, xmm14, xmm9
	vfmaddcsh xmm17, xmm29, xmm25
	vfmaddcsh xmm3{k2}, xmm3, xmm6
	vfmaddcsh xmm0{k1}{z}, xmm3, dword [0xd90]
	vfmaddcsh xmm0, xmm6, xmm6, {rz-sae}
	vfmaddcsh xmm6, xmm5, dword [eax+1]
	vfmaddcsh xmm3, xmm7, dword [eax+64]
	vfmaddcsh xmm1, xmm2, [0x6d9]
