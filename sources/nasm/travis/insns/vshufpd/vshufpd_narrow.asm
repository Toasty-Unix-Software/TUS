	vshufpd xmm2, xmm0, xmm1, 0x41
	vshufpd xmm2, xmm1, 0x41
	vshufpd xmm5, xmm0, xmm4, 0x68
	vshufpd ymm0, ymm2, ymm6, 0xa4
	vshufpd ymm0, ymm6, 0xa4
	vshufpd ymm2, ymm7, ymm5, 0x56
	vshufpd xmm0, xmm4, xmm3, 0xa7
	vshufpd xmm0, xmm3, 0xa7
	vshufpd xmm3, xmm0, xmm2, 0xc
	vshufpd ymm4, ymm2, ymm5, 0x3a
	vshufpd ymm4, ymm5, 0x3a
	vshufpd ymm2, ymm7, ymm0, 0xe6

%ifdef ERROR
	vshufpd xmm9, xmm11, xmm14, 0x48
	vshufpd ymm9, ymm9, ymm12, 0x79
	vshufpd xmm15, xmm15, xmm13, 0x50
	vshufpd ymm10, ymm11, ymm9, 0xfe
	vshufpd xmm20, xmm21, xmm24, 0x4
	vshufpd ymm27, ymm25, ymm29, 0x72
	vshufpd xmm21, xmm21, xmm20, 0xad
	vshufpd ymm24, ymm29, ymm25, 0x92
%endif
