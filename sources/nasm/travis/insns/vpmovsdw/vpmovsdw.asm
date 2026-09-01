default rel
	vpmovsdw xmm3, xmm1
	vpmovsdw xmm0, xmm6
	vpmovsdw xmm4, ymm5
	vpmovsdw xmm4, ymm2
	vpmovsdw ymm1, zmm6
	vpmovsdw ymm5, zmm1
	vpmovsdw qword [0xc60], xmm2
	vpmovsdw qword [0x878], xmm0
	vpmovsdw xmm14, xmm10
	vpmovsdw xmm15, ymm10
	vpmovsdw ymm12, zmm9
	vpmovsdw qword [0xd91], xmm11
	vpmovsdw xmm23, xmm18
	vpmovsdw xmm28, ymm18
	vpmovsdw ymm23, zmm31
	vpmovsdw qword [0x57f], xmm16
	vpmovsdw xmm7{k7}, xmm6
	vpmovsdw xmm1{k5}, ymm3
	vpmovsdw ymm5{k5}, zmm7
	vpmovsdw qword [0x545]{k1}, xmm7
	vpmovsdw oword [0x49d]{k1}, ymm4
	vpmovsdw yword [0x308]{k6}, zmm2
	vpmovsdw qword [eax+1], xmm6
	vpmovsdw qword [eax+64], xmm0
	vpmovsdw oword [eax+1], ymm3
	vpmovsdw oword [eax+64], ymm5
	vpmovsdw yword [eax+1], zmm2
	vpmovsdw yword [eax+64], zmm7
	vpmovsdw [0xa9b], xmm7
	vpmovsdw [0xa01], ymm7
	vpmovsdw [0x3a7], zmm1
