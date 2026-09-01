	vpternlogd xmm5, xmm4, xmm1, 0x3a
	vpternlogd xmm0, xmm7, xmm1, 0xb7
	vpternlogd ymm6, ymm5, ymm7, 0x1b
	vpternlogd ymm1, ymm4, ymm3, 0xa1
	vpternlogd zmm6, zmm4, zword [0x8fe], 0x39
	vpternlogd zmm4, zmm6, zword [0x83f], 0xd6

%ifdef ERROR
	vpternlogd xmm14, xmm11, xmm11, 0x9a
	vpternlogd ymm13, ymm12, ymm9, 0x7
	vpternlogd zmm12, zmm14, zmm10, 0xaf
	vpternlogd xmm27, xmm23, xmm21, 0xcb
	vpternlogd ymm28, ymm20, ymm30, 0xd9
	vpternlogd zmm18, zmm29, zmm26, 0x79
%endif
