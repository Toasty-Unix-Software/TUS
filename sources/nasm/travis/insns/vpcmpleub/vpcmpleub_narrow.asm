	vpcmpleub k4, xmm7, xmm0
	vpcmpleub k3, xmm1, oword [0xf6e]
	vpcmpleub k6, ymm5, yword [0xebf]
	vpcmpleub k6, ymm0, yword [0xdb7]
	vpcmpleub k1, zmm7, zmm2
	vpcmpleub k2, zmm2, zmm2

%ifdef ERROR
	vpcmpleub k3, xmm8, xmm13
	vpcmpleub k1, ymm15, ymm12
	vpcmpleub k3, zmm11, zmm15
	vpcmpleub k2, xmm22, xmm27
	vpcmpleub k5, ymm19, ymm20
	vpcmpleub k3, zmm20, zmm28
%endif
