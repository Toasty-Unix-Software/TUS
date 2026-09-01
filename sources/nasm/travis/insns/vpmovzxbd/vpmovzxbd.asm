default rel
	vpmovzxbd xmm7, xmm1
	vpmovzxbd xmm6, xmm2
	vpmovzxbd ymm2, qword [0x810]
	vpmovzxbd ymm0, qword [0x62e]
	vpmovzxbd ymm4, xmm5
	vpmovzxbd ymm7, xmm3
	vpmovzxbd xmm2, xmm6
	vpmovzxbd xmm4, xmm3
	vpmovzxbd xmm15, xmm10
	vpmovzxbd ymm11, qword [0xa16]
	vpmovzxbd ymm13, xmm15
	vpmovzxbd xmm13, xmm8
	vpmovzxbd xmm26, xmm17
	vpmovzxbd ymm24, qword [0x632]
	vpmovzxbd ymm18, xmm30
	vpmovzxbd xmm19, xmm21
	vpmovzxbd xmm5{k1}, xmm3
	vpmovzxbd ymm6{k1}, xmm5
	vpmovzxbd zmm5{k2}, xmm2
	vpmovzxbd xmm0{k4}{z}, dword [0xc4b]
	vpmovzxbd ymm5{k1}{z}, xmm5
	vpmovzxbd zmm2{k2}{z}, oword [0xaf5]
	vpmovzxbd xmm7, dword [eax+1]
	vpmovzxbd xmm6, dword [eax+64]
	vpmovzxbd ymm6, qword [eax+1]
	vpmovzxbd ymm0, qword [eax+64]
	vpmovzxbd xmm2, dword [eax+1]
	vpmovzxbd xmm3, dword [eax+64]
	vpmovzxbd ymm3, qword [eax+1]
	vpmovzxbd ymm4, qword [eax+64]
	vpmovzxbd zmm4, oword [eax+1]
	vpmovzxbd zmm6, oword [eax+64]
	vpmovzxbd xmm0, [0xb5f]
	vpmovzxbd ymm0, [0x3d1]
	vpmovzxbd xmm5, [0xbcf]
	vpmovzxbd ymm4, [0x922]
	vpmovzxbd zmm5, [0x244]
