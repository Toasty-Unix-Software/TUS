	vminpd xmm2, xmm3, oword [0x270]
	vminpd xmm2, oword [0x270]
	vminpd xmm4, xmm1, oword [0x153]
	vminpd ymm0, ymm6, ymm0
	vminpd ymm0, ymm0
	vminpd ymm0, ymm1, ymm6
	vminpd xmm6, xmm1, xmm1
	vminpd xmm6, xmm1
	vminpd xmm2, xmm4, xmm6
	vminpd ymm2, ymm5, yword [0xc9f]
	vminpd ymm2, yword [0xc9f]
	vminpd ymm6, ymm5, ymm1

%ifdef ERROR
	vminpd xmm15, xmm12, xmm15
	vminpd ymm12, ymm8, ymm14
	vminpd xmm12, xmm10, xmm9
	vminpd ymm10, ymm11, ymm13
	vminpd xmm21, xmm28, xmm19
	vminpd ymm24, ymm26, ymm27
	vminpd xmm16, xmm21, xmm29
	vminpd ymm23, ymm28, ymm25
%endif
