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
		sim_struct->dongle_array[i].queue_n = 0;
		sim_struct->dongle_array[i].queue[0] = NULL;
		sim_struct->dongle_array[i].queue[1] = NULL;
		pthread_cond_broadcast(&sim_struct->dongle_array[i].cond);
		pthread_mutex_unlock(&sim_struct->dongle_array[i].dongle_lock);
		i++;
	}
	if (coder)
		log_output(coder, BURNED_OUT);
	return (NULL);
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
