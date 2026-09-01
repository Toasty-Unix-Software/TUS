	vpermpd ymm0, ymm5, 0xb6
	vpermpd ymm2, ymm2, 0xd7
	vpermpd ymm5, yword [0x9e8], 0x80
	vpermpd ymm2, ymm3, 0xd0
	vpermpd zmm3, zword [0x8f7], 0xb
	vpermpd zmm5, zmm5, 0x50
	vpermpd ymm0, ymm4, ymm1
	vpermpd ymm0, ymm1
	vpermpd ymm3, ymm0, ymm6

%ifdef ERROR
	vpermpd ymm11, ymm10, 0x79
	vpermpd ymm9, ymm10, 0x52
	vpermpd zmm9, zmm14, 0xc7
	vpermpd ymm9, ymm14, ymm9
	vpermpd ymm25, ymm19, 0x63
	vpermpd ymm28, ymm27, 0x31
	vpermpd zmm23, zmm27, 0x73
	vpermpd ymm19, ymm20, ymm28
%endif
