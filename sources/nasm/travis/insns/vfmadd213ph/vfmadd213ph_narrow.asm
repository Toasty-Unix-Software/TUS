	vfmadd213ph xmm2, xmm0, oword [0xf56]
	vfmadd213ph xmm2, oword [0xf56]
	vfmadd213ph xmm6, xmm3, xmm0
	vfmadd213ph ymm6, ymm3, ymm2
	vfmadd213ph ymm6, ymm2
	vfmadd213ph ymm0, ymm0, ymm1
	vfmadd213ph zmm7, zmm1, zmm4
	vfmadd213ph zmm7, zmm4
	vfmadd213ph zmm0, zmm6, zmm7

%ifdef ERROR
	vfmadd213ph xmm15, xmm12, xmm12
	vfmadd213ph ymm15, ymm10, ymm10
	vfmadd213ph zmm15, zmm14, zmm9
	vfmadd213ph xmm19, xmm31, xmm30
	vfmadd213ph ymm29, ymm30, ymm24
	vfmadd213ph zmm17, zmm19, zmm28
%endif
