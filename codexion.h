#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_dongle
{
	int			dongle_id;
}				t_dongle;

typedef enum e_status
{
	COMPILING = 1,
	DEBUGGING = 2,
	REFACTORING = 3,
	WAITING = 4

}				status;

typedef struct s_coder
{
	int			cod_num;
	long		last_comp_time;
	int			n_compiles;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	status		cur_status;

}				t_coder;

typedef struct s_parsing
{
	int			coders;
	long		t_to_burn;
	long		t_to_copm;
	long		t_to_debug;
	long		t_to_refac;
	int			copm_req;
	long		cooldown;
	char		*scheduler;
	long		start_time;
}				t_parsing;

typedef struct s_simulation
{
	t_coder		**coder_array;
	t_dongle	**dongle_array;
	long		start_time;
}				t_simulation;

t_parsing		*parsing(char **argv);
t_simulation	*simulation_init(t_parsing *pars_struct);

int				ft_isdigit(int c);
long			get_cur_time_ns(struct timeval tv);

t_coder			*ft_new_coder(int coder_id);
t_dongle		*ft_new_dongle(int dongle_id);

#endif
