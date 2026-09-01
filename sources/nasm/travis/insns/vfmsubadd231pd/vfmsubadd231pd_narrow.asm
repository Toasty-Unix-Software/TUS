	vfmsubadd231pd xmm2, xmm3, oword [0x2a7]
	vfmsubadd231pd xmm6, xmm7, xmm7
	vfmsubadd231pd ymm0, ymm3, ymm6
	vfmsubadd231pd ymm6, ymm5, ymm0
	vfmsubadd231pd xmm2, xmm5, xmm3
	vfmsubadd231pd xmm1, xmm1, oword [0x396]
	vfmsubadd231pd ymm0, ymm7, ymm3
	vfmsubadd231pd ymm5, ymm0, ymm1

%ifdef ERROR
	vfmsubadd231pd xmm15, xmm12, xmm13
	vfmsubadd231pd ymm9, ymm12, ymm14
	vfmsubadd231pd xmm10, xmm9, xmm12
	vfmsubadd231pd ymm12, ymm8, ymm10
	vfmsubadd231pd xmm22, xmm18, xmm31
	vfmsubadd231pd ymm25, ymm28, ymm28
	vfmsubadd231pd xmm30, xmm24, xmm18
	vfmsubadd231pd ymm26, ymm28, ymm26
%endif
