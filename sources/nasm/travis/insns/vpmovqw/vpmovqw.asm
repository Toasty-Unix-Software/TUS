default rel
	vpmovqw xmm5, xmm2
	vpmovqw xmm1, xmm3
	vpmovqw xmm2, ymm3
	vpmovqw xmm2, ymm4
	vpmovqw xmm4, zmm7
	vpmovqw xmm5, zmm1
	vpmovqw dword [0xeb3], xmm7
	vpmovqw dword [0x585], xmm6
	vpmovqw xmm13, xmm9
	vpmovqw xmm8, ymm10
	vpmovqw xmm14, zmm15
	vpmovqw dword [0x860], xmm8
	vpmovqw xmm27, xmm22
	vpmovqw xmm31, ymm30
	vpmovqw xmm16, zmm23
	vpmovqw dword [0x148], xmm16
	vpmovqw xmm4{k6}, xmm6
	vpmovqw xmm7{k2}, ymm7
	vpmovqw xmm5{k2}, zmm6
	vpmovqw dword [0xff8]{k7}, xmm7
	vpmovqw qword [0x9c7]{k4}, ymm1
	vpmovqw oword [0xf67]{k3}, zmm1
	vpmovqw dword [eax+1], xmm5
	vpmovqw dword [eax+64], xmm4
	vpmovqw qword [eax+1], ymm2
	vpmovqw qword [eax+64], ymm6
	vpmovqw oword [eax+1], zmm5
	vpmovqw oword [eax+64], zmm1
	vpmovqw [0x1fb], xmm0
	vpmovqw [0x7ff], ymm2
	vpmovqw [0xcbe], zmm0
