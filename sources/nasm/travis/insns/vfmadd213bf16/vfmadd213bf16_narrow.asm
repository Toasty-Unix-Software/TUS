	vfmadd213bf16 xmm4, xmm6, oword [0x1c3]
	vfmadd213bf16 xmm1, xmm1, oword [0x2ff]
	vfmadd213bf16 ymm3, ymm5, ymm5
	vfmadd213bf16 ymm6, ymm6, ymm2
	vfmadd213bf16 zmm7, zmm2, zmm4
	vfmadd213bf16 zmm6, zmm3, zword [0xabc]

%ifdef ERROR
	vfmadd213bf16 xmm9, xmm14, xmm15
	vfmadd213bf16 ymm11, ymm12, ymm8
	vfmadd213bf16 zmm13, zmm9, zmm10
	vfmadd213bf16 xmm24, xmm23, xmm24
	vfmadd213bf16 ymm28, ymm26, ymm25
	vfmadd213bf16 zmm24, zmm18, zmm20
%endif
