#include "../includes/asm.h"
#include "../libft/includes/libft.h"

t_instruction	set_instructions_argument_types(t_instruction instructions)
{
	instructions.instruct_arg[0][1] = T_DIR;

	instructions.instruct_arg[1][1] = T_DIR;
	instructions.instruct_arg[1][2] = T_IND | T_REG;

	instructions.instruct_arg[2][1] = T_REG;
	instructions.instruct_arg[2][2] = T_IND | T_REG;

	instructions.instruct_arg[3][1] = T_REG;
	instructions.instruct_arg[3][2] = T_REG;
	instructions.instruct_arg[3][3] = T_REG;

	instructions.instruct_arg[4][1] = T_REG;
	instructions.instruct_arg[4][2] = T_REG;
	instructions.instruct_arg[4][3] = T_REG;

	instructions.instruct_arg[5][1] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[5][2] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[5][3] = T_REG;

	instructions.instruct_arg[6][1] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[6][2] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[6][3] = T_REG;

	instructions.instruct_arg[7][1] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[7][2] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[7][3] = T_REG;

	instructions.instruct_arg[8][1] = T_DIR;

	instructions.instruct_arg[9][1] = T_REG | T_DIR | T_DIR;
	instructions.instruct_arg[9][2] = T_DIR | T_REG;
	instructions.instruct_arg[9][3] = T_REG;

	instructions.instruct_arg[10][1] = T_REG;
	instructions.instruct_arg[10][2] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[10][3] = T_REG | T_DIR;

	instructions.instruct_arg[11][1] = T_DIR;

	instructions.instruct_arg[12][1] = T_DIR;
	instructions.instruct_arg[12][2] = T_IND | T_REG;

	instructions.instruct_arg[13][1] = T_REG | T_DIR | T_IND;
	instructions.instruct_arg[13][2] = T_REG | T_DIR;
	instructions.instruct_arg[13][3] = T_REG;

	instructions.instruct_arg[14][1] = T_DIR;

	instructions.instruct_arg[15][1] = T_REG;
	return (instructions);
}

t_instruction	set_instructions_argument_nbr(t_instruction instructions)
{
	instructions.instruct_arg[0][0] = 1;
	instructions.instruct_arg[1][0] = 2;
	instructions.instruct_arg[2][0] = 2;
	instructions.instruct_arg[3][0] = 3;
	instructions.instruct_arg[4][0] = 3;
	instructions.instruct_arg[5][0] = 3;
	instructions.instruct_arg[6][0] = 3;
	instructions.instruct_arg[7][0] = 3;
	instructions.instruct_arg[8][0] = 1;
	instructions.instruct_arg[9][0] = 3;
	instructions.instruct_arg[10][0] = 3;
	instructions.instruct_arg[11][0] = 1;
	instructions.instruct_arg[12][0] = 2;
	instructions.instruct_arg[13][0] = 3;
	instructions.instruct_arg[14][0] = 1;
	instructions.instruct_arg[15][0] = 1;
	return (set_instructions_argument_types(instructions));
}

t_instruction	set_instructions(void)
{
	t_instruction instructions;

	ft_strcpy(instructions.names[0], "live");
	ft_strcpy(instructions.names[1], "ld");
	ft_strcpy(instructions.names[2], "st");
	ft_strcpy(instructions.names[3], "add");
	ft_strcpy(instructions.names[4], "sub");
	ft_strcpy(instructions.names[5], "and");
	ft_strcpy(instructions.names[6], "or");
	ft_strcpy(instructions.names[7], "xor");
	ft_strcpy(instructions.names[8], "zjmp");
	ft_strcpy(instructions.names[9], "ldi");
	ft_strcpy(instructions.names[10], "sti");
	ft_strcpy(instructions.names[11], "fork");
	ft_strcpy(instructions.names[12], "lld");
	ft_strcpy(instructions.names[13], "lldi");
	ft_strcpy(instructions.names[14], "lfork");
	ft_strcpy(instructions.names[15], "aff");
	return (set_instructions_argument_nbr(instructions));
}
