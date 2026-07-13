#include "codexion.h"

t_dongle	**dongle_init(int coders_n)
{
	int			i;
	t_dongle	**dongle_array;

	i = 1;
	dongle_array = malloc(sizeof(t_dongle *) * coders_n);
	if (!dongle_array)
		return (NULL);
	while (i <= coders_n)
	{
		dongle_array[i - 1] = ft_new_dongle(i);
		if (!dongle_array[i - 1])
			return (NULL);
		i++;
	}
	return (dongle_array);
}

void	dongle_to_coders(t_dongle **dongle_array, t_coder **coder_array,
		int coder_n)
{
	int	i;

	i = 1;
	coder_array[0]->right_dongle = dongle_array[0];
	coder_array[0]->left_dongle = dongle_array[coder_n - 1];
	while (i < coder_n)
	{
		coder_array[i]->left_dongle = coder_array[i - 1]->right_dongle;
		coder_array[i]->right_dongle = dongle_array[i];
		i++;
	}
	free(dongle_array);
}
t_coder	**coders_array(int coders_n)
{
	t_coder	**coder_array;
	t_coder	*coder;
	int		i;

	coder_array = malloc(sizeof(t_coder *) * coders_n);
	if (!coder_array)
		return (NULL);
	i = 1;
	while (i <= coders_n)
	{
		coder_array[i - 1] = ft_new_coder(i);
		if (!coder_array[i - 1])
			return (NULL);
		i++;
	}
	return (coder_array);
}

t_simulation	*simulation_init(t_parsing *pars_struct)
{
	t_simulation	*sim_struct;

	sim_struct = malloc(sizeof(t_simulation));
	if (!sim_struct)
		return (NULL);
	sim_struct->coder_array = coders_array(pars_struct->coders);
	sim_struct->dongle_array = dongle_init(pars_struct->coders);
	if (!sim_struct->coder_array | !sim_struct->dongle_array)
		return (NULL);
	sim_struct->start_time = pars_struct->start_time;
	dongle_to_coders(sim_struct->dongle_array, sim_struct->coder_array,
		pars_struct->coders);
	return (sim_struct);
}
