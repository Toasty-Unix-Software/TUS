default rel
	vcmpeqps xmm3, xmm4, xmm7
	vcmpeqps xmm3, xmm7
	vcmpeqps xmm5, xmm6, xmm3
	vcmpeqps ymm7, ymm7, yword [0x7e3]
	vcmpeqps ymm7, yword [0x7e3]
	vcmpeqps ymm0, ymm5, yword [0xd1f]
	vcmpeqps k2, xmm6, xmm2
	vcmpeqps k6, xmm2, oword [0xca1]
	vcmpeqps k3, ymm6, ymm4
	vcmpeqps k6, ymm6, ymm0
	vcmpeqps xmm11, xmm13, xmm13
	vcmpeqps ymm13, ymm11, ymm14
	vcmpeqps k6, xmm10, xmm10
	vcmpeqps k6, ymm10, ymm10
	vcmpeqps k4{k1}, xmm0, xmm3
	vcmpeqps k1{k2}, ymm7, ymm6
	vcmpeqps k2{k6}, zmm5, zmm4
	vcmpeqps k3, zmm6, zmm6, {sae}
	vcmpeqps xmm4, xmm5, oword [eax+1]
	vcmpeqps xmm6, xmm2, oword [eax+64]
	vcmpeqps ymm4, ymm3, yword [eax+1]
	vcmpeqps ymm0, ymm5, yword [eax+64]
	vcmpeqps k3, xmm6, oword [eax+1]
	vcmpeqps k4, xmm5, oword [eax+64]
	vcmpeqps k2, ymm6, yword [eax+1]
	vcmpeqps k3, ymm0, yword [eax+64]
	vcmpeqps k5, zmm7, zword [eax+1]
	vcmpeqps k7, zmm6, zword [eax+64]
	vcmpeqps xmm2, xmm1, [0x820]
	vcmpeqps ymm7, ymm7, [0x2c0]
	vcmpeqps k3, xmm0, [0xd6d]
	vcmpeqps k2, ymm0, [0x616]
	vcmpeqps k5, zmm7, [0x7a4]
