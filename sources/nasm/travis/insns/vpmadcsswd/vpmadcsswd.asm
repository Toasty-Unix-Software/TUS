default rel
	vpmadcsswd xmm5, xmm5, xmm1, xmm0
	vpmadcsswd xmm5, xmm1, xmm0
	vpmadcsswd xmm6, xmm5, xmm3, xmm0
	vpmadcsswd xmm14, xmm8, xmm14, xmm10
	vpmadcsswd xmm0, xmm6, oword [eax+1], xmm6
	vpmadcsswd xmm0, xmm2, oword [eax+64], xmm0
	vpmadcsswd xmm3, xmm6, [0x8a9], xmm2
