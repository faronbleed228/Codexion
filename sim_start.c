#include "codexion.h"

static int	cond_init(t_simulation *sim_struct)
{
	int	i;

	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		if (pthread_cond_init(&sim_struct->dongle_array[i++].cond, NULL) != 0)
			return (0);
		sim_struct->free_struct->cond++;
	}
	return (1);
}

static int	mutex_init(t_simulation *sim_struct)
{
	int	i;

	if (pthread_mutex_init(&sim_struct->stop_lock, NULL) != 0)
		return (0);
	sim_struct->free_struct->sim_mutex++;
	if (pthread_mutex_init(&sim_struct->log_lock, NULL) != 0)
		return (0);
	sim_struct->free_struct->sim_mutex++;
	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		if (pthread_mutex_init(&sim_struct->dongle_array[i].dongle_lock,
				NULL) != 0)
			return (0);
		sim_struct->free_struct->dongle_mutex++;
		if (pthread_mutex_init(&sim_struct->coder_array[i++].coder_mutex,
				NULL) != 0)
			return (0);
		sim_struct->free_struct->coder_mutex++;
	}
	return (1);
}

void	*coder_routine(void *coder)
{
	t_coder	*cur_coder;
	int		i;

	i = 0;
	cur_coder = coder;
	while (i < 4)
	{
		if (cur_coder->sim_struct->stop_simulation == 1)
			return (NULL);
		if (cur_coder->cod_num % 2 == 0)
		{
			take_dongle(cur_coder, cur_coder->left_dongle);
			take_dongle(cur_coder, cur_coder->right_dongle);
		}
		else if (cur_coder->cod_num % 2 != 0)
		{
			take_dongle(cur_coder, cur_coder->right_dongle);
			take_dongle(cur_coder, cur_coder->left_dongle);
		}
		coder_compiling(cur_coder);
		if (cur_coder->sim_struct->stop_simulation == 1)
			return (NULL);
		log_output(cur_coder, COMPILING);
		usleep(cur_coder->sim_struct->pars_struct->t_to_copm * 1000);
		leave_dongle(cur_coder->left_dongle);
		leave_dongle(cur_coder->right_dongle);
		if (cur_coder->sim_struct->stop_simulation == 1)
			return (NULL);
		log_output(cur_coder, DEBUGGING);
		usleep(cur_coder->sim_struct->pars_struct->t_to_debug * 1000);
		if (cur_coder->sim_struct->stop_simulation == 1)
			return (NULL);
		log_output(cur_coder, REFACTORING);
		usleep(cur_coder->sim_struct->pars_struct->t_to_refac * 1000);
		i++;
	}
	return (NULL);
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
		sim_struct->free_struct->coder_thread++;
	}
	if (pthread_create(&sim_struct->thread_id, NULL, &monitor,
			(void *)sim_struct) != 0)
		return (0);
	sim_struct->free_struct->sim_thread++;
	return (1);
}

int	simulation_start(t_simulation *sim_struct)
{
	if (cond_init(sim_struct) == 0)
		return (0);
	if (mutex_init(sim_struct) == 0)
		return (0);
	if (thread_init(sim_struct) == 0)
		return (0);
	return (1);
}
