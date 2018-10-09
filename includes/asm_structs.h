#ifndef ASM_STRUCTS_H
# define ASM_STRUCTS_H

# define INSTRUCT_NBR			16

typedef struct					s_op
{
	char						*name;
	unsigned int				parameters_nbr;
	unsigned char				parameter_types[3];
	unsigned int				instruction_nbr;
	unsigned int				cycles;
	char						*description;
	unsigned int				why;
	unsigned int				double_dir_size;
}								t_op;

typedef struct					s_instructions
{
	char						names[INSTRUCT_NBR][64];
	unsigned char				instruct_arg[INSTRUCT_NBR][4];
}								t_instruction;

typedef	struct					s_fd
{
	int							read;
	int							write;
}								t_fd;

typedef struct					s_labels
{
	unsigned int				pos_in_file;
	unsigned int				name_len;
	unsigned int				label_pos;
	unsigned int				write_pos;
	unsigned int				byte_size;
	void						*next;
}								t_label;

typedef struct					s_label_info
{
	unsigned int				n;
	unsigned int				label_categories;
	t_label						*label_list;
}								t_label_info;

typedef struct					s_token
{
	unsigned int				token[4][2];
	unsigned int				func_nbr;
	unsigned int				line_nbr;
}								t_token;

typedef struct					s_cmd
{
	unsigned char				cmd_binary;
	unsigned char				one_cmd;
	unsigned char				cmd_size;
}								t_cmd;

typedef struct					s_info
{
	unsigned int				error;

	char						*file;
	unsigned int				read_pos;

	t_token						*tokens;
	unsigned int				token_struct_size;
	unsigned int				token_nbr;

	char						file_name[256];
	t_label_info				label_info;
	t_header					header;
	char						*to_write;

	unsigned int				cmd_begin_pos;
	unsigned int				to_write_size;
	unsigned int				write_pos;
}								t_info;

#endif
