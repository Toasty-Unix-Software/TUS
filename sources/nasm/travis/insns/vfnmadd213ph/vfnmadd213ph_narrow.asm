	vfnmadd213ph xmm6, xmm0, xmm0
	vfnmadd213ph xmm6, xmm0
	vfnmadd213ph xmm7, xmm1, xmm3
	vfnmadd213ph ymm1, ymm3, ymm3
	vfnmadd213ph ymm1, ymm3
	vfnmadd213ph ymm7, ymm7, ymm6
	vfnmadd213ph zmm0, zmm5, zmm4
	vfnmadd213ph zmm0, zmm4
	vfnmadd213ph zmm7, zmm5, zmm1

%ifdef ERROR
	vfnmadd213ph xmm15, xmm14, xmm9
	vfnmadd213ph ymm13, ymm8, ymm9
	vfnmadd213ph zmm8, zmm10, zmm14
	vfnmadd213ph xmm18, xmm19, xmm30
	vfnmadd213ph ymm27, ymm20, ymm30
	vfnmadd213ph zmm27, zmm29, zmm17
%endif
