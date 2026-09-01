default rel
	vpmovusdw xmm3, xmm4
	vpmovusdw xmm3, xmm1
	vpmovusdw xmm3, ymm4
	vpmovusdw xmm3, ymm4
	vpmovusdw ymm2, zmm6
	vpmovusdw ymm1, zmm4
	vpmovusdw qword [0x356], xmm6
	vpmovusdw qword [0x7f0], xmm4
	vpmovusdw xmm15, xmm12
	vpmovusdw xmm14, ymm8
	vpmovusdw ymm11, zmm9
	vpmovusdw qword [0xeb4], xmm11
	vpmovusdw xmm22, xmm17
	vpmovusdw xmm26, ymm17
	vpmovusdw ymm20, zmm23
	vpmovusdw qword [0xd94], xmm21
	vpmovusdw xmm6{k7}, xmm5
	vpmovusdw xmm2{k1}, ymm5
	vpmovusdw ymm2{k3}, zmm6
	vpmovusdw qword [0xf1c]{k2}, xmm0
	vpmovusdw oword [0xfa0]{k6}, ymm3
	vpmovusdw yword [0x524]{k2}, zmm3
	vpmovusdw qword [eax+1], xmm6
	vpmovusdw qword [eax+64], xmm5
	vpmovusdw oword [eax+1], ymm0
	vpmovusdw oword [eax+64], ymm2
	vpmovusdw yword [eax+1], zmm6
	vpmovusdw yword [eax+64], zmm2
	vpmovusdw [0x38b], xmm5
	vpmovusdw [0xdc7], ymm6
	vpmovusdw [0x1ca], zmm2
