default rel
	vcmpordps xmm0, xmm7, oword [0x1e8]
	vcmpordps xmm0, oword [0x1e8]
	vcmpordps xmm7, xmm6, xmm7
	vcmpordps ymm1, ymm5, ymm2
	vcmpordps ymm1, ymm2
	vcmpordps ymm1, ymm1, ymm1
	vcmpordps k4, xmm2, xmm3
	vcmpordps k5, xmm4, oword [0xe5f]
	vcmpordps k4, ymm3, ymm5
	vcmpordps k6, ymm5, yword [0xab2]
	vcmpordps xmm11, xmm9, xmm11
	vcmpordps ymm8, ymm14, ymm10
	vcmpordps k6, xmm12, xmm10
	vcmpordps k6, ymm9, ymm12
	vcmpordps k3{k1}, xmm1, oword [0x927]
	vcmpordps k7{k7}, ymm1, ymm7
	vcmpordps k3{k3}, zmm7, zmm4
	vcmpordps k7, zmm5, zmm7, {sae}
	vcmpordps xmm3, xmm3, oword [eax+1]
	vcmpordps xmm4, xmm6, oword [eax+64]
	vcmpordps ymm3, ymm1, yword [eax+1]
	vcmpordps ymm5, ymm5, yword [eax+64]
	vcmpordps k5, xmm0, oword [eax+1]
	vcmpordps k2, xmm1, oword [eax+64]
	vcmpordps k6, ymm4, yword [eax+1]
	vcmpordps k2, ymm0, yword [eax+64]
	vcmpordps k6, zmm4, zword [eax+1]
	vcmpordps k3, zmm3, zword [eax+64]
	vcmpordps xmm7, xmm4, [0x9bd]
	vcmpordps ymm5, ymm2, [0x73e]
	vcmpordps k1, xmm2, [0xcbd]
	vcmpordps k1, ymm3, [0x5ac]
	vcmpordps k4, zmm5, [0x42b]
