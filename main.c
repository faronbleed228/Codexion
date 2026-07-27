/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:31:19 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/07/27 21:41:44 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coders_print(t_coder coder);
void	parsing_print(t_parsing *pars_struct);

int	main(int argc, char **argv)
{
	t_parsing		*pars_struct;
	int				i;
	t_simulation	*sim_struct;

	if (argc != 9)
	{
		fprintf(stderr, "Not enough arguments");
		return (1);
	}
	pars_struct = parsing(argv);
	if (pars_struct == NULL)
	{
		fprintf(stderr, "Validation Error");
		return (1);
	}
	// parsing_print(pars_struct);
	sim_struct = simulation_init(pars_struct);
	if (!sim_struct)
	{
		fprintf(stderr, "Coders Error");
		return (1);
	}
	i = 0;
	if (simulation_start(sim_struct) == 0)
	{
		stop_sim(sim_struct, NULL);
		join_threads(sim_struct, sim_struct->free_struct);
		free_everything(sim_struct, sim_struct->free_struct);
		return (1);
	}
	join_threads(sim_struct, sim_struct->free_struct);
	free_everything(sim_struct, sim_struct->free_struct);
}
