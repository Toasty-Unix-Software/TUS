	vprold xmm2, oword [0xbd4], 0x7d
	vprold xmm2, 0x7d
	vprold xmm0, xmm5, 0xb6
	vprold ymm6, ymm0, 0xf
	vprold ymm6, 0xf
	vprold ymm5, yword [0xd9d], 0xb7
	vprold zmm3, zword [0x26b], 0x85
	vprold zmm3, 0x85
	vprold zmm5, zmm7, 0xae

%ifdef ERROR
	vprold xmm8, xmm14, 0x45
	vprold ymm8, ymm12, 0xfa
	vprold zmm12, zmm14, 0xcc
	vprold xmm26, xmm18, 0xf7
	vprold ymm24, ymm17, 0x18
	vprold zmm31, zmm26, 0x20
%endif
