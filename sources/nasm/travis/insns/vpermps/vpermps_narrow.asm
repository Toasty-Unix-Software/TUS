	vpermps ymm3, ymm4, yword [0x6af]
	vpermps ymm3, yword [0x6af]
	vpermps ymm6, ymm7, ymm4
	vpermps ymm7, ymm1, yword [0x662]
	vpermps ymm7, yword [0x662]
	vpermps ymm6, ymm3, ymm6
	vpermps zmm7, zmm3, zmm2
	vpermps zmm7, zmm2
	vpermps zmm3, zmm4, zmm1

%ifdef ERROR
	vpermps ymm10, ymm11, ymm11
	vpermps ymm8, ymm8, ymm12
	vpermps zmm8, zmm11, zmm13
	vpermps ymm20, ymm23, ymm20
	vpermps ymm20, ymm21, ymm29
	vpermps zmm26, zmm23, zmm28
%endif
