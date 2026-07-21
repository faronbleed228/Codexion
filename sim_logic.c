#include "codexion.h"

void	take_dongle(t_coder *coder, t_dongle *dongle_struct)
{
	long long		cooldown;
	struct timespec	ts;
	struct timeval	tv;

	cooldown = coder->sim_struct->pars_struct->cooldown;
	pthread_mutex_lock(&dongle_struct->dongle_lock);
	gettimeofday(&tv, NULL);
	while (dongle_struct->is_taken == true || dongle_struct->last_comp_t
		+ cooldown > get_cur_time_ns(tv))
	{
		ts = get_absolute_time(cooldown);
		pthread_cond_timedwait(&dongle_struct->cond,
			&dongle_struct->dongle_lock, &ts);
		gettimeofday(&tv, NULL);
	}
	dongle_struct->is_taken = true;
	pthread_mutex_unlock(&dongle_struct->dongle_lock);
	log_output(coder, TOOK_DONGLE);
}
void	leave_dongle(t_dongle *dongle_struct)
{
	struct timeval	tv;

	pthread_mutex_lock(&dongle_struct->dongle_lock);
	gettimeofday(&tv, NULL);
	dongle_struct->is_taken = false;
	dongle_struct->last_comp_t = get_cur_time_ns(tv);
	pthread_mutex_unlock(&dongle_struct->dongle_lock);
	pthread_cond_broadcast(&dongle_struct->cond);
}

void	stop_sim(t_simulation *sim_struct, t_coder *coder)
{
	int	i;

	log_output(coder, BURNED_OUT);
	pthread_mutex_lock(&sim_struct->stop_lock);
	sim_struct->stop_simulation = 1;
	pthread_mutex_unlock(&sim_struct->stop_lock);
	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		pthread_cond_broadcast(&sim_struct->dongle_array[i].cond);
		i++;
	}
}

void	monitor(t_simulation *sim_struct)
{
	int				i;
	long long		cur_time;
	struct timeval	tv;
	long long		last_copm_time;

	i = 0;
	while (true)
	{
		i = 0;
		gettimeofday(&tv, NULL);
		cur_time = get_cur_time_ns(tv);
		while (i < sim_struct->pars_struct->coders)
		{
			last_copm_time = sim_struct->coder_array[i].last_comp_time;
			if (cur_time - last_copm_time >= sim_struct->pars_struct->t_to_burn)
			{
				stop_sim(sim_struct, &sim_struct->coder_array[i]);
				return ;
			}
			i++;
		}
	}
}
