	vpshldq xmm0, xmm0, xmm2, 0xc9
	vpshldq xmm0, xmm2, 0xc9
	vpshldq xmm7, xmm6, oword [0xd1c], 0x52
	vpshldq ymm7, ymm5, yword [0x8e5], 0xaa
	vpshldq ymm7, yword [0x8e5], 0xaa
	vpshldq ymm0, ymm7, yword [0xf22], 0x83
	vpshldq zmm4, zmm3, zword [0x786], 0x4a
	vpshldq zmm4, zword [0x786], 0x4a
	vpshldq zmm4, zmm1, zmm5, 0xc9

%ifdef ERROR
	vpshldq xmm9, xmm12, xmm13, 0x22
	vpshldq ymm10, ymm10, ymm9, 0x1d
	vpshldq zmm8, zmm14, zmm10, 0x2d
	vpshldq xmm16, xmm31, xmm20, 0xa7
	vpshldq ymm20, ymm31, ymm24, 0xe
	vpshldq zmm24, zmm20, zmm20, 0xa
%endif
