	vpermt2w xmm7, xmm6, xmm4
	vpermt2w xmm3, xmm6, xmm0
	vpermt2w ymm7, ymm4, yword [0x947]
	vpermt2w ymm4, ymm7, ymm3
	vpermt2w zmm6, zmm6, zmm5
	vpermt2w zmm4, zmm0, zmm2

%ifdef ERROR
	vpermt2w xmm14, xmm11, xmm14
	vpermt2w ymm14, ymm8, ymm13
	vpermt2w zmm14, zmm12, zmm8
	vpermt2w xmm19, xmm17, xmm26
	vpermt2w ymm29, ymm31, ymm16
	vpermt2w zmm28, zmm19, zmm22
%endif
