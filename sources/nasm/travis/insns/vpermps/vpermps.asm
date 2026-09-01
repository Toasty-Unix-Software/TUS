default rel
	vpermps ymm3, ymm4, yword [0x6af]
	vpermps ymm3, yword [0x6af]
	vpermps ymm6, ymm7, ymm4
	vpermps ymm7, ymm1, yword [0x662]
	vpermps ymm7, yword [0x662]
	vpermps ymm6, ymm3, ymm6
	vpermps zmm7, zmm3, zmm2
	vpermps zmm7, zmm2
	vpermps zmm3, zmm4, zmm1
	vpermps ymm10, ymm11, ymm11
	vpermps ymm8, ymm8, ymm12
	vpermps zmm8, zmm11, zmm13
	vpermps ymm20, ymm23, ymm20
	vpermps ymm20, ymm21, ymm29
	vpermps zmm26, zmm23, zmm28
	vpermps ymm2{k1}, ymm3, ymm2
	vpermps zmm3{k4}, zmm0, zmm0
	vpermps ymm4{k6}{z}, ymm3, yword [0xbc2]
	vpermps zmm2{k3}{z}, zmm5, zword [0x772]
	vpermps ymm2, ymm4, yword [eax+1]
	vpermps ymm5, ymm5, yword [eax+64]
	vpermps ymm2, ymm4, yword [eax+1]
	vpermps ymm7, ymm4, yword [eax+64]
	vpermps zmm0, zmm0, zword [eax+1]
	vpermps zmm0, zmm5, zword [eax+64]
	vpermps ymm7, ymm3, [0xb77]
	vpermps ymm3, ymm0, [0x57a]
	vpermps zmm5, zmm1, [0x211]
