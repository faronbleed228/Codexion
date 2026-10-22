/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_logic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:31:04 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/07/28 14:32:43 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Lock: caller holds dongle_lock; EDF reads coder times under coder_mutex. */
static void	select_queue(t_coder *coder, t_dongle *dongle)
{
	if (strcmp(coder->sim_struct->pars_struct->scheduler, "edf") == 0)
		edf_queue(dongle);
	else
		fifo_queue(dongle);
}

static void	wait_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;

	add_to_queue(dongle, coder);
	select_queue(coder, dongle);
	while (is_dongle_busy(dongle, coder, coder->sim_struct->pars_struct)
		&& check_simulation(coder->sim_struct))
	{
		ts = get_absolute_time(1);
		pthread_cond_timedwait(&dongle->cond, &dongle->dongle_lock, &ts);
	}
}

/* Lock order: dongle_lock -> stop_lock. No coder mutex is held here. */
static void	claim_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim_struct->stop_lock);
	if (!coder->sim_struct->stop_simulation)
	{
		dongle->is_taken = true;
		dongle->owner = coder;
		delete_coder(dongle);
		dongle->next_coder = -1;
	}
	pthread_mutex_unlock(&coder->sim_struct->stop_lock);
}

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_lock);
	if (check_simulation(coder->sim_struct))
	{
		wait_dongle(coder, dongle);
		claim_dongle(coder, dongle);
	}
	pthread_mutex_unlock(&dongle->dongle_lock);
	if (check_simulation(coder->sim_struct))
		log_output(coder, TOOK_DONGLE);
}

void	leave_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timeval	tv;

	pthread_mutex_lock(&dongle->dongle_lock);
	if (dongle->owner != coder)
	{
		pthread_mutex_unlock(&dongle->dongle_lock);
		return ;
	}
	dongle->owner = NULL;
	gettimeofday(&tv, NULL);
	dongle->last_used_t = get_cur_time_ms(tv);
	dongle->is_taken = false;
	select_queue(coder, dongle);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->dongle_lock);
}
