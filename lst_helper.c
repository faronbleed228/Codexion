/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_helper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:31:25 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/07/27 18:45:00 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder	ft_new_coder(int coder_id, t_simulation *sim_struct)
{
	t_coder	s_ptr;

	s_ptr.cod_num = coder_id;
	s_ptr.right_dongle = NULL;
	s_ptr.left_dongle = NULL;
	s_ptr.cur_status = NOT_WAITING;
	s_ptr.last_comp_time = sim_struct->start_time;
	s_ptr.n_compiles = 0;
	s_ptr.sim_struct = sim_struct;
	return (s_ptr);
}

t_dongle	ft_new_dongle(int dongle_id)
{
	t_dongle	s_ptr;

	s_ptr.dongle_id = dongle_id;
	s_ptr.last_used_t = 0;
	s_ptr.is_taken = false;
	s_ptr.owner = NULL;
	s_ptr.queue_n = 0;
	s_ptr.next_coder = -1;
	s_ptr.queue[0] = NULL;
	s_ptr.queue[1] = NULL;
	return (s_ptr);
}
