/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_logic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:31:04 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/07/28 14:32:43 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*stop_sim(t_simulation *sim_struct, t_coder *coder)
{
	int	i;

	pthread_mutex_lock(&sim_struct->stop_lock);
	sim_struct->stop_simulation = 1;
	pthread_mutex_unlock(&sim_struct->stop_lock);
	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		pthread_mutex_lock(&sim_struct->dongle_array[i].dongle_lock);
		pthread_cond_broadcast(&sim_struct->dongle_array[i].cond);
		pthread_mutex_unlock(&sim_struct->dongle_array[i].dongle_lock);
		i++;
	}
	if (coder)
		log_output(coder, BURNED_OUT);
	return (NULL);
}

void	take_dongle(t_coder *coder, t_dongle *dongle_struct)
{
	struct timespec	ts;

	pthread_mutex_lock(&dongle_struct->dongle_lock);
	add_to_queue(dongle_struct, coder);
	while (is_dongle_busy(dongle_struct, coder, coder->sim_struct->pars_struct)
		&& check_simulation(coder->sim_struct))
	{
		ts = get_absolute_time(coder->sim_struct->pars_struct->cooldown);
		pthread_cond_timedwait(&dongle_struct->cond,
			&dongle_struct->dongle_lock, &ts);
	}
	dongle_struct->is_taken = true;
	delete_coder(dongle_struct);
	dongle_struct->next_coder = -1;
	pthread_mutex_unlock(&dongle_struct->dongle_lock);
	if (check_simulation(coder->sim_struct) == false)
		return ;
	log_output(coder, TOOK_DONGLE);
}

void	leave_dongle(t_coder *coder_struct, t_dongle *dongle_struct)
{
	struct timeval	tv;
	char			*scheduler;

	scheduler = coder_struct->sim_struct->pars_struct->scheduler;
	pthread_mutex_lock(&dongle_struct->dongle_lock);
	gettimeofday(&tv, NULL);
	dongle_struct->last_used_t = get_cur_time_ms(tv);
	dongle_struct->is_taken = false;
	if (dongle_struct->queue_n != 0 && strcmp(scheduler, "fifo") == 0)
		fifo_queue(dongle_struct);
	else if (dongle_struct->queue_n != 0 && strcmp(scheduler, "edf") == 0)
		edf_queue(dongle_struct);
	pthread_cond_broadcast(&dongle_struct->cond);
	pthread_mutex_unlock(&dongle_struct->dongle_lock);
}

void	*monitor(void *void_struct)
{
	t_simulation	*sim_struct;
	t_coder			*burned_coder;

	sim_struct = void_struct;
	while (true)
	{
		pthread_mutex_lock(&sim_struct->stop_lock);
		burned_coder = check_burnout(sim_struct);
		if (burned_coder != NULL)
		{
			pthread_mutex_unlock(&sim_struct->stop_lock);
			return (stop_sim(sim_struct, burned_coder));
		}
		if (check_compiles(sim_struct) == sim_struct->pars_struct->coders)
		{
			pthread_mutex_unlock(&sim_struct->stop_lock);
			return (stop_sim(sim_struct, NULL));
		}
		pthread_mutex_unlock(&sim_struct->stop_lock);
		usleep(1);
	}
}

void	coder_compiling(t_coder *coder)
{
	struct timeval	tv;

	if (check_simulation(coder->sim_struct) == false)
		return ;
	(pthread_mutex_lock(&coder->coder_mutex));
	coder->n_compiles++;
	gettimeofday(&tv, NULL);
	coder->last_comp_time = get_cur_time_ms(tv);
	pthread_mutex_unlock(&coder->coder_mutex);
}
