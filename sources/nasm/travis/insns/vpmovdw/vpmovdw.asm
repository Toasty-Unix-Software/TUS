default rel
	vpmovdw xmm5, xmm5
	vpmovdw xmm0, xmm3
	vpmovdw xmm1, ymm1
	vpmovdw xmm7, ymm2
	vpmovdw ymm4, zmm0
	vpmovdw ymm2, zmm7
	vpmovdw qword [0x64f], xmm1
	vpmovdw qword [0xe5e], xmm5
	vpmovdw xmm14, xmm15
	vpmovdw xmm12, ymm8
	vpmovdw ymm14, zmm15
	vpmovdw qword [0x397], xmm11
	vpmovdw xmm17, xmm27
	vpmovdw xmm29, ymm20
	vpmovdw ymm19, zmm31
	vpmovdw qword [0xd7d], xmm25
	vpmovdw xmm6{k1}, xmm7
	vpmovdw xmm7{k7}, ymm4
	vpmovdw ymm1{k4}, zmm6
	vpmovdw qword [0x33c]{k3}, xmm4
	vpmovdw oword [0x407]{k5}, ymm2
	vpmovdw yword [0xc2b]{k3}, zmm1
	vpmovdw qword [eax+1], xmm1
	vpmovdw qword [eax+64], xmm3
	vpmovdw oword [eax+1], ymm3
	vpmovdw oword [eax+64], ymm6
	vpmovdw yword [eax+1], zmm1
	vpmovdw yword [eax+64], zmm5
	vpmovdw [0xdf5], xmm7
	vpmovdw [0x845], ymm7
	vpmovdw [0xc1b], zmm1
