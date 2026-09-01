	vfnmadd213ps xmm0, xmm2, xmm7
	vfnmadd213ps xmm0, xmm2, oword [0x666]
	vfnmadd213ps ymm7, ymm6, yword [0xdce]
	vfnmadd213ps ymm2, ymm2, yword [0x5b6]
	vfnmadd213ps xmm0, xmm1, xmm3
	vfnmadd213ps xmm4, xmm0, xmm1
	vfnmadd213ps ymm7, ymm3, ymm6
	vfnmadd213ps ymm3, ymm4, yword [0x20d]

%ifdef ERROR
	vfnmadd213ps xmm13, xmm8, xmm8
	vfnmadd213ps ymm11, ymm12, ymm11
	vfnmadd213ps xmm14, xmm15, xmm15
	vfnmadd213ps ymm10, ymm14, ymm13
	vfnmadd213ps xmm26, xmm16, xmm23
	vfnmadd213ps ymm29, ymm21, ymm25
	vfnmadd213ps xmm29, xmm16, xmm22
	vfnmadd213ps ymm29, ymm22, ymm17
%endif
