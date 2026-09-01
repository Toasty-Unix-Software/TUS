	vfnmadd231pd xmm4, xmm5, xmm0
	vfnmadd231pd xmm0, xmm3, xmm1
	vfnmadd231pd ymm5, ymm4, ymm4
	vfnmadd231pd ymm7, ymm2, yword [0x563]
	vfnmadd231pd xmm5, xmm2, xmm5
	vfnmadd231pd xmm3, xmm0, oword [0xd65]
	vfnmadd231pd ymm0, ymm4, ymm1
	vfnmadd231pd ymm0, ymm7, ymm4

%ifdef ERROR
	vfnmadd231pd xmm10, xmm13, xmm14
	vfnmadd231pd ymm13, ymm8, ymm12
	vfnmadd231pd xmm13, xmm12, xmm15
	vfnmadd231pd ymm12, ymm9, ymm11
	vfnmadd231pd xmm30, xmm16, xmm19
	vfnmadd231pd ymm21, ymm18, ymm28
	vfnmadd231pd xmm30, xmm28, xmm21
	vfnmadd231pd ymm20, ymm18, ymm26
%endif
