default rel
	vcmpneqps xmm5, xmm1, xmm3
	vcmpneqps xmm5, xmm3
	vcmpneqps xmm4, xmm0, xmm5
	vcmpneqps ymm2, ymm3, ymm0
	vcmpneqps ymm2, ymm0
	vcmpneqps ymm0, ymm4, yword [0xeac]
	vcmpneqps k4, xmm0, xmm5
	vcmpneqps k7, xmm3, xmm4
	vcmpneqps k7, ymm0, ymm2
	vcmpneqps k5, ymm0, yword [0xa86]
	vcmpneqps xmm12, xmm15, xmm13
	vcmpneqps ymm13, ymm10, ymm12
	vcmpneqps k2, xmm12, xmm9
	vcmpneqps k7, ymm10, ymm15
	vcmpneqps k4{k2}, xmm2, xmm2
	vcmpneqps k3{k5}, ymm6, yword [0x104]
	vcmpneqps k5{k5}, zmm2, zmm1
	vcmpneqps k7, zmm7, zmm2, {sae}
	vcmpneqps xmm3, xmm5, oword [eax+1]
	vcmpneqps xmm3, xmm3, oword [eax+64]
	vcmpneqps ymm0, ymm6, yword [eax+1]
	vcmpneqps ymm1, ymm7, yword [eax+64]
	vcmpneqps k3, xmm3, oword [eax+1]
	vcmpneqps k4, xmm5, oword [eax+64]
	vcmpneqps k3, ymm2, yword [eax+1]
	vcmpneqps k6, ymm1, yword [eax+64]
	vcmpneqps k6, zmm5, zword [eax+1]
	vcmpneqps k3, zmm6, zword [eax+64]
	vcmpneqps xmm2, xmm5, [0x7ff]
	vcmpneqps ymm5, ymm0, [0x348]
	vcmpneqps k6, xmm0, [0x655]
	vcmpneqps k5, ymm6, [0xfe3]
	vcmpneqps k1, zmm6, [0x1e5]
