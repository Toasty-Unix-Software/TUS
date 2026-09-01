	vminmaxpd xmm7, xmm7, oword [0x69e], 0x15
	vminmaxpd xmm6, xmm0, xmm2, 0x34
	vminmaxpd ymm0, ymm0, yword [0xc16], 0x9d
	vminmaxpd ymm7, ymm7, yword [0x897], 0x7a
	vminmaxpd zmm4, zmm5, zmm4, 0xc3
	vminmaxpd zmm7, zmm7, zword [0xdcd], 0xfa

%ifdef ERROR
	vminmaxpd xmm11, xmm8, xmm12, 0xa2
	vminmaxpd ymm11, ymm10, ymm15, 0x5a
	vminmaxpd zmm8, zmm13, zmm9, 0x1
	vminmaxpd xmm30, xmm26, xmm18, 0x1d
	vminmaxpd ymm22, ymm19, ymm21, 0x1a
	vminmaxpd zmm20, zmm28, zmm23, 0xc6
%endif
