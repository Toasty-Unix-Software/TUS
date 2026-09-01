	vfmadd132pd xmm2, xmm4, xmm3
	vfmadd132pd xmm0, xmm1, xmm7
	vfmadd132pd ymm6, ymm7, yword [0x911]
	vfmadd132pd ymm4, ymm1, ymm3
	vfmadd132pd xmm7, xmm0, oword [0xfb6]
	vfmadd132pd xmm5, xmm4, oword [0x1fa]
	vfmadd132pd ymm2, ymm5, ymm7
	vfmadd132pd ymm6, ymm2, ymm7

%ifdef ERROR
	vfmadd132pd xmm15, xmm12, xmm13
	vfmadd132pd ymm8, ymm13, ymm14
	vfmadd132pd xmm10, xmm9, xmm8
	vfmadd132pd ymm11, ymm13, ymm10
	vfmadd132pd xmm30, xmm22, xmm19
	vfmadd132pd ymm20, ymm23, ymm23
	vfmadd132pd xmm23, xmm20, xmm29
	vfmadd132pd ymm23, ymm20, ymm16
%endif
