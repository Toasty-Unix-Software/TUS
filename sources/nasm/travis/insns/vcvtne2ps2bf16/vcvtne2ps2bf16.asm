default rel
	vcvtne2ps2bf16 xmm0, xmm2, xmm7
	vcvtne2ps2bf16 xmm0, xmm7
	vcvtne2ps2bf16 xmm3, xmm4, xmm6
	vcvtne2ps2bf16 ymm6, ymm5, ymm4
	vcvtne2ps2bf16 ymm6, ymm4
	vcvtne2ps2bf16 ymm5, ymm4, ymm6
	vcvtne2ps2bf16 zmm7, zmm1, zword [0x796]
	vcvtne2ps2bf16 zmm7, zword [0x796]
	vcvtne2ps2bf16 zmm1, zmm2, zmm5
	vcvtne2ps2bf16 xmm8, xmm14, xmm14
	vcvtne2ps2bf16 ymm8, ymm8, ymm11
	vcvtne2ps2bf16 zmm14, zmm9, zmm13
	vcvtne2ps2bf16 xmm24, xmm17, xmm25
	vcvtne2ps2bf16 ymm17, ymm22, ymm27
	vcvtne2ps2bf16 zmm28, zmm20, zmm21
	vcvtne2ps2bf16 xmm6{k1}, xmm6, xmm1
	vcvtne2ps2bf16 ymm2{k1}, ymm4, ymm6
	vcvtne2ps2bf16 zmm6{k7}, zmm4, zmm3
	vcvtne2ps2bf16 xmm1{k2}{z}, xmm4, xmm6
	vcvtne2ps2bf16 ymm2{k5}{z}, ymm5, yword [0x84b]
	vcvtne2ps2bf16 zmm5{k3}{z}, zmm2, zword [0xf2c]
	vcvtne2ps2bf16 xmm0, xmm0, oword [eax+1]
	vcvtne2ps2bf16 xmm2, xmm4, oword [eax+64]
	vcvtne2ps2bf16 ymm1, ymm5, yword [eax+1]
	vcvtne2ps2bf16 ymm4, ymm1, yword [eax+64]
	vcvtne2ps2bf16 zmm3, zmm5, zword [eax+1]
	vcvtne2ps2bf16 zmm5, zmm7, zword [eax+64]
	vcvtne2ps2bf16 xmm1, xmm3, [0x6d5]
	vcvtne2ps2bf16 ymm3, ymm6, [0xe59]
	vcvtne2ps2bf16 zmm6, zmm3, [0xa57]
