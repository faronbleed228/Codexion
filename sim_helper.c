/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:30:59 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/07/27 22:39:40 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	choose_dongle(t_coder *cur_coder)
{
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
}

void	choose_leave_dongle(t_coder *cur_coder)
{
	if (cur_coder->cod_num % 2 != 0)
	{
		leave_dongle(cur_coder, cur_coder->left_dongle);
		leave_dongle(cur_coder, cur_coder->right_dongle);
	}
	else if (cur_coder->cod_num % 2 == 0)
	{
		leave_dongle(cur_coder, cur_coder->right_dongle);
		leave_dongle(cur_coder, cur_coder->left_dongle);
	}
}

int	check_compiles(t_simulation *sim_struct)
{
	int	i;
	int	total;
	int	j;

	i = 0;
	j = 0;
	total = sim_struct->pars_struct->copm_req;
	while (i < sim_struct->pars_struct->coders)
	{
		printf("ID: %i,COMP: %i\n", sim_struct->coder_array[i].cod_num,
			sim_struct->coder_array[i].n_compiles);
		pthread_mutex_lock(&sim_struct->coder_array[i].coder_mutex);
		if (sim_struct->coder_array[i].n_compiles >= total)
		{
			j++;
		}
		pthread_mutex_unlock(&sim_struct->coder_array[i].coder_mutex);
		i++;
	}
	printf("comped coders %i\n", j);
	return (j);
}

t_coder	*check_burnout(t_simulation *sim_struct)
{
	long long		cur_time;
	struct timeval	tv;
	long long		last_copm_time;
	int				i;

	i = 0;
	while (i < sim_struct->pars_struct->coders)
	{
		pthread_mutex_lock(&sim_struct->coder_array[i].coder_mutex);
		gettimeofday(&tv, NULL);
		cur_time = get_cur_time_ms(tv);
		last_copm_time = sim_struct->coder_array[i].last_comp_time;
		if (cur_time - last_copm_time >= sim_struct->pars_struct->t_to_burn)
		{
			pthread_mutex_unlock(&sim_struct->coder_array[i].coder_mutex);
			return (&sim_struct->coder_array[i]);
		}
		pthread_mutex_unlock(&sim_struct->coder_array[i].coder_mutex);
		i++;
	}
	return (NULL);
}

bool	check_simulation(t_simulation *sim_struct)
{
	bool	sim_status;

	pthread_mutex_lock(&sim_struct->stop_lock);
	if (sim_struct->stop_simulation == 1)
		sim_status = false;
	else
		sim_status = true;
	pthread_mutex_unlock(&sim_struct->stop_lock);
	return (sim_status);
}

int	is_dongle_busy(t_dongle *dongle, t_coder *coder, t_parsing *pars)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (dongle->is_taken)
		return (1);
	if (get_cur_time_ms(tv) < dongle->last_used_t + pars->cooldown)
		return (1);
	if (dongle->next_coder != coder->cod_num && dongle->next_coder != -1)
		return (1);
	return (0);
}
