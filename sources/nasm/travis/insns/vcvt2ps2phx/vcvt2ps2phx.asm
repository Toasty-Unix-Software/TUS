default rel
	vcvt2ps2phx xmm1, xmm1, xmm7
	vcvt2ps2phx xmm0, xmm6, oword [0x1f8]
	vcvt2ps2phx ymm3, ymm6, ymm0
	vcvt2ps2phx ymm1, ymm5, ymm3
	vcvt2ps2phx zmm1, zmm0, zmm1
	vcvt2ps2phx zmm0, zmm4, zword [0x579]
	vcvt2ps2phx xmm11, xmm9, xmm9
	vcvt2ps2phx ymm8, ymm8, ymm15
	vcvt2ps2phx zmm13, zmm8, zmm14
	vcvt2ps2phx xmm27, xmm21, xmm31
	vcvt2ps2phx ymm16, ymm17, ymm23
	vcvt2ps2phx zmm22, zmm25, zmm28
	vcvt2ps2phx xmm3{k2}, xmm2, xmm7
	vcvt2ps2phx ymm7{k2}, ymm1, ymm5
	vcvt2ps2phx zmm5{k6}, zmm1, zmm1
	vcvt2ps2phx xmm2{k7}{z}, xmm2, xmm3
	vcvt2ps2phx ymm1{k2}{z}, ymm1, ymm4
	vcvt2ps2phx zmm1{k1}{z}, zmm4, zmm4
	vcvt2ps2phx xmm6, xmm1, oword [eax+1]
	vcvt2ps2phx xmm1, xmm0, oword [eax+64]
	vcvt2ps2phx ymm7, ymm7, yword [eax+1]
	vcvt2ps2phx ymm6, ymm0, yword [eax+64]
	vcvt2ps2phx zmm0, zmm7, zword [eax+1]
	vcvt2ps2phx zmm3, zmm1, zword [eax+64]
	vcvt2ps2phx xmm7, xmm0, [0x5b1]
	vcvt2ps2phx ymm3, ymm3, [0xac9]
	vcvt2ps2phx zmm4, zmm7, [0x35d]
