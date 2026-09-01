	invlpga ax, ecx
	invlpga ax, ecx
	invlpga eax, ecx
	invlpga eax, ecx
	invlpga
	invlpga

%ifdef ERROR
	invlpga rax, ecx
	invlpga rax, ecx
%endif
