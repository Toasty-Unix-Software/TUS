default rel
	vmaxsh xmm3, xmm1, xmm4
	vmaxsh xmm1, xmm6, word [0xcc2]
	vmaxsh xmm4, xmm5, xmm7
	vmaxsh xmm4, xmm7
	vmaxsh xmm6, xmm4, xmm1
	vmaxsh xmm14, xmm9, xmm11
	vmaxsh xmm14, xmm10, xmm14
	vmaxsh xmm17, xmm21, xmm18
	vmaxsh xmm30, xmm20, xmm19
	vmaxsh xmm7{k2}, xmm0, xmm2
	vmaxsh xmm6{k5}, xmm3, word [0x956]
	vmaxsh xmm1{k1}{z}, xmm6, xmm3
	vmaxsh xmm3{k4}{z}, xmm3, word [0xadb]
	vmaxsh xmm0, xmm7, xmm4, {sae}
	vmaxsh xmm7, xmm0, xmm4, {sae}
	vmaxsh xmm0, xmm1, word [eax+1]
	vmaxsh xmm5, xmm5, word [eax+64]
	vmaxsh xmm5, xmm6, word [eax+1]
	vmaxsh xmm6, xmm4, word [eax+64]
	vmaxsh xmm2, xmm2, [0x3b7]
	vmaxsh xmm0, xmm0, [0xb6d]
