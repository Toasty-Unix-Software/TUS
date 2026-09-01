	vpclmullqlqdq xmm7, xmm3, xmm1
	vpclmullqlqdq xmm7, xmm1
	vpclmullqlqdq xmm6, xmm2, xmm3
	vpclmullqlqdq ymm6, ymm1, ymm3
	vpclmullqlqdq ymm6, ymm3
	vpclmullqlqdq ymm6, ymm5, ymm4
	vpclmullqlqdq zmm7, zmm0, zmm2
	vpclmullqlqdq zmm7, zmm2
	vpclmullqlqdq zmm0, zmm3, zmm0

%ifdef ERROR
	vpclmullqlqdq xmm9, xmm11, xmm9
	vpclmullqlqdq ymm14, ymm13, ymm11
	vpclmullqlqdq zmm9, zmm14, zmm15
	vpclmullqlqdq xmm21, xmm25, xmm29
	vpclmullqlqdq ymm26, ymm29, ymm30
	vpclmullqlqdq zmm31, zmm16, zmm19
%endif
