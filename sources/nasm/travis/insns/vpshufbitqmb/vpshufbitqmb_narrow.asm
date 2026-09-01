	vpshufbitqmb k7, xmm2, oword [0x57f]
	vpshufbitqmb k5, xmm2, xmm1
	vpshufbitqmb k2, ymm1, ymm1
	vpshufbitqmb k6, ymm5, ymm5
	vpshufbitqmb k3, zmm6, zword [0xbbf]
	vpshufbitqmb k5, zmm7, zmm5

%ifdef ERROR
	vpshufbitqmb k5, xmm13, xmm9
	vpshufbitqmb k3, ymm8, ymm13
	vpshufbitqmb k2, zmm11, zmm10
	vpshufbitqmb k3, xmm23, xmm30
	vpshufbitqmb k7, ymm21, ymm19
	vpshufbitqmb k1, zmm25, zmm20
%endif
