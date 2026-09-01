	kmovw k2, k6
	kmovw k7, k6
	kmovw word [0x4f1], k1
	kmovw word [0xf8e], k2
	kmovw k6, esi
	kmovw k5, ebp
	kmovw k3, cx
	kmovw k5, bp

%ifdef ERROR
	kmovw k3, r8d
	kmovw k4, r14w
	kmovw k4, r25d
	kmovw k5, r29w
%endif
