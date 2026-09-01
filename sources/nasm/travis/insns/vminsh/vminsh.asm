default rel
	vminsh xmm1, xmm7, xmm6
	vminsh xmm4, xmm5, xmm6
	vminsh xmm4, xmm1, word [0xb7a]
	vminsh xmm4, word [0xb7a]
	vminsh xmm4, xmm7, xmm1
	vminsh xmm13, xmm14, xmm9
	vminsh xmm10, xmm13, xmm10
	vminsh xmm28, xmm25, xmm26
	vminsh xmm28, xmm27, xmm19
	vminsh xmm1{k3}, xmm0, word [0xc9c]
	vminsh xmm0{k3}, xmm1, xmm7
	vminsh xmm5{k3}{z}, xmm1, xmm6
	vminsh xmm3{k6}{z}, xmm1, xmm1
	vminsh xmm1, xmm7, xmm2, {sae}
	vminsh xmm7, xmm4, xmm4, {sae}
	vminsh xmm5, xmm7, word [eax+1]
	vminsh xmm2, xmm7, word [eax+64]
	vminsh xmm5, xmm5, word [eax+1]
	vminsh xmm1, xmm7, word [eax+64]
	vminsh xmm2, xmm3, [0x27e]
	vminsh xmm5, xmm2, [0x62c]
