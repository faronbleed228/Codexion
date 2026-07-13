#include "codexion.h"

void	coders_print(t_coder *coder);

int	main(int argc, char **argv)
{
	t_parsing	*pars_struct;
	int			i;
	t_coder		*first_coder;

	if (argc != 9)
	{
		fprintf(stderr, "Not enough arguments");
		return (1);
	}
	pars_struct = parsing(argv);
	if (pars_struct == NULL)
	{
		fprintf(stderr, "Validation Error");
		return (1);
	}
	printf("Number of coders: %i\n", pars_struct->coders);
	printf("Time to burn: %i\n", pars_struct->t_to_burn);
	printf("Time to compile: %i\n", pars_struct->t_to_copm);
	printf("Time to debug: %i\n", pars_struct->t_to_debug);
	printf("Time to refactor: %i\n", pars_struct->t_to_refac);
	printf("Comp required: %i\n", pars_struct->copm_req);
	printf("Cooldown time: %i\n", pars_struct->cooldown);
	printf("Scheduler: %s\n\n", pars_struct->scheduler);
	first_coder = coders_init(pars_struct);
	i = 0;
	while (i < pars_struct->coders)
	{
		coders_print(first_coder);
		i++;
		first_coder = first_coder->right_coder;
		printf("\n");
	}
}

void	coders_print(t_coder *coder)
{
	printf("Coder NUMBER:%i\n", coder->cod_num);
	printf("Current status: %i\n", coder->cur_status);
	printf("Last comp time: %i\n", coder->last_comp_time);
	printf("Left Coder Id: %i\n", coder->left_dongle->dongle_id);
	printf("Right Coder id: %i\n", coder->right_dongle->dongle_id);
	printf("Number of compiles: %i\n", coder->n_compiles);
}
