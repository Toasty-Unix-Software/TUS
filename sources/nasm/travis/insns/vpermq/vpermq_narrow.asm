	vpermq ymm4, ymm3, 0x6e
	vpermq ymm7, yword [0xadf], 0xed
	vpermq ymm1, ymm3, 0x29
	vpermq ymm3, ymm2, 0x33
	vpermq zmm7, zmm6, 0xcc
	vpermq zmm7, zword [0xfd7], 0xa
	vpermq ymm0, ymm4, yword [0xaa0]
	vpermq ymm0, yword [0xaa0]
	vpermq ymm1, ymm4, yword [0x4d5]

%ifdef ERROR
	vpermq ymm9, ymm12, 0xf7
	vpermq ymm15, ymm11, 0x29
	vpermq zmm12, zmm11, 0x95
	vpermq ymm14, ymm10, ymm15
	vpermq ymm18, ymm20, 0x2b
	vpermq ymm19, ymm25, 0xee
	vpermq zmm18, zmm25, 0x4f
	vpermq ymm21, ymm26, ymm20
%endif
