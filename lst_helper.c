#include "codexion.h"

t_coder	ft_new_coder(int coder_id)
{
	t_coder	s_ptr;

	s_ptr.cod_num = coder_id;
	s_ptr.right_dongle = NULL;
	s_ptr.left_dongle = NULL;
	s_ptr.cur_status = WAITING;
	s_ptr.last_comp_time = 0;
	s_ptr.n_compiles = 0;
	return (s_ptr);
}

t_dongle	ft_new_dongle(int dongle_id)
{
	t_dongle	s_ptr;

	s_ptr.dongle_id = dongle_id;
	s_ptr.last_comp_t = 0;
	return (s_ptr);
}
