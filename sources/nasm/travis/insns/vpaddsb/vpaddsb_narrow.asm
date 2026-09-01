	vpaddsb xmm7, xmm0, xmm6
	vpaddsb xmm7, xmm6
	vpaddsb xmm3, xmm2, oword [0xf4c]
	vpaddsb ymm0, ymm7, ymm7
	vpaddsb ymm0, ymm7
	vpaddsb ymm0, ymm4, ymm6
	vpaddsb xmm3, xmm6, oword [0xa82]
	vpaddsb xmm3, oword [0xa82]
	vpaddsb xmm6, xmm6, xmm5
	vpaddsb ymm6, ymm2, yword [0xc53]
	vpaddsb ymm6, yword [0xc53]
	vpaddsb ymm0, ymm2, ymm2

%ifdef ERROR
	vpaddsb xmm11, xmm14, xmm12
	vpaddsb ymm12, ymm11, ymm12
	vpaddsb xmm12, xmm11, xmm12
	vpaddsb ymm9, ymm9, ymm15
	vpaddsb xmm31, xmm21, xmm20
	vpaddsb ymm29, ymm29, ymm25
	vpaddsb xmm20, xmm16, xmm18
	vpaddsb ymm31, ymm29, ymm28
%endif
