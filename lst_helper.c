#include "codexion.h"

t_coder	ft_new_coder(int coder_id, t_simulation *sim_struct)
{
	t_coder	s_ptr;

	s_ptr.cod_num = coder_id;
	s_ptr.right_dongle = NULL;
	s_ptr.left_dongle = NULL;
	s_ptr.cur_status = WAITING;
	s_ptr.last_comp_time = sim_struct->start_time;
	s_ptr.n_compiles = 0;
	s_ptr.sim_struct = sim_struct;
	return (s_ptr);
}

t_dongle	ft_new_dongle(int dongle_id)
{
	t_dongle	s_ptr;

	s_ptr.dongle_id = dongle_id;
	s_ptr.last_comp_t = 0;
	s_ptr.is_taken = false;
	return (s_ptr);
}
