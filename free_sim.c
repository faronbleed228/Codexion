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
		if (sim_struct->free_struct)
			free(sim_struct->free_struct);
		if (sim_struct->queue_struct)
		{
			if (sim_struct->queue_struct->queue)
				free(sim_struct->queue_struct->queue);
			free(sim_struct->queue_struct);
		}
		free(sim_struct);
	}
}

void	free_everything(t_simulation *sim_struct, t_free *free_struct)
{
	int	i;

	i = 0;
	while (free_struct->cond > i)
		pthread_cond_destroy(&sim_struct->dongle_array[i++].cond);
	if (free_struct->queue_mutex == 1)
		pthread_mutex_destroy(&sim_struct->queue_struct->queue_lock);
	if (free_struct->sim_mutex-- != 0)
		pthread_mutex_destroy(&sim_struct->stop_lock);
	if (free_struct->sim_mutex-- != 0)
		pthread_mutex_destroy(&sim_struct->log_lock);
	i = 0;
	while (free_struct->dongle_mutex > i)
		pthread_mutex_destroy(&sim_struct->dongle_array[i++].dongle_lock);
	i = 0;
	while (free_struct->coder_mutex > i)
		pthread_mutex_destroy(&sim_struct->coder_array[i++].coder_mutex);
	free_sim(sim_struct);
}

void	join_threads(t_simulation *sim_struct, t_free *free_struct)
{
	int	i;

	i = 0;
	while (i < free_struct->coder_thread)
	{
		pthread_join(sim_struct->coder_array[i].thread_id, NULL);
		i++;
	}
	if (free_struct->sim_thread != 0)
		pthread_join(sim_struct->thread_id, NULL);
}
