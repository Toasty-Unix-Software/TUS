	vpermilpd xmm2, xmm6, xmm7
	vpermilpd xmm2, xmm7
	vpermilpd xmm1, xmm6, xmm7
	vpermilpd ymm3, ymm7, ymm7
	vpermilpd ymm3, ymm7
	vpermilpd ymm7, ymm7, ymm4
	vpermilpd xmm0, oword [0xde1], 0x2b
	vpermilpd xmm3, xmm3, 0x43
	vpermilpd ymm1, ymm2, 0x4b
	vpermilpd ymm0, yword [0xc84], 0x73

%ifdef ERROR
	vpermilpd xmm10, xmm10, xmm14
	vpermilpd ymm9, ymm13, ymm10
	vpermilpd xmm11, xmm9, 0xc
	vpermilpd ymm10, ymm13, 0x49
	vpermilpd xmm30, xmm29, xmm21
	vpermilpd ymm16, ymm19, ymm18
	vpermilpd xmm28, xmm17, 0xb0
	vpermilpd ymm18, ymm18, 0xef
%endif
