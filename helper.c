#include "codexion.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long	get_cur_time_ns(struct timeval tv)
{
	long	time_ms;

	time_ms = (tv.tv_usec / 1000) + (tv.tv_sec * 1000);
	return (time_ms);
}
