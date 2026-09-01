	vfmadd231pd xmm5, xmm7, xmm6
	vfmadd231pd xmm2, xmm3, xmm7
	vfmadd231pd ymm6, ymm7, ymm2
	vfmadd231pd ymm2, ymm4, ymm2
	vfmadd231pd xmm0, xmm3, xmm7
	vfmadd231pd xmm7, xmm2, oword [0x763]
	vfmadd231pd ymm5, ymm2, yword [0xa0c]
	vfmadd231pd ymm1, ymm3, yword [0x80f]

%ifdef ERROR
	vfmadd231pd xmm10, xmm15, xmm14
	vfmadd231pd ymm15, ymm8, ymm14
	vfmadd231pd xmm13, xmm9, xmm15
	vfmadd231pd ymm9, ymm8, ymm9
	vfmadd231pd xmm16, xmm31, xmm25
	vfmadd231pd ymm16, ymm29, ymm24
	vfmadd231pd xmm21, xmm24, xmm22
	vfmadd231pd ymm21, ymm23, ymm30
%endif
