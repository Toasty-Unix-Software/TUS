	vpermi2q xmm2, xmm5, xmm2
	vpermi2q xmm5, xmm6, oword [0x4eb]
	vpermi2q ymm7, ymm5, ymm5
	vpermi2q ymm0, ymm2, yword [0x2f1]
	vpermi2q zmm7, zmm6, zmm4
	vpermi2q zmm6, zmm5, zmm5

%ifdef ERROR
	vpermi2q xmm9, xmm14, xmm13
	vpermi2q ymm13, ymm15, ymm14
	vpermi2q zmm14, zmm12, zmm9
	vpermi2q xmm30, xmm25, xmm26
	vpermi2q ymm30, ymm30, ymm26
	vpermi2q zmm22, zmm22, zmm16
%endif
