#include "codexion.h"

void	*stop_sim(t_simulation *sim_struct, t_coder *coder)
{
	int	i;

	pthread_mutex_lock(&sim_struct->stop_lock);
	sim_struct->stop_simulation = 1;
	pthread_mutex_unlock(&sim_struct->stop_lock);
	if (coder)
		log_output(coder, BURNED_OUT);
	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		pthread_cond_broadcast(&sim_struct->dongle_array[i].cond);
		i++;
	}
	return (NULL);
}

void	take_dongle(t_coder *coder, t_dongle *dongle_struct)
{
	long long		cooldown;
	struct timespec	ts;
	struct timeval	tv;

	cooldown = coder->sim_struct->pars_struct->cooldown;
	pthread_mutex_lock(&dongle_struct->dongle_lock);
	gettimeofday(&tv, NULL);
	while ((dongle_struct->is_taken == true || dongle_struct->last_used_t
			+ cooldown < get_cur_time_ms(tv)))
	{
		if (coder->sim_struct->stop_simulation == 1)
		{
			pthread_mutex_unlock(&dongle_struct->dongle_lock);
			return ;
		}
		ts = get_absolute_time(cooldown);
		pthread_cond_timedwait(&dongle_struct->cond,
			&dongle_struct->dongle_lock, &ts);
		gettimeofday(&tv, NULL);
	}
	dongle_struct->is_taken = true;
	pthread_mutex_unlock(&dongle_struct->dongle_lock);
	if (coder->sim_struct->stop_simulation == 1)
		return ;
	log_output(coder, TOOK_DONGLE);
}

void	leave_dongle(t_dongle *dongle_struct)
{
	struct timeval	tv;

	pthread_mutex_lock(&dongle_struct->dongle_lock);
	gettimeofday(&tv, NULL);
	dongle_struct->is_taken = false;
	dongle_struct->last_used_t = get_cur_time_ms(tv);
	pthread_mutex_unlock(&dongle_struct->dongle_lock);
	pthread_cond_broadcast(&dongle_struct->cond);
}

void	*monitor(void *void_struct)
{
	t_simulation	*sim_struct;
	t_coder			*burned_coder;

	sim_struct = void_struct;
	while (true)
	{
		if (check_compiles(sim_struct) == 1)
			return (stop_sim(sim_struct, NULL));
		burned_coder = check_burnout(sim_struct);
		if (burned_coder != NULL)
			return (stop_sim(sim_struct, burned_coder));
	}
}

void	coder_compiling(t_coder *coder)
{
	struct timeval	tv;

	pthread_mutex_lock(&coder->coder_mutex);
	gettimeofday(&tv, NULL);
	coder->last_comp_time = get_cur_time_ms(tv);
	coder->n_compiles++;
	pthread_mutex_unlock(&coder->coder_mutex);
}
