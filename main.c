/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ypopovyc <@student.42vienna.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/27 16:31:19 by ypopovyc          #+#    #+#             */
/*   Updated: 2026/07/28 11:53:26 by ypopovyc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void				coders_print(t_coder coder);
void				parsing_print(t_parsing *pars_struct);
static t_parsing	*parsing_validation(int argc, char **argv);

int	main(int argc, char **argv)
{
	t_parsing		*pars_struct;
	t_simulation	*sim_struct;

	if (argc != 9)
	{
		fprintf(stderr, "Not enough arguments");
		return (1);
	}
	pars_struct = parsing_validation(argc, argv);
	if (pars_struct == NULL)
		return (1);
	sim_struct = simulation_init(pars_struct);
	if (!sim_struct)
		return (1);
	if (simulation_start(sim_struct) == 0)
	{
		if (sim_struct->free_struct->coder_thread > 0)
			stop_sim(sim_struct, NULL);
		join_threads(sim_struct, sim_struct->free_struct);
		free_everything(sim_struct, sim_struct->free_struct);
		return (1);
	}
	join_threads(sim_struct, sim_struct->free_struct);
	free_everything(sim_struct, sim_struct->free_struct);
}

static t_parsing	*parsing_validation(int argc, char **argv)
{
	t_parsing	*pars_struct;

	if (argc != 9)
	{
		fprintf(stderr, "Not enough arguments\n");
		return (NULL);
	}
	pars_struct = parsing(argv);
	if (pars_struct == NULL)
	{
		fprintf(stderr, "Validation Error\n");
		return (NULL);
	}
	if (pars_struct->copm_req == 0)
	{
		fprintf(stderr,
			"Compilation requiered = 0. No need to create threads\n");
		free(pars_struct);
		return (NULL);
	}
	return (pars_struct);
}
