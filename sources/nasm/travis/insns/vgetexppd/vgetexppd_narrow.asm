	vgetexppd xmm2, xmm5
	vgetexppd xmm3, xmm1
	vgetexppd ymm0, yword [0xf02]
	vgetexppd ymm2, ymm0
	vgetexppd zmm6, zmm4
	vgetexppd zmm5, zword [0x4da]

%ifdef ERROR
	vgetexppd xmm14, xmm13
	vgetexppd ymm15, ymm11
	vgetexppd zmm13, zmm10
	vgetexppd xmm22, xmm30
	vgetexppd ymm28, ymm22
	vgetexppd zmm31, zmm17
%endif
