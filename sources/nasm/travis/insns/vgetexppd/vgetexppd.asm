default rel
	vgetexppd xmm2, xmm5
	vgetexppd xmm3, xmm1
	vgetexppd ymm0, yword [0xf02]
	vgetexppd ymm2, ymm0
	vgetexppd zmm6, zmm4
	vgetexppd zmm5, zword [0x4da]
	vgetexppd xmm14, xmm13
	vgetexppd ymm15, ymm11
	vgetexppd zmm13, zmm10
	vgetexppd xmm22, xmm30
	vgetexppd ymm28, ymm22
	vgetexppd zmm31, zmm17
	vgetexppd xmm5{k7}, xmm1
	vgetexppd ymm3{k6}, ymm7
	vgetexppd zmm3{k5}, zmm2
	vgetexppd xmm6{k3}{z}, xmm5
	vgetexppd ymm4{k2}{z}, yword [0xf6e]
	vgetexppd zmm3{k3}{z}, zmm0
	vgetexppd zmm4, zmm1, {sae}
	vgetexppd xmm1, oword [eax+1]
	vgetexppd xmm0, oword [eax+64]
	vgetexppd ymm4, yword [eax+1]
	vgetexppd ymm7, yword [eax+64]
	vgetexppd zmm1, zword [eax+1]
	vgetexppd zmm4, zword [eax+64]
	vgetexppd xmm5, [0xdc4]
	vgetexppd ymm5, [0x43b]
	vgetexppd zmm0, [0x12d]
