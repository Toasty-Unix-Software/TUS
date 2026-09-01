	vmaxbf16 xmm2, xmm5, oword [0x9bd]
	vmaxbf16 xmm4, xmm1, xmm2
	vmaxbf16 ymm2, ymm1, ymm7
	vmaxbf16 ymm7, ymm4, yword [0x76b]
	vmaxbf16 zmm6, zmm5, zmm2
	vmaxbf16 zmm2, zmm6, zword [0x2cc]

%ifdef ERROR
	vmaxbf16 xmm12, xmm12, xmm15
	vmaxbf16 ymm13, ymm8, ymm11
	vmaxbf16 zmm11, zmm10, zmm11
	vmaxbf16 xmm27, xmm18, xmm21
	vmaxbf16 ymm29, ymm23, ymm22
	vmaxbf16 zmm20, zmm21, zmm19
%endif
