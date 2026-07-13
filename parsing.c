#include "codexion.h"

int	ft_check_int_len(char *str)
{
	char	str_max[10] = "2147483647";
	int		i;

	i = 0;
	if (strlen(&str[i]) > 10)
		return (0);
	if (strlen(&str[i]) == 10)
	{
		while (str[i])
		{
			if (str[i] < str_max[i])
				return (1);
			if (str[i] > str_max[i])
				return (0);
			i++;
		}
	}
	return (1);
}

int	ft_int_valid(char *str)
{
	int	j;
	int	digits;

	digits = 0;
	j = 0;
	if (str[j] == '-')
		return (0);
	if (str[j] == '+')
		j++;
	while (str[j] == '0')
	{
		j++;
		digits++;
	}
	if (ft_check_int_len(&str[j]) == 0)
		return (0);
	while (str[j])
	{
		if (ft_isdigit(str[j]) == 0)
			return (0);
		j++;
		digits++;
	}
	return (digits);
}

int	validator(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < 8)
	{
		if (strlen(argv[i]) == 0)
			return (0);
		if (ft_int_valid(argv[i]) == 0)
			return (0);
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (0);
	return (1);
}

t_parsing	*parsing(char **argv)
{
	int				i;
	t_parsing		*pars_struck;
	struct timeval	tv;

	i = 1;
	if (validator(argv) == 0)
		return (NULL);
	pars_struck = malloc(sizeof(t_parsing));
	if (!pars_struck)
		return (NULL);
	gettimeofday(&tv, NULL);
	pars_struck->coders = atoi(argv[1]);
	pars_struck->t_to_burn = atoi(argv[2]);
	pars_struck->t_to_copm = atoi(argv[3]);
	pars_struck->t_to_debug = atoi(argv[4]);
	pars_struck->t_to_refac = atoi(argv[5]);
	pars_struck->copm_req = atoi(argv[6]);
	pars_struck->cooldown = atoi(argv[7]);
	pars_struck->scheduler = argv[8];
	pars_struck->start_time = get_cur_time_ns(tv);
	return (pars_struck);
}
