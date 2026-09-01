default rel
	vcvtph2w xmm7, xmm4
	vcvtph2w xmm3, xmm7
	vcvtph2w ymm0, yword [0x161]
	vcvtph2w ymm6, ymm6
	vcvtph2w zmm3, zword [0x101]
	vcvtph2w zmm0, zword [0x5cc]
	vcvtph2w xmm11, xmm9
	vcvtph2w ymm11, ymm13
	vcvtph2w zmm10, zmm14
	vcvtph2w xmm25, xmm30
	vcvtph2w ymm28, ymm31
	vcvtph2w zmm28, zmm30
	vcvtph2w xmm5{k6}, xmm2
	vcvtph2w ymm5{k4}, ymm4
	vcvtph2w zmm3{k3}, zmm5
	vcvtph2w xmm2{k3}{z}, xmm6
	vcvtph2w ymm2{k7}{z}, ymm0
	vcvtph2w zmm2{k5}{z}, zmm6
	vcvtph2w zmm7, zmm7, {rn-sae}
	vcvtph2w xmm4, oword [eax+1]
	vcvtph2w xmm0, oword [eax+64]
	vcvtph2w ymm1, yword [eax+1]
	vcvtph2w ymm2, yword [eax+64]
	vcvtph2w zmm7, zword [eax+1]
	vcvtph2w zmm4, zword [eax+64]
	vcvtph2w xmm1, [0xd1a]
	vcvtph2w ymm1, [0xc7e]
	vcvtph2w zmm5, [0x40e]
