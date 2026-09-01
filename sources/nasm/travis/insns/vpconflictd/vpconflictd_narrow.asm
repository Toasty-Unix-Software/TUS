	vpconflictd xmm4, xmm3
	vpconflictd xmm0, xmm2
	vpconflictd ymm3, ymm1
	vpconflictd ymm2, ymm3
	vpconflictd zmm7, zmm6
	vpconflictd zmm3, zword [0xa82]

%ifdef ERROR
	vpconflictd xmm9, xmm11
	vpconflictd ymm13, ymm13
	vpconflictd zmm11, zmm14
	vpconflictd xmm26, xmm30
	vpconflictd ymm18, ymm29
	vpconflictd zmm28, zmm23
%endif
