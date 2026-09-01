	vrsqrt28pd zmm2, zword [0xd93]
	vrsqrt28pd zmm4, zword [0x470]

%ifdef ERROR
	vrsqrt28pd zmm12, zmm11
	vrsqrt28pd zmm29, zmm22
%endif
