	vfnmsub231pd xmm2, xmm5, xmm6
	vfnmsub231pd xmm3, xmm6, xmm5
	vfnmsub231pd ymm7, ymm5, ymm3
	vfnmsub231pd ymm3, ymm6, ymm5
	vfnmsub231pd xmm0, xmm0, xmm0
	vfnmsub231pd xmm7, xmm4, xmm1
	vfnmsub231pd ymm0, ymm5, yword [0x889]
	vfnmsub231pd ymm4, ymm7, ymm0

%ifdef ERROR
	vfnmsub231pd xmm12, xmm14, xmm11
	vfnmsub231pd ymm9, ymm10, ymm10
	vfnmsub231pd xmm10, xmm14, xmm9
	vfnmsub231pd ymm14, ymm10, ymm11
	vfnmsub231pd xmm30, xmm25, xmm20
	vfnmsub231pd ymm18, ymm16, ymm30
	vfnmsub231pd xmm18, xmm21, xmm16
	vfnmsub231pd ymm28, ymm23, ymm23
%endif
