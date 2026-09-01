default rel
	vcmpeq_usps xmm6, xmm4, xmm4
	vcmpeq_usps xmm6, xmm4
	vcmpeq_usps xmm4, xmm0, xmm4
	vcmpeq_usps ymm4, ymm7, yword [0xf8d]
	vcmpeq_usps ymm4, yword [0xf8d]
	vcmpeq_usps ymm7, ymm0, yword [0x3fd]
	vcmpeq_usps k6, xmm7, xmm6
	vcmpeq_usps k6, xmm1, xmm6
	vcmpeq_usps k4, ymm5, yword [0xd33]
	vcmpeq_usps k3, ymm2, yword [0xd6c]
	vcmpeq_usps xmm15, xmm12, xmm10
	vcmpeq_usps ymm10, ymm12, ymm14
	vcmpeq_usps k2, xmm13, xmm13
	vcmpeq_usps k5, ymm15, ymm13
	vcmpeq_usps k3{k1}, xmm1, oword [0xc0f]
	vcmpeq_usps k4{k6}, ymm3, ymm7
	vcmpeq_usps k1{k4}, zmm1, zmm4
	vcmpeq_usps k5, zmm7, zmm6, {sae}
	vcmpeq_usps xmm2, xmm6, oword [eax+1]
	vcmpeq_usps xmm6, xmm4, oword [eax+64]
	vcmpeq_usps ymm0, ymm7, yword [eax+1]
	vcmpeq_usps ymm7, ymm1, yword [eax+64]
	vcmpeq_usps k6, xmm7, oword [eax+1]
	vcmpeq_usps k2, xmm4, oword [eax+64]
	vcmpeq_usps k1, ymm6, yword [eax+1]
	vcmpeq_usps k7, ymm0, yword [eax+64]
	vcmpeq_usps k4, zmm1, zword [eax+1]
	vcmpeq_usps k6, zmm1, zword [eax+64]
	vcmpeq_usps xmm0, xmm1, [0x823]
	vcmpeq_usps ymm4, ymm7, [0xd28]
	vcmpeq_usps k1, xmm0, [0xb0d]
	vcmpeq_usps k1, ymm5, [0x80d]
	vcmpeq_usps k3, zmm0, [0x358]
