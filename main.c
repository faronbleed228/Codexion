#include "codexion.h"

void	coders_print(t_coder coder);
void	parsing_print(t_parsing *pars_struct);

int	main(int argc, char **argv)
{
	t_parsing		*pars_struct;
	int				i;
	t_simulation	*sim_struct;

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
	parsing_print(pars_struct);
	sim_struct = simulation_init(pars_struct);
	if (!sim_struct)
	{
		fprintf(stderr, "Coders Error");
		return (1);
	}
	i = 0;
	while (i < pars_struct->coders)
	{
		coders_print(sim_struct->coder_array[i]);
		printf("\n");
		i++;
	}
	if (simulation_start(sim_struct) != 0)
	{
		// free_everything(sim_struct, sim_struct->free_struct);
	}
}

void	coders_print(t_coder coder)
{
	printf("Coder NUMBER:%i\n", coder.cod_num);
	printf("Current status: %i\n", coder.cur_status);
	printf("Last comp time: %li\n", coder.last_comp_time);
	printf("Left dongle Id: %i\n", coder.left_dongle->dongle_id);
	printf("Right dongle id: %i\n", coder.right_dongle->dongle_id);
	printf("Number of compiles: %i\n", coder.n_compiles);
}

void	parsing_print(t_parsing *pars_struct)
{
	printf("Number of coders: %i\n", pars_struct->coders);
	printf("Time to burn: %li\n", pars_struct->t_to_burn);
	printf("Time to compile: %li\n", pars_struct->t_to_copm);
	printf("Time to debug: %li\n", pars_struct->t_to_debug);
	printf("Time to refactor: %li\n", pars_struct->t_to_refac);
	printf("Comp required: %li\n", pars_struct->copm_req);
	printf("Cooldown time: %li\n", pars_struct->cooldown);
	printf("Scheduler: %s\n\n", pars_struct->scheduler);
}
