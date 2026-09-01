	vfnmadd132pd xmm7, xmm6, xmm4
	vfnmadd132pd xmm0, xmm7, oword [0xdbb]
	vfnmadd132pd ymm4, ymm2, ymm3
	vfnmadd132pd ymm6, ymm5, ymm1
	vfnmadd132pd xmm5, xmm5, xmm0
	vfnmadd132pd xmm2, xmm7, xmm1
	vfnmadd132pd ymm2, ymm3, ymm1
	vfnmadd132pd ymm3, ymm2, yword [0x98c]

%ifdef ERROR
	vfnmadd132pd xmm11, xmm9, xmm14
	vfnmadd132pd ymm10, ymm8, ymm9
	vfnmadd132pd xmm8, xmm9, xmm9
	vfnmadd132pd ymm15, ymm12, ymm9
	vfnmadd132pd xmm27, xmm18, xmm30
	vfnmadd132pd ymm17, ymm16, ymm21
	vfnmadd132pd xmm17, xmm29, xmm20
	vfnmadd132pd ymm28, ymm27, ymm19
%endif
