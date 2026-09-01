	vpclmullqhqdq xmm5, xmm2, xmm6
	vpclmullqhqdq xmm5, xmm6
	vpclmullqhqdq xmm1, xmm0, oword [0x10f]
	vpclmullqhqdq ymm0, ymm0, ymm4
	vpclmullqhqdq ymm0, ymm4
	vpclmullqhqdq ymm5, ymm1, ymm0
	vpclmullqhqdq zmm1, zmm0, zword [0x66d]
	vpclmullqhqdq zmm1, zword [0x66d]
	vpclmullqhqdq zmm7, zmm6, zmm2

%ifdef ERROR
	vpclmullqhqdq xmm8, xmm14, xmm11
	vpclmullqhqdq ymm15, ymm8, ymm15
	vpclmullqhqdq zmm13, zmm10, zmm8
	vpclmullqhqdq xmm31, xmm19, xmm24
	vpclmullqhqdq ymm23, ymm24, ymm17
	vpclmullqhqdq zmm20, zmm27, zmm22
%endif
