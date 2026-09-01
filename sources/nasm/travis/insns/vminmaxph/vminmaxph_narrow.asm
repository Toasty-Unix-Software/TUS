	vminmaxph xmm2, xmm4, oword [0xc19], 0x6d
	vminmaxph xmm3, xmm2, xmm0, 0xfd
	vminmaxph ymm4, ymm2, ymm7, 0x54
	vminmaxph ymm1, ymm6, ymm4, 0x8d
	vminmaxph zmm5, zmm7, zmm5, 0xab
	vminmaxph zmm1, zmm0, zword [0x3a8], 0x51

%ifdef ERROR
	vminmaxph xmm15, xmm13, xmm8, 0x39
	vminmaxph ymm8, ymm15, ymm14, 0x3
	vminmaxph zmm14, zmm9, zmm8, 0x46
	vminmaxph xmm19, xmm30, xmm30, 0xaf
	vminmaxph ymm19, ymm22, ymm31, 0x33
	vminmaxph zmm30, zmm19, zmm19, 0x53
%endif
