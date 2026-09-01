default rel
	vcvtuw2ph xmm5, xmm4
	vcvtuw2ph xmm3, xmm1
	vcvtuw2ph ymm6, ymm1
	vcvtuw2ph ymm3, ymm3
	vcvtuw2ph zmm2, zmm6
	vcvtuw2ph zmm6, zmm3
	vcvtuw2ph xmm9, xmm8
	vcvtuw2ph ymm8, ymm9
	vcvtuw2ph zmm8, zmm8
	vcvtuw2ph xmm26, xmm30
	vcvtuw2ph ymm19, ymm25
	vcvtuw2ph zmm17, zmm21
	vcvtuw2ph xmm7{k1}, xmm1
	vcvtuw2ph ymm2{k2}, ymm6
	vcvtuw2ph zmm2{k4}, zmm1
	vcvtuw2ph xmm4{k3}{z}, oword [0x8d2]
	vcvtuw2ph ymm1{k2}{z}, ymm7
	vcvtuw2ph zmm7{k3}{z}, zmm7
	vcvtuw2ph zmm7, zmm6, {rn-sae}
	vcvtuw2ph xmm6, oword [eax+1]
	vcvtuw2ph xmm3, oword [eax+64]
	vcvtuw2ph ymm5, yword [eax+1]
	vcvtuw2ph ymm4, yword [eax+64]
	vcvtuw2ph zmm3, zword [eax+1]
	vcvtuw2ph zmm5, zword [eax+64]
	vcvtuw2ph xmm5, [0xf03]
	vcvtuw2ph ymm4, [0x2fd]
	vcvtuw2ph zmm7, [0x415]
