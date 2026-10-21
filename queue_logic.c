/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_logic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:31:11 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/07/28 11:09:59 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	delete_coder(t_dongle *dongle_struct)
{
	if (dongle_struct->queue_n == 1)
	{
		dongle_struct->queue[0] = NULL;
		dongle_struct->queue_n--;
	}
	else if (dongle_struct->queue_n == 2)
	{
		dongle_struct->queue[0] = dongle_struct->queue[1];
		dongle_struct->queue[1] = NULL;
		dongle_struct->queue_n--;
	}
}

void	add_to_queue(t_dongle *dongle_struct, t_coder *coder_struct)
{
	dongle_struct->queue[dongle_struct->queue_n] = coder_struct;
	dongle_struct->queue_n++;
}

void	fifo_queue(t_dongle *dongle_struct)
{
	if (dongle_struct->queue_n != -1)
		dongle_struct->next_coder = dongle_struct->queue[0]->cod_num;
	else
		dongle_struct->next_coder = -1;
}

static int	edf_logic(t_coder *coder_1, t_coder *coder_2,
		t_dongle *dongle_struct)
{
	struct timeval	tv;
	long long		cur_time;
	long long		time_past_01;
	long long		time_past_02;

	pthread_mutex_lock(&coder_1->coder_mutex);
	pthread_mutex_lock(&coder_2->coder_mutex);
	gettimeofday(&tv, NULL);
	cur_time = get_cur_time_ms(tv);
	time_past_01 = cur_time - coder_1->last_comp_time;
	time_past_02 = cur_time - coder_2->last_comp_time;
	pthread_mutex_unlock(&coder_1->coder_mutex);
	pthread_mutex_unlock(&coder_2->coder_mutex);
	if (time_past_01 > time_past_02)
	{
		dongle_struct->queue[0] = coder_1;
		return (coder_1->cod_num);
	}
	dongle_struct->queue[0] = coder_2;
	return (coder_2->cod_num);
}

void	edf_queue(t_dongle *dongle_struct)
{
	t_coder	*coder_1;
	t_coder	*coder_2;

	if (dongle_struct->queue_n == 2)
	{
		coder_1 = dongle_struct->queue[0];
		coder_2 = dongle_struct->queue[1];
		dongle_struct->next_coder = edf_logic(coder_1, coder_2, dongle_struct);
		dongle_struct->queue_n--;
	}
	else if (dongle_struct->queue_n == 1)
	{
		dongle_struct->next_coder = dongle_struct->queue[0]->cod_num;
		dongle_struct->queue_n--;
	}
}
