	vpcmpgtud k2, xmm0, xmm5
	vpcmpgtud k6, xmm1, xmm3
	vpcmpgtud k2, ymm4, ymm3
	vpcmpgtud k2, ymm3, ymm3
	vpcmpgtud k1, zmm5, zmm3
	vpcmpgtud k2, zmm0, zword [0xd34]

%ifdef ERROR
	vpcmpgtud k4, xmm8, xmm10
	vpcmpgtud k6, ymm14, ymm12
	vpcmpgtud k4, zmm12, zmm12
	vpcmpgtud k4, xmm28, xmm28
	vpcmpgtud k7, ymm21, ymm28
	vpcmpgtud k4, zmm30, zmm25
%endif
