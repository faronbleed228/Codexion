#include "codexion.h"

t_dongle	*dongle_coder(t_parsing *pars_struct, t_dongle *left_dongle)
{
	t_coder		*new_coder;
	t_dongle	*right_dongle;
	int			id;

	id = 2;
	while (id <= pars_struct->coders)
	{
		right_dongle = ft_new_dongle(id);
		if (!right_dongle)
			return (NULL);
		new_coder = ft_new_coder(id);
		if (!new_coder)
			return (NULL);
		new_coder->left_dongle = left_dongle;
		new_coder->right_dongle = right_dongle;
		left_dongle = right_dongle;
		id++;
	}
	return (left_dongle);
}

t_coder	*coders_init(t_parsing *pars_struct)
{
	t_coder		*middle_coder;
	t_coder		*first_coder;
	t_dongle	*first_d;
	t_dongle	*last_dongle;

	first_d = ft_new_dongle(1);
	if (!first_d)
		return (NULL);
	first_coder = ft_new_coder(1);
	if (!first_coder)
		return (NULL);
	first_coder->right_dongle = first_d;
	last_dongle = dongle_coder(pars_struct, first_d);
	first_coder->left_dongle = last_dongle;
	return (first_coder);
}
