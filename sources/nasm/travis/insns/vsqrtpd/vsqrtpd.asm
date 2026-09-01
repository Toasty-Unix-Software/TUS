default rel
	vsqrtpd xmm7, oword [0x3a8]
	vsqrtpd xmm1, xmm6
	vsqrtpd ymm1, ymm3
	vsqrtpd ymm0, ymm7
	vsqrtpd xmm0, oword [0xb0a]
	vsqrtpd xmm5, xmm7
	vsqrtpd ymm5, ymm5
	vsqrtpd ymm0, ymm2
	vsqrtpd xmm10, xmm10
	vsqrtpd ymm15, ymm14
	vsqrtpd xmm12, xmm11
	vsqrtpd ymm12, ymm12
	vsqrtpd xmm27, xmm24
	vsqrtpd ymm31, ymm18
	vsqrtpd xmm18, xmm23
	vsqrtpd ymm22, ymm29
	vsqrtpd xmm3{k1}, xmm6
	vsqrtpd ymm5{k1}, yword [0x4be]
	vsqrtpd zmm1{k5}, zmm7
	vsqrtpd xmm7{k6}{z}, oword [0x3c3]
	vsqrtpd ymm2{k3}{z}, ymm2
	vsqrtpd zmm6{k1}{z}, zmm2
	vsqrtpd zmm3, zmm4, {rn-sae}
	vsqrtpd xmm1, oword [eax+1]
	vsqrtpd xmm7, oword [eax+64]
	vsqrtpd ymm7, yword [eax+1]
	vsqrtpd ymm3, yword [eax+64]
	vsqrtpd xmm5, oword [eax+1]
	vsqrtpd xmm1, oword [eax+64]
	vsqrtpd ymm2, yword [eax+1]
	vsqrtpd ymm6, yword [eax+64]
	vsqrtpd zmm7, zword [eax+1]
	vsqrtpd zmm1, zword [eax+64]
	vsqrtpd xmm1, [0xb2a]
	vsqrtpd ymm1, [0x753]
	vsqrtpd xmm5, [0x804]
	vsqrtpd ymm3, [0xf13]
	vsqrtpd zmm3, [0x380]
