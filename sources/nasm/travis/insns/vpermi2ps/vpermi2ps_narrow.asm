	vpermi2ps xmm3, xmm7, xmm4
	vpermi2ps xmm7, xmm7, oword [0xb1b]
	vpermi2ps ymm7, ymm2, ymm1
	vpermi2ps ymm3, ymm2, ymm7
	vpermi2ps zmm4, zmm6, zmm4
	vpermi2ps zmm2, zmm2, zmm0

%ifdef ERROR
	vpermi2ps xmm12, xmm10, xmm10
	vpermi2ps ymm11, ymm10, ymm15
	vpermi2ps zmm14, zmm13, zmm11
	vpermi2ps xmm24, xmm22, xmm18
	vpermi2ps ymm24, ymm20, ymm28
	vpermi2ps zmm29, zmm29, zmm28
%endif
