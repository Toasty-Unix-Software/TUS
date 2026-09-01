	vfnmadd213pd xmm1, xmm7, xmm1
	vfnmadd213pd xmm5, xmm1, oword [0xcda]
	vfnmadd213pd ymm0, ymm0, ymm3
	vfnmadd213pd ymm7, ymm6, ymm3
	vfnmadd213pd xmm0, xmm1, xmm5
	vfnmadd213pd xmm6, xmm0, oword [0xab1]
	vfnmadd213pd ymm0, ymm7, ymm3
	vfnmadd213pd ymm2, ymm6, ymm5

%ifdef ERROR
	vfnmadd213pd xmm9, xmm8, xmm9
	vfnmadd213pd ymm15, ymm15, ymm12
	vfnmadd213pd xmm14, xmm15, xmm10
	vfnmadd213pd ymm12, ymm14, ymm12
	vfnmadd213pd xmm23, xmm21, xmm16
	vfnmadd213pd ymm19, ymm30, ymm21
	vfnmadd213pd xmm27, xmm30, xmm22
	vfnmadd213pd ymm21, ymm26, ymm23
%endif
