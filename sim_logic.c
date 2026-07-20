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
