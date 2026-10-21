/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:31:28 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/07/28 14:23:52 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long long	get_cur_time_ms(struct timeval tv)
{
	long long	time_ms;

	time_ms = (tv.tv_usec / 1000) + (tv.tv_sec * 1000);
	return (time_ms);
}

struct timespec	get_absolute_time(long long waiting)
{
	struct timeval	tv;
	struct timespec	ts;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + waiting / 1000;
	ts.tv_nsec = tv.tv_usec * 1000 + (waiting % 1000) * 1000000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}
	return (ts);
}

void	log_output(t_coder *coder, t_status message)
{
	struct timeval	tv;
	long long		cur_time;
	char			*messages[5];

	messages[0] = "is compiling";
	messages[1] = "is debugging";
	messages[2] = "is refactoring";
	messages[3] = "has taken a dongle";
	messages[4] = "burned out";
	gettimeofday(&tv, NULL);
	cur_time = get_cur_time_ms(tv) - coder->sim_struct->start_time;
	pthread_mutex_lock(&coder->sim_struct->log_lock);
	fprintf(stdout, "%lli %i %s\n", cur_time, coder->cod_num,
		messages[message]);
	pthread_mutex_unlock(&coder->sim_struct->log_lock);
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
