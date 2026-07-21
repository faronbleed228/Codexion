#include "codexion.h"

static t_dongle	*dongle_init(int coders_n, t_simulation *sim_struct)
{
	int			i;
	t_dongle	*dongle_array;

	i = 1;
	dongle_array = malloc(sizeof(t_dongle) * coders_n);
	if (!dongle_array)
		return (NULL);
	while (i <= coders_n)
	{
		dongle_array[i - 1] = ft_new_dongle(i, sim_struct);
		i++;
	}
	return (dongle_array);
}

static void	dongle_to_coders(t_dongle *dongle_array, t_coder *coder_array,
		int coder_n)
{
	int	i;

	i = 1;
	coder_array[0].right_dongle = &dongle_array[0];
	coder_array[0].left_dongle = &dongle_array[coder_n - 1];
	while (i < coder_n)
	{
		coder_array[i].left_dongle = coder_array[i - 1].right_dongle;
		coder_array[i].right_dongle = &dongle_array[i];
		i++;
	}
}
static t_coder	*coders_array(int coders_n, t_simulation *sim_struct)
{
	t_coder	*coder_array;
	t_coder	*coder;
	int		i;

	coder_array = malloc(sizeof(t_coder) * coders_n);
	if (!coder_array)
		return (NULL);
	i = 1;
	while (i <= coders_n)
	{
		coder_array[i - 1] = ft_new_coder(i, sim_struct);
		i++;
	}
	return (coder_array);
}

t_free	*free_init(void)
{
	t_free	*free_struct;

	free_struct = malloc(sizeof(t_free));
	if (!free_struct)
		return (NULL);
	free_struct->cond = 0;
	free_struct->coder_mutex = 0;
	free_struct->sim_mutex = 0;
	free_struct->dongle_mutex = 0;
	free_struct->coder_thread = 0;
	free_struct->sim_thread = 0;
	return (free_struct);
}
t_simulation	*simulation_init(t_parsing *pars_struct)
{
	t_simulation	*sim_struct;

	sim_struct = malloc(sizeof(t_simulation));
	if (!sim_struct)
	{
		free(pars_struct);
		return (NULL);
	}
	sim_struct->start_time = pars_struct->start_time;
	sim_struct->stop_simulation = 0;
	sim_struct->free_struct = free_init();
	sim_struct->coder_array = coders_array(pars_struct->coders, sim_struct);
	sim_struct->dongle_array = dongle_init(pars_struct->coders, sim_struct);
	sim_struct->pars_struct = pars_struct;
	if (!sim_struct->coder_array | !sim_struct->dongle_array | !sim_struct->free_struct)
	{
		free_sim(sim_struct);
		return (NULL);
	}
	dongle_to_coders(sim_struct->dongle_array, sim_struct->coder_array,
		pars_struct->coders);
	return (sim_struct);
}
