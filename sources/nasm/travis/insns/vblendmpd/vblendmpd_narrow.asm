	vblendmpd xmm2, xmm1, xmm3
	vblendmpd xmm2, xmm2, oword [0x1a4]
	vblendmpd ymm2, ymm7, ymm6
	vblendmpd ymm4, ymm4, ymm4
	vblendmpd zmm7, zmm3, zmm0
	vblendmpd zmm1, zmm1, zmm4

%ifdef ERROR
	vblendmpd xmm9, xmm9, xmm14
	vblendmpd ymm8, ymm10, ymm9
	vblendmpd zmm11, zmm15, zmm9
	vblendmpd xmm25, xmm21, xmm29
	vblendmpd ymm18, ymm19, ymm17
	vblendmpd zmm17, zmm19, zmm19
%endif
