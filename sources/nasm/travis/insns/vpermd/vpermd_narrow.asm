	vpermd ymm4, ymm2, ymm3
	vpermd ymm4, ymm3
	vpermd ymm5, ymm3, yword [0x290]
	vpermd ymm3, ymm0, ymm1
	vpermd ymm3, ymm1
	vpermd ymm0, ymm4, ymm5
	vpermd zmm0, zmm5, zmm0
	vpermd zmm0, zmm0
	vpermd zmm3, zmm1, zmm7

%ifdef ERROR
	vpermd ymm11, ymm11, ymm12
	vpermd ymm9, ymm10, ymm13
	vpermd zmm12, zmm13, zmm10
	vpermd ymm31, ymm24, ymm21
	vpermd ymm21, ymm21, ymm17
	vpermd zmm19, zmm19, zmm29
%endif
