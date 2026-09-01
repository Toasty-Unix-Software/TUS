default rel
	v4fmaddps zmm3, zmm5, [0xe02]
	v4fmaddps zmm5, zmm4, [0x78e]
	v4fmaddps zmm13, zmm14, [0xef2]
	v4fmaddps zmm24, zmm25, [0xb52]
	v4fmaddps zmm1{k7}, zmm4, [0x131]
	v4fmaddps zmm0{k5}{z}, zmm0, [0x60f]
	v4fmaddps zmm0, zmm1, [eax+1]
	v4fmaddps zmm7, zmm6, [eax+64]
