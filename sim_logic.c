#include "codexion.h"

void	stop_sim(t_simulation *sim_struct, t_coder *coder)
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
	int				i;
	long long		cur_time;
	t_simulation	*sim_struct;
	struct timeval	tv;
	long long		last_copm_time;

	sim_struct = void_struct;
	i = 0;
	while (true)
	{
		i = 0;
		while (i < sim_struct->pars_struct->coders)
		{
			pthread_mutex_lock(&sim_struct->coder_array[i].coder_mutex);
			gettimeofday(&tv, NULL);
			cur_time = get_cur_time_ms(tv);
			last_copm_time = sim_struct->coder_array[i].last_comp_time;
			// printf("Current time: %lli\n", cur_time);
			// printf("Last comp time: %lli\n", last_copm_time);
			// printf("Time to burn: %lli\n",
			// sim_struct->pars_struct->t_to_burn);
			// printf("Cosder: %i\n", i);
			if (cur_time - last_copm_time >= sim_struct->pars_struct->t_to_burn)
			{
				stop_sim(sim_struct, &sim_struct->coder_array[i]);
				pthread_mutex_unlock(&sim_struct->coder_array[i].coder_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&sim_struct->coder_array[i].coder_mutex);
			i++;
		}
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
