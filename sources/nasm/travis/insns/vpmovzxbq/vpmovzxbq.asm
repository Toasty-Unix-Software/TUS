default rel
	vpmovzxbq xmm1, word [0x166]
	vpmovzxbq xmm1, word [0x2ba]
	vpmovzxbq ymm5, dword [0x405]
	vpmovzxbq ymm3, dword [0x4ad]
	vpmovzxbq ymm6, xmm2
	vpmovzxbq ymm2, xmm5
	vpmovzxbq xmm6, xmm3
	vpmovzxbq xmm0, xmm5
	vpmovzxbq xmm11, xmm14
	vpmovzxbq ymm14, dword [0x2ec]
	vpmovzxbq ymm14, xmm12
	vpmovzxbq xmm11, xmm12
	vpmovzxbq xmm27, xmm29
	vpmovzxbq ymm27, dword [0x2ed]
	vpmovzxbq ymm26, xmm20
	vpmovzxbq xmm25, xmm25
	vpmovzxbq xmm7{k4}, xmm3
	vpmovzxbq ymm4{k4}, xmm2
	vpmovzxbq zmm4{k3}, xmm2
	vpmovzxbq xmm1{k2}{z}, xmm0
	vpmovzxbq ymm0{k3}{z}, dword [0x67a]
	vpmovzxbq zmm3{k5}{z}, qword [0x1ea]
	vpmovzxbq xmm0, word [eax+1]
	vpmovzxbq xmm1, word [eax+64]
	vpmovzxbq ymm0, dword [eax+1]
	vpmovzxbq ymm7, dword [eax+64]
	vpmovzxbq xmm7, word [eax+1]
	vpmovzxbq xmm2, word [eax+64]
	vpmovzxbq ymm3, dword [eax+1]
	vpmovzxbq ymm4, dword [eax+64]
	vpmovzxbq zmm6, qword [eax+1]
	vpmovzxbq zmm3, qword [eax+64]
	vpmovzxbq xmm4, [0x4b0]
	vpmovzxbq ymm1, [0xdda]
	vpmovzxbq xmm1, [0x5bd]
	vpmovzxbq ymm3, [0xeb5]
	vpmovzxbq zmm7, [0xc96]
