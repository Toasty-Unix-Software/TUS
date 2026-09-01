	vfmadd213pd xmm6, xmm0, xmm3
	vfmadd213pd xmm5, xmm1, xmm5
	vfmadd213pd ymm6, ymm6, yword [0xde9]
	vfmadd213pd ymm1, ymm4, ymm7
	vfmadd213pd xmm1, xmm4, oword [0x6f8]
	vfmadd213pd xmm4, xmm4, xmm4
	vfmadd213pd ymm3, ymm5, yword [0xbb2]
	vfmadd213pd ymm7, ymm6, yword [0xa78]

%ifdef ERROR
	vfmadd213pd xmm8, xmm12, xmm10
	vfmadd213pd ymm13, ymm10, ymm11
	vfmadd213pd xmm13, xmm12, xmm8
	vfmadd213pd ymm11, ymm15, ymm14
	vfmadd213pd xmm16, xmm19, xmm29
	vfmadd213pd ymm27, ymm18, ymm25
	vfmadd213pd xmm26, xmm20, xmm22
	vfmadd213pd ymm26, ymm28, ymm18
%endif
