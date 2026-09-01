	vmaxpd xmm4, xmm5, xmm0
	vmaxpd xmm4, xmm0
	vmaxpd xmm4, xmm5, xmm5
	vmaxpd ymm0, ymm2, ymm5
	vmaxpd ymm0, ymm5
	vmaxpd ymm2, ymm1, yword [0x330]
	vmaxpd xmm1, xmm3, xmm5
	vmaxpd xmm1, xmm5
	vmaxpd xmm6, xmm4, xmm2
	vmaxpd ymm0, ymm1, ymm6
	vmaxpd ymm0, ymm6
	vmaxpd ymm5, ymm4, yword [0x5d8]

%ifdef ERROR
	vmaxpd xmm13, xmm10, xmm11
	vmaxpd ymm12, ymm12, ymm8
	vmaxpd xmm12, xmm14, xmm8
	vmaxpd ymm14, ymm13, ymm10
	vmaxpd xmm16, xmm31, xmm25
	vmaxpd ymm17, ymm24, ymm20
	vmaxpd xmm19, xmm19, xmm18
	vmaxpd ymm28, ymm29, ymm19
%endif
