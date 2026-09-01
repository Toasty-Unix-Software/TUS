default rel
	vcvtdq2ph xmm2, xmm5
	vcvtdq2ph xmm3, xmm2
	vcvtdq2ph xmm1, yword [0xc05]
	vcvtdq2ph xmm1, ymm3
	vcvtdq2ph ymm0, zmm2
	vcvtdq2ph ymm5, zmm6
	vcvtdq2ph xmm11, xmm12
	vcvtdq2ph xmm8, ymm13
	vcvtdq2ph ymm9, zmm8
	vcvtdq2ph xmm27, xmm31
	vcvtdq2ph xmm18, ymm20
	vcvtdq2ph ymm18, zmm19
	vcvtdq2ph xmm1{k5}, xmm6
	vcvtdq2ph xmm1{k5}, ymm6
	vcvtdq2ph ymm7{k2}, zmm0
	vcvtdq2ph xmm1{k3}{z}, xmm6
	vcvtdq2ph xmm2{k7}{z}, yword [0x767]
	vcvtdq2ph ymm7{k6}{z}, zword [0x893]
	vcvtdq2ph ymm4, zmm3, {rz-sae}
	vcvtdq2ph xmm2, oword [eax+1]
	vcvtdq2ph xmm4, oword [eax+64]
	vcvtdq2ph xmm2, yword [eax+1]
	vcvtdq2ph xmm7, yword [eax+64]
	vcvtdq2ph ymm2, zword [eax+1]
	vcvtdq2ph ymm5, zword [eax+64]
	vcvtdq2ph xmm2, [0xf09]
	vcvtdq2ph xmm2, [0xedf]
	vcvtdq2ph ymm4, [0xc85]
