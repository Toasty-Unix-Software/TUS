	vpopcntq xmm2, xmm2
	vpopcntq xmm1, xmm7
	vpopcntq ymm0, ymm5
	vpopcntq ymm5, ymm3
	vpopcntq zmm4, zword [0x4db]
	vpopcntq zmm6, zmm6

%ifdef ERROR
	vpopcntq xmm12, xmm13
	vpopcntq ymm9, ymm13
	vpopcntq zmm13, zmm9
	vpopcntq xmm19, xmm17
	vpopcntq ymm27, ymm20
	vpopcntq zmm27, zmm19
%endif
