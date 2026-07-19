#include "codexion.h"

void	free_sim(t_simulation *sim_struct)
{
	if (sim_struct)
	{
		if (sim_struct->coder_array)
			free(sim_struct->coder_array);
		if (sim_struct->dongle_array)
			free(sim_struct->dongle_array);
		if (sim_struct->pars_struct)
			free(sim_struct->pars_struct);
		free(sim_struct);
	}
}

int	free_mutex(t_simulation *sim_struct, int counter)
{
	int	i;

	i = 0;
	while (i < counter)
	{
		pthread_mutex_destroy(&sim_struct->dongle_array[i].lock);
		i++;
	}
	return (0);
}

int	free_cond(t_simulation *sim_struct, int counter)
{
	int	i;

	i = 0;
	while (i < counter)
	{
		pthread_cond_destroy(&sim_struct->dongle_array[i].cond);
		i++;
	}
	return (0);
}

void	ft_end_thread(int counter, t_simulation *sim_struct)
{
	int	i;

	i = 0;
	while (i < counter)
	{
		pthread_join(sim_struct->coder_array[i].thread_id, NULL);
		i++;
	}
}
