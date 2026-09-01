	vfnmadd213bf16 xmm3, xmm2, xmm0
	vfnmadd213bf16 xmm0, xmm3, oword [0xb74]
	vfnmadd213bf16 ymm4, ymm6, ymm5
	vfnmadd213bf16 ymm1, ymm3, yword [0xf59]
	vfnmadd213bf16 zmm5, zmm3, zmm5
	vfnmadd213bf16 zmm7, zmm1, zword [0xf15]

%ifdef ERROR
	vfnmadd213bf16 xmm14, xmm10, xmm10
	vfnmadd213bf16 ymm12, ymm11, ymm13
	vfnmadd213bf16 zmm15, zmm12, zmm8
	vfnmadd213bf16 xmm18, xmm17, xmm29
	vfnmadd213bf16 ymm26, ymm30, ymm30
	vfnmadd213bf16 zmm18, zmm23, zmm31
%endif
