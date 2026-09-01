	vfmadd132bf16 xmm1, xmm5, xmm5
	vfmadd132bf16 xmm2, xmm6, xmm3
	vfmadd132bf16 ymm6, ymm7, yword [0xb04]
	vfmadd132bf16 ymm7, ymm4, ymm4
	vfmadd132bf16 zmm0, zmm4, zword [0x5fc]
	vfmadd132bf16 zmm2, zmm7, zmm7

%ifdef ERROR
	vfmadd132bf16 xmm12, xmm12, xmm12
	vfmadd132bf16 ymm13, ymm12, ymm8
	vfmadd132bf16 zmm9, zmm15, zmm13
	vfmadd132bf16 xmm27, xmm22, xmm18
	vfmadd132bf16 ymm17, ymm27, ymm19
	vfmadd132bf16 zmm26, zmm23, zmm20
%endif
