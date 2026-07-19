#include "codexion.h"

static int	mutex_cond_init(t_simulation *sim_struct)
{
	int	i;

	if (pthread_mutex_init(&sim_struct->lock, NULL) != 0)
		return (0);
	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		if (pthread_cond_init(&sim_struct->dongle_array[i++].cond, NULL) != 0)
		{
			fprintf(stderr, "Cond init failed");
			return (free_cond(sim_struct, --i));
		}
	}
	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		if (pthread_mutex_init(&sim_struct->dongle_array[i++].lock, NULL) != 0)
		{
			fprintf(stderr, "Mutex init failed");
			free_cond(sim_struct, sim_struct->pars_struct->coders);
			return (free_mutex(sim_struct, --i));
		}
	}
	return (1);
}

void	*coder_routine(void *coder)
{
	return (0);
}

static int	thread_init(t_simulation *sim_struct)
{
	int	i;

	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		if (pthread_create(&sim_struct->coder_array[i].thread_id, NULL,
				&coder_routine, (void *)&sim_struct->coder_array[i]) != 0)
			return (i);
		i++;
	}
	return (i);
}

int	simulation_start(t_simulation *sim_struct)
{
	int	counter;

	if (mutex_cond_init(sim_struct) == 0)
		return (0);
	counter = thread_init(sim_struct);
	if (counter != sim_struct->pars_struct->coders)
	{
		printf("I was here");
		ft_end_thread(counter, sim_struct);
		return (0);
	}
	return (1);
}
