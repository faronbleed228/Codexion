#include "codexion.h"

static int	mutex_cond_init(t_simulation *sim_struct)
{
	int	i;

	if (pthread_mutex_init(&sim_struct->stop_lock, NULL) != 0)
		return (0);
	sim_struct->free_struct->mutex++;
	if (pthread_mutex_init(&sim_struct->log_lock, NULL) != 0)
		return (0);
	sim_struct->free_struct->mutex++;
	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		if (pthread_cond_init(&sim_struct->dongle_array[i++].cond, NULL) != 0)
			return (0);
		sim_struct->free_struct->cond++;
	}
	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		if (pthread_mutex_init(&sim_struct->dongle_array[i++].dongle_lock,
				NULL) != 0)
			return (0);
		sim_struct->free_struct->mutex++;
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
			return (0);
		i++;
		sim_struct->free_struct->threads++;
	}
	return (1);
}

int	simulation_start(t_simulation *sim_struct)
{
	if (mutex_cond_init(sim_struct) == 0)
		return (0);
	if (thread_init(sim_struct) == 0)
		return (0);
	return (1);
}
