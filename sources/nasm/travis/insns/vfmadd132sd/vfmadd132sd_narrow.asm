	vfmadd132sd xmm4, xmm3, qword [0x3db]
	vfmadd132sd xmm5, xmm6, xmm6
	vfmadd132sd xmm3, xmm5, xmm6
	vfmadd132sd xmm4, xmm6, xmm5

%ifdef ERROR
	vfmadd132sd xmm8, xmm11, xmm15
	vfmadd132sd xmm14, xmm8, xmm9
	vfmadd132sd xmm31, xmm27, xmm30
	vfmadd132sd xmm29, xmm24, xmm19
%endif
