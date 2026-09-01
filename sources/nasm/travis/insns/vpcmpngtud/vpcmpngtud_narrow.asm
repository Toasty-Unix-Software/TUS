	vpcmpngtud k6, xmm4, oword [0xb74]
	vpcmpngtud k7, xmm1, xmm7
	vpcmpngtud k5, ymm4, yword [0x9aa]
	vpcmpngtud k6, ymm1, ymm4
	vpcmpngtud k2, zmm7, zmm7
	vpcmpngtud k2, zmm3, zword [0x4cc]

%ifdef ERROR
	vpcmpngtud k7, xmm9, xmm11
	vpcmpngtud k1, ymm12, ymm9
	vpcmpngtud k4, zmm9, zmm12
	vpcmpngtud k6, xmm19, xmm23
	vpcmpngtud k5, ymm25, ymm25
	vpcmpngtud k1, zmm19, zmm20
%endif
