	vcvtne2ps2bf16 xmm0, xmm2, xmm7
	vcvtne2ps2bf16 xmm0, xmm7
	vcvtne2ps2bf16 xmm3, xmm4, xmm6
	vcvtne2ps2bf16 ymm6, ymm5, ymm4
	vcvtne2ps2bf16 ymm6, ymm4
	vcvtne2ps2bf16 ymm5, ymm4, ymm6
	vcvtne2ps2bf16 zmm7, zmm1, zword [0x796]
	vcvtne2ps2bf16 zmm7, zword [0x796]
	vcvtne2ps2bf16 zmm1, zmm2, zmm5

%ifdef ERROR
	vcvtne2ps2bf16 xmm8, xmm14, xmm14
	vcvtne2ps2bf16 ymm8, ymm8, ymm11
	vcvtne2ps2bf16 zmm14, zmm9, zmm13
	vcvtne2ps2bf16 xmm24, xmm17, xmm25
	vcvtne2ps2bf16 ymm17, ymm22, ymm27
	vcvtne2ps2bf16 zmm28, zmm20, zmm21
%endif
