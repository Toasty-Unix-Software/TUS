	vcvt2ps2phx xmm1, xmm1, xmm7
	vcvt2ps2phx xmm0, xmm6, oword [0x1f8]
	vcvt2ps2phx ymm3, ymm6, ymm0
	vcvt2ps2phx ymm1, ymm5, ymm3
	vcvt2ps2phx zmm1, zmm0, zmm1
	vcvt2ps2phx zmm0, zmm4, zword [0x579]

%ifdef ERROR
	vcvt2ps2phx xmm11, xmm9, xmm9
	vcvt2ps2phx ymm8, ymm8, ymm15
	vcvt2ps2phx zmm13, zmm8, zmm14
	vcvt2ps2phx xmm27, xmm21, xmm31
	vcvt2ps2phx ymm16, ymm17, ymm23
	vcvt2ps2phx zmm22, zmm25, zmm28
%endif
